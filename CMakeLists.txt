
# Copyright (c) 2023 Vitaly Dikov
# 
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.20)

set(CMAKE_PROJECT_INCLUDE_BEFORE "CMakePre.cmake")

project("CMakeBuildBoostLib"  VERSION 0.2.2)

option(ENABLE_TESTS "Enable tests" OFF)

# message("~~ [INFO] ~~ Before BCP_MODULE_LIST_LENGTH: ${BCP_MODULE_LIST_LENGTH}")
# if(BCP_MODULE_LIST_LENGTH)
#   set(BCP_MODULE_LIST_LENGTH FALSE CACHE BOOL "" FORCE)
# else()
#   set(BCP_MODULE_LIST_LENGTH TRUE CACHE BOOL "" FORCE)
# endif()
# message("~~ [INFO] ~~ After BCP_MODULE_LIST_LENGTH: ${BCP_MODULE_LIST_LENGTH}")
#set(BCP_MODULE_LIST_QQQ TRUE CACHE BOOL "" FORCE)

# if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
#   file(GLOB_RECURSE CL_EXE_LIST "C:/Program Files*/cl.exe")
#   list(LENGTH CL_EXE_LIST CL_EXE_LIST_LENGTH)
#   message("~~ [INFO] ~~ CL_EXE_LIST: ${CL_EXE_LIST}")
#   # set(CMAKE_C_COMPILER "" CACHE STRING "" FORCE)
#   # set(CMAKE_CXX_COMPILER "" CACHE STRING "" FORCE)
# endif()


set(EXTERNAL_PROJECTS_DIR "${CMAKE_SOURCE_DIR}/.external")

#**********************************************************************
set(EXEC__BIN_DIR "${CMAKE_BINARY_DIR}/src")
set(EXEC__INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/EXEC")

#**********************************************************************
if(ENABLE_TESTS)
  # set paths
  set(GOOGLETEST_LIB_ROOT_DIR      "${EXTERNAL_PROJECTS_DIR}/googletest")
  set(GOOGLETEST_LIB_REPOS_DIR     "${GOOGLETEST_LIB_ROOT_DIR}/repos")
  set(GOOGLETEST_LIB_FCSTUFF_DIR   "${GOOGLETEST_LIB_ROOT_DIR}/cmake-fetchcontent-stuff")

  # download and unpack GOOGLETEST library
  include(FetchContent)
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY  https://github.com/google/googletest.git
    GIT_TAG         "release-1.12.1"
    PREFIX          ${GOOGLETEST_LIB_FCSTUFF_DIR}
    SOURCE_DIR      ${GOOGLETEST_LIB_REPOS_DIR}
  )

  # Prevent overriding the parent project's compiler/linker
  # settings on Windows
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

  FetchContent_MakeAvailable(googletest)

  enable_testing()

  add_subdirectory("test")
endif()



set(MES_PREF "~~ [INFO] ~~ ")

message(${MES_PREF} "CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")
message(${MES_PREF} "CMAKE_MAKE_PROGRAM: ${CMAKE_MAKE_PROGRAM}")
message(${MES_PREF} "CMAKE_TOOLCHAIN_FILE: ${CMAKE_TOOLCHAIN_FILE}")
message(${MES_PREF} "CMAKE_GENERATOR_TOOLSET: ${CMAKE_GENERATOR_TOOLSET}")
file(GLOB_RECURSE VCVARSALL_LIST "C:/Program Files*/vcvarsall.bat")
list(LENGTH VCVARSALL_LIST VCVARSALL_LIST_LENGTH)
if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
  if(${VCVARSALL_LIST_LENGTH} EQUAL 1)
    message(${MES_PREF} "VCVARSALL_LIST: ${VCVARSALL_LIST}")
  else()
    message(FATAL_ERROR "\"vcvarsall.bat\" not found")
  endif()
endif()



cmake_path(GET CMAKE_BINARY_DIR FILENAME CONFIG_NAME)

# set paths
set(BOOST_LIB_ROOT_DIR    "${EXTERNAL_PROJECTS_DIR}/boost")
set(BOOST_LIB_REPOS_DIR   "${BOOST_LIB_ROOT_DIR}/repos")
set(BOOST_LIB_BUILD_DIR   "${BOOST_LIB_ROOT_DIR}/build")
set(BOOST_LIB_FCSTUFF_DIR "${BOOST_LIB_ROOT_DIR}/cmake-fetchcontent-stuff")
if (CONFIG_NAME)
  set(BOOST_LIB_INSTALL_DIR "${BOOST_LIB_ROOT_DIR}-install/${CONFIG_NAME}")
else()
  set(BOOST_LIB_INSTALL_DIR "${BOOST_LIB_ROOT_DIR}-install")
endif()

set(BOOST_LIB_INCLUDE_DIR "${BOOST_LIB_INSTALL_DIR}/include")
set(BOOST_LIB_LIB_DIR     "${BOOST_LIB_INSTALL_DIR}/lib")

# set script options and commands for the build
set(BOOST_LIB_BUILD_OPTIONS         "--includedir=${BOOST_LIB_INCLUDE_DIR}")
list(APPEND BOOST_LIB_BUILD_OPTIONS "--libdir=${BOOST_LIB_LIB_DIR}")
list(APPEND BOOST_LIB_BUILD_OPTIONS "--build-dir=${BOOST_LIB_BUILD_DIR}")
list(APPEND BOOST_LIB_BUILD_OPTIONS "--build-type=minimal")
list(APPEND BOOST_LIB_BUILD_OPTIONS "--no-cmake-config")

set(BOOST_LIB_BUILD_PROPERTIES            "address-model=64")
list(APPEND BOOST_LIB_BUILD_PROPERTIES    "link=static")
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  list(APPEND BOOST_LIB_BUILD_PROPERTIES  "variant=debug")
else()
  list(APPEND BOOST_LIB_BUILD_PROPERTIES  "variant=release")
endif()

set(BCP_MODULE_LIST         "build")
list(APPEND BCP_MODULE_LIST "bootstrap.bat")
list(APPEND BCP_MODULE_LIST "bootstrap.sh")
list(APPEND BCP_MODULE_LIST "boostcpp.jam")
list(APPEND BCP_MODULE_LIST "boost-build.jam")
list(APPEND BCP_MODULE_LIST "config")
list(APPEND BCP_MODULE_LIST "bimap")
list(APPEND BCP_MODULE_LIST "circular_buffer")
# list(LENGTH BCP_MODULE_LIST BCP_MODULE_LIST_LENGTH_NEW)
# set(BCP_MODULE_LIST_LENGTH_OLD "${BCP_MODULE_LIST_LENGTH}")
# if (NOT ${BCP_MODULE_LIST_LENGTH_NEW} STREQUAL ${BCP_MODULE_LIST_LENGTH_OLD})
#   set(BCP_MODULE_LIST_LENGTH ${BCP_MODULE_LIST_LENGTH_NEW} CACHE STRING "" FORCE)
#   set(REINSTALL_BOOST TRUE)
# else()
#   set(REINSTALL_BOOST FALSE)
# endif()
# message(${MES_PREF} "REINSTALL_BOOST: ${REINSTALL_BOOST}")
string(SHA256 BCP_MODULE_LIST_HASH_NEW "${BCP_MODULE_LIST}")
set(BCP_MODULE_LIST_HASH_OLD "${BCP_MODULE_LIST_HASH}")
string(COMPARE EQUAL "${BCP_MODULE_LIST_HASH_NEW}" "${BCP_MODULE_LIST_HASH_OLD}" BCP_MODULE_LIST_HASHES_ARE_EQUAL)
if (NOT ${BCP_MODULE_LIST_HASHES_ARE_EQUAL})
  set(BCP_MODULE_LIST_HASH ${BCP_MODULE_LIST_HASH_NEW} CACHE STRING "" FORCE)
  set(REINSTALL_BOOST TRUE)
else()
  set(REINSTALL_BOOST FALSE)
endif()
message(${MES_PREF} "REINSTALL_BOOST: ${REINSTALL_BOOST}")
message(${MES_PREF} "BCP_MODULE_LIST_HASH: ${BCP_MODULE_LIST_HASH}")


file(MAKE_DIRECTORY "${BOOST_LIB_INSTALL_DIR}")
file(GLOB BOOST_LIB_LIST "${BOOST_LIB_LIB_DIR}/*boost*")
list(LENGTH BOOST_LIB_LIST BOOST_LIB_LIST_LENGTH)

# download and unpack BOOST library
include(FetchContent)
FetchContent_Declare(
  boost
  GIT_REPOSITORY  https://github.com/boostorg/boost.git
  GIT_TAG         "boost-1.81.0"
  PREFIX          ${BOOST_LIB_FCSTUFF_DIR}
  SOURCE_DIR      ${BOOST_LIB_REPOS_DIR}
)
set(FETCHCONTENT_QUIET OFF CACHE BOOL "" FORCE)

FetchContent_GetProperties(boost)
if(NOT boost_POPULATED)
  FetchContent_Populate(boost)
endif()

message(${MES_PREF} "boost_POPULATED: ${boost_POPULATED}")
message(${MES_PREF} "BOOST_LIB_LIST_LENGTH: ${BOOST_LIB_LIST_LENGTH}")
# run build and installation
# if(${boost_POPULATED} AND NOT ${BOOST_LIB_LIST_LENGTH})
if(${boost_POPULATED})
  message(${MES_PREF} "CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")
  message(${MES_PREF} "BOOST_LIB_BUILD_OPTIONS: ${BOOST_LIB_BUILD_OPTIONS}")
  message(${MES_PREF} "BOOST_LIB_BUILD_PROPERTIES: ${BOOST_LIB_BUILD_PROPERTIES}")
  if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    message(${MES_PREF} "EXECUTE MSVC")
    # execute_process(
    #   COMMAND           ${VCVARSALL_LIST} "x64" "&&" "bootstrap.bat"
    #   WORKING_DIRECTORY ${BOOST_LIB_REPOS_DIR}
    #   COMMAND_ECHO      STDOUT
    # )
    # execute_process(
    #   COMMAND           ${VCVARSALL_LIST} "x64" "&&" "b2" ${BOOST_LIB_BUILD_OPTIONS} ${BOOST_LIB_BUILD_PROPERTIES} "install"
    #   WORKING_DIRECTORY ${BOOST_LIB_REPOS_DIR}
    #   COMMAND_ECHO      STDOUT
    # )

    file(GLOB B2_EXE_PATH "${BOOST_LIB_REPOS_DIR}/b2*")
    list(LENGTH B2_EXE_PATH B2_EXE_PATH_LENGTH)
    if (B2_EXE_PATH_LENGTH EQUAL 0)
      execute_process(
        COMMAND           ${VCVARSALL_LIST} "x64" "&&" "bootstrap.bat"
        WORKING_DIRECTORY ${BOOST_LIB_REPOS_DIR}
        COMMAND_ECHO      STDOUT
      )
    else()
      message(${MES_PREF} "${B2_EXE_PATH} is exists")
    endif()

    file(GLOB BCP_EXE_PATH "${BOOST_LIB_REPOS_DIR}/dist/bin/bcp*")
    list(LENGTH BCP_EXE_PATH BCP_EXE_PATH_LENGTH)
    if (BCP_EXE_PATH_LENGTH EQUAL 0)
      execute_process(
        COMMAND           ${VCVARSALL_LIST} "x64" "&&" ".\\b2" "tools/bcp"
        WORKING_DIRECTORY ${BOOST_LIB_REPOS_DIR}
        COMMAND_ECHO      STDOUT
      )
    else()
      message(${MES_PREF} "${BCP_EXE_PATH} is exists")
    endif()

    if(${REINSTALL_BOOST})
      file(MAKE_DIRECTORY "${BOOST_LIB_REPOS_DIR}-minimal")
      execute_process(
        COMMAND           ${VCVARSALL_LIST} "x64" "&&" ".\\bcp" "--boost=${BOOST_LIB_REPOS_DIR}" ${BCP_MODULE_LIST} "${BOOST_LIB_REPOS_DIR}-minimal"
        WORKING_DIRECTORY "${BOOST_LIB_REPOS_DIR}/dist/bin"
        COMMAND_ECHO      STDOUT
      )
      execute_process(
        COMMAND           ${VCVARSALL_LIST} "x64" "&&" "bootstrap.bat"
        WORKING_DIRECTORY "${BOOST_LIB_REPOS_DIR}-minimal/tools/build"
        COMMAND_ECHO      STDOUT
      )
      execute_process(
        COMMAND           ${VCVARSALL_LIST} "x64" "&&" "bootstrap.bat"
        WORKING_DIRECTORY "${BOOST_LIB_REPOS_DIR}-minimal"
        COMMAND_ECHO      STDOUT
      )
      execute_process(
        COMMAND           ${VCVARSALL_LIST} "x64" "&&" ".\\b2" ${BOOST_LIB_BUILD_OPTIONS} ${BOOST_LIB_BUILD_PROPERTIES} "install"
        WORKING_DIRECTORY "${BOOST_LIB_REPOS_DIR}-minimal"
        COMMAND_ECHO      STDOUT
      )
    endif()
  elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
    message(${MES_PREF} "EXECUTE GNU")
    set(BOOST_LIB_B2_BUILD_DIR "${BOOST_LIB_REPOS_DIR}/boost-build")
    # execute_process(
    #   COMMAND           "bootstrap.bat" "gcc"
    #   WORKING_DIRECTORY "${BOOST_LIB_REPOS_DIR}/tools/build"
    #   COMMAND_ECHO      STDOUT
    # )
    # execute_process(
    #   COMMAND           "b2" "--prefix=${BOOST_LIB_B2_BUILD_DIR}" "install"
    #   WORKING_DIRECTORY "${BOOST_LIB_REPOS_DIR}/tools/build"
    #   COMMAND_ECHO      STDOUT
    # )
    # execute_process(
    #   COMMAND           "${BOOST_LIB_B2_BUILD_DIR}/b2" ${BOOST_LIB_BUILD_OPTIONS} ${BOOST_LIB_BUILD_PROPERTIES} "toolset=gcc" "install"
    #   WORKING_DIRECTORY ${BOOST_LIB_REPOS_DIR}
    #   COMMAND_ECHO      STDOUT
    # )
    execute_process(
      COMMAND           "bootstrap.bat" "gcc"
      WORKING_DIRECTORY "${BOOST_LIB_REPOS_DIR}/tools/build"
      COMMAND_ECHO      STDOUT
    )
    execute_process(
      COMMAND           ".\\b2" "--prefix=${BOOST_LIB_B2_BUILD_DIR}" "install"
      WORKING_DIRECTORY "${BOOST_LIB_REPOS_DIR}/tools/build"
      COMMAND_ECHO      STDOUT
    )
    execute_process(
      COMMAND           ".\\b2" "tools/bcp" #"toolset=gcc"
      WORKING_DIRECTORY ${BOOST_LIB_B2_BUILD_DIR}
      COMMAND_ECHO      STDOUT
    )
    execute_process(
      COMMAND           ".\\bcp" "--boost=${BOOST_LIB_REPOS_DIR}" "circular_buffer" "chrono" "${BOOST_LIB_INSTALL_DIR}"
      WORKING_DIRECTORY "${BOOST_LIB_REPOS_DIR}/dist/bin"
      COMMAND_ECHO      STDOUT
    )
  endif()
endif()

#**********************************************************************
add_executable(hello "src/hello.cpp")

install(TARGETS hello DESTINATION "${EXEC__INSTALL_DIR}")
