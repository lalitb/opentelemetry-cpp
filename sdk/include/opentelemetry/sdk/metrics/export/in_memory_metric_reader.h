// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef ENABLE_METRICS_PREVIEW

#  include "opentelemetry/common/spin_lock_mutex.h"
#  include "opentelemetry/sdk/metrics/metric_reader.h"
#  include "opentelemetry/version.h"

#  include <mutex>

OPENTELEMETRY_BEGIN_NAMESPACE
namespace sdk
{
namespace metrics
{

class InMemoryMetricReader : public MetricReader
{

public:
  InMemoryMetricReader(
      AggregationTemporality aggregation_temporality = AggregationTemporality::kCumulative)
      : MetricReader(aggregation_temporality)
  {}

  ResourceMetrics GetMetricData()
  {
    std::lock_guard<opentelemetry::common::SpinLockMutex> guard(GetLock());
    ResourceMetrics metric_data_p;
    auto status = Collect([&metric_data_p](ResourceMetrics &metric_data) {
      metric_data_p = std::move(metric_data);
      return true;
    });
    return metric_data_p;
  }

private:
  bool OnForceFlush(std::chrono::microseconds timeout) noexcept override { return true; }

  bool OnShutDown(std::chrono::microseconds timeout) noexcept override { return true; }

  static opentelemetry::common::SpinLockMutex &GetLock() noexcept
  {
    static opentelemetry::common::SpinLockMutex lock;
    return lock;
  }
};

}  // namespace metrics
}  // namespace sdk
OPENTELEMETRY_END_NAMESPACE
#endif
