CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
SET(projname segmenter_py)
PROJECT(${projname})

FILE(GLOB_RECURSE headers
	"src/*.h"
)

FILE(GLOB_RECURSE sources
	"src/*.cpp"
)

SET(directories
	src
	src/segmenter_python_api
)

SET(dependencies
    common
    video_saver
    video_segmenter
    video_streams_manager
)

GROUP_FILES("${directories}")

INCLUDE_DIRECTORIES(src)

SET(CMAKE_SHARED_LIBRARY_PREFIX_CXX "lib")
SET(CMAKE_SHARED_LIBRARY_SUFFIX_CXX ".so")
ADD_LIBRARY(${projname} SHARED ${headers} ${sources})

FOREACH(dep ${dependencies})
    INCLUDE_DIRECTORIES("../${dep}/src")
ENDFOREACH()
TARGET_LINK_LIBRARIES(${projname} ${dependencies})

FIND_PACKAGE(Boost COMPONENTS python REQUIRED)
SET (Boost_USE_STATIC_LIBS ON) 
SET (Boost_MULTITHREADED ON)
SET (Boost_USE_STATIC_RUNTIME ON)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

FIND_PACKAGE(PythonLibs REQUIRED)
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(${projname} ${Boost_LIBRARIES} ${PYTHON_LIBRARY})

IF (MSVC)
	SET_TARGET_PROPERTIES(${projname} PROPERTIES LINK_FLAGS "/SAFESEH:NO")
ENDIF()

INSTALL(TARGETS ${projname}
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib
)

ADD_CUSTOM_COMMAND(TARGET ${projname}
	           POST_BUILD
		   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${projname}> "${CMAKE_SOURCE_DIR}/../../web_api/")
