cmake_minimum_required(VERSION 3.0)

SET(LIBNOISE_VERSION "1.0.1")

OPTION(BUILD_SHARED_LIBS "Build shared libraries for libnoise" ON)
OPTION(BUILD_LIBNOISE_DOCUMENTATION "Create doxygen documentation for developers" OFF)
OPTION(BUILD_LIBNOISE_UTILS "Build utility functions for use with libnoise" ON)
OPTION(BUILD_LIBNOISE_EXAMPLES "Build libnoise examples" OFF)

project(libnoise VERSION ${LIBNOISE_VERSION})

ADD_SUBDIRECTORY(src)

IF (BUILD_LIBNOISE_UTILS)
    ADD_SUBDIRECTORY(noiseutils)
ENDIF ()
IF (BUILD_LIBNOISE_EXAMPLES)
    ADD_SUBDIRECTORY(examples)
ENDIF ()
IF (BUILD_LIBNOISE_DOCUMENTATION)
    ADD_SUBDIRECTORY(doc)
ENDIF ()


#ADD_SUBDIRECTORY(samples)
