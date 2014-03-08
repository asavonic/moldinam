cmake_minimum_required(VERSION 2.8.7)
set(PROJECT_NAME_STR tests)
project(${PROJECT_NAME_STR} C CXX)

function( add_test_executable TEST_NAME TEST_SRC )
    add_executable(${TEST_NAME} ${TEST_SRC})
    add_dependencies(${TEST_NAME} googletest)

    if(NOT WIN32)
        target_link_libraries(${TEST_NAME} pthread)
        target_link_libraries(${TEST_NAME} gtest_main)
    else()

        target_link_libraries(${TEST_NAME}
               debug ${GTEST_LIBS_DIR}/DebugLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${CMAKE_FIND_LIBRARY_SUFFIXES}
               optimized ${GTEST_LIBS_DIR}/ReleaseLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${CMAKE_FIND_LIBRARY_SUFFIXES}
        )
        target_link_libraries(${TEST_NAME}
               debug ${GTEST_LIBS_DIR}/DebugLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main${CMAKE_FIND_LIBRARY_SUFFIXES}
               optimized ${GTEST_LIBS_DIR}/ReleaseLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main${CMAKE_FIND_LIBRARY_SUFFIXES}
        )
    endif()

    add_test(${TEST_NAME} ${TEST_NAME})

<<<<<<< HEAD
    INSTALL( FILES ${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME}
             PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_READ GROUP_EXECUTE
=======
    #    INSTALL( PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME} 
    #        DESTINATION ${CMAKE_INSTALL_PREFIX}/tests
    #)
    INSTALL( FILES ${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME}
>>>>>>> 324c679a6cdf8634cb2df5bd11b051c480e14449
             DESTINATION tests COMPONENT tests  OPTIONAL)

endfunction(add_test_executable) 

add_test_executable( cpp_sorter_test cpp_sorter_test.cpp  )
