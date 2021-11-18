// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#pragma once

enum class InstrumentType
{
  kCounter,
  kHistogram,
  kUpDownCounter,
  kObservableCounter,
  kObservableGauge,
  kObservableUpDownCounter
};

class SyncInstrument
{}