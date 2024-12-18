#include "stress.h"  // Project-specific header

#include <chrono>
#include <csignal>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "opentelemetry/common/attribute_value.h"
#include "opentelemetry/metrics/meter_provider.h"
#include "opentelemetry/metrics/provider.h"
#include "opentelemetry/nostd/shared_ptr.h"
#include "opentelemetry/sdk/metrics/aggregation/aggregation_config.h"
#include "opentelemetry/sdk/metrics/export/periodic_exporting_metric_reader_factory.h"
#include "opentelemetry/sdk/metrics/export/periodic_exporting_metric_reader_options.h"
#include "opentelemetry/sdk/metrics/instruments.h"
#include "opentelemetry/sdk/metrics/meter_provider.h"
#include "opentelemetry/sdk/metrics/meter_provider_factory.h"
#include "opentelemetry/sdk/metrics/metric_reader.h"
#include "opentelemetry/sdk/metrics/push_metric_exporter.h"
#include "opentelemetry/sdk/metrics/state/filtered_ordered_attribute_map.h"
#include "opentelemetry/sdk/metrics/view/instrument_selector.h"
#include "opentelemetry/sdk/metrics/view/instrument_selector_factory.h"
#include "opentelemetry/sdk/metrics/view/meter_selector.h"
#include "opentelemetry/sdk/metrics/view/meter_selector_factory.h"
#include "opentelemetry/sdk/metrics/view/view.h"
#include "opentelemetry/sdk/metrics/view/view_factory.h"

namespace metrics_sdk = opentelemetry::sdk::metrics;
namespace metrics_api = opentelemetry::metrics;

namespace
{
class MockMetricExporter : public opentelemetry::sdk::metrics::PushMetricExporter
{
public:
  opentelemetry::sdk::common::ExportResult Export(
      const opentelemetry::sdk::metrics::ResourceMetrics & /*data*/) noexcept override
  {
    // Ignore all metrics and return success
    return opentelemetry::sdk::common::ExportResult::kSuccess;
  }

  bool ForceFlush(std::chrono::microseconds /*timeout*/) noexcept override
  {
    return true;  // No-op
  }

  bool Shutdown(std::chrono::microseconds /*timeout*/) noexcept override
  {
    return true;  // No-op
  }

  opentelemetry::sdk::metrics::AggregationTemporality GetAggregationTemporality(
      opentelemetry::sdk::metrics::InstrumentType) const noexcept override
  {
    return opentelemetry::sdk::metrics::AggregationTemporality::kDelta;
  }
};

void InitMetrics(const std::string &name)
{

  metrics_sdk::PeriodicExportingMetricReaderOptions options;
  options.export_interval_millis = std::chrono::milliseconds(1000);
  options.export_timeout_millis  = std::chrono::milliseconds(500);
  auto exporter                  = std::make_unique<MockMetricExporter>();

  auto reader =
      metrics_sdk::PeriodicExportingMetricReaderFactory::Create(std::move(exporter), options);

  auto provider = metrics_sdk::MeterProviderFactory::Create();
  provider->AddMetricReader(std::move(reader));
  std::shared_ptr<opentelemetry::metrics::MeterProvider> api_provider(std::move(provider));

  metrics_api::Provider::SetMeterProvider(api_provider);
}

void CleanupMetrics()
{
  std::shared_ptr<metrics_api::MeterProvider> none;
  metrics_api::Provider::SetMeterProvider(none);
}

void CounterExample(opentelemetry::nostd::shared_ptr<metrics_api::Meter> meter)
{
  auto counter = meter->CreateDoubleCounter("metrics_stress_test_counter");

  for (int i = 0; i < 20; ++i)
  {
    counter->Add(1.0);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

}  // namespace

int main(int argc, char *argv[])
{
  InitMetrics("metrics_stress_test");
  auto provider = metrics_api::Provider::GetMeterProvider();
  auto meter    = provider->GetMeter("metrics_stress_test", "1.0.0");

  auto func = [&meter]() { CounterExample(meter); };

  StressTest test(func, std::thread::hardware_concurrency());

  std::signal(SIGINT, [](int) { StressTest::signal_handler(); });

  test.run();

  CleanupMetrics();
  return 0;
}
