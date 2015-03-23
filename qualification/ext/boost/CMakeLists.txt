

if(NOT BOOST_ROOT)
    SET (BOOST_ROOT "${PROJECT_SOURCE_DIR}/ext/ext/boost_1_56_0")
    message("BOOST_ROOT set to ${BOOST_ROOT}")
endif(NOT BOOST_ROOT)


set(Boost_USE_STATIC_LIBS   ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_NO_ZLIB 0)

find_package( Boost 1.56 COMPONENTS
                chrono
                date_time
                iostreams
                log_setup
                log
                filesystem
                program_options
                regex
                signals
                serialization
                system
                thread
                zlib
                REQUIRED )

set(Boost_LIBRARIES ${Boost_LIBRARIES} PARENT_SCOPE)

