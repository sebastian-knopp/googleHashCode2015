

file(GLOB HASHSOLVER_SOURCES *.h
                           *.hpp
                           *.cpp)

add_executable(HashSolver ${HASHSOLVER_SOURCES})

include_directories("${PROJECT_SOURCE_DIR}/libs")
include_directories("${PROJECT_BINARY_DIR}")
include_directories(SYSTEM ${Boost_INCLUDE_DIR} )

target_link_libraries (HashSolver
                       base
                       ${Boost_LIBRARIES}
                       ws2_32)


# unit test
set(PROJECT_TEST_NAME HashsolverTest)
file(GLOB TEST_SRC_FILES test/*.cpp)
include_directories(SYSTEM "${GTEST_INCLUDE_DIRS}")
include_directories(SYSTEM "${GMOCK_INCLUDE_DIRS}")
add_executable(${PROJECT_TEST_NAME} ${TEST_SRC_FILES})
add_dependencies(${PROJECT_TEST_NAME} googletest)
target_link_libraries(${PROJECT_TEST_NAME} base
                                           ${Boost_LIBRARIES}
                                           ws2_32
                                           gtest
                                           gtest_main)
enable_testing()
add_test(HashSolverTest ${PROJECT_TEST_NAME})

