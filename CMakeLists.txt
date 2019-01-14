cmake_minimum_required(VERSION 3.10)

set(PROJECT conway)

project(${PROJECT})

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE release)
endif()

MESSAGE(STATUS "BUILDING FOR ${CMAKE_BUILD_TYPE} VERSION")

set(BIN_DIR ${PROJECT_SOURCE_DIR}/bin)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR}/debug)
else()
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})
endif()

set(MAIN_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)


if(CMAKE_BUILD_TYPE MATCHES "Debug")

  add_executable(conway
    ${MAIN_SOURCE_DIR}/main.c
    ${MAIN_SOURCE_DIR}/game.c
    ${MAIN_SOURCE_DIR}/draw.c
    )

  # target_link_libraries(${PROJECT} SDL2)
  set(PROGRAM_LIBS SDL2)

else()
  add_executable(${PROJECT}
    ${MAIN_SOURCE_DIR}/main.c
    ${MAIN_SOURCE_DIR}/game.c
    )

endif()

# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")
# target_link_libraries(${PROJECT} OpenMP)

# gcc and clang specific flags
MESSAGE(STATUS "CMAKE_C_COMPILER_ID : " ${CMAKE_C_COMPILER_ID})
if (${CMAKE_C_COMPILER_ID} MATCHES "GNU")
  target_compile_options(${PROJECT} PUBLIC
    "$<$<CONFIG:RELEASE>:-O3>"
    "$<$<CONFIG:DEBUG>:-Og>"
    "$<$<CONFIG:DEBUG>:-gdwarf>"
    "$<$<CONFIG:DEBUG>:-DDEBUG>"
    "-Wall"
    "-Wextra"
    "-Wshadow"
    )
elseif (${CMAKE_C_COMPILER_ID} MATCHES "Clang")
  target_compile_options(${PROJECT} PUBLIC
    "$<$<CONFIG:RELEASE>:-O3>"
    "$<$<CONFIG:DEBUG>:-O0>"
    "$<$<CONFIG:DEBUG>:-Og>"
    "$<$<CONFIG:DEBUG>:-gdwarf>"
    "$<$<CONFIG:DEBUG>:-DDEBUG>"
    "-Weverything"
    "-Wno-gnu-zero-variadic-macro-arguments"
    "-Wno-weak-vtables"
    "-fno-omit-frame-pointer"
    "-fsanitize=integer"
    "-fsanitize=undefined"
    "-fno-sanitize=vptr"
    )
endif ()

target_link_libraries(${PROJECT}
  ${PROGRAM_LIBS}
  "$<$<C_COMPILER_ID:Clang>:-fsanitize=integer>"
  "$<$<C_COMPILER_ID:Clang>:-fsanitize=undefined>"
  "$<$<C_COMPILER_ID:Clang>:-fsanitize=vptr>"
  )

