CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
SET(projname common)
PROJECT(${projname})

FILE(GLOB_RECURSE headers
	"src/*.h"
)

FILE(GLOB_RECURSE sources
	"src/*.cpp"
)

SET(directories
	src
	src/common
	src/common/concurrent
)

GROUP_FILES("${directories}")

INCLUDE_DIRECTORIES(src)

ADD_LIBRARY(${projname} ${headers} ${sources})


FIND_PACKAGE(Boost REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

IF (MSVC)
	SET_TARGET_PROPERTIES(${projname} PROPERTIES LINK_FLAGS "/SAFESEH:NO")
ENDIF()
