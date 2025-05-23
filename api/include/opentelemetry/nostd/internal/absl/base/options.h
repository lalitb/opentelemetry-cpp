#ifndef OTABSL_BASE_OPTIONS_H_
#define OTABSL_BASE_OPTIONS_H_

// Copyright 2019 The Abseil Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// -----------------------------------------------------------------------------
// File: options.h
// -----------------------------------------------------------------------------
//
// This file contains Abseil configuration options for setting specific
// implementations instead of letting Abseil determine which implementation to
// use at compile-time. Setting these options may be useful for package or build
// managers who wish to guarantee ABI stability within binary builds (which are
// otherwise difficult to enforce).
//
// *** IMPORTANT NOTICE FOR PACKAGE MANAGERS:  It is important that
// maintainers of package managers who wish to package Abseil read and
// understand this file! ***
//
// Abseil contains a number of possible configuration endpoints, based on
// parameters such as the detected platform, language version, or command-line
// flags used to invoke the underlying binary. As is the case with all
// libraries, binaries which contain Abseil code must ensure that separate
// packages use the same compiled copy of Abseil to avoid a diamond dependency
// problem, which can occur if two packages built with different Abseil
// configuration settings are linked together. Diamond dependency problems in
// C++ may manifest as violations to the One Definition Rule (ODR) (resulting in
// linker errors), or undefined behavior (resulting in crashes).
//
// Diamond dependency problems can be avoided if all packages utilize the same
// exact version of Abseil. Building from source code with the same compilation
// parameters is the easiest way to avoid such dependency problems. However, for
// package managers who cannot control such compilation parameters, we are
// providing the file to allow you to inject ABI (Application Binary Interface)
// stability across builds. Settings options in this file will neither change
// API nor ABI, providing a stable copy of Abseil between packages.
//
// Care must be taken to keep options within these configurations isolated
// from any other dynamic settings, such as command-line flags which could alter
// these options. This file is provided specifically to help build and package
// managers provide a stable copy of Abseil within their libraries and binaries;
// other developers should not have need to alter the contents of this file.
//
// -----------------------------------------------------------------------------
// Usage
// -----------------------------------------------------------------------------
//
// For any particular package release, set the appropriate definitions within
// this file to whatever value makes the most sense for your package(s). Note
// that, by default, most of these options, at the moment, affect the
// implementation of types; future options may affect other implementation
// details.
//
// NOTE: the defaults within this file all assume that Abseil can select the
// proper Abseil implementation at compile-time, which will not be sufficient
// to guarantee ABI stability to package managers.

// Include a standard library header to allow configuration based on the
// standard library in use.
// Using C++20 feature-test macros when possible, otherwise fall back to
// ciso646/iso646.h.There are warnings when including ciso646 in C++17 mode
#ifdef __has_include
#  if __has_include(<version>)
#    include <version>
#  endif
#elif defined(_MSC_VER) && \
    ((defined(__cplusplus) && __cplusplus >= 202002L) || (defined(_MSVC_LANG) && _MSVC_LANG >= 202002L))
#  if _MSC_VER >= 1922
#    include <version>
#  endif
#else
#  if defined(__GNUC__) && !defined(__clang__) && !defined(__apple_build_version__)
#    pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Wcpp"
#  elif defined(__clang__) || defined(__apple_build_version__)
#    pragma clang diagnostic push
#    pragma clang diagnostic ignored "-Wcpp"
#  endif
#  include <ciso646>
#  if defined(__GNUC__) && !defined(__clang__) && !defined(__apple_build_version__)
#      pragma GCC diagnostic pop
#  elif defined(__clang__) || defined(__apple_build_version__)
#    pragma clang diagnostic pop
#  endif
#endif

// -----------------------------------------------------------------------------
// Type Compatibility Options
// -----------------------------------------------------------------------------
//
// OTABSL_OPTION_USE_STD_ANY
//
// This option controls whether absl::OTABSL_OPTION_NAMESPACE_NAME::any is implemented as an alias to
// std::any, or as an independent implementation.
//
// A value of 0 means to use Abseil's implementation.  This requires only C++11
// support, and is expected to work on every toolchain we support.
//
// A value of 1 means to use an alias to std::any.  This requires that all code
// using Abseil is built in C++17 mode or later.
//
// A value of 2 means to detect the C++ version being used to compile Abseil,
// and use an alias only if a working std::any is available.  This option is
// useful when you are building your entire program, including all of its
// dependencies, from source.  It should not be used otherwise -- for example,
// if you are distributing Abseil in a binary package manager -- since in
// mode 2, absl::OTABSL_OPTION_NAMESPACE_NAME::any will name a different type, with a different mangled name
// and binary layout, depending on the compiler flags passed by the end user.
// For more info, see https://abseil.io/about/design/dropin-types.
//
// User code should not inspect this macro.  To check in the preprocessor if
// absl::OTABSL_OPTION_NAMESPACE_NAME::any is a typedef of std::any, use the feature macro OTABSL_USES_STD_ANY.

#define OTABSL_OPTION_USE_STD_ANY 0


// OTABSL_OPTION_USE_STD_OPTIONAL
//
// This option controls whether absl::OTABSL_OPTION_NAMESPACE_NAME::optional is implemented as an alias to
// std::optional, or as an independent implementation.
//
// A value of 0 means to use Abseil's implementation.  This requires only C++11
// support, and is expected to work on every toolchain we support.
//
// A value of 1 means to use an alias to std::optional.  This requires that all
// code using Abseil is built in C++17 mode or later.
//
// A value of 2 means to detect the C++ version being used to compile Abseil,
// and use an alias only if a working std::optional is available.  This option
// is useful when you are building your program from source.  It should not be
// used otherwise -- for example, if you are distributing Abseil in a binary
// package manager -- since in mode 2, absl::OTABSL_OPTION_NAMESPACE_NAME::optional will name a different
// type, with a different mangled name and binary layout, depending on the
// compiler flags passed by the end user.  For more info, see
// https://abseil.io/about/design/dropin-types.

// User code should not inspect this macro.  To check in the preprocessor if
// absl::OTABSL_OPTION_NAMESPACE_NAME::optional is a typedef of std::optional, use the feature macro
// OTABSL_USES_STD_OPTIONAL.

#define OTABSL_OPTION_USE_STD_OPTIONAL 0


// OTABSL_OPTION_USE_STD_STRING_VIEW
//
// This option controls whether absl::OTABSL_OPTION_NAMESPACE_NAME::string_view is implemented as an alias to
// std::string_view, or as an independent implementation.
//
// A value of 0 means to use Abseil's implementation.  This requires only C++11
// support, and is expected to work on every toolchain we support.
//
// A value of 1 means to use an alias to std::string_view.  This requires that
// all code using Abseil is built in C++17 mode or later.
//
// A value of 2 means to detect the C++ version being used to compile Abseil,
// and use an alias only if a working std::string_view is available.  This
// option is useful when you are building your program from source.  It should
// not be used otherwise -- for example, if you are distributing Abseil in a
// binary package manager -- since in mode 2, absl::OTABSL_OPTION_NAMESPACE_NAME::string_view will name a
// different type, with a different mangled name and binary layout, depending on
// the compiler flags passed by the end user.  For more info, see
// https://abseil.io/about/design/dropin-types.
//
// User code should not inspect this macro.  To check in the preprocessor if
// absl::OTABSL_OPTION_NAMESPACE_NAME::string_view is a typedef of std::string_view, use the feature macro
// OTABSL_USES_STD_STRING_VIEW.

#define OTABSL_OPTION_USE_STD_STRING_VIEW 0

// OTABSL_OPTION_USE_STD_VARIANT
//
// This option controls whether absl::OTABSL_OPTION_NAMESPACE_NAME::variant is implemented as an alias to
// std::variant, or as an independent implementation.
//
// A value of 0 means to use Abseil's implementation.  This requires only C++11
// support, and is expected to work on every toolchain we support.
//
// A value of 1 means to use an alias to std::variant.  This requires that all
// code using Abseil is built in C++17 mode or later.
//
// A value of 2 means to detect the C++ version being used to compile Abseil,
// and use an alias only if a working std::variant is available.  This option
// is useful when you are building your program from source.  It should not be
// used otherwise -- for example, if you are distributing Abseil in a binary
// package manager -- since in mode 2, absl::OTABSL_OPTION_NAMESPACE_NAME::variant will name a different
// type, with a different mangled name and binary layout, depending on the
// compiler flags passed by the end user.  For more info, see
// https://abseil.io/about/design/dropin-types.
//
// User code should not inspect this macro.  To check in the preprocessor if
// absl::OTABSL_OPTION_NAMESPACE_NAME::variant is a typedef of std::variant, use the feature macro
// OTABSL_USES_STD_VARIANT.

#define OTABSL_OPTION_USE_STD_VARIANT 0


// OTABSL_OPTION_NAMESPACE_NAME
//
// All codes in otabsl are under OTABSL_OPTION_NAMESPACE_NAME, we do not use inline namespace to avoid
// conlict with external Abseil.

#define OTABSL_OPTION_NAMESPACE_NAME otel_v1

#endif  // OTABSL_BASE_OPTIONS_H_
