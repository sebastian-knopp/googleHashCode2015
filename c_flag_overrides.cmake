
if(MSVC)
        set(CMAKE_C_FLAGS_DEBUG_INIT "/D_DEBUG /MDd /Zi /Ob0 /Od /RTC1")
        set(CMAKE_C_FLAGS_MINSIZEREL_INIT     "/MD /O1 /Ob1 /D NDEBUG")
        set(CMAKE_C_FLAGS_RELEASE_INIT        "/MD /O2 /Ob2 /D NDEBUG")
        set(CMAKE_C_FLAGS_RELWITHDEBINFO_INIT "/MD /Zi /O2 /Ob1 /D NDEBUG")
endif()

