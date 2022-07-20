// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#ifndef ENABLE_METRICS_PREVIEW
#  include "opentelemetry/sdk/metrics/async_instruments.h"
#  include "opentelemetry/sdk/metrics/state/metric_storage.h"
#  include "opentelemetry/sdk_config.h"

#  include <cmath>

OPENTELEMETRY_BEGIN_NAMESPACE
namespace sdk
{
namespace metrics
{

ObservableInstrument::ObservableInstrument(InstrumentDescriptor instrument_descriptor,
                                           std::unique_ptr<WritableMetricStorage> storage)
    : instrument_descriptor_(instrument_descriptor),
      storage_(std::move(storage)),
      observable_registry_{new ObservableRegistry()},
      Asynchronous(instrument_descriptor_.name_,
                   instrument_descriptor.description_,
                   instrument_descriptor_.unit_)
{}

void ObservableInstrument::AddCallback(opentelemetry::metrics::ObservableCallbackPtr callback,
                                       void *state) noexcept
{
  observable_registry_->AddCallback(callback, state, this);
}

void ObservableInstrument::RemoveCallback(opentelemetry::metrics::ObservableCallbackPtr callback,
                                          void *state) noexcept
{
  observable_registry_->AddCallback(callback, state, this);
}

const InstrumentDescriptor &ObservableInstrument::GetInstrumentDescriptor()
{
  return instrument_descriptor_;
}

const WritableMetricStorage *ObservableInstrument::GetMetricStorage()
{
  return storage_.get();
}

}  // namespace metrics
}  // namespace sdk
OPENTELEMETRY_END_NAMESPACE
#endif