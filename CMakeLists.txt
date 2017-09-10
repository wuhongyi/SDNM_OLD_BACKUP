# $Id: CMakeLists.txt 68058 2013-03-13 14:47:43Z gcosmo $

#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
#set(CMAKE_C_COMPILER "/usr/local/bin/gcc")  #调用自己安装在local的gcc
#set(CMAKE_CXX_COMPILER "/usr/local/bin/g++") #调用自己安装在local的g++
set(PROJ_NAME wu)
project(${PROJ_NAME})
set(CMAKE_CXX_FLAGS "-std=c++11 -Wno-deprecated") #支持最新c++11
#c++0x是新的C++意思。而-Wno-deprecated是表示 no-deprecated这类警告忽略。
ADD_DEFINITIONS(-DG4PHPDEBUG)
SET(CMAKE_BUILD_TYPE Debug) #编译调试版本
#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()
#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})
#----------------------------------------------------------------------------
# Find ROOT (required package)
#
find_package(ROOT REQUIRED)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
include_directories(${PROJECT_SOURCE_DIR}/include ${Geant4_INCLUDE_DIR} ${ROOT_INCLUDE_DIR}) #
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc   )
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh )


# ROOT_GENERATE_DICTIONARY(wuDict ${PROJECT_SOURCE_DIR}/include/Event.hh  LINKDEF ${PROJECT_SOURCE_DIR}/include/wuG4LinkDef.h)  
# add_library(wuDict SHARED wuDict.cxx)#生成动态链接库
# set(libsuffix .so)
# set(ROOT_LIBRARY_PROPERTIES ${ROOT_LIBRARY_PROPERTIES} SUFFIX ${libsuffix})
# set_target_properties(wuDict PROPERTIES ${ROOT_LIBRARY_PROPERTIES})
# target_link_libraries(wuDict  ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} )

#
#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#

add_executable(${PROJ_NAME} main.cc ${sources} ${headers})
target_link_libraries(${PROJ_NAME} ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})#

# add_executable(${PROJ_NAME} main.cc ${sources} ${headers})
# target_link_libraries(${PROJ_NAME} wuDict ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})#

# add_executable(readHits readHits.cc  ${sources} ${headers})
# target_link_libraries(readHits wuDict ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} ${libs} )
#
#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build B1. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(EXAMPLEB1_SCRIPTS
   autorun.sh
   ReadData.txt
   init.mac
   init_vis.mac
   vis.mac
   wu.in
   wu.out
   run8core.sh
  )

foreach(_script ${EXAMPLEB1_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()


