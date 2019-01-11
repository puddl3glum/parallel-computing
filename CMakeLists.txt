cmake_minimum_required(VERSION 3.10)
project(conway)

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

  target_link_libraries(conway SDL2)

else()
  add_executable(conway
    ${MAIN_SOURCE_DIR}/main.c
    ${MAIN_SOURCE_DIR}/game.c
    )

endif()

# gcc and clang specific flags
MESSAGE(STATUS "CMAKE_C_COMPILER_ID : " ${CMAKE_C_COMPILER_ID})
if (${CMAKE_C_COMPILER_ID} MATCHES "GNU")
  target_compile_options(conway PUBLIC
    "$<$<CONFIG:RELEASE>:-O3>"
    "$<$<CONFIG:DEBUG>:-Og>"
    "$<$<CONFIG:DEBUG>:-gdwarf>"
    "$<$<CONFIG:DEBUG>:-DDEBUG>"
    "-Wall"
    "-Wextra"
    "-Wshadow"
    )
elseif (${CMAKE_C_COMPILER_ID} MATCHES "Clang")
  target_compile_options(conway PUBLIC
    "$<$<CONFIG:RELEASE>:-O3>"
    "$<$<CONFIG:DEBUG>:-O0>"
    "-Weverything"
    "-Wno-c++98-compat"
    "-Wno-c++98-compat-pedantic"
    "-Wno-gnu-zero-variadic-macro-arguments"
    "-Wno-weak-vtables"
    "-fno-omit-frame-pointer"
    "-fsanitize=integer"
    "-fsanitize=undefined"
    "-fno-sanitize=vptr"
    )
endif ()
