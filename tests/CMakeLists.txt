cmake_minimum_required(VERSION 2.8.7)
set(PROJECT_NAME_STR tests)
project(${PROJECT_NAME_STR} C CXX)

get_property(MD_LIB_NAME GLOBAL PROPERTY PROJECT_LIB_NAME)

function( add_test_executable TEST_NAME TEST_SRC )
    include_directories(${GTEST_INCLUDE_DIR})
    message(${MD_INCLUDE_DIR})
    include_directories(${MD_INCLUDE_DIR})

    add_executable(${TEST_NAME} ${TEST_SRC})

    target_link_libraries(${TEST_NAME} ${MD_LIB_NAME} gtest_main )

    INSTALL( FILES ${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME}
             PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_READ GROUP_EXECUTE
             DESTINATION tests COMPONENT tests  OPTIONAL)

endfunction(add_test_executable) 

function(add_opencl_test_executable TEST_NAME)
  include_directories(${OPENCL_INCLUDE_DIRS})
  add_test_executable(${TEST_NAME} ${ARGN})
  target_link_libraries(${TEST_NAME} moldynam_opencl moldynam_native ${OPENCL_LIBRARIES})
endfunction()

function(add_native_test_executable TEST_NAME)
  include_directories(${OPENCL_INCLUDE_DIRS})
  add_test_executable(${TEST_NAME} ${ARGN})
  target_link_libraries(${TEST_NAME} moldynam_native ${OPENCL_LIBRARIES})
endfunction()

add_test_executable( algorithms_test src/algorithms_test.cpp  )
add_test_executable( file_io_test src/file_io_test.cpp  )

add_native_test_executable( config_test src/config_test.cpp  )
add_native_test_executable( native_platform_test src/native_platform_test.cpp  )
add_opencl_test_executable( opencl_platform_test src/opencl_platform_test.cpp  )

#installation of executable files
INSTALL ( FILES
          run_tests.py

          PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_READ GROUP_EXECUTE
          DESTINATION tests COMPONENT tests  OPTIONAL
        )
                   
#installation for other files
INSTALL ( FILES

          testfiles/test_file_io_read_from_file.xyz
          testfiles/config_test.conf

          PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ
          DESTINATION tests COMPONENT tests  OPTIONAL
        )
                   
