cmake_minimum_required(VERSION 3.30)
project(NetCore)

SET(CMAKE_C_STANDARD 99)
SET(CMAKE_C_STANDARD_REQUIRED TRUE)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(TEST_DIR ${CMAKE_SOURCE_DIR}/testing)

SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${INCLUDE_DIR})

set(MAIN_SOURCE_FILES
    ${SRC_DIR}/main.c
    ${SRC_DIR}/tuntap.c
    ${SRC_DIR}/utils.c
    ${SRC_DIR}/vn_device.c
    ${SRC_DIR}/sk_buff.c
)

set(TEST_SOURCE_FILES
    ${TEST_DIR}/test_double_linked_list.c
    ${TEST_DIR}/test_sk_buff.c
)

# main executable
add_executable(${PROJECT_NAME} ${MAIN_SOURCE_FILES})

# main
add_custom_target(run
    COMMAND ${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMENT "Build and run NetCore"
)

# Find all test files dynamically in the `testing` directory
file(GLOB TEST_FILES "${TEST_DIR}/*.c")

message(STATUS "Test files found: ${TEST_FILES}")

# Collect all test target names
set(TEST_TARGETS)

foreach(TEST_FILE ${TEST_FILES})
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    set(UNIQUE_TEST_NAME test_${TEST_NAME})
    add_executable(${UNIQUE_TEST_NAME} ${TEST_FILE} ${SRC_DIR}/sk_buff.c ${SRC_DIR}/tuntap.c ${SRC_DIR}/utils.c)
    list(APPEND TEST_TARGETS ${UNIQUE_TEST_NAME})
endforeach()


# Add run_tests custom target to depend on all test executables
add_custom_target(run_tests
    COMMENT "Running all tests..."
)

foreach(TEST_TARGET ${TEST_TARGETS})
    add_dependencies(run_tests ${TEST_TARGET})  # Ensure each test is built
    add_custom_command(
        TARGET run_tests
        COMMAND $<TARGET_FILE:${TEST_TARGET}>  # Run the test
        COMMENT "Running ${TEST_TARGET}..."
    )
endforeach()

