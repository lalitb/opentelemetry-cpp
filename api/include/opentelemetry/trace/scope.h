// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "opentelemetry/context/runtime_context.h"
#include "opentelemetry/trace/span.h"
#include "opentelemetry/version.h"

OPENTELEMETRY_BEGIN_NAMESPACE
namespace trace
{
/**
 * Controls how long a span is active.
 *
 * On creation of the Scope object, the given span is set to the currently
 * active span. On destruction, the given span is ended and the previously
 * active span will be the currently active span again.
 */

class Scope final
{
public:
  /**
   * Initialize a new scope.
   * @param span the given span will be set as the currently active span.
   */
  Scope(const nostd::unique_ptr<Span> &span) noexcept
      : token_(context::RuntimeContext::Attach(
            context::RuntimeContext::GetCurrent().SetValue(kSpanKey, span.get()))),
        span_(span.get()),
        isCleanupDone(false)
  {
    span_->SetScope(this);
  }

  ~Scope()
  {
    if (!isCleanupDone)
    {
      span_->ClearScope();
    }
    isCleanupDone = true;
  }
  Scope(const Scope &) = default;
  Scope(Scope &&)      = default;

private:
  nostd::unique_ptr<context::Token> token_;
  const Span *span_;
  bool isCleanupDone;
};

// Span desctructor
// remove Span from currently active destructor

inline trace::Span::~Span()
{
  if (scope_)
  {
    delete scope_;
  }
}

}  // namespace trace
OPENTELEMETRY_END_NAMESPACE
