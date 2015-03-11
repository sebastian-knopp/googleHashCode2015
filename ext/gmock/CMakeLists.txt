
execute_process(
  COMMAND ${CMAKE_COMMAND} -E tar xzf ${EXT_SOURCE_DIR}/gmock-1.7.0.zip
  WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
)

set(gtest_force_shared_crt ON CACHE INTERNAL "")
set(gtest_disable_pthreads ON CACHE INTERNAL "")

add_subdirectory(${PROJECT_BINARY_DIR}/gmock-1.7.0 bin)

set(GMOCK_INCLUDE_DIRS ${PROJECT_BINARY_DIR}/gmock-1.7.0/include PARENT_SCOPE)

# gmock includes gtest
set(GTEST_INCLUDE_DIRS ${PROJECT_BINARY_DIR}/gmock-1.7.0/gtest/include PARENT_SCOPE)

