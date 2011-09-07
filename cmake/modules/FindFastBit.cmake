# - Find FastBit
# Find the FastBit includes and library
# This module defines
#  FASTBIT_INCLUDE_DIR, where to find ibis.h
#  FASTBIT_LIBRARIES, the libraries needed to use FASTBIT.
#  FASTBIT_FOUND, If false, do not try to use FASTBIT.
#
# Copyright (c) 2011, Philipp Fehre, <philipp.fehre@googlemail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

# Add the fastbit include paths here

if(FASTBIT_INCLUDE_DIR AND FASTBIT_LIBRARIES)
   set(FASTBIT_FOUND TRUE)

else(FASTBIT_INCLUDE_DIR AND FASTBIT_LIBRARIES)

	find_path(FASTBIT_INCLUDE_DIR ibis.h
	    /usr/local/include
      )

  find_library(FASTBIT_LIBRARIES NAMES fastbit libfastbit
     PATHS
     /usr/lib
     /usr/local/lib
     )
      
  if(FASTBIT_INCLUDE_DIR AND FASTBIT_LIBRARIES)
    set(FASBIT_FOUND TRUE)
    message(STATUS "Found FastBit: ${FASTBIT_INCLUDE_DIR}, ${FASTBIT_LIBRARIES}")
    INCLUDE_DIRECTORIES(${FASTBIT_INCLUDE_DIR})
  else(FASTBIT_INCLUDE_DIR AND FASTBIT_LIBRARIES)
    set(FASTBIT_FOUND FALSE)
    message(STATUS "FastBit not found.")
  endif(FASTBIT_INCLUDE_DIR AND FASTBIT_LIBRARIES)

  mark_as_advanced(FASTBIT_INCLUDE_DIR FASTBIT_LIBRARIES)

endif(FASTBIT_INCLUDE_DIR AND FASTBIT_LIBRARIES)
