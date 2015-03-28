

cmake_minimum_required(VERSION 2.8)



project (HashCodeA211 CXX)


file(GLOB HASHCODESOLVER_SOURCES *.h
                                 *.hpp
                                 *.cpp)


if (CMAKE_COMPILER_IS_GNUCXX)
    set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11" )
    set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Werror -Wextra -Wall" )
    set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wcast-qual" )
    set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wlogical-op" )
    set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wno-aggressive-loop-optimizations" )
    set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wwrite-strings" )
    set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wredundant-decls" )
    set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wdisabled-optimization" )
endif()


add_executable(hashCodeSolver ${HASHCODESOLVER_SOURCES} )



