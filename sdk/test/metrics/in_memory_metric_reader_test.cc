
// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#ifndef ENABLE_METRICS_PREVIEW

#  include "opentelemetry/sdk/metrics/export/in_memory_metric_reader.h"
#  include "opentelemetry/sdk/metrics/export/metric_producer.h"
#  include "opentelemetry/sdk/metrics/metric_exporter.h"

#  include <gtest/gtest.h>

using namespace opentelemetry::sdk::metrics;
using namespace opentelemetry;

class MockMetricProducer : public MetricProducer
{
public:
  bool Collect(nostd::function_ref<bool(ResourceMetrics &)> callback) noexcept override
  {
    ResourceMetrics data;
    callback(data);
    return true;
  }

private:
  size_t data_sent_size_;
};

TEST(InMemoryMetricReader, BasicTests)
{

  InMemoryMetricReader reader;
  MockMetricProducer producer;
  reader.SetMetricProducer(&producer);
  EXPECT_NO_THROW(reader.GetMetricData());
  reader.Shutdown();
}
#endif
