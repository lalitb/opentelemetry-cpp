// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include "opentelemetry/sdk/metrics/state/attributes_hashmap.h"

#include <string>

#include "opentelemetry/version.h"

OPENTELEMETRY_BEGIN_NAMESPACE
namespace sdk
{
namespace metrics
{

// NOLINTBEGIN(bugprone-throwing-static-initialization)
// These globals require non-trivial construction (std::string and a small
// map). Defining them in a single TU keeps the (theoretical) throwing static
// initialization isolated, instead of replicating the warning in every TU
// that includes attributes_hashmap.h.
const std::string kAttributesLimitOverflowKey = "otel.metric.overflow";
const bool kAttributesLimitOverflowValue      = true;
const MetricAttributes kOverflowAttributes    = {
    {kAttributesLimitOverflowKey, kAttributesLimitOverflowValue}};
// NOLINTEND(bugprone-throwing-static-initialization)

}  // namespace metrics
}  // namespace sdk
OPENTELEMETRY_END_NAMESPACE
