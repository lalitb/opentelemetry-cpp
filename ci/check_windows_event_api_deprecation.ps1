# Copyright The OpenTelemetry Authors
# SPDX-License-Identifier: Apache-2.0

$ErrorActionPreference = "Stop"
trap { $host.SetShouldExit(1) }

$SRC_DIR = (Get-Item -Path ".\").FullName
$CHECK_DIR = Join-Path "$SRC_DIR" "build/windows-deprecation-check"
$BUILD_DIR = Join-Path "$CHECK_DIR" "build"

if (Test-Path "$CHECK_DIR") {
  Remove-Item -Path "$CHECK_DIR" -Recurse -Force
}
New-Item -Path "$CHECK_DIR" -ItemType Directory | Out-Null

$CMAKE_LISTS = @"
cmake_minimum_required(VERSION 3.15)
project(windows_deprecation_check LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(logger_only OBJECT logger_only.cpp)
target_include_directories(logger_only PRIVATE "${SRC_DIR}/api/include")
if (MSVC)
  target_compile_options(logger_only PRIVATE /W4 /WX /we4996)
endif()

add_library(event_use OBJECT event_use.cpp)
target_include_directories(event_use PRIVATE "${SRC_DIR}/api/include")
if (MSVC)
  target_compile_options(event_use PRIVATE /W4 /WX /we4996)
endif()
"@

$LOGGER_ONLY = @"
#include "opentelemetry/logs/provider.h"

int main()
{
  auto lp = opentelemetry::logs::Provider::GetLoggerProvider();
  (void)lp;
  return 0;
}
"@

$EVENT_USE = @"
#include "opentelemetry/logs/provider.h"

int main()
{
  auto ep = opentelemetry::logs::Provider::GetEventLoggerProvider();
  (void)ep;
  return 0;
}
"@

Set-Content -Path (Join-Path "$CHECK_DIR" "CMakeLists.txt") -Value $CMAKE_LISTS
Set-Content -Path (Join-Path "$CHECK_DIR" "logger_only.cpp") -Value $LOGGER_ONLY
Set-Content -Path (Join-Path "$CHECK_DIR" "event_use.cpp") -Value $EVENT_USE

Push-Location -Path "$CHECK_DIR"
try {
  cmake -S . -B build
  $exit = $LASTEXITCODE
  if ($exit -ne 0) {
    exit $exit
  }

  cmake --build "$BUILD_DIR" --target logger_only
  $exit = $LASTEXITCODE
  if ($exit -ne 0) {
    throw "Expected logger_only target to compile without deprecation errors"
  }

  $EVENT_LOG = Join-Path "$CHECK_DIR" "event_use_build.log"
  cmake --build "$BUILD_DIR" --target event_use *> "$EVENT_LOG"
  $exit = $LASTEXITCODE
  if ($exit -eq 0) {
    throw "Expected event_use target to fail with C4996 (/we4996), but it compiled successfully"
  }

  $event_output = Get-Content -Path "$EVENT_LOG" -Raw
  if ($event_output -notmatch "C4996") {
    throw "event_use failed, but not due to C4996 deprecation warning. See $EVENT_LOG"
  }

  Write-Output "Windows deprecation behavior check passed: logger-only succeeds and Event API usage fails with C4996 (/we4996)."
}
finally {
  Pop-Location
}
