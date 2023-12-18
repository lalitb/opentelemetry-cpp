#include "in_memory_metric_exporter.h"

class InMemoryMetricExporterFactory
{
  public:
    /**
     * Create a InMemoryMetricExporter with a default buffer size.
     * @param [out] data the InMemoryMetricExporter the exporter will write to,
     *                   for the caller to inspect
     */
    static std::unique_ptr<InMemoryMetricExporter>
    Create(std::shared_ptr<InMemoryMetricData>& data);
};
