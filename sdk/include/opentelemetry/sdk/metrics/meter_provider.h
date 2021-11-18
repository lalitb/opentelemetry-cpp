// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <memory>
#include <vector>

#include "opentelemetry/metrics/meter_provider.h"
#include "opentelemetry/nostd/shared_ptr.h"
#include "opentelemetry/sdk/metrics/meter.h"
#include "opentelemetry/sdk/metrics/meter_context.h"
#include "opentelemetry/sdk/resource/resource.h"

OPENTELEMETRY_BEGIN_NAMESPACE
namespace sdk
{
namespace metrics
{
class MeterProvider final : public opentelemetry::metrics::MeterProvider
{
public:
  /**
   * Initialize a new meter provider
   * @param resource  The resources for this meter provider.
   */
  explicit MeterProvider(opentelemetry::sdk::resource::Resource resource =
                             opentelemetry::sdk::resource::Resource::Create({})) noexcept;

  /**
   * Initialize a new meter provider with a specified context
   * @param context The shared meter configuration/pipeline for this provider.
   */
  explicit MeterProvider(std::shared_ptr<sdk::metrics::MeterContext> context) noexcept;

  nostd::shared_ptr<Meter> GetMeter(nostd::string_view library_name,
                                    nostd::string_view library_version = "",
                                    nostd::string_view schema_url      = "") noexcept override;

  /**
   * Obtain the resource associated with this meter provider.
   * @return The resource for this meter provider.
   */
  const opentelemetry::sdk::resource::Resource &GetResource() const noexcept;

  /**
   * Shutdown the meter provider.
   */
  bool Shutdown() noexcept;

  /**
   * Force flush the meter provider.
   */
  bool ForceFlush(std::chrono::microseconds timeout = (std::chrono::microseconds::max)()) noexcept;

private:
  // // order of declaration is important here - meter should destroy only after resource.
  std::vector<std::shared_ptr<opentelemetry::sdk::metrics::Meter>> meters_;
  std::shared_ptr<opentelemetry::sdk::metrics::MeterContext> context_;
};
}  // namespace metrics
OPENTELEMETRY_END_NAMESPACE
