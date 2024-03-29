
# Copyright (c) 2023 Vitaly Dikov
# 
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.20)

set(CMAKE_PROJECT_INCLUDE_BEFORE "CMakePre.cmake")

project("CMakeBuildBoostLib"  VERSION 1.0.0)


#**********************************************************************
# set common paths
set(EXTERNAL_PROJECTS_DIR   "${CMAKE_SOURCE_DIR}/.external")
set(EXEC_INSTALL_DIR        "${CMAKE_INSTALL_PREFIX}/EXEC")

#**********************************************************************
# set paths
set(BOOST_LIB_ROOT_DIR      "${EXTERNAL_PROJECTS_DIR}/boost")
set(BOOST_LIB_REPOS_DIR     "${BOOST_LIB_ROOT_DIR}/repos")
set(BOOST_LIB_REPOS_MIN_DIR "${BOOST_LIB_ROOT_DIR}/repos-minimal")
set(BOOST_LIB_BUILD_DIR     "${BOOST_LIB_ROOT_DIR}/build")
set(BOOST_LIB_FCSTUFF_DIR   "${BOOST_LIB_ROOT_DIR}/cmake-fetchcontent-stuff")
cmake_path(GET CMAKE_BINARY_DIR FILENAME CONFIG_NAME)
if (CONFIG_NAME)
  set(BOOST_LIB_INSTALL_DIR "${BOOST_LIB_ROOT_DIR}-install/${CONFIG_NAME}")
else()
  set(BOOST_LIB_INSTALL_DIR "${BOOST_LIB_ROOT_DIR}-install")
endif()

set(BOOST_LIB_INCLUDE_DIR   "${BOOST_LIB_INSTALL_DIR}/include")
set(BOOST_LIB_LIB_DIR       "${BOOST_LIB_INSTALL_DIR}/lib")

#**********************************************************************
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

# set bcp module-list
# vvv required modules vvv
set(BCP_MODULE_LIST         "build")
list(APPEND BCP_MODULE_LIST "bootstrap.bat")
list(APPEND BCP_MODULE_LIST "bootstrap.sh")
list(APPEND BCP_MODULE_LIST "boostcpp.jam")
list(APPEND BCP_MODULE_LIST "boost-build.jam")
list(APPEND BCP_MODULE_LIST "config")
# ^^^ required modules ^^^
# vvv optional modules vvv
list(APPEND BCP_MODULE_LIST "circular_buffer")
# ^^^ optional modules ^^^

#**********************************************************************
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

# run build and installation
if(${boost_POPULATED})
  file(GLOB B2_EXE_PATH "${BOOST_LIB_REPOS_DIR}/b2*")
  list(LENGTH B2_EXE_PATH B2_EXE_PATH_LENGTH)
  if (B2_EXE_PATH_LENGTH EQUAL 0)
    if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
      set(EXECUTE_PROCESS_COMMAND ${VCVARSALL_FILE} "x64" "&&" ".\\bootstrap.bat" "msvc")
    elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL GNU AND WIN32)
      set(EXECUTE_PROCESS_COMMAND ".\\bootstrap.bat" "gcc")
    elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL GNU AND LINUX)
      set(EXECUTE_PROCESS_COMMAND "./bootstrap.sh" "gcc")
    endif()
    execute_process(
      COMMAND           ${EXECUTE_PROCESS_COMMAND}
      WORKING_DIRECTORY ${BOOST_LIB_REPOS_DIR}
      COMMAND_ECHO      STDOUT
    )
  else()
    message(${MES_PREF_INFO} "\"${B2_EXE_PATH}\" already exists")
  endif()


  file(GLOB BCP_EXE_PATH "${BOOST_LIB_REPOS_DIR}/dist/bin/bcp*")
  list(LENGTH BCP_EXE_PATH BCP_EXE_PATH_LENGTH)
  if (BCP_EXE_PATH_LENGTH EQUAL 0)
    if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
      set(EXECUTE_PROCESS_COMMAND ${VCVARSALL_FILE} "x64" "&&" ".\\b2" "tools/bcp" "toolset=msvc")
    elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL GNU AND WIN32)
      set(EXECUTE_PROCESS_COMMAND ".\\b2" "tools/bcp" "toolset=gcc")
    elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL GNU AND LINUX)
      set(EXECUTE_PROCESS_COMMAND "./b2" "tools/bcp" "toolset=gcc")
    endif()
    execute_process(
      COMMAND           ${EXECUTE_PROCESS_COMMAND}
      WORKING_DIRECTORY ${BOOST_LIB_REPOS_DIR}
      COMMAND_ECHO      STDOUT
    )
  else()
    message(${MES_PREF_INFO} "\"${BCP_EXE_PATH}\" already exists")
  endif()


  # check bcp module-list for changes
  set(BCP_MODULE_LIST_HASH_FILENAME "bcp-module-list-hash")
  string(SHA256 BCP_MODULE_LIST_HASH_NEW "${BCP_MODULE_LIST}")
  file(MAKE_DIRECTORY ${BOOST_LIB_REPOS_MIN_DIR})
  file(TOUCH "${BOOST_LIB_REPOS_MIN_DIR}/${BCP_MODULE_LIST_HASH_FILENAME}")
  file(READ "${BOOST_LIB_REPOS_MIN_DIR}/${BCP_MODULE_LIST_HASH_FILENAME}" BCP_MODULE_LIST_HASH_OLD)
  string(COMPARE EQUAL "${BCP_MODULE_LIST_HASH_NEW}" "${BCP_MODULE_LIST_HASH_OLD}" BCP_MODULE_LIST_HASHES_ARE_EQUAL)
  if (NOT ${BCP_MODULE_LIST_HASHES_ARE_EQUAL})
    file(REMOVE_RECURSE ${BOOST_LIB_REPOS_MIN_DIR})
    file(MAKE_DIRECTORY ${BOOST_LIB_REPOS_MIN_DIR})
    file(WRITE "${BOOST_LIB_REPOS_MIN_DIR}/${BCP_MODULE_LIST_HASH_FILENAME}" "${BCP_MODULE_LIST_HASH_NEW}")
    if(WIN32)
      set(EXECUTE_PROCESS_COMMAND ".\\bcp" "--boost=${BOOST_LIB_REPOS_DIR}" ${BCP_MODULE_LIST} "${BOOST_LIB_REPOS_MIN_DIR}")
    elseif(LINUX)
      set(EXECUTE_PROCESS_COMMAND "./bcp" "--boost=${BOOST_LIB_REPOS_DIR}" ${BCP_MODULE_LIST} "${BOOST_LIB_REPOS_MIN_DIR}")
    endif()
    execute_process(
      COMMAND           ${EXECUTE_PROCESS_COMMAND}
      WORKING_DIRECTORY "${BOOST_LIB_REPOS_DIR}/dist/bin"
      COMMAND_ECHO      STDOUT
    )
  else()
    message(${MES_PREF_INFO} "bcp module-list has no changes")
  endif()


  file(MAKE_DIRECTORY ${BOOST_LIB_INSTALL_DIR})
  file(TOUCH "${BOOST_LIB_INSTALL_DIR}/${BCP_MODULE_LIST_HASH_FILENAME}")
  file(READ "${BOOST_LIB_INSTALL_DIR}/${BCP_MODULE_LIST_HASH_FILENAME}" BCP_MODULE_LIST_HASH_OLD)
  string(COMPARE EQUAL "${BCP_MODULE_LIST_HASH_NEW}" "${BCP_MODULE_LIST_HASH_OLD}" BCP_MODULE_LIST_HASHES_ARE_EQUAL)
  if(NOT ${BCP_MODULE_LIST_HASHES_ARE_EQUAL})
    file(REMOVE_RECURSE ${BOOST_LIB_INSTALL_DIR})
    file(MAKE_DIRECTORY ${BOOST_LIB_INSTALL_DIR})
    file(WRITE "${BOOST_LIB_INSTALL_DIR}/${BCP_MODULE_LIST_HASH_FILENAME}" "${BCP_MODULE_LIST_HASH_NEW}")
    if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
      set(EXECUTE_PROCESS_COMMAND_1 ${VCVARSALL_FILE} "x64" "&&" "bootstrap.bat" "msvc")
      set(EXECUTE_PROCESS_COMMAND_2 ${VCVARSALL_FILE} "x64" "&&" "bootstrap.bat" "msvc")
      set(EXECUTE_PROCESS_COMMAND_3 ${VCVARSALL_FILE} "x64" "&&" ".\\b2" "toolset=msvc" ${BOOST_LIB_BUILD_OPTIONS} ${BOOST_LIB_BUILD_PROPERTIES} "install")
    elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL GNU AND WIN32)
      set(EXECUTE_PROCESS_COMMAND_1 ".\\bootstrap.bat" "gcc")
      set(EXECUTE_PROCESS_COMMAND_2 ".\\bootstrap.bat" "gcc")
      set(EXECUTE_PROCESS_COMMAND_3 ".\\b2" "toolset=gcc" ${BOOST_LIB_BUILD_OPTIONS} ${BOOST_LIB_BUILD_PROPERTIES} "install")
    elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL GNU AND LINUX)
      set(EXECUTE_PROCESS_COMMAND_1 "./bootstrap.sh" "gcc")
      set(EXECUTE_PROCESS_COMMAND_2 "./bootstrap.sh" "gcc")
      set(EXECUTE_PROCESS_COMMAND_3 "./b2" "toolset=gcc" ${BOOST_LIB_BUILD_OPTIONS} ${BOOST_LIB_BUILD_PROPERTIES} "install")
    endif()
    execute_process(
      COMMAND           ${EXECUTE_PROCESS_COMMAND_1}
      WORKING_DIRECTORY "${BOOST_LIB_REPOS_MIN_DIR}/tools/build"
      COMMAND_ECHO      STDOUT
    )
    execute_process(
      COMMAND           ${EXECUTE_PROCESS_COMMAND_2}
      WORKING_DIRECTORY ${BOOST_LIB_REPOS_MIN_DIR}
      COMMAND_ECHO      STDOUT
    )
    execute_process(
      COMMAND           ${EXECUTE_PROCESS_COMMAND_3}
      WORKING_DIRECTORY ${BOOST_LIB_REPOS_MIN_DIR}
      COMMAND_ECHO      STDOUT
    )
  else()
    message(${MES_PREF_INFO} "Boost libraries are already installed")
  endif()
endif()

set(BOOST_INCLUDES "${BOOST_LIB_INCLUDE_DIR}")
if(WIN32)
  string(APPEND BOOST_INCLUDES "/boost-1_81")
endif()
file(GLOB_RECURSE BOOST_LIBS "${BOOST_LIB_LIB_DIR}")

#**********************************************************************
add_executable(main "src/main.cpp")

target_include_directories(main PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}"
                                PRIVATE "${BOOST_INCLUDES}"
)

target_link_libraries(main PRIVATE "${BOOST_LIBS}")

install(TARGETS main DESTINATION "${EXEC_INSTALL_DIR}")
