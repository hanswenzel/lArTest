cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

set(name lArTest)
#set(Boost_NO_BOOST_CMAKE  ON)
project(${name})

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
add_definitions(-DG4ANALYSIS_USE) 

include(${Geant4_USE_FILE})


#find_package(Boost REQUIRED)
#if(NOT BOOST_FOUND)
#  message(STATUS "lArTest: BOOST package not found. --> lArTest disabled")
#  return()
#endif()

find_package(ROOT REQUIRED)
if(NOT ROOT_FOUND)
  message(STATUS "lArTest: ROOT package not found. --> lArTest disabled")
  return()
endif()

message(STATUS "${ROOT_USE_FILE}=${${ROOT_USE_FILE}}")

include_directories(${CMAKE_CURRENT_LIST_DIR}/include
                   /home/wenzel/boost_1_63_0  
                    ${Geant4_INCLUDE_DIR}
                    ${ROOT_INCLUDE_DIR})

link_directories( /home/wenzel/boost_1_63_0/stage/lib
                  ${ROOT_LIBRARY_DIR} )


file(GLOB sources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc )


add_library(lAr SHARED  ${sources} )

set(libsuffix .so)

set(ROOT_LIBRARY_PROPERTIES ${ROOT_LIBRARY_PROPERTIES}
      SUFFIX ${libsuffix})

SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)


add_executable(${name} ${name}.cc)

target_link_libraries(${name}  lAr  ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})


install (TARGETS ${name} DESTINATION bin)
install (TARGETS lAr DESTINATION lib)
install (FILES vis.mac init_vis.mac muons.in profile.in protodune.gdml lArBox.gdml scripts/Ana.C DESTINATION bin)
