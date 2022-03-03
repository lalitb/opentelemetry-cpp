// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#ifndef ENABLE_METRICS_PREVIEW
#  include "opentelemetry/sdk/metrics/metric_reader.h"
#  include "opentelemetry/sdk/metrics/export/metric_producer.h"

OPENTELEMETRY_BEGIN_NAMESPACE
namespace sdk
{
namespace metrics
{

MetricReader::MetricReader(AggregationTemporarily aggregation_temporarily)
    : aggregation_temporarily_(aggregation_temporarily)
{}

void MetricReader::SetMetricProducer(MetricProducer *metric_producer)
{
  metric_producer_ = metric_producer;
}

AggregationTemporarily MetricReader::GetAggregationTemporarily() const noexcept
{
  return aggregation_temporarily_;
}

bool MetricReader::Collect(nostd::function_ref<bool(MetricData)> callback) noexcept
{
  if (!metric_producer_)
  {
    OTEL_INTERNAL_LOG_WARN(
        "MetricReader::Collect Cannot invoke Collect(). No MetricProducer registered for "
        "collection!")
  }
  if (shutdown_)
  {
    OTEL_INTERNAL_LOG_WARN("MetricReader::Collect Cannot invoke Collect(). Shutdown in progress!");
  }

  return metric_producer_->Collect(callback);
}

bool MetricReader::Shutdown() noexcept
{
  bool status = true;
  if (shutdown_)
  {
    OTEL_INTERNAL_LOG_WARN("MetricReader::Shutdown Cannot invoke shutown twice!");
  }
  if (!OnShutDown())
  {
    status = false;
    OTEL_INTERNAL_LOG_ERROR("MetricReader::OnShutDown Shutdown failed. Will not be tried again!");
  }
  shutdown_ = true;
  return status;
}

/** Flush metric read by this reader **/
bool MetricReader::ForceFlush(std::chrono::microseconds timeout) noexcept
{
  bool status = true;
  if (shutdown_)
  {
    OTEL_INTERNAL_LOG_WARN("MetricReader::Shutdown Cannot invoke Force flush on shutdown reader!");
  }
  if (!OnForceFlush())
  {
    status = false;
    OTEL_INTERNAL_LOG_ERROR("MetricReader::OnForceFlush failed!");
  }
  return status;
}
}  // namespace metrics
}  // namespace sdk
OPENTELEMETRY_END_NAMESPACE
#endif