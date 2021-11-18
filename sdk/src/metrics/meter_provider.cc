/ Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include "opentelemetry/sdk/metrics/meter_provider.h"
#include "opentelemetry/sdk_config.h"

        OPENTELEMETRY_BEGIN_NAMESPACE namespace sdk
{
  namespace metrics
  {
  MeterProvider::MeterProvider(std::shared_ptr<sdk::metrics::MeterContext> context) noexcept
      : context_{context}
  {}

  MeterProvider::MeterProvider(opentelemetry::sdk::resource::Resource resource) noexcept
  {
    context_ = std::make_shared<MeterContext>(resource);
  }

  nostd::shared_ptr<opentelemetry::metrics::Meter> MeterProvider::GetMeter(
      nostd::string_view library_name,
      nostd::string_view library_version,
      nostd::string_view schema_url) noexcept
  {
    if (library_name.data() == nullptr)
    {
      OTEL_INTERNAL_LOG_ERROR("[MeterProvider::GetMeter] Library name is null.");
      library_name = "";
    }
    else if (library_name == "")
    {
      OTEL_INTERNAL_LOG_ERROR("[MeterProvider::GetMeter] Library name is empty.");
    }

    const std::lock_guard<std::mutex> guard(lock_);

    for (auto &meter : meterss_)
    {
      auto &meter_lib = meter->GetInstrumentationLibrary();
      if (meter_lib.equal(library_name, library_version, schema_url))
      {
        return nostd::shared_ptr<opentelemetry::meter::Meter>{meter};
      }
    }

    auto lib = InstrumentationLibrary::Create(library_name, library_version, schema_url);
    meters_.push_back(std::shared_ptr<opentelemetry::sdk::metrics::Meter>(
        new sdk::metrics::Meter(context_, std::move(lib))));
    return nostd::shared_ptr<opentelemetry::metrics::Meter>{meters_.back()};
  }

  const opentelemetry::sdk::resource::Resource &MeterProvider::GetResource() const noexcept
  {
    return context_->GetResource();
  }

  bool MeterProvider::Shutdown() noexcept { return context_->Shutdown(); }

  bool MeterProvider::ForceFlush(std::chrono::microseconds timeout) noexcept
  {
    return context_->ForceFlush(timeout);
  }

  }  // namespace metrics
}  // namespace sdk
OPENTELEMETRY_END_NAMESPACE