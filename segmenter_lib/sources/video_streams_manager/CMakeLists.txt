CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
SET(projname video_streams_manager)
PROJECT(${projname})

FILE(GLOB_RECURSE headers
	"src/*.h"
)

FILE(GLOB_RECURSE sources
	"src/*.cpp"
)

SET(directories
	src
	src/video_streams_manager
	src/video_streams_manager/configs
)

SET(dependencies
    common
    video_saver
    video_segmenter
)

GROUP_FILES("${directories}")

INCLUDE_DIRECTORIES(src)

ADD_LIBRARY(${projname} ${headers} ${sources})

FOREACH(dep ${dependencies})
    INCLUDE_DIRECTORIES("../${dep}/src")
ENDFOREACH()
TARGET_LINK_LIBRARIES(${projname} ${dependencies})

IF (MSVC)
	SET_TARGET_PROPERTIES(${projname} PROPERTIES LINK_FLAGS "/SAFESEH:NO")
ENDIF()

FIND_PACKAGE(Boost REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
