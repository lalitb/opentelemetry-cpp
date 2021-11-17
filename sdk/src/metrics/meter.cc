// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include "opentelemetry/sdk/metrics/meter.h"
#include "opentelemetry/nostd/shared_ptr.h"

#include <memory>

OPENTELEMETRY_BEGIN_NAMESPACE

namespace metrics_api = opentelemetry::metrics;
namespace sdk
{
namespace metrics
{

Meter::Meter(opentelemetry::sdk::resource::Resource &resource,
                  std::unique_ptr<InstrumentationLibrary> instrumentation_library) noexcept
  : instrumentation_library_{std::move(instrumentation_library)}, context_{context}
{
}

nostd::shared_ptr<metrics_api::Counter<long>> Meter::CreateLongCounter(
    nostd::string_view name,
    nostd::string_view description,
    nostd::string_view unit) noexcept
{}

nostd::shared_ptr<metrics_api::Counter<double>> Meter::CreateDoubleCounter(
    nostd::string_view name,
    nostd::string_view description,
    nostd::string_view unit) noexcept
{}

  nostd::shared_ptr<metrics_api::ObservableCounter<long>> Meter::CreateLongObservableCounter(
    nostd::string_view name,
    void (*callback)(metrics_api::ObserverResult<long> &),
    nostd::string_view description,
    nostd::string_view unit) noexcept
    {}

  nostd::shared_ptr<metrics_api::ObservableCounter<double>> Meter::CreateDoubleObservableCounter(
    nostd::string_view name,
    void (*callback)(metrics_api::ObserverResult<double> &),
    nostd::string_view description,
    nostd::string_view unit) noexcept
    {}

  nostd::shared_ptr<metrics_api::Histogram<long>> Meter::CreateLongHistogram(
    nostd::string_view name,
    nostd::string_view description = "",
    nostd::string_view unit        = "1") noexcept
    {}

  nostd::shared_ptr<metrics_api::Histogram<double>> Meter::CreateDoubleHistogram(
    nostd::string_view name,
    nostd::string_view description = "",
    nostd::string_view unit        = "1") noexcept
    {}

  nostd::shared_ptr<ObservableGauge<long>> Meter::CreateLongObservableGauge(
      nostd::string_view name,
      void (*callback)(metrics_api::ObserverResult<long> &),
      nostd::string_view description = "",
      nostd::string_view unit        = "1") noexcept
      {}

  nostd::shared_ptr<metrics_api::ObservableGauge<double>> Meter::CreateDoubleObservableGauge(
      nostd::string_view name,
      void (*callback)(metrics_api::ObserverResult<double> &),
      nostd::string_view description = "",
      nostd::string_view unit        = "1") noexcept
      {}

  nostd::shared_ptr<metrics_api::UpDownCounter<long>> Meter::CreateLongUpDownCounter(
      nostd::string_view name,
      nostd::string_view description = "",
      nostd::string_view unit        = "1") noexcept
      {}

  nostd::shared_ptr<metrics_api::UpDownCounter<double>> Meter::CreateDoubleUpDownCounter(
      nostd::string_view name,
      nostd::string_view description = "",
      nostd::string_view unit        = "1") noexcept
      {}

  nostd::shared_ptr<metrics_api::ObservableUpDownCounter<long>> Meter::CreateLongObservableUpDownCounter(
      nostd::string_view name,
      void (*callback)(metrics_api::ObserverResult<long> &),
      nostd::string_view description = "",
      nostd::string_view unit        = "1") noexcept
    {}

  nostd::shared_ptr<ObservableUpDownCounter<double>> Meter::CreateDoubleObservableUpDownCounter(
      nostd::string_view name,
      void (*callback)(metrics_api::ObserverResult<double> &),
      nostd::string_view description = "",
      nostd::string_view unit        = "1") noexcept
      {}

