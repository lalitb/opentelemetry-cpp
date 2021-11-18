// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "opentelemetry/common/attribute_value.h"
#include "opentelemetry/common/key_value_iterable.h"
#include "opentelemetry/nostd/string_view.h"
#include "opentelemetry/sdk/common/empty_attributes.h"
#include "opentelemetry/sdk/instrumentationlibrary/instrumentation_library.h"
#include "opentelemetry/sdk/resource/resource.h"
#include "opentelemetry/version.h"

OPENTELEMETRY_BEGIN_NAMESPACE
namespace sdk
{
namespace metrics
{

/**
 * Maintains a representation of a metrics measurement in a format that can be processed by a
 * recorder.
 *
 * This class is thread-compatible.
 */

class Recordable
{};
}  // namespace metrics
}  // namespace sdk
OPENTELEMETRY_END_NAMESPACE
