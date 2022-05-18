function(gtest_add_tests)

  if (ARGC LESS 1)
    message(FATAL_ERROR "No arguments supplied to gtest_add_tests()")
  endif()

  set(options
      SKIP_DEPENDENCY
  )
  set(oneValueArgs
      TARGET
      WORKING_DIRECTORY
      TEST_PREFIX
      TEST_SUFFIX
      TEST_LIST
  )
  set(multiValueArgs
      SOURCES
      EXTRA_ARGS
  )
  set(allKeywords ${options} ${oneValueArgs} ${multiValueArgs})

  unset(sources)
  if("${ARGV0}" IN_LIST allKeywords)
    cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    set(autoAddSources YES)
  else()
    # Non-keyword syntax, convert to keyword form
    if (ARGC LESS 3)
      message(FATAL_ERROR "gtest_add_tests() without keyword options requires at least 3 arguments")
    endif()
    set(ARGS_TARGET     "${ARGV0}")
    set(ARGS_EXTRA_ARGS "${ARGV1}")
    if(NOT "${ARGV2}" STREQUAL "AUTO")
      set(ARGS_SOURCES "${ARGV}")
      list(REMOVE_AT ARGS_SOURCES 0 1)
    endif()
  endif()

  # The non-keyword syntax allows the first argument to be an arbitrary
  # executable rather than a target if source files are also provided. In all
  # other cases, both forms require a target.
  if(NOT TARGET "${ARGS_TARGET}" AND NOT ARGS_SOURCES)
    message(FATAL_ERROR "${ARGS_TARGET} does not define an existing CMake target")
  endif()
  if(NOT ARGS_WORKING_DIRECTORY)
    unset(workDir)
  else()
    set(workDir WORKING_DIRECTORY "${ARGS_WORKING_DIRECTORY}")
  endif()

  if(NOT ARGS_SOURCES)
    get_property(ARGS_SOURCES TARGET ${ARGS_TARGET} PROPERTY SOURCES)
  endif()

  unset(testList)

  set(gtest_case_name_regex ".*\\( *([A-Za-z_0-9]+) *, *([A-Za-z_0-9]+) *\\).*")
  set(gtest_test_type_regex "(TYPED_TEST|TEST_?[FP]?)")

  foreach(source IN LISTS ARGS_SOURCES)
    if(NOT ARGS_SKIP_DEPENDENCY)
      set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${source})
    endif()
    file(READ "${source}" contents)
    string(REGEX MATCHALL "${gtest_test_type_regex} *\\(([A-Za-z_0-9 ,]+)\\)" found_tests "${contents}")
    foreach(hit ${found_tests})
      string(REGEX MATCH "${gtest_test_type_regex}" test_type ${hit})

      # Parameterized tests have a different signature for the filter
      if("x${test_type}" STREQUAL "xTEST_P")
        string(REGEX REPLACE ${gtest_case_name_regex}  "*/\\1.\\2/*" gtest_test_name ${hit})
      elseif("x${test_type}" STREQUAL "xTEST_F" OR "x${test_type}" STREQUAL "xTEST")
        string(REGEX REPLACE ${gtest_case_name_regex} "\\1.\\2" gtest_test_name ${hit})
      elseif("x${test_type}" STREQUAL "xTYPED_TEST")
        string(REGEX REPLACE ${gtest_case_name_regex} "\\1/*.\\2" gtest_test_name ${hit})
      else()
        message(WARNING "Could not parse GTest ${hit} for adding to CTest.")
        continue()
      endif()

      # Make sure tests disabled in GTest get disabled in CTest
      if(gtest_test_name MATCHES "(^|\\.)DISABLED_")
        # Add the disabled test if CMake is new enough
        # Note that this check is to allow backwards compatibility so this
        # module can be copied locally in projects to use with older CMake
        # versions
        if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.8.20170401)
          string(REGEX REPLACE
                 "(^|\\.)DISABLED_" "\\1"
                 orig_test_name "${gtest_test_name}"
          )
          set(ctest_test_name
              ${ARGS_TEST_PREFIX}${orig_test_name}${ARGS_TEST_SUFFIX}
          )
          add_test(NAME ${ctest_test_name}
                   ${workDir}
                   COMMAND ${ARGS_TARGET}
                     --gtest_also_run_disabled_tests
                     --gtest_filter=${gtest_test_name}
                     ${ARGS_EXTRA_ARGS}
          )
          set_tests_properties(${ctest_test_name} PROPERTIES DISABLED TRUE)
          list(APPEND testList ${ctest_test_name})
        endif()
      else()
        set(ctest_test_name ${ARGS_TEST_PREFIX}${gtest_test_name}${ARGS_TEST_SUFFIX})
        add_test(NAME ${ctest_test_name}
                 ${workDir}
                 COMMAND ${ARGS_TARGET}
                   --gtest_filter=${gtest_test_name}
                   ${ARGS_EXTRA_ARGS}
        )
        list(APPEND testList ${ctest_test_name})
      endif()
    endforeach()
  endforeach()

  if(ARGS_TEST_LIST)
    set(${ARGS_TEST_LIST} ${testList} PARENT_SCOPE)
  endif()

endfunction()