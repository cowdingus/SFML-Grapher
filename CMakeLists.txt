cmake_minimum_required(VERSION 3.16.3)

project(SFML_Grapher)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_executable("${PROJECT_NAME}" ${SOURCES})

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

target_compile_features(
	"${PROJECT_NAME}" PUBLIC
	cxx_std_17
	)

target_include_directories(
	"${PROJECT_NAME}" PUBLIC
	include/
	)

target_link_libraries(
	"${PROJECT_NAME}" PUBLIC
	sfml-graphics
	sfml-window
	sfml-system
	)
