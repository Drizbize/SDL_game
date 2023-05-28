cmake_minimum_required(VERSION 3.16)

project(Game C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_SUPPRESS_REGENERATION true)   # remove 'ZERO_CHECK' target

if(NOT CMAKE_MSVC_RUNTIME_LIBRARY)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# lib extern
set(EXTERN_DIR ${PROJECT_SOURCE_DIR}/extern)
set(SDL_BIN_DIR ${EXTERN_DIR}/sdl/bin)
set(SDL_LIB_DIR ${EXTERN_DIR}/sdl/lib)
set(SDL_INCLUDE_DIR ${EXTERN_DIR}/sdl/include)
set(GLAD_INCLUDE_DIR ${EXTERN_DIR}/glad/include)
set(GLAD_SOURCE_DIR ${EXTERN_DIR}/glad/src)

# Preproc macro
add_compile_definitions(UNICODE)    # for windows.h (mostly)

# Package resources
set(GAME_OUTPUT_PREFIX ${PROJECT_SOURCE_DIR}/output/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${GAME_OUTPUT_PREFIX}/lib) # For static libraries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${GAME_OUTPUT_PREFIX}/bin) # For exe files

# Create output directory, filled with dlls and resources
set(GAME_RUNTIME ${PROJECT_SOURCE_DIR}/output/${CMAKE_BUILD_TYPE}/bin/)

file(MAKE_DIRECTORY ${GAME_RUNTIME})

file(INSTALL ${SDL_BIN_DIR} DESTINATION ${GAME_RUNTIME})

# Subs
add_subdirectory(src)