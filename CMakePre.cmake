set(MES_PREF_NOTICE "** [NOTICE] ** ")
set(MES_PREF_INFO   "** [INFO] ** ")
set(MES_PREF_VARS   "** [VARS] ** ")


if (WIN32 AND ${CMAKE_C_COMPILER} STREQUAL "cl" AND ${CMAKE_CXX_COMPILER} STREQUAL "cl")
  message(${MES_PREF_NOTICE} "If you get the error message \"Compiler not found\", run the CMake executable after running \"vcvarsall.bat\".")
  file(GLOB_RECURSE VCVARSALL_LIST "C:/Program Files*/vcvarsall.bat")
  list(POP_BACK VCVARSALL_LIST VCVARSALL_FILE)
  if(VCVARSALL_FILE)
    message(${MES_PREF_INFO} "File \"vcvarsall.bat\" found")
    message(${MES_PREF_VARS} "VCVARSALL_FILE: \"${VCVARSALL_FILE}\"")
  else()
    message(${MES_PREF_INFO} "File \"vcvarsall.bat\" not found")
  endif()
endif()
