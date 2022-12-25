# Sets the minimum version of CMake required to build your native library.
# This ensures that a certain set of CMake features is available to
# your build.

cmake_minimum_required(VERSION 3.14.3)

project(calculator C CXX ASM)
enable_language(ASM)

cmake_policy(SET CMP0115 OLD)

if(HEXAGON_SDK_ROOT)
    include(${HEXAGON_SDK_ROOT}/build/cmake/hexagon_fun.cmake)
else()
    include(${HEXAGON_CMAKE_ROOT}/hexagon_fun.cmake)
endif()

# Base Include dirs for the Project
set(common_incs
    ${CMAKE_CURRENT_SOURCE_DIR}/src/
    ${CMAKE_CURRENT_BINARY_DIR}/
    ${HEXAGON_SDK_ROOT}/incs/
    ${HEXAGON_SDK_ROOT}/incs/stddef/
    ${HEXAGON_SDK_ROOT}/utils/examples
)

# Setting NO_QURT_INC flag.
# This will be used when building for Hexagon to avoid linking to QuRT libraries.
set(NO_QURT_INC 1)

if(${OS_TYPE} MATCHES "HLOS")

    #################################################
    # HLOS Dynamic Stub Library
    #################################################
    add_library(calculator SHARED ${CMAKE_CURRENT_BINARY_DIR}/calculator_stub)

    #################################################
    # HLOS Executable
    #################################################
    add_executable(calculator_device
        ${CMAKE_CURRENT_SOURCE_DIR}/src/calculator_main
        ${CMAKE_CURRENT_SOURCE_DIR}/src/calculator_test
        ${HEXAGON_SDK_ROOT}/utils/examples/dsp_capabilities_utils
        ${HEXAGON_SDK_ROOT}/utils/examples/pd_status_notification
    )

    if (NOT ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        set_target_properties(calculator_device PROPERTIES OUTPUT_NAME "calculator")
    endif()

    # Add compile definitions to the target
    target_compile_definitions(calculator_device PUBLIC VERIFY_PRINT_ERROR)

    # Add common HLOS-specific linker options
    link_options(calculator_device)

    # Add include directories to the build
    include_directories(${common_incs})

    # Build interface files using user IDL
    build_idl(inc/calculator.idl calculator)

    # Find the DSP from provided domain value, default is CDSP
    choose_dsprpc(${DSP_TYPE} dsprpc)

    # Link target with HLOS-specific custom libraries
    link_custom_library(calculator_device rpcmem)
    link_custom_library(calculator_device getopt_custom)

    link_custom_library(calculator ${dsprpc})
    link_custom_library(calculator_device ${dsprpc})
    # Mark the dependencies for target
    add_dependencies(calculator_device calculator)

    # Link the libs and the binary
    target_link_libraries(calculator_device calculator)

    # Copy binaries from ship to CMAKE_CURRENT_BINARY_DIR folder
    copy_binaries(calculator_device calculator)

else()

    #################################################
    # Hexagon Dynamic Skel Library
    #################################################
    set(srcs
        ${CMAKE_CURRENT_BINARY_DIR}/calculator_skel
        ${CMAKE_CURRENT_SOURCE_DIR}/src/calculator_imp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/version
    )

    add_library(calculator_skel_OBJS
        OBJECT
        ${srcs}
    )

    add_library(calculator_skel SHARED $<TARGET_OBJECTS:calculator_skel_OBJS>)

    # Set CXX Flags
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions")

    # Add include directories to the build
    include_directories(${common_incs})

    #################################################
    # Hexagon Simulator Test Executable
    #################################################
    add_executable(calculator_q
        ${CMAKE_CURRENT_SOURCE_DIR}/src/calculator_test_main
        ${CMAKE_CURRENT_SOURCE_DIR}/src/calculator_imp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/version
        ${CMAKE_CURRENT_SOURCE_DIR}/src/calculator_test
        ${HEXAGON_SDK_ROOT}/utils/examples/dsp_capabilities_utils
        ${HEXAGON_SDK_ROOT}/utils/examples/pd_status_notification
    )

    #Build static skel library for building simulator test
    add_library(calculator_skel_static STATIC $<TARGET_OBJECTS:calculator_skel_OBJS>)

    # Build interface files using user IDL
    build_idl("inc/calculator.idl" calculator_skel_OBJS)

    include_directories(${common_incs})

    # Link target with custom libraries

    # rtld, test_util and atomic should be linked to simulator
    # test if it is run as a non-QURT based test
    if(NO_QURT_INC EQUAL 1)
        link_custom_library(calculator_q test_util)
        link_custom_library(calculator_q atomic)
        link_custom_library(calculator_q rtld)
    endif()

    link_custom_library(calculator_q rpcmem)

    # Link target with dependencies
    add_dependencies(calculator_q calculator_skel_static)
    target_link_libraries(calculator_q calculator_skel_static)

    # Copy binaries from ship to CMAKE_CURRENT_BINARY_DIR folder
    copy_binaries(calculator_q calculator_skel)

    runHexagonSim(calculator_q)

endif()

# vim: set noet fenc=utf-8 ff=unix ft=cmake :
