#pragma once

#include "opentelemetry/common/spin_lock_mutex.h"
#include "opentelemetry/sdk/metrics/data/metric_data.h"
#include "opentelemetry/sdk/metrics/push_metric_exporter.h"

#include <opentelemetry/sdk/instrumentationscope/instrumentation_scope.h>

#include <iostream>
#include <opentelemetry/sdk/resource/resource.h>
#include <optional>
#include <vector>

//  Tuple: (name of the instrumentation scope, name of instrument descriptor)
//  e.g. ("library_name", "metrics_name")
using InMemoryMetricDataKey = std::tuple<std::string, std::string>;
using InMemoryMetricData = std::map<
    InMemoryMetricDataKey,
    std::map<opentelemetry::sdk::metrics::PointAttributes, opentelemetry::sdk::metrics::PointType>>;

class InMemoryMetricExporter final : public opentelemetry::sdk::metrics::PushMetricExporter
{
  public:
    explicit InMemoryMetricExporter(
        opentelemetry::sdk::metrics::AggregationTemporality aggregation_temporality =
            opentelemetry::sdk::metrics::AggregationTemporality::kCumulative) noexcept;

    opentelemetry::sdk::common::ExportResult
    Export(const opentelemetry::sdk::metrics::ResourceMetrics& data) noexcept override;

    opentelemetry::sdk::metrics::AggregationTemporality
    GetAggregationTemporality(
        opentelemetry::sdk::metrics::InstrumentType instrument_type) const noexcept override;

    bool
    ForceFlush(
        std::chrono::microseconds timeout = (std::chrono::microseconds::max)()) noexcept override;

    bool
    Shutdown(
        std::chrono::microseconds timeout = (std::chrono::microseconds::max)()) noexcept override;

    bool
    Clear() noexcept
    {
        map_->clear();
        return true;
    };

    /**
     * @brief Save reference to the in-memory metric data.
     *
     * To query InMemoryMetricExporter at a later time, save a reference to the in-memory metric
     * data before std::move-ing the unique_ptr<InMemoryMetricExporter> to a MetricReader.
     *
     * PeriodicExportingMetricReader in particular requires a unique_ptr.
     *
     * @return Returns a shared pointer to this exporters InMemoryMetricData
     */
    std::shared_ptr<InMemoryMetricData>
    GetData() noexcept
    {
        return map_;
    }


    /**
     * Convenience query helper methods
     * Query helper methods will all be static as caller is expected to have saved a reference to
     * InMemoryMetricData using GetData()
     */

    static std::optional<opentelemetry::sdk::metrics::PointType>
    GetPointData(std::shared_ptr<InMemoryMetricData> metricData,
                 InMemoryMetricDataKey metric_data_key,
                 const opentelemetry::sdk::metrics::PointAttributes& attr);

    template <typename T>
    static std::optional<T>
    GetPointData(std::shared_ptr<InMemoryMetricData> metricData,
                 InMemoryMetricDataKey metric_data_key,
                 const opentelemetry::sdk::metrics::PointAttributes& attr)
    {
        auto pointData = GetPointData(metricData, metric_data_key, attr);
        if (pointData.has_value()) {
            return std::get<T>(pointData.value());
        }

        return {};
    };

  private:
    void
    InsertPointData(InMemoryMetricDataKey metric_data_key,
                    const opentelemetry::sdk::metrics::PointDataAttributes& data);

    bool
    isShutdown() const noexcept;

    bool is_shutdown_ = false;
    mutable opentelemetry::common::SpinLockMutex lock_;

    opentelemetry::sdk::metrics::AggregationTemporality aggregation_temporality_;
    std::shared_ptr<InMemoryMetricData> map_;
};