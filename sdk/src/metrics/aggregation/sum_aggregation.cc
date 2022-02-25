// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#ifndef ENABLE_METRICS_PREVIEW
#  include "opentelemetry/sdk/metrics/aggregation/sum_aggregation.h"
#  include "opentelemetry/version.h"

#  include <mutex>

OPENTELEMETRY_BEGIN_NAMESPACE
namespace sdk
{
namespace metrics
{

LongSumAggregation::LongSumAggregation(bool is_monotonic)
    : InstrumentMonotonicityAwareAggregation(is_monotonic),
      start_epoch_nanos_(opentelemetry::common::SystemTimestamp(std::chrono::system_clock::now())),
      sum_(0l)
{}

void LongSumAggregation::Aggregate(long value, const PointAttributes &attributes) noexcept
{
  const std::lock_guard<opentelemetry::common::SpinLockMutex> locked(lock_);
  sum_ += value;
}

PointType LongSumAggregation::Collect() noexcept
{
  opentelemetry::common::SystemTimestamp current_ts(std::chrono::system_clock::now());
  SumPointData sum;
  {
    const std::lock_guard<opentelemetry::common::SpinLockMutex> locked(lock_);
    PopulateSumPointData<long>(sum, start_epoch_nanos_, current_ts, sum_, IsMonotonic());
    start_epoch_nanos_ = current_ts;
    sum_               = 0;
  }
  return sum;
}

std::unique_ptr<Aggregation> LongSumAggregation::Diff(Aggregation& prev, Aggregation& current) noexcept
{
  std::unique_ptr<Aggregation> diff_agg(new LongSumAggregation());
  diff_agg->Aggregate(current.sum_ - prev.sum_);

}
  
std::unique_ptr<Aggregation> LongSumAggregation::Merge(Aggregation &prev, Aggregation& delta) noexcept
{
  std::unique_ptr<Aggregation> merge_agg(new LongSumAggregation());
  merge_agg->Aggregate(current.sum_ - prev.sum_);
}

DoubleSumAggregation::DoubleSumAggregation(bool is_monotonic)
    : InstrumentMonotonicityAwareAggregation(is_monotonic),
      start_epoch_nanos_(opentelemetry::common::SystemTimestamp(std::chrono::system_clock::now())),
      sum_(0L)
{}

void DoubleSumAggregation::Aggregate(double value, const PointAttributes &attributes) noexcept
{
  const std::lock_guard<opentelemetry::common::SpinLockMutex> locked(lock_);
  sum_ += value;
}

PointType DoubleSumAggregation::Collect() noexcept
{
  opentelemetry::common::SystemTimestamp current_ts(std::chrono::system_clock::now());
  SumPointData sum;
  {
    const std::lock_guard<opentelemetry::common::SpinLockMutex> locked(lock_);
    PopulateSumPointData<double>(sum, start_epoch_nanos_, current_ts, sum_, IsMonotonic());
    start_epoch_nanos_ = current_ts;
    sum_               = 0;
  }
  return sum;
}
std::unique_ptr<Aggregation> DoubleSumAggregation::Diff(Aggregation& prev, Aggregation& current) noexcept
{
  std::unique_ptr<Aggregation> diff_agg(new DoubleSumAggregation());
  diff_agg->Aggregate(current.sum_ - prev.sum_);

}
  
std::unique_ptr<Aggregation> DoubleSumAggregation::Merge(Aggregation &prev, Aggregation& delta) noexcept
{
  std::unique_ptr<Aggregation> merge_agg(new DoubleSumAggregation());
  merge_agg->Aggregate(current.sum_ - prev.sum_);
}

}  // namespace metrics
}  // namespace sdk
OPENTELEMETRY_END_NAMESPACE
#endif