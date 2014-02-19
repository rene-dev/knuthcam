project(KnuthCAM)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)
 
set(wxWidgets_CONFIGURATION mswu)
find_package(wxWidgets COMPONENTS core base gl REQUIRED)
include(${wxWidgets_USE_FILE})

# create bundle on apple compiles
if(APPLE)
add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${SRC_LIST})
else(APPLE)
add_executable(${PROJECT_NAME} ${SRC_LIST})
endif(APPLE)

target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})
