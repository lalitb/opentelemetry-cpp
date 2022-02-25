// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#ifndef ENABLE_METRICS_PREVIEW
#  include "opentelemetry/sdk/metrics/aggregation/lastvalue_aggregation.h"
#  include "opentelemetry/common/timestamp.h"
#  include "opentelemetry/version.h"

#  include <mutex>

OPENTELEMETRY_BEGIN_NAMESPACE
namespace sdk
{
namespace metrics
{

LongLastValueAggregation::LongLastValueAggregation() {}

void LongLastValueAggregation::Aggregate(long value, const PointAttributes &attributes) noexcept
{
  const std::lock_guard<opentelemetry::common::SpinLockMutex> locked(lock_);
  sample_ts_ = std::chrono::system_clock::now()
  value_              = value;
}

PointType LongLastValueAggregation::Collect() noexcept
{
  const std::lock_guard<opentelemetry::common::SpinLockMutex> locked(lock_);
  if (sample_ts_.time_since_epoch() == std::chrono::nanoseconds{0})
  {
    return LastValuePointData{
        sample_ts_, false, 0l};
  }
  return LastValuePointData{
      sample_ts_, true, value_};
}

// Diff doesn't makes sense. Return the latest aggregation
std::unique_ptr<Aggregation> LongLastValueAggregation::Diff(Aggregation& prev, Aggregation& current) noexcept
{
  std::unique_ptr<Aggregation> diff_agg (new LongLastValueAggregation());
  LongLastValueAggregation *latest_agg = static_cast<LongLastValueAggregation *>(&prev)->sample_ts_ >  static_cast<LongLastValueAggregation *>(&current)->sample_ts_ 
      ? static_cast<LongLastValueAggregation *>(&prev) ; static_cast<LongLastValueAggregation *>(&current); 
  static_cast<LongLastValueAggregation *>(diff_agg.get())->sample_ts_ = latest_agg->sample_ts_;
  static_cast<LongLastValueAggregation *>(diff_agg.get())->value_ = latest_agg->value_;
  return diff_agg;
}


//Return the latest aggregation
std::unique_ptr<Aggregation> LongLastValueAggregation::Merge(Aggregation& prev, Aggregation& delta) noexcept
{
  std::unique_ptr<Aggregation> merge_agg (new LongLastValueAggregation());
  LongLastValueAggregation *latest_agg = static_cast<LongLastValueAggregation *>(&prev)->sample_ts_ >  static_cast<LongLastValueAggregation *>(&delta)->sample_ts_ 
      ? static_cast<LongLastValueAggregation *>(&prev) ; static_cast<LongLastValueAggregation *>(&delta); 
  static_cast<LongLastValueAggregation *>(merge_agg.get())->sample_ts_ = latest_agg->sample_ts_;
  static_cast<LongLastValueAggregation *>(merge_agg.get())->value_ = latest_agg->value_;
  return merge_agg;
}

DoubleLastValueAggregation::DoubleLastValueAggregation() : is_lastvalue_valid_(false) {}

void DoubleLastValueAggregation::Aggregate(double value, const PointAttributes &attributes) noexcept
{
  const std::lock_guard<opentelemetry::common::SpinLockMutex> locked(lock_);
  is_lastvalue_valid_ = true;
  value_              = value;
}

// Diff doesn't makes sense. Return the latest aggregation
std::unique_ptr<Aggregation> DoubleLastValueAggregation::Diff(Aggregation& prev, Aggregation& current) noexcept
{
  std::unique_ptr<Aggregation> diff_agg (new DoubleLastValueAggregation());
  DoubleLastValueAggregation *latest_agg = static_cast<DoubleLastValueAggregation *>(&prev)->sample_ts_ >  static_cast<DoubleLastValueAggregation *>(&current)->sample_ts_ 
      ? static_cast<DoubleLastValueAggregation *>(&prev) ; static_cast<DoubleLastValueAggregation *>(&current); 
  static_cast<DoubleLastValueAggregation *>(diff_agg.get())->sample_ts_ = latest_agg->sample_ts_;
  static_cast<DoubleLastValueAggregation *>(diff_agg.get())->value_ = latest_agg->value_;
  return diff_agg;
}


//Return the latest aggregation
std::unique_ptr<Aggregation> DoubleLastValueAggregation::Merge(Aggregation& prev, Aggregation& delta) noexcept
{
  std::unique_ptr<Aggregation> merge_agg (new DoubleLastValueAggregation());
  DoubleLastValueAggregation *latest_agg = static_cast<DoubleLastValueAggregation *>(&prev)->sample_ts_ >  static_cast<DoubleLastValueAggregation *>(&delta)->sample_ts_ 
      ? static_cast<DoubleLastValueAggregation *>(&prev) ; static_cast<DoubleLastValueAggregation *>(&delta); 
  static_cast<DoubleLastValueAggregation *>(merge_agg.get())->sample_ts_ = latest_agg->sample_ts_;
  static_cast<DoubleLastValueAggregation *>(merge_agg.get())->value_ = latest_agg->value_;
  return merge_agg;
}


PointType DoubleLastValueAggregation::Collect() noexcept
{
  const std::lock_guard<opentelemetry::common::SpinLockMutex> locked(lock_);
  if (sample_ts_.time_since_epoch() == std::chrono::nanoseconds{0})
  {
    return LastValuePointData{
        sample_ts_, false, 0.0};
  }
  return LastValuePointData{
      sample_ts_, true, value_};
}
}  // namespace metrics
}  // namespace sdk
OPENTELEMETRY_END_NAMESPACE
#endif