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

template <typename... ArgumentType>
void trace(ArgumentType &&... args) noexcept;

template <typename... ArgumentType>
void debug(ArgumentType &&... args) noexcept;

template <typename... ArgumentType>
void info(ArgumentType &&... args) noexcept;

template <typename... ArgumentType>
void warn(ArgumentType &&... args) noexcept;

template <typename... ArgumentType>
void error(ArgumentType &&... args) noexcept;

template <typename... ArgumentType>
void fatal(ArgumentType &&... args) noexcept;

}

```

## Expected Log Code Sample

```cpp

#include <iostream>
#include <log>

int main()
{
    // log with default_logger();
    std::log::info("Hello std::log");

    std::log::warn("Hello std::log", {{"key1": 1}, {"key2": 2}});

    // create a new logger.
    std::shared_ptr<logger> new_logger = ...;
    std::log::set_default_logger(new_logger);

    // log to the new logger which defines where the log should be exported.
    std::log::error("Error occurred", {{"error_code", 3}});

    // reset to the default logger.
    std::log::set_default_logger(nullptr);

    return 0;
}

```

## Significant Changes to Reivew

## Wording

## Acknowledgements
https://github.com/open-telemetry/opentelemetry-cpp/graphs/contributors