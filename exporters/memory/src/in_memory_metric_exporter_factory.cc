#include "opentelemetry/exporters/memory/in_memory_metric_exporter_factory.h"

std::unique_ptr<InMemoryMetricExporter>
InMemoryMetricExporterFactory::Create(std::shared_ptr<InMemoryMetricData>& data)
{
    std::unique_ptr<InMemoryMetricExporter> exporter = std::make_unique<InMemoryMetricExporter>();
    data = exporter->GetData();
    return exporter;
}