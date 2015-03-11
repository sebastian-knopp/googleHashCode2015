
set(BASE_SRC_FILES_PATTERN
        *.h
        *.hpp
        detail/*.h
        detail/*.hpp
        src/*.cpp
        src/*.h
        src/*.hpp

# Needed for the external dbg library to obtain stacktrace programatically
        src/ext/dbg/include/*.*
        src/ext/dbg/src/*.*
        src/ext/dbg/src/*.*
        src/ext/dbg/src/windows/*.*
)

if (MSVC)
    set(BASE_SRC_FILES_PATTERN ${BASE_SRC_FILES_PATTERN} src/ext/dbg/src/windows/msvc/*.*)
else()
    set(BASE_SRC_FILES_PATTERN ${BASE_SRC_FILES_PATTERN} src/ext/dbg/src/windows/mingw/*.*)
endif()

file(GLOB BASE_SRC_FILES ${BASE_SRC_FILES_PATTERN})




add_library(base ${BASE_SRC_FILES})


include_directories("${PROJECT_SOURCE_DIR}/libs/base")
include_directories(SYSTEM ${Boost_INCLUDE_DIR} )
include_directories(SYSTEM "${GTEST_INCLUDE_DIRS}")
include_directories(SYSTEM "${GMOCK_INCLUDE_DIRS}")

# Needed for the external dbg library to obtain stacktrace programatically
include_directories(SYSTEM "${PROJECT_SOURCE_DIR}/libs/base/src/ext")
include_directories(SYSTEM "${PROJECT_SOURCE_DIR}/libs/base/src/ext/dbg/include")
include_directories(SYSTEM "${PROJECT_SOURCE_DIR}/libs/base/src/ext/dbg/src")
include_directories(SYSTEM "${PROJECT_SOURCE_DIR}/libs/base/src/ext/dbg/src/windows")
include_directories(SYSTEM "${PROJECT_SOURCE_DIR}/libs/base/src/ext/dbg/src/windows/mingw")


set(PROJECT_TEST_NAME baseTest)


file(GLOB TEST_SRC_FILES test/*.cpp)
add_executable(${PROJECT_TEST_NAME} ${TEST_SRC_FILES})
add_dependencies(${PROJECT_TEST_NAME} googletest)


target_link_libraries(${PROJECT_TEST_NAME} base
                                           ${Boost_LIBRARIES}
                                           ws2_32
                                           gtest
                                           gtest_main
                                           gmock)


enable_testing()
add_test(baseTest ${PROJECT_TEST_NAME})

