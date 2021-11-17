// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "opentelemetry/sdk/resource/resource.h"
#include "opentelemetry/version.h"

OPENTELEMETRY_BEGIN_NAMESPACE
namespace sdk
{
namespace metrics
{

/**
 * MetricReader defines the interface to collect metrics from SDK
*/

class MetricReader
{
public:
  virtual ~MetricReader() = default;

  /**
   * Collect the metrics from SDK.
   * @return return the status of the operation.
   */
  bool Collect() noexcept = 0;

  /**
   * Shut down the metric reader.
   * @param timeout an optional timeout.
   * @return return the status of the operation.
   */
  virtual bool Shutdown(
      std::chrono::microseconds timeout = std::chrono::microseconds::max()) noexcept = 0;

}
}
OPENTELEMETRY_END_NAMESPACE