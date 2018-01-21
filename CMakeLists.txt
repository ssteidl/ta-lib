CMAKE_MINIMUM_REQUIRED(VERSION 3.5.1)

PROJECT(tcltalib)

find_library(TCL_LIBRARY NAMES tcl86 tcl8.6)

include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)
ExternalProject_Add(ta-lib
  PREFIX ta-lib
  GIT_REPOSITORY git@github.com:ssteidl/ta-lib.git
  INSTALL_DIR ${CMAKE_BINARY_DIR}/chroot
)

include_directories(${SHANE_PREFIX}/include/ ${TCL_INCLUDE_PATH})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
link_directories(${SHANE_PREFIX}/lib)

# add_library(tclta SHARED 
#                   tcltalib.cpp 
#                   tcl_obj_utils.cpp)

		
# target_link_libraries(tclta ta_lib)

