# Locate Elementum Shared library
# This module defines
#  ESHARED_FOUND, if false, do not try to link to EShared
#  ESHARED_LIBRARIES
#  ESHARED_INCLUDE_DIR, where to find Pixy.h
#

SET(EShared_PATHS
  /home/kandie/Workspace/Projects/Elementum/Shared
  /Volumes/kandiehack/Workspace/Projects/Elementum/Shared
  /Users/kandie/Workspace/Projects/Elementum/Shared
)

FIND_PATH(ESHARED_INCLUDE_DIRS Pixy.h
  HINTS
  PATH_SUFFIXES include
  PATHS
  ${EShared_PATHS}
)

FIND_LIBRARY(ESHARED_LIBRARY
  NAMES EShared
  HINTS
  PATH_SUFFIXES lib
  PATHS
  ${EShared_PATHS}
)

FIND_LIBRARY(ESHARED_MODULES_LIBRARY
  NAMES EShared_Modules
  HINTS
  PATH_SUFFIXES lib
  PATHS
  ${EShared_PATHS}
)

IF(ESHARED_LIBRARY AND ESHARED_MODULES_LIBRARY)
  FIND_LIBRARY(LUA_MATH_LIBRARY m)
  SET( ESHARED_LIBRARIES "${ESHARED_LIBRARY};${ESHARED_MODULES_LIBRARY}" CACHE STRING "Elementum shared Libraries")
  SET( ESHARED_INCLUDE_DIRS "${ESHARED_INCLUDE_DIRS};${ESHARED_INCLUDE_DIRS}/../modules/include" )
  MESSAGE(STATUS "Found Elementum shared libraries")
  MESSAGE(STATUS "Includes: ${ESHARED_INCLUDE_DIRS}")
ELSE()
  MESSAGE(STATUS "Error! Could not find Elementum shared libraries")
ENDIF()

#INCLUDE(${CMAKE_CURRENT_LIST_DIR}/FindPackageHandleStandardArgs.cmake)
# handle the QUIETLY and REQUIRED arguments and set LUA_FOUND to TRUE if
# all listed variables are TRUE
#FIND_PACKAGE_HANDLE_STANDARD_ARGS(EShared  DEFAULT_MSG  ESHARED_LIBRARIES ESHARED_INCLUDE_DIRS)

SET(ESHARED_LIBRARY_DIR ${EShared_PATHS})
MARK_AS_ADVANCED(ESHARED_FOUND ESHARED_INCLUDE_DIRS ESHARED_LIBRARY_DIR ESHARED_LIBRARIES ESHARED_LIBRARY ESHARED_MODULES_LIBRARY)
