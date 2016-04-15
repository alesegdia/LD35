cmake_minimum_required(VERSION 2.6)
project(LD35)

add_subdirectory(module/alligator/)

list(APPEND CMAKE_CXX_FLAGS "-std=c++11 -g -ftest-coverage -fprofile-arcs")

file(GLOB_RECURSE CPPs src/**/*.cpp)
file(GLOB_RECURSE HPPs src/**/*.h)

include_directories(module/alligator/src)

if(WIN32)
	set(EXECUTABLE_OUTPUT_PATH ${CMAKE_HOME_DIRECTORY}/bin/win32)
	list(APPEND CMAKE_EXE_LINKER_FLAGS "-static-libstdc++ -static-libgcc")
	add_executable(ld35-w32 ${CPPs} ${HPPs} src/main.cpp)
	target_link_libraries(ld35-w32 alligator)
	target_link_libraries(ld35-w32 allegro-5.0.10-monolith-mt)
elseif(UNIX)
	set(EXECUTABLE_OUTPUT_PATH ${CMAKE_HOME_DIRECTORY}/bin/linux)
	add_executable(ld35 ${CPPs} ${HPPs} src/main.cpp)
	include_directories(ld35 ${ALLIGATOR_INCLUDES})
	target_link_libraries(ld35 alligator)
	target_link_libraries(ld35 allegro)
	target_link_libraries(ld35 allegro_image)
	target_link_libraries(ld35 allegro_primitives)
	target_link_libraries(ld35 allegro_font)
	target_link_libraries(ld35 allegro_ttf)
	target_link_libraries(ld35 allegro_audio)
	target_link_libraries(ld35 allegro_acodec)
endif()


