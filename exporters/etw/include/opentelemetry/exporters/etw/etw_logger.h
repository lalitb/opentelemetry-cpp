/ Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <algorithm>
#include <atomic>

#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <sstream>
#include <vector>

#include "opentelemetry/nostd/shared_ptr.h"
#include "opentelemetry/nostd/string_view.h"
#include "opentelemetry/nostd/unique_ptr.h"
#include "opentelemetry/nostd/variant.h"

#include "opentelemetry/common/key_value_iterable_view.h"

#include "opentelemetry/trace/span.h"
#include "opentelemetry/trace/span_id.h"
#include "opentelemetry/trace/trace_id.h"
#include "opentelemetry/logs/logger_provider.h

#include "opentelemetry/exporters/etw/etw_fields.h"
#include "opentelemetry/exporters/etw/etw_properties.h"
#include "opentelemetry/exporters/etw/etw_provider.h"
#include "opentelemetry/exporters/etw/utils.h"

    namespace trace = opentelemetry::trace;

OPENTELEMETRY_BEGIN_NAMESPACE
namespace exporter
{
namespace etw
{

/**
 * @brief TracerProvider Options passed via SDK API.
 */
using LoggerProviderOptions =
    std::map<std::string, nostd::variant<std::string, uint64_t, float, bool>>;

/**
 * @brief LoggerProvider runtime configuration class. Internal representation
 * of LoggerProviderOptions used by various components of SDK.
 */
typedef struct
{
  ETWProvider::EventFormat
      encoding;  // Event encoding to use for this provider (TLD, MsgPack, XML, etc.).
} LoggerProviderConfiguration;

/**
 * @brief Helper template to convert a variant value from TracerProviderOptions to
 * LoggerProviderConfiguration
 *
 * @param options           TracerProviderOptions passed on API surface
 * @param key               Option name
 * @param value             Reference to destination value
 * @param defaultValue      Default value if option is not supplied
 */
template <typename T>
static inline void GetOption(const LoggerProviderOptions &options,
                             const char *key,
                             T &value,
                             T defaultValue)
{
  auto it = options.find(key);
  if (it != options.end())
  {
    auto val = it->second;
    value    = nostd::get<T>(val);
  }
  else
  {
    value = defaultValue;
  }
}

/**
 * @brief Helper template to convert encoding config option to EventFormat.
 * Configuration option passed as `options["encoding"] = "MsgPack"`.
 * Default encoding is TraceLogging Dynamic Manifest (TLD).
 *
 * Valid encoding names listed below.
 *
 * For MessagePack encoding:
 * - "MSGPACK"
 * - "MsgPack"
 * - "MessagePack"
 *
 * For XML encoding:
 * - "XML"
 * - "xml"
 *
 * For TraceLogging Dynamic encoding:
 * - "TLD"
 * - "tld"
 *
 */
static inline ETWProvider::EventFormat GetEncoding(const LoggerProviderOptions &options)
{
  ETWProvider::EventFormat evtFmt = ETWProvider::EventFormat::ETW_MANIFEST;

  auto it = options.find("encoding");
  if (it != options.end())
  {
    auto varValue   = it->second;
    std::string val = nostd::get<std::string>(varValue);

#pragma warning(push)
#pragma warning(disable : 4307) /* Integral constant overflow - OK while computing hash */
    auto h = utils::hashCode(val.c_str());
    switch (h)
    {
      case CONST_HASHCODE(MSGPACK):
        // nobrk
      case CONST_HASHCODE(MsgPack):
        // nobrk
      case CONST_HASHCODE(MessagePack):
        evtFmt = ETWProvider::EventFormat::ETW_MSGPACK;
        break;

      case CONST_HASHCODE(XML):
        // nobrk
      case CONST_HASHCODE(xml):
        evtFmt = ETWProvider::EventFormat::ETW_XML;
        break;

      case CONST_HASHCODE(TLD):
        // nobrk
      case CONST_HASHCODE(tld):
        // nobrk
        evtFmt = ETWProvider::EventFormat::ETW_MANIFEST;
        break;

      default:
        break;
    }
#pragma warning(pop)
  }

  return evtFmt;
}

/**
 * @brief Logger  class that allows to send logs to ETW Provider.
 */
class Logger : public logs::Logger
{

  /**
   * @brief Parent provider of this Tracer
   */
  etw::LoggerProvider &loggerProvider_;

  /**
   * @brief ProviderId (Name or GUID)
   */
  std::string provId;

  /**
   * @brief Encoding (Manifest, MessagePack or XML)
   */
  ETWProvider::EventFormat encoding;

  /**
   * @brief Provider Handle
   */
  ETWProvider::Handle &provHandle;

  std::atomic<bool> isClosed_{true};

  /**
   * @brief ETWProvider is a singleton that aggregates all ETW writes.
   * @return
   */
  static ETWProvider &etwProvider()
  {
    static ETWProvider instance;  // C++11 magic static
    return instance;
  };

  /**
   * @brief Init a reference to etw::ProviderHandle
   * @return Provider Handle
   */
  ETWProvider::Handle &initProvHandle()
  {
    isClosed_ = false;
    return etwProvider().open(provId, encoding);
  }

public:
  /**
   * @brief Tracer constructor
   * @param parent Parent LoggerProvider
   * @param providerId ProviderId - Name or GUID
   * @param encoding ETW encoding format to use.
   */
  Logger(etw::TracerProvider &parent,
         nostd::string_view providerId     = "",
         ETWProvider::EventFormat encoding = ETWProvider::EventFormat::ETW_MANIFEST)
      : logs::Logger(),
        loggerProvider_(parent),
        provId(providerId.data(), providerId.size()),
        encoding(encoding),
        provHandle(initProvHandle())
  {}

  void Log(Severity severity,
           nostd::string_view name,
           nostd::string_view body,
           const common::KeyValueIterable &resource,
           const common::KeyValueIterable &attributes,
           trace::TraceId trace_id,
           trace::SpanId span_id,
           trace::TraceFlags trace_flags,
           common::SystemTimestamp timestamp) noexcept override
  {

#ifdef RTTI_ENABLED
    common::KeyValueIterable &attribs = const_cast<common::KeyValueIterable &>(attributes);
    // common::KeyValueIterable &resr = const_cast<common::KeyValueIterable &>(resource);
    Properties *evt = dynamic_cast<Properties *>(&attribs);
    // Properties *res                   = dynamic_cast<Properties *>(&resr);

    if (evt != nullptr)
    {
      // Pass as a reference to original modifyable collection without creating a copy
      return StartSpan(name, body, *evt, trace_id, span_id, trace_flags, timestamp);
    }
#endif
    Properties evtCopy = attributes;
    Properties resCopy = resource;
    return StartSpan(name, body, evtCopy, trace_id, span_id, trace_flags, timestamp);
  }

  virtual void Log(Severity severity,
                   nostd::string_view name,
                   nostd::string_view body,
                   Properties &evt,
                   trace::TraceId trace_id,
                   trace::SpanId span_id,
                   trace::TraceFlags trace_flags,
                   common::SystemTimestamp timestamp) noexcept
  {
    const auto &cfg = GetConfiguration(tracerProvider_);
    // Populate Etw.EventName attribute at envelope level
    evt[ETW_FIELD_NAME] = ETW_VALUE_LOG;

#ifdef HAVE_FIELD_TIME
    {
      auto timeNow        = std::chrono::system_clock::now().time_since_epoch();
      auto millis         = std::chrono::duration_cast<std::chrono::milliseconds>(timeNow).count();
      evt[ETW_FIELD_TIME] = utils::formatUtcTimestampMsAsISO8601(millis);
    }
#endif

    evt[ETW_FIELD_SPAN_ID]                   = ToLowerBase16(span_id);
    evt[ETW_FIELD_TRACE_ID]                  = ToLowerBase16(trace_id);
    evt[ETW_FIELD_PAYLOAD_NAME]              = std::string(name.data(), name.length());
    std::chrono::system_clock::time_point ts = timestamp;
    int64_t tsMs =
        std::chrono::duration_cast<std::chrono::milliseconds>(ts.time_since_epoch()).count();
    evt[ETW_FIELD_ENDTTIME] = utils::formatUtcTimestampMsAsISO8601(tsMs);
    evt[ETW_FIELD_LOG_SEVERITY_TEXT] =
        opentelemetry::logs::SeverityNumToText[static_cast<int>(severity)];
    evt[ETW_FIELD_LOG_SEVERITY_NUM] = severity;
    evt[ETW_FIELD_LOG_BODY]         = std::string(body.data(), body.length());
    etwProvider().write(provHandle, evt, nullptr, nullptr, 0, encoding);
  }

  /**
   * @brief Force flush data to Tracer, spending up to given amount of microseconds to flush.
   * NOTE: this method has no effect for the realtime streaming Tracer.
   *
   * @param timeout Allow Tracer to drop data if timeout is reached
   * @return
   */
  void ForceFlushWithMicroseconds(uint64_t) noexcept override{};

  /**
   * @brief Close tracer, spending up to given amount of microseconds to flush and close.
   * NOTE: This method decrements the reference count on current ETW Provider Handle and
   * closes it if reference count on that provider handle is zero.
   *
   * @param  timeout Allow Tracer to drop data if timeout is reached.
   * @return
   */

  void CloseWithMicroseconds(uint64_t) noexcept override
  {
    // Close once only
    if (!isClosed_.exchange(true))
    {
      etwProvider().close(provHandle);
    }
  }