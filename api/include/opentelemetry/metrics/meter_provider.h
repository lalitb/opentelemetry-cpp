// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifdef ENABLE_METRICS_PREVIEW

#  include "opentelemetry/metrics/meter.h"
#  include "opentelemetry/nostd/shared_ptr.h"
#  include "opentelemetry/nostd/string_view.h"
#  include "opentelemetry/metrics/provider.h"

OPENTELEMETRY_BEGIN_NAMESPACE
namespace metrics
{
/**
 * Creates new Meter instances.
 */
class MeterProvider
{
public:
  virtual ~MeterProvider() = default;
  /**
   * Gets or creates a named Meter instance.
   *
   * Optionally a version can be passed to create a named and versioned Meter
   * instance.
   */
  virtual nostd::shared_ptr<Meter> GetMeter(nostd::string_view library_name,
                                            nostd::string_view library_version = "",
                                            nostd::string_view schema_url      = "") = 0;

};

class ProxyMeterProvider: public MeterProvider 
{
  nostd::shared_ptr<Meter> GetMeter(nostd::string_view library_name,
                                    nostd::string_view library_version,
                                    nostd::string_view schema_url)
  {
    return ProxyMeter(library_name, library_version, schema_url);
  }


}
}  // namespace metrics
OPENTELEMETRY_END_NAMESPACE
#endif
