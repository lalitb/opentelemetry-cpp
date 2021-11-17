// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "opentelemetry/sdk/resource/resource.h"
#include "opentelemetry/version.h"

OPENTELEMETRY_BEGIN_NAMESPACE
namespace sdk
{
namespace metrics
{

/**
 * A class which stores the MeterProvider context.

 */
class MeterContext
{
public:
  explicit MeterContext(opentelemetry::sdk::resource::Resource resource =
                            opentelemetry::sdk::resource::Resource::Create({})) noexcept;

  /**
   * Obtain the resource associated with this meter context.
   * @return The resource for this meter context.
   */
  const opentelemetry::sdk::resource::Resource &GetResource() const noexcept;

  /**
   * Force all active SpanProcessors to flush any buffered spans
   * within the given timeout.
   */
  bool ForceFlush(std::chrono::microseconds timeout = (std::chrono::microseconds::max)()) noexcept;

  /**
   * Shutdown the span processor associated with this meter provider.
   */
  bool Shutdown() noexcept;

private:
  opentelemetry::sdk::resource::Resource resource_;
  std::vector<std::unique_ptr<MeterExporter>> exporters_;
  std::unique_ptr<std::unique_ptr<MeterReader>> readers_;
  std::unique_ptr<std::unique_ptr<View>> views_;
};

}  // namespace metrics
}  // namespace sdk
OPENTELEMETRY_END_NAMESPACE
