add_library(http_client_nosend http_client_nosend.cc)

set_target_properties(http_client_nosend PROPERTIES EXPORT_NAME
                                                    http_client_nosend)

target_link_libraries(http_client_nosend opentelemetry_api opentelemetry_ext)

install(
  TARGETS http_client_nosend
  EXPORT "${PROJECT_NAME}-target"
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif()
