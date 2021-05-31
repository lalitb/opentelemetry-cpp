// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "opentelemetry/detail/preprocessor.h"

#define OPENTELEMETRY_SDK_ABI_VERSION_NO 0
#define OPENTELEMETRY_SDK_VERSION "0.7.0"
#define OPENTELEMETRY_SDK_ABI_VERSION OPENTELEMETRY_STRINGIFY(OPENTELEMETRY_SDK_ABI_VERSION_NO)

// clang-format off
#define OPENTELEMETRY_SDK_BEGIN_NAMESPACE \
  namespace opentelemetry { namespace sdk { inline namespace OPENTELEMETRY_CONCAT(v, OPENTELEMETRY_SDK_ABI_VERSION_NO) {

#define OPENTELEMETRY_SDK_END_NAMESPACE \
  }}}

#define OPENTELEMETRY_SDK_NAMESPACE opentelemetry :: sdk :: OPENTELEMETRY_CONCAT(v, OPENTELEMETRY_SDK_ABI_VERSION_NO)

// clang-format on
