// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef ENABLE_METRICS_PREVIEW
#  include <chrono>
#  include "opentelemetry/sdk/common/global_log_handler.h"
#  include "opentelemetry/sdk/metrics/data/metric_data.h"
#  include "opentelemetry/sdk/metrics/instruments.h"
#  include "opentelemetry/version.h"

#  include <memory>

OPENTELEMETRY_BEGIN_NAMESPACE
namespace sdk
{
namespace metrics
{

class MetricProducer;
/**
 * MetricReader defines the interface to collect metrics from SDK
 */
class MetricReader
{
public:
  MetricReader(
      AggregationTemporarily aggregation_temporarily = AggregationTemporarily::kCummulative);

  void SetMetricProducer(std::shared_ptr<MetricProducer> &&metric_producer);

  /**
   * Collect the metrics from SDK.
   * @return return the status of the operation.
   */
  bool Collect(nostd::function_ref<bool(MetricData)> callback) noexcept;

  AggregationTemporarily GetAggregationTemporarily() const noexcept;

  /**
   * Shutdown the meter reader.
   */
  bool Shutdown() noexcept;

  /**
   * Force flush the metric read by the reader.
   */
  bool ForceFlush(std::chrono::microseconds timeout = (std::chrono::microseconds::max)()) noexcept;

  virtual bool OnForceFlush() noexcept = 0;

  virtual bool OnShutDown() noexcept = 0;

  virtual void OnInitialized() noexcept {}

private:
  std::shared_ptr<MetricProducer> metric_producer_;
  AggregationTemporarily aggregation_temporarily_;
  bool shutdown_;
};
}  // namespace metrics
}  // namespace sdk
OPENTELEMETRY_END_NAMESPACE
#endif
