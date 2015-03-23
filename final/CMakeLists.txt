

cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0020 NEW)


project (HashCodeA211 CXX)


file(GLOB HASHCODESOLVER_SOURCES *.h
                                 *.hpp
                                 *.cpp)

add_executable(hashCodeSolver ${HASHCODESOLVER_SOURCES} )



