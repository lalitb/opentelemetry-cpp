# Copyright The OpenTelemetry Authors
# SPDX-License-Identifier: Apache-2.0

add_library(
  opentelemetry_http_client_curl http_client_factory_curl.cc
                                 http_client_curl.cc http_operation_curl.cc)

set_target_properties(opentelemetry_http_client_curl
                      PROPERTIES EXPORT_NAME http_client_curl)
set_target_version(opentelemetry_http_client_curl)
target_link_libraries(opentelemetry_http_client_curl
                      PUBLIC opentelemetry_common)

unset(CURL_IMPORTED_TARGET_NAME)

foreach(FIND_CURL_IMPORTED_TARGET CURL::libcurl CURL::libcurl_shared
                                  CURL::libcurl_static)
  if(TARGET ${FIND_CURL_IMPORTED_TARGET})
    set(CURL_IMPORTED_TARGET_NAME ${FIND_CURL_IMPORTED_TARGET})
    break()
  endif()
endforeach()

if(TARGET ${CURL_IMPORTED_TARGET_NAME})
  target_link_libraries(
    opentelemetry_http_client_curl
    PUBLIC opentelemetry_ext
    PRIVATE ${CURL_IMPORTED_TARGET_NAME})

  # Some versions of libcurl do not export the link directories, which may cause
  # link errors
  project_build_tools_get_imported_location(CURL_LIB_FILE_PATH
                                            ${CURL_IMPORTED_TARGET_NAME})
  get_filename_component(CURL_LIB_DIR_PATH "${CURL_LIB_FILE_PATH}" DIRECTORY)

  if(CURL_LIB_DIR_PATH)
    target_link_directories(opentelemetry_http_client_curl PUBLIC
                            "$<BUILD_INTERFACE:${CURL_LIB_DIR_PATH}>")
  endif()
else()
  target_include_directories(opentelemetry_http_client_curl
                             INTERFACE "${CURL_INCLUDE_DIRS}")
  target_link_libraries(
    opentelemetry_http_client_curl
    PUBLIC opentelemetry_ext
    PRIVATE ${CURL_LIBRARIES})
endif()

if(WITH_CURL_LOGGING)
  target_compile_definitions(opentelemetry_http_client_curl
                             PRIVATE ENABLE_CURL_LOGGING)
endif()

if(WITH_OTLP_HTTP_COMPRESSION)
  if(TARGET ZLIB::ZLIB)
    target_link_libraries(
      opentelemetry_http_client_curl
      PUBLIC opentelemetry_ext
      PRIVATE ZLIB::ZLIB)
  else()
    target_include_directories(opentelemetry_http_client_curl
                               INTERFACE "${ZLIB_INCLUDE_DIRS}")
    target_link_libraries(
      opentelemetry_http_client_curl
      PUBLIC opentelemetry_ext
      PRIVATE ${ZLIB_LIBRARIES})
  endif()
endif()

if(OPENTELEMETRY_INSTALL)
  install(
    TARGETS opentelemetry_http_client_curl
    EXPORT "${PROJECT_NAME}-ext_http_curl-target"
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT ext_http_curl)

  install(
    EXPORT "${PROJECT_NAME}-ext_http_curl-target"
    FILE "${PROJECT_NAME}-ext_http_curl-target.cmake"
    NAMESPACE "${PROJECT_NAME}::"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
    COMPONENT ext_http_curl)
endif()
