// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef ENABLE_METRICS_PREVIEW
#  include "opentelemetry/version.h"
OPENTELEMETRY_BEGIN_NAMESPACE
namespace sdk
{
namespace metrics
{
template <class T>
class WritableMetricStorage {
public:
  void Record (T value, const common::KeyValueIterable &attributes ) ;
};

template <class T>
class MultiWritableMetricStorage : public WritableMetricStorage<T>
{
public:
    MultiWritableMetricStorage(std::vector<std::unique_ptr<WritableMetricStorage<T>> storages):storages_{std::move(storages)} {}
    void Record(T value, const common::KeyValueIterable &attributes){
        for (auto const &s: storages_){
            s->Record<T>(value, attributes);
        }
    }

private:
std::vector<std::unique_ptr<WritableMetricStorage<T>> storages_;

};

}
}
OPENTELEMETRY_END_NAMESPACE

