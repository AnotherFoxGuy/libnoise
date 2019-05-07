SET(PROJECT_NAME libnoiseutils)

IF (BUILD_SHARED_LIBS)
    add_library(noiseutils SHARED noiseutils.cpp)
ELSE ()
    add_library(noiseutils STATIC noiseutils.cpp)
ENDIF ()

SET_TARGET_PROPERTIES(noiseutils PROPERTIES LIBNOISE_VERSION ${LIBNOISE_VERSION})

target_link_libraries(noiseutils noise)

# I would like to see more projects using these defaults
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    ADD_DEFINITIONS("-Wall -ansi -pedantic -O3")
endif ()

# Where to look for noise headers
ADD_DEFINITIONS("-I${PROJECT_SOURCE_DIR}/src")

# install include files into /usr/include
INSTALL(FILES "${PROJECT_SOURCE_DIR}/noiseutils/noiseutils.h" DESTINATION
        "${CMAKE_INSTALL_PREFIX}/include/noise")

# install libraries into /lib
INSTALL(TARGETS noiseutils DESTINATION "${CMAKE_INSTALL_PREFIX}/lib")
