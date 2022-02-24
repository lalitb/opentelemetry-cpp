// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef ENABLE_METRICS_PREVIEW
#include "opentelemetry/metrics/observer_result.h"
#  include "opentelemetry/sdk/metrics/state/attributes_hashmap.h"


#include<map>

OPENTELEMETRY_BEGIN_NAMESPACE
namespace sdk
{
namespace metrics
{
template <class T>
class ObserverResult final : public opentelemetry::metricsObserverResult<T>
{
 ObserverResult(AttributesProcessor attributes_processor): attributes_processor_(attributes_processor)
 {
 }

public:
  virtual void Observe(T value) noexcept override
  {
      data_->Set({}, value);
  }

  virtual void Observer(T value, const common::KeyValueIterable &attributes) noexcept override
  {
      auto attr        = attributes_processor_->process(attributes);
      data_->Set(attr, value);
  }

private:

  std::unique_ptr<AttributesHashMap<T>> data_;
  AttributesProcessor *attributes_processor;

};
} // namespace metrics
} // sdk

OPENTELEMETRY_END_NAMESPACE
#endif
