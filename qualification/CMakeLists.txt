cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0020 NEW)


set(CMAKE_USER_MAKE_RULES_OVERRIDE
   ${CMAKE_CURRENT_SOURCE_DIR}/c_flag_overrides.cmake)
set(CMAKE_USER_MAKE_RULES_OVERRIDE_CXX
   ${CMAKE_CURRENT_SOURCE_DIR}/cxx_flag_overrides.cmake)


project (GoogleHashCode2015 CXX)
enable_testing()


#set overall version number
set (Version 1.0.0)


# determine git revision number
find_package(Git)
execute_process(COMMAND ${GIT_EXECUTABLE} describe --always
                WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                OUTPUT_VARIABLE GIT_REVISION )
STRING(REGEX REPLACE "(\r?\n)+$" "" GIT_REVISION "${GIT_REVISION}")


# determine compiler version
if (CMAKE_COMPILER_IS_GNUCXX)
    execute_process(COMMAND ${CMAKE_CXX_COMPILER} --version
                    OUTPUT_VARIABLE COMPILER_VERSION_INFO )
    STRING(REGEX REPLACE "(\r?\n)+.*$" "" COMPILER_VERSION_INFO "${COMPILER_VERSION_INFO}")
else()
    execute_process(COMMAND ${CMAKE_CXX_COMPILER}
                    ERROR_VARIABLE COMPILER_VERSION_INFO )
    STRING(REGEX REPLACE "(\r?\n)+.*$" "" COMPILER_VERSION_INFO "${COMPILER_VERSION_INFO}")
endif()


if(CMAKE_BUILD_TYPE MATCHES RELEASE)
    set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -s" )
else()
#    set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -D_GLIBCXX_DEBUG" )
#    set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -D_GLIBCXX_DEBUG_ASSERT" )
#    set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -D_GLIBCXX_DEBUG_PEDASSERT" )
endif()


if (CMAKE_SYSTEM_NAME MATCHES Windows)
  set(EXT_SOURCE_DIR $ENV{USERPROFILE}/Dropbox/ext-src)
else()
  set(EXT_SOURCE_DIR $ENV{HOME}/Dropbox/ext-src)
endif()


if (MSVC)
    set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS" )
endif()


set(EXT_PROJECTS_DIR ${PROJECT_SOURCE_DIR}/ext)
add_subdirectory(${EXT_PROJECTS_DIR}/boost)
add_subdirectory(${EXT_PROJECTS_DIR}/gmock)


if (CMAKE_COMPILER_IS_GNUCXX)
    set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11 -std=gnu++0x" )
    set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Werror -Wextra -Wall" )
else()
    set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /W3 /WX" )
endif()


add_subdirectory (libs)
add_subdirectory (tools)

