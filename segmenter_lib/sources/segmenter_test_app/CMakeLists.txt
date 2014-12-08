CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
SET(projname segmenter_test_app)
PROJECT(${projname})

FILE(GLOB_RECURSE headers
	"src/*.h"
)

FILE(GLOB_RECURSE sources
	"src/*.cpp"
)

SET(directories
	src
)

GROUP_FILES("${directories}")

SET(dependencies
    common
	video_streams_manager
	video_segmenter
	video_saver
)

FOREACH(dep ${dependencies})
	INCLUDE_DIRECTORIES("../${dep}/src")
ENDFOREACH()

INCLUDE_DIRECTORIES (
    ${FFMPEG_LIBAVCODEC_INCLUDE_DIRS}
    ${FFMPEG_LIBAVFORMAT_INCLUDE_DIRS}
    ${FFMPEG_LIBSWSCALE_INCLUDE_DIRS}
#   ${FFMPEG_LIBAVUTIL_INCLUDE_DIRS}
    "${FFMPEG_ROOT}/include"
)

ADD_EXECUTABLE(${projname} ${headers} ${sources})

TARGET_LINK_LIBRARIES( ${projname}
	${FFMPEG_LIBAVCODEC_LIBRARIES}
	${FFMPEG_LIBAVFORMAT_LIBRARIES}
	${FFMPEG_LIBAVUTIL_LIBRARIES}
	${FFMPEG_LIBSWSCALE_LIBRARIES}
	${FFMPEG_LIBAVDEVICE_LIBRARIES}
)

TARGET_LINK_LIBRARIES( ${projname} ${dependencies})

FIND_PACKAGE(Boost COMPONENTS iostreams REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(${projname} ${Boost_LIBRARIES})

IF (MSVC)
	SET_TARGET_PROPERTIES(${projname} PROPERTIES LINK_FLAGS "/SAFESEH:NO")
ENDIF()

INSTALL(TARGETS ${projname}
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib)

INSTALL(FILES ${CMAKE_SOURCE_DIR}/configs.json DESTINATION bin)

add_custom_command(TARGET ${projname} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                    ${CMAKE_SOURCE_DIR}/configs.json $<TARGET_FILE_DIR:${projname}>)
