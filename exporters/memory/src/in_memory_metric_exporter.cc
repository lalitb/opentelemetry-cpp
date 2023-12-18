#include "opentelemetry/exporters/memory/in_memory_metric_exporter.h"

#include "opentelemetry/sdk/metrics/data/metric_data.h"
#include "opentelemetry/sdk/metrics/export/metric_producer.h"
#include <optional>
#include <tuple>
#include <vector>
#include <mutex>

std::optional<opentelemetry::sdk::metrics::PointType>
InMemoryMetricExporter::GetPointData(std::shared_ptr<InMemoryMetricData> metricData,
                                     InMemoryMetricDataKey metric_data_key,
                                     const opentelemetry::sdk::metrics::PointAttributes& attr)
{
    if (auto existingInstr = metricData->find(metric_data_key);
        existingInstr != metricData->end()) {
        if (auto existingAttribute = existingInstr->second.find(attr);
            existingAttribute != existingInstr->second.end()) {
            return existingAttribute->second;
        }
    }

    return {};
}

InMemoryMetricExporter::InMemoryMetricExporter(
    opentelemetry::sdk::metrics::AggregationTemporality aggregation_temporality) noexcept
    : aggregation_temporality_(aggregation_temporality),
      map_(std::make_shared<InMemoryMetricData>())
{
}

opentelemetry::sdk::common::ExportResult
InMemoryMetricExporter::Export(const opentelemetry::sdk::metrics::ResourceMetrics& data) noexcept
{
    if (isShutdown()) {
        return opentelemetry::sdk::common::ExportResult::kFailure;
    }

    for (auto& scopeMetric : data.scope_metric_data_) {
        for (auto& metricData : scopeMetric.metric_data_) {
            for (auto& pointData : metricData.point_data_attr_) {
                InsertPointData(std::make_tuple(scopeMetric.scope_->GetName(),
                                                metricData.instrument_descriptor.name_),
                                pointData);
            }
        }
    }

    return opentelemetry::sdk::common::ExportResult::kSuccess;
}

opentelemetry::sdk::metrics::AggregationTemporality InMemoryMetricExporter::
    GetAggregationTemporality(
        opentelemetry::sdk::metrics::InstrumentType /* instrument_type */) const noexcept
{
    return aggregation_temporality_;
}

bool
InMemoryMetricExporter::ForceFlush(std::chrono::microseconds timeout) noexcept
{
    const std::lock_guard<opentelemetry::common::SpinLockMutex> locked(lock_);
    return true;
}

bool
InMemoryMetricExporter::Shutdown(std::chrono::microseconds timeout) noexcept
{
    const std::lock_guard<opentelemetry::common::SpinLockMutex> locked(lock_);
    is_shutdown_ = true;
    return true;
}

bool
InMemoryMetricExporter::isShutdown() const noexcept
{
    const std::lock_guard<opentelemetry::common::SpinLockMutex> locked(lock_);
    return is_shutdown_;
}

void
InMemoryMetricExporter::InsertPointData(
    InMemoryMetricDataKey metric_data_key,
    const opentelemetry::sdk::metrics::PointDataAttributes& data)
{
    if (auto existingInstr = map_->find(metric_data_key); existingInstr != map_->end()) {
        if (auto existingAttribute = existingInstr->second.find(data.attributes);
            existingAttribute != existingInstr->second.end()) {
            existingInstr->second.erase(existingAttribute);
        }

        existingInstr->second.emplace(data.attributes, data.point_data);
    } else {
        map_->emplace(metric_data_key, std::map<opentelemetry::sdk::metrics::PointAttributes,
                                                opentelemetry::sdk::metrics::PointType> {
                                           {data.attributes, data.point_data}});
    }
}