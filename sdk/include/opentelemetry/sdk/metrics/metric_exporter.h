// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "opentelemetry/sdk/metrics/recordable.h"
#include "opentelemetry/sdk/resource/resource.h"
#include "opentelemetry/version.h"

OPENTELEMETRY_BEGIN_NAMESPACE
namespace sdk
{
namespace metrics
{

/**
 * MetricExporter defines the interface to be used by metrics libraries to
 *  push export metrics to the OpenTelemetry exporters.
 */

class MetricExporter
{
public:
  virtual ~MetricExorter() = default;

  /**
   * Exports a batch of metrics recordables. This method must not be called
   * concurrently for the same exporter instance.
   * @param spans a span of unique pointers to metrics recordables
   */
  virtual sdk::common::ExportResult Export(
      const nostd::span<std::unique_ptr<opentelemetry::sdk::metrics::Recordable>>
          &spans) noexcept = 0;

  /**
   * Force flush the meter provider.
   */
  bool ForceFlush(std::chrono::microseconds timeout = (std::chrono::microseconds::max)()) noexcept;

  /**
   * Shut down the metric reader.
   * @param timeout an optional timeout.
   * @return return the status of the operation.
   */
  virtual bool Shutdown(
      std::chrono::microseconds timeout = std::chrono::microseconds::max()) noexcept = 0;
}
}  // namespace metrics
OPENTELEMETRY_END_NAMESPACE