// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef ENABLE_METRICS_PREVIEW
#  include "opentelemetry/sdk/metrics/data/metric_data.h"
#  include "opentelemetry/sdk/metrics/export/metric_producer.h"

OPENTELEMETRY_BEGIN_NAMESPACE
namespace sdk
{
namespace metrics
{

class MetricReader;
class MeterContext;

class CollectorHandle
{
public:
  virtual AggregationTemporality GetAggregationTemporality() noexcept = 0;
};

/**
 * An internal opaque interface that the MetricReader receives as
 * MetricProducer. It acts as the storage key to the internal metric stream
 * state for each MetricReader.
 */

class MetricCollector : public MetricProducer, public CollectorHandle
{
public:
  MetricCollector(std::shared_ptr<MeterContext> &&context,
                  std::unique_ptr<MetricReader> metric_reader);

  AggregationTemporality GetAggregationTemporality() noexcept override;

  /**
   * The callback to be called for each metric exporter. This will only be those
   * metrics that have been produced since the last time this method was called.
   *
   * @return a status of completion of method.
   */
  bool Collect(nostd::function_ref<bool(MetricData)> callback) noexcept override;

  bool ForceFlush(std::chrono::microseconds timeout = std::chrono::microseconds::max()) noexcept;
  bool Shutdown(std::chrono::microseconds timeout = std::chrono::microseconds::max()) noexcept;

private:
  std::shared_ptr<MeterContext> meter_context_;
  std::shared_ptr<MetricReader> metric_reader_;
};
}  // namespace metrics
}  // namespace sdk
OPENTELEMETRY_END_NAMESPACE
#endif
