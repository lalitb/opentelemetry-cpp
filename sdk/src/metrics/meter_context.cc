// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include "opentelemetry/sdk/metrics/meter_context.h"

OPENTELEMETRY_BEGIN_NAMESPACE
namespace sdk
{
namespace metrics
{

MeterContext::MeterContext(opentelemetry::sdk::resource::Resource resource) noexcept
    : resource_(resource)
{}

const opentelemetry::sdk::resource::Resource &MeterContext::GetResource() const noexcept
{
  return resource_;
}

bool MeterContext::ForceFlush(std::chrono::microseconds timeout) noexcept
{
  return processor_->ForceFlush(timeout);
}

bool MeterContext::Shutdown() noexcept
{
  return processor_->Shutdown();
}

}  // namespace metrics
}  // namespace sdk
OPENTELEMETRY_END_NAMESPACE
