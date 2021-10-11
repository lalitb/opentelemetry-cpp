// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifdef ENABLE_METRICS_PREVIEW
#  include <mutex>

#  include "opentelemetry/common/spin_lock_mutex.h"
#  include "opentelemetry/metrics/meter_provider.h"
#  include "opentelemetry/metrics/noop.h"
#  include "opentelemetry/nostd/shared_ptr.h"

OPENTELEMETRY_BEGIN_NAMESPACE
namespace metrics
{
/**
 * Stores the singleton global MeterProvider.
 */
class Provider
{
public:
  /**
   * Returns the singleton MeterProvider.
   *
   * By default, a no-op MeterProvider is returned. This will never return a
   * nullptr MeterProvider.
   */
  static nostd::shared_ptr<MeterProvider> GetMeterProvider() noexcept
  {
    std::lock_guard<common::SpinLockMutex> guard(GetLock());
    if (IsGlobalProviderSet())
      return nostd::shared_ptr<MeterProvider>(GetGlobalProvider());
    else
      return nostd::shared_ptr<MeterProvider>(GetProxyProvider());
  }

  /**
   * Changes the singleton MeterProvider.
   */
  static void SetGlobalMeterProvider(nostd::shared_ptr<MeterProvider> tp) noexcept
  {
    std::lock_guard<common::SpinLockMutex> guard(GetLock());
    GetProvider() = tp;
    IsGlobalProviderSet() = true;
  }


private:
  static nostd::shared_ptr<MeterProvider> &GetGlobalProvider() noexcept
  {
    static nostd::shared_ptr<MeterProvider> provider(new NoopMeterProvider);
    return provider;
  }

  static nostd::shared_ptr<MeterProvider> &GetProxyProvider() noexcept
  {
    static nostd::shared_ptr<MeterProvider> provider(new ProxyMeterProvider());
    return provider;
  }
  static bool &IsGlobalProviderSet() noexcept 
  {
    static bool isSet = false;
    return isSet;
  }

  static common::SpinLockMutex &GetLock() noexcept
  {
    static common::SpinLockMutex lock;
    return lock;
  }
};

}  // namespace metrics
OPENTELEMETRY_END_NAMESPACE
#endif
