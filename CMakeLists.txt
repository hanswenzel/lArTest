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
if(WITH_GEANT4_UIVIliblArTestClassesDict.soS)
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


#----------------------------------------------------------------------------
# Find ROOT (required package)
#
find_package(ROOT REQUIRED)

# ROOT version 6 required
if(ROOT_FOUND)
  STRING(REGEX MATCH "6.*" VERSION6MATCH ${ROOT_VERSION})
  if(NOT VERSION6MATCH)
    message(FATAL_ERROR "lArTest requires ROOT 6")
  endif()
endif()

# Include ROOT's CMake functions for dictionary generation
include("${ROOT_DIR}/modules/RootNewMacros.cmake")

#----------------------------------------------------------------------------
# P01 requires shared libraries
#
if(NOT Geant4_shared_FOUND)
  message(FATAL_ERROR "lArTest must use shared libraries")
endif()





message(STATUS "${ROOT_USE_FILE}=${${ROOT_USE_FILE}}")

include_directories(${CMAKE_CURRENT_LIST_DIR}/include
                   /home/wenzel/boost_1_63_0  
                    ${Geant4_INCLUDE_DIR}
                    ${ROOT_INCLUDE_DIRS})
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
#----------------------------------------------------------------------------
# Generate dictionaries, add ROOT libraries properties
#
REFLEX_GENERATE_DICTIONARY(lArTestClasses include/lArTestClasses.hh SELECTION xml/selection.xml)
add_library(lArTestClassesDict SHARED ${sources} lArTestClasses.cxx)
set(libsuffix .so)
set(ROOT_LIBRARY_PROPERTIES ${ROOT_LIBRARY_PROPERTIES} SUFFIX ${libsuffix})
set_target_properties(lArTestClassesDict PROPERTIES ${ROOT_LIBRARY_PROPERTIES})
target_link_libraries(lArTestClassesDict  ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})


link_directories( /home/wenzel/boost_1_63_0/stage/lib
                  ${ROOT_LIBRARY_DIR} )



#add_library(lAr SHARED  ${sources} )

set(libsuffix .so)

set(ROOT_LIBRARY_PROPERTIES ${ROOT_LIBRARY_PROPERTIES}
      SUFFIX ${libsuffix})

SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)


add_executable(${name} ${name}.cc)
target_link_libraries(${name}  lArTestClassesDict  ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})


add_executable(readHits readHits.cc )
target_link_libraries(readHits lArTestClassesDict ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

#add_custom_target( ${name} DEPENDS ${name} readHits )
install (TARGETS ${name} DESTINATION bin)
install (TARGETS  readHits DESTINATION bin)
install(TARGETS lArTestClassesDict DESTINATION lib)
#install (TARGETS lAr DESTINATION lib)
install(FILES ${PROJECT_BINARY_DIR}/lArTestClasses_rdict.pcm DESTINATION lib)
install (FILES vis.mac init_vis.mac muons.in muons_tiny.in profile.in protodune.gdml lArBox.gdml tinysed.gdml tinylArBox_color.gdml materials.xml lArDet_split.gdml SimpleExtension.xsd scripts/Ana.C DESTINATION bin)
