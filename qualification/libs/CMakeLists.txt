

if (CMAKE_COMPILER_IS_GNUCXX)
    set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wcast-qual" )
    set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wlogical-op" )
    set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wno-aggressive-loop-optimizations")
    set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wwrite-strings" )
endif()


add_subdirectory (base)

