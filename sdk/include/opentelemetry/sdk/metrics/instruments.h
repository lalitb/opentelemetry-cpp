// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef ENABLE_METRICS_PREVIEW
OPENTELEMETRY_BEGIN_NAMESPACE
namespace sdk
{
namespace metrics
{
enum class InstrumentType {
    kCounter,
    kHistogram,
    kUpDownCounter,
    kObservableCounter,
    kObservableGauge,
    kObservableUpDownCounter
};

enum class ValueType {
    INT,
    LONG,
    FLOAT,
    DOUBLE
};

template <class T>
struct InstrumentDescriptor {
    std::string name;
    std::string description;
    std::string unit;
    InstrumentType type; 
};

class InstrumentSelector {
public:
InstrumentSelector(nostd::string name, InstrumentType type): name(name_), type_(type) {}
InstrumentType GetType(){return type_;}
nostd::string GetNameFilter() { return name_;}

private:
std::string name_;
InstrumentType type_;
};



}
OPENTELEMETRY_END_NAMESPACE
#endif
