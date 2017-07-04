#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(Yield_Validation)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
  find_package(MPI REQUIRED)
else()
  find_package(Geant4 REQUIRED)
endif()
  set(CMAKE_CXX_COMPILER mpicxx)
  include_directories(${MPI_INCLUDE_PATH})

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)


SET(Geant4_LIBRARIES ${Geant4_LIBRARIES})
ADD_LIBRARY(mpi SHARED IMPORTED)
SET_PROPERTY(TARGET mpi APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
SET_TARGET_PROPERTIES(mpi PROPERTIES
#  IMPORTED_LOCATION_RELEASE "${Geant4_DIR}/../libmpi.so"
  IMPORTED_SONAME_RELEASE "libmpi.so"
)
#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(Yield_Validation Yield_Validation.cc ${sources} ${headers})
target_link_libraries(Yield_Validation ${Geant4_LIBRARIES})

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build the project. This is so that we can run the executable directly 
# because it relies on these scripts being in the current working directory.
#


set(Profile_SCRIPTS
  vis.mac
  run.mac
  )


foreach(_script ${INITSET_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# For internal Geant4 use - but has no effect if you build this
# example standalone
#
#add_custom_target(Yield_Validation DEPENDS Yield_Validation)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS Yield_Validation DESTINATION bin)


