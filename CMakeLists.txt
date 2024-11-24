cmake_minimum_required(VERSION 3.30)
project(NetCore)

SET(CMAKE_C_STANDARD 99)
SET(CMAKE_C_STANDARD_REQUIRED TRUE)

include_directories(include)

set(SOURCES
    src/main.c
    src/tuntap.c
)

add_executable(${PROJECT_NAME} ${SOURCES})

add_custom_target(run
    COMMAND ${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMENT "Build and run NetCore"
)
