

cmake_minimum_required(VERSION 2.8)



project (HashCodeA211 CXX)


file(GLOB HASHCODESOLVER_SOURCES *.h
                                 *.hpp
                                 *.cpp)


set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11" )

add_executable(hashCodeSolver ${HASHCODESOLVER_SOURCES} )



