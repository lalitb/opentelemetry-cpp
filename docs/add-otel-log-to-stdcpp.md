# A Proposal to add log library

## Motivation

In current C++ Standard Library, there is no standadized way of logging which is a common practice in other langauges like Java, Python, etc. This proposal suggests a set of logging APIs that target the C++ standard library (under the std:: namespace) to help mitigate the issue.

## Design Decisions

The design is based on opentelemetry-cpp Logs APIs which is an extensible solution, and can export all the logs to the open-telemetry exporters.

## Log APIs

Here is the list of the proposed functions.

```cpp

namespace std::log

{

enum class Severity;

std::shared_ptr<std::logger> default_logger();

void set_default_logger(std::shared_ptr<std::logger> default_logger);

template <typename... ArgumentType>
void log(ArgumentType &&... args) noexcept;

template <class... ArgumentType>
void trace(ArgumentType &&... args) noexcept;

template <class... ArgumentType>
void debug(ArgumentType &&... args) noexcept;

template <class... ArgumentType>
void info(ArgumentType &&... args) noexcept;

template <class... ArgumentType>
void warn(ArgumentType &&... args) noexcept;

template <class... ArgumentType>
void error(ArgumentType &&... args) noexcept;

template <class... ArgumentType>
void fatal(ArgumentType &&... args) noexcept;

}

```

## Log sample

```cpp

#include <iostream>
#include <log>

int main()
{
    // log with default_logger();
    log::info("Hello std::log");

    log::warn("Hello std::log", {{"key1": 1}, {"key2": 2}});

    log::error("Error occurred", {{"error_code", 3}});

    return 0;
}

```

## Significant changes to reivew

## Wording

## Acknowledgements
https://github.com/open-telemetry/opentelemetry-cpp/graphs/contributors