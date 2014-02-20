project(KnuthCAM)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
add_definitions("-std=c++11")
#set (CMAKE_VERBOSE_MAKEFILE true)
find_package(GLM REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})

find_library(DXFLIB dxflib REQUIRED)

set(SRC_LIST main.cpp glview.cpp ../contour.cpp ../dxfparser.cpp ../easygl.cpp ../svg.cpp)

set(wxWidgets_CONFIGURATION mswu)
find_package(wxWidgets COMPONENTS core base gl REQUIRED)
include(${wxWidgets_USE_FILE})

if(APPLE)
add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${SRC_LIST})
else(APPLE)
add_executable(${PROJECT_NAME} ${SRC_LIST})
endif(APPLE)

target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES} ${DXFLIB})
