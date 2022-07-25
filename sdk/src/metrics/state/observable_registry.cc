// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#ifndef ENABLE_METRICS_PREVIEW

#  include "opentelemetry/sdk/metrics/state/observable_registry.h"
#  include "opentelemetry/sdk/metrics/async_instruments.h"
#  include "opentelemetry/sdk/metrics/observer_result.h"
#  include "opentelemetry/sdk/metrics/state/metric_storage.h"

OPENTELEMETRY_BEGIN_NAMESPACE
namespace sdk
{
namespace metrics
{

void ObservableRegistry::AddCallback(opentelemetry::metrics::ObservableCallbackPtr callback,
                                     void *state,
                                     opentelemetry::metrics::ObservableInstrument *instrument)
{
  // TBD - Check if existing
  std::unique_ptr<ObservableCallbackRecord> record(
      new ObservableCallbackRecord{callback, state, instrument});
  std::unique_lock<std::mutex> lk(callbacks_m_);
  callbacks_.push_back(std::move(record));
}

void ObservableRegistry::RemoveCallback(opentelemetry::metrics::ObservableCallbackPtr callback,
                                        void *state,
                                        opentelemetry::metrics::ObservableInstrument *instrument)
{
  auto new_end = std::remove_if(
      callbacks_.begin(), callbacks_.end(),
      [callback, state, instrument](const std::unique_ptr<ObservableCallbackRecord> &record) {
        return record->callback == callback && record->state == state &&
               record->instrument == instrument;
      });
  std::unique_lock<std::mutex> lk(callbacks_m_);
  callbacks_.erase(new_end, callbacks_.end());
}

void ObservableRegistry::Observe(opentelemetry::common::SystemTimestamp collection_ts)
{
  std::unique_lock<std::mutex> lk(callbacks_m_);
  for (auto &callback_wrap : callbacks_)
  {
    auto value_type =
        static_cast<opentelemetry::sdk::metrics::ObservableInstrument *>(callback_wrap->instrument)
            ->GetInstrumentDescriptor()
            .value_type_;
    auto storage =
        static_cast<opentelemetry::sdk::metrics::ObservableInstrument *>(callback_wrap->instrument)
            ->GetMetricStorage();
    if (value_type == InstrumentValueType::kDouble)
    {
      nostd::shared_ptr<opentelemetry::metrics::ObserverResultT<double>> ob_res(
          new opentelemetry::sdk::metrics::ObserverResultT<double>());
      callback_wrap->callback(ob_res, callback_wrap->state);
      storage->RecordDouble(
          static_cast<opentelemetry::sdk::metrics::ObserverResultT<double> *>(ob_res.get())
              ->GetMeasurements(),
          collection_ts);
    }
    else
    {
      nostd::shared_ptr<opentelemetry::metrics::ObserverResultT<long>> ob_res(
          new opentelemetry::sdk::metrics::ObserverResultT<long>());
      callback_wrap->callback(ob_res, callback_wrap->state);
      storage->RecordLong(
          static_cast<opentelemetry::sdk::metrics::ObserverResultT<long> *>(ob_res.get())
              ->GetMeasurements(),
          collection_ts);
    }
  }
}

}  // namespace metrics
}  // namespace sdk
OPENTELEMETRY_END_NAMESPACE
#endif