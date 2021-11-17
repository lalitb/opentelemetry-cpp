// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "opentelemetry/metrics_new/meter.h"
#include "opentelemetry/sdk/instrumentationlibrary/instrumentation_library.h"
#include "opentelemetry/sdk/resource/resource.h"
#include "opentelemetry/version.h"

OPENTELEMETRY_BEGIN_NAMESPACE
namespace sdk
{
namespace metrics
{

class Meter final : public opentelemetry::metrics::Meter
{
public:
  /** Construct a new Meter with the given  pipeline. */
  explicit Meter(opentelemetry::sdk::resource::Resource &resource,
                  std::unique_ptr<InstrumentationLibrary> instrumentation_library =
                      InstrumentationLibrary::Create("")) noexcept;

  nostd::shared_ptr<opentelemetry::metrics::Counter<long>> CreateLongCounter(
      nostd::string_view name,
      nostd::string_view description = "",
      nostd::string_view unit        = "1") noexcept override;

  nostd::shared_ptr<opentelemetry::metrics::Counter<double>> CreateDoubleCounter(
      nostd::string_view name,
      nostd::string_view description = "",
      nostd::string_view unit        = "1") noexcept override;

  nostd::shared_ptr<opentelemetry::metrics::ObservableCounter<long>> CreateLongObservableCounter(
      nostd::string_view name,
      void (*callback)(opentelemetry::metrics::ObserverResult<long> &),
      nostd::string_view description = "",
      nostd::string_view unit        = "1") noexcept override;

  nostd::shared_ptr<opentelemetry::metrics::ObservableCounter<double>> CreateDoubleObservableCounter(
      nostd::string_view name,
      void (*callback)(opentelemetry::metrics::ObserverResult<double> &),
      nostd::string_view description = "",
      nostd::string_view unit        = "1") noexcept override;

  nostd::shared_ptr<opentelemetry::metrics::Histogram<long>> CreateLongHistogram(
      nostd::string_view name,
      nostd::string_view description = "",
      nostd::string_view unit        = "1") noexcept override;

  nostd::shared_ptr<opentelemetry::metrics::Histogram<double>> CreateDoubleHistogram(
      nostd::string_view name,
      nostd::string_view description = "",
      nostd::string_view unit        = "1") noexcept override;

  nostd::shared_ptr<opentelemetry::metrics::ObservableGauge<long>> CreateLongObservableGauge(
      nostd::string_view name,
      void (*callback)(opentelemetry::metrics::ObserverResult<long> &),
      nostd::string_view description = "",
      nostd::string_view unit        = "1") noexcept override;

  nostd::shared_ptr<opentelemetry::metrics::ObservableGauge<double>> CreateDoubleObservableGauge(
      nostd::string_view name,
      void (*callback)(ObserverResult<double> &),
      nostd::string_view description = "",
      nostd::string_view unit        = "1") noexcept override;

  nostd::shared_ptr<opentelemetry::metrics::UpDownCounter<long>> CreateLongUpDownCounter(
      nostd::string_view name,
      nostd::string_view description = "",
      nostd::string_view unit        = "1") noexcept override;

  nostd::shared_ptr<opentelemetry::metrics::UpDownCounter<double>> CreateDoubleUpDownCounter(
      nostd::string_view name,
      nostd::string_view description = "",
      nostd::string_view unit        = "1") noexcept override;

  nostd::shared_ptr<opentelemetry::metrics::ObservableUpDownCounter<long>> CreateLongObservableUpDownCounter(
      nostd::string_view name,
      void (*callback)(opentelemetry::metrics::ObserverResult<long> &),
      nostd::string_view description = "",
      nostd::string_view unit        = "1") noexcept override;

  nostd::shared_ptr<opentelemetry::metrics::ObservableUpDownCounter<double>> CreateDoubleObservableUpDownCounter(
      nostd::string_view name,
      void (*callback)(opentelemetry::metrics::ObserverResult<double> &),
      nostd::string_view description = "",
      nostd::string_view unit        = "1") noexcept override;

}
}  // namespace metrics
}  // namespace sdk
OPENTELEMETRY_END_NAMESPACE
