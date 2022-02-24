// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef ENABLE_METRICS_PREVIEW
#  include "opentelemetry/common/key_value_iterable_view.h"
#  include "opentelemetry/sdk/common/attributemap_hash.h"
#  include "opentelemetry/sdk/instrumentationlibrary/instrumentation_library.h"
#  include "opentelemetry/sdk/metrics/aggregation/default_aggregation.h"
#  include "opentelemetry/sdk/metrics/state/attributes_hashmap.h"
#  include "opentelemetry/sdk/metrics/state/metric_storage.h"
#  include "opentelemetry/sdk/metrics/view/attributes_processor.h"
#  include "opentelemetry/sdk/metrics/view/view.h"
#  include "opentelemetry/sdk/resource/resource.h"

#  include <memory>

OPENTELEMETRY_BEGIN_NAMESPACE
namespace sdk
{
namespace metrics
{

template <class T>
class ASyncMetricStorage : public MetricStorage, public WritableMetricStorage
{
public:
  AsyncMetricStorage(InstrumentDescriptor instrument_descriptor,
                    const AggregationType aggregation_type,
                    void (*measurement_collection_callback)(ObserverResult<T> &),
                    AttributesProcessor *attributes_processor = new DefaultAttributesProcessor())
      : instrument_descriptor_(instrument_descriptor),
        aggregation_type_{aggregation_type},
        callback_{callback},
        attributes_hashmap_(new AttributesHashMap()),
        attributes_processor_{attributes_processor}
  {
  }

  bool Collect(
      MetricCollector *collector,
      nostd::span<MetricCollector *> collectors,
      opentelemetry::sdk::instrumentationlibrary::InstrumentationLibrary *instrumentation_library,
      opentelemetry::sdk::resource::Resource *resource,
      nostd::function_ref<bool(MetricData)> metric_collection_callback) noexcept override
  {
    ObserverResult<T> ob_res(attributes_processor);
    measurement_collection_callback_(ob_res);
    ob_res.data_->GetAllEnteries([](const MetricAttributes &attributes, T value) {
      auto agg = DefaultAggregation::CreateAggregation(aggregation_type_, instrument_descriptor_);

      if (cumulative_attributes_hashmap_.Has(attributes) {
        auto previous = cumulative_attributes_hashmap_.Get(attributes);
        if (aggregation_type_ == AggregationType::kSum){
          if (instrument_descriptor_.value_type_ == InstrumentValueType::kLong) {
              agg->RecordLong(value - previous->);
          }
          else 
           {
              agg->RecordDouble(value);
           }
            agg->diff(previous, );
            })

      return true;


    });
    for (auto &m : ob_res.data_) {
      auto agg = DefaultAggregation::CreateAggregation(aggregation_type_, instrument_descriptor_);
      size_t count = 0;
      hash_map.GetAllEnteries([&count](const MetricAttributes &attributes, Aggregation &aggregation) {
    count++;
    return true;
  });
      if (instrument_descriptor_.value_type_ == InstrumentValueType::kLong) {
        agg->RecordLong(m.second, )
      }
    
    if (callback(MetricData()))
    {
      return true;
    }
    return false;
  }

private:
  void (*measurement_collection_callback_)(ObserverResult<T> &);
  std::unique_ptr<AttributesHashMap<Aggregation>> active_attributes_hashmap_;
  std::unique_ptr<AttributesHashMap<Aggregation>> cumulative_attributes_hashmap_;
  InstrumentDescriptor instrument_descriptor_;
  AggregationType aggregation_type_;
  AttributesProcessor *attributes_processor_;  
};

}  // namespace metrics
}  // namespace sdk
OPENTELEMETRY_END_NAMESPACE
#endif