// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#ifndef ENABLE_METRICS_PREVIEW

#  include "opentelemetry/sdk/metrics/export/periodic_exporting_metric_reader.h"
#  include "opentelemetry/sdk/metrics/export/metric_producer.h"
#  include "opentelemetry/sdk/metrics/metric_exporter.h"

#  include <gtest/gtest.h>

using namespace opentelemetry;
using namespace opentelemetry::sdk::instrumentationlibrary;
using namespace opentelemetry::sdk::metrics;

class MockPushMetricExporter : public MetricExporter
{
public:
  opentelemetry::sdk::common::ExportResult Export(const MetricData &record) noexcept override
  {
    records.push_back(record);
    return opentelemetry::sdk::common::ExportResult::kSuccess;
  }

  bool ForceFlush(
      std::chrono::microseconds timeout = (std::chrono::microseconds::max)()) noexcept override
  {
    return false;
  }

  bool Shutdown(std::chrono::microseconds timeout = std::chrono::microseconds(0)) noexcept override
  {
    return true;
  }

  size_t GetDataCount() { return records.size(); }

private:
  std::vector<MetricData> records;
};

class MockMetricProducer : public MetricProducer
{
public:
  MockMetricProducer(std::chrono::microseconds sleep_ms = std::chrono::microseconds::zero())
      : sleep_ms_{sleep_ms}
  {}

  bool Collect(nostd::function_ref<bool(MetricData)> callback) noexcept override
  {
    std::this_thread::sleep_for(sleep_ms_);
    MetricData data;
    callback(data);
    data_sent_size_++;
    return true;
  }

  size_t GetDataCount() { return data_sent_size_; }

private:
  std::chrono::microseconds sleep_ms_;
  size_t data_sent_size_;
};

TEST(PeriodicExporingMetricReader, BasicTests)
{
  std::unique_ptr<MetricExporter> exporter(new MockPushMetricExporter());
  PeriodicExportingMetricReaderOptions options;
  auto exporter_ptr = exporter.get();
  PeriodicExportingMetricReader reader(std::move(exporter), options);
  MockMetricProducer producer;
  reader.SetMetricProducer(&producer);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  reader.Shutdown();
  EXPECT_EQ(static_cast<MockPushMetricExporter *>(exporter_ptr)->GetDataCount(),
            static_cast<MockMetricProducer *>(&producer)->GetDataCount());
}

#endif