set(CMAKE_PROJECT_NAME "opentelemetry-cpp")
set(CPACK_SYSTEM_NAME "${OS_RELEASE_ID}-${OS_RELEASE_VERSION_ID}")
set(CPACK_PACKAGING_INSTALL_PREFIX "/usr/local")
set(CPACK_GENERATOR "RPM")

set(CPACK_RPM_PACKAGE_DESCRIPTION "OpenTelemetry C++ for Linux")
set(CPACK_RPM_PACKAGE_DESCRIPTION_SUMMARY "OpenTelemetry C++ for Linux - C++ Implementation of OpenTelemetry Specification")
set(CPACK_RPM_PACKAGE_CONTACT "OpenTelemetry C++")

set(MAJOR_VERSION "1")
set(MINOR_VERSION "6")
set(MINOR_VERSION "1")

set(CPACK_PACKAGE_DESCRIPTION "OpenTelemetry C++ for Linux")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "OpenTelemetry C++ for Linux - C++ Implementation of OpenTelemetry Specification")
set(CPACK_PACKAGE_VENDOR "OpenTelemetry")
set(CPACK_PACKAGE_CONTACT "OpenTelemetry C++")
set(CPACK_PACKAGE_VERSION_MAJOR "${MAJOR_VERSION}")
set(CPACK_PACKAGE_VERSION_MINOR "${MINOR_VERSION}")
set(CPACK_PACKAGE_VERSION_PATCH "${PATCH_VERSION}")
# FIXME: add architecture name in file name
set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}-${CPACK_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")
message("-- Package name: ${CPACK_PACKAGE_FILE_NAME}.rpm")