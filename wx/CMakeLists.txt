project(KnuthCAM)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
add_definitions("-std=c++11")
add_definitions("-DSRCDIR=\"${PROJECT_SOURCE_DIR}\"") 
#ADD_CUSTOM_COMMANDset (CMAKE_VERBOSE_MAKEFILE true)
find_package(GLM REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})

find_library(DXFLIB dxflib REQUIRED)

set(SRC_LIST
main.cpp
glview.cpp glview.hpp
mainframe.cpp mainframe.hpp
../contour.cpp ../contour.hpp
../dxfparser.cpp ../dxfparser.hpp
../easygl.cpp ../easygl.hpp
../svg.cpp ../svg.hpp
../knuthcam.hpp
../seg.cpp ../seg.hpp
)

set(wxWidgets_CONFIGURATION mswu)
find_package(wxWidgets 3.0 COMPONENTS core base gl REQUIRED)
include(${wxWidgets_USE_FILE})

if(APPLE)
add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${SRC_LIST})
else(APPLE)
add_executable(${PROJECT_NAME} ${SRC_LIST})
endif(APPLE)

target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES} ${DXFLIB})
