CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
SET(projname video_saver)
PROJECT(${projname})

FILE(GLOB_RECURSE headers
	"src/*.h"
)

FILE(GLOB_RECURSE sources
	"src/*.cpp"
)

SET(directories
	src
	src/video_saver
	src/video_saver/http
)

SET(dependencies
    common
    video_segmenter
)

GROUP_FILES("${directories}")

INCLUDE_DIRECTORIES(src)

ADD_LIBRARY(${projname} ${headers} ${sources})

FOREACH(dep ${dependencies})
    INCLUDE_DIRECTORIES("../${dep}/src")
ENDFOREACH()
TARGET_LINK_LIBRARIES(${projname} ${dependencies})

FIND_PACKAGE(CURL)
INCLUDE_DIRECTORIES(${CURL_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(${projname} ${CURL_LIBRARY})

FIND_PACKAGE(Boost REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

IF (MSVC)
	SET_TARGET_PROPERTIES(${projname} PROPERTIES LINK_FLAGS "/SAFESEH:NO")
ENDIF()
