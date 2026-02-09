cmake_minimum_required(VERSION 3.22)
project(DICOMPractice
    VERSION 0.1
    DESCRIPTION "Dicom practice project"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True) 
set(CMAKE_CXX_EXTENSIONS OFF)

option(USE_DCMTK "Use DCMTK library for DICOM" ON)

if(USE_DCMTK)
    find_package(DCMTK)
    if(DCMTK_FOUND)
        message(STATUS "Found DCMTK: ${DCMTK_INCLUDE_DIRS}")
        add_compile_definitions(HAS_DCMTK)
    else()
        message(STATUS "DCMTK not found.")
        message(STATUS "Install DCMTK: vcpkg install dcmtk OR apt-get install libdcmtk-dev")
    endif()
endif()

add_executable(dicompractice
    src/main.cpp
)




message(STATUS "")
message(STATUS "=== DICOMPractice Configuration Summary ===")
message(STATUS "Version:        ${PROJECT_VERSION}")
message(STATUS "Build type:     ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard:   ${CMAKE_CXX_STANDARD}")
message(STATUS "DCMTK:          ${DCMTK_FOUND}")
message(STATUS "============================================")
message(STATUS "")