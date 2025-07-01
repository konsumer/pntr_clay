# This will build the example

project(pntr_clay_example)

cmake_minimum_required(VERSION 3.18)

include(FetchContent)

if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
  set(CMAKE_POLICY_DEFAULT_CMP0135 NEW)
endif()

FetchContent_Declare(pntr URL https://github.com/RobLoach/pntr/archive/refs/heads/master.zip)
FetchContent_MakeAvailable(pntr)

FILE(GLOB SRC_FILES src/*.c)
add_executable(${PROJECT_NAME} ${SRC_FILES})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} pntr)


IF (EMSCRIPTEN)
  SET(CMAKE_EXECUTABLE_SUFFIX ".mjs")
  TARGET_LINK_OPTIONS(${PROJECT_NAME} PRIVATE -sASSERTIONS=1 -sASYNCIFY -sALLOW_MEMORY_GROWTH=1)
  add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE_DIR:${PROJECT_NAME}>/${PROJECT_NAME}.wasm" "${CMAKE_SOURCE_DIR}/docs/${PROJECT_NAME}.wasm"
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE_DIR:${PROJECT_NAME}>/${PROJECT_NAME}.mjs" "${CMAKE_SOURCE_DIR}/docs/${PROJECT_NAME}.mjs"
  )
ENDIF()