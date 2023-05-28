# Find sources
set(GAME_SOURCE_DIR "${PROJECT_SOURCE_DIR}/src/Game")
file(GLOB_RECURSE GAME_SOURCES_FILES ${GAME_SOURCE_DIR}/*.c* ${GAME_SOURCE_DIR}/*.h* ${GAME_SOURCE_DIR}/*.txt)

# Create targets
add_library(Game STATIC ${GAME_SOURCES_FILES} ${GLAD_SOURCE_DIR}/glad.c)
add_dependencies(Game Utils)

# Setup tab sources
foreach(_source IN ITEMS ${GAME_SOURCES_FILES})
    get_filename_component(_source_path "${_source}" PATH)
    file(RELATIVE_PATH _source_path_rel "${GAME_SOURCE_DIR}" "${_source_path}")
    string(REPLACE "/" "\\" _group_path "${_source_path_rel}")
    source_group("${_group_path}" FILES "${_source}")
endforeach()

# Properties
set_target_properties(Game PROPERTIES FOLDER "Game")
set_target_properties(Game PROPERTIES OUTPUT_NAME "Game")
set_target_properties(Game PROPERTIES LINK_FLAGS "${IGNORE_WARNINGS}") # Missing pdb

# Includes
target_include_directories(Game PUBLIC "${SOURCE_DIR}")
target_include_directories(Game PUBLIC "${EXTERN_DIR}")
target_include_directories(Game PUBLIC "${GLAD_INCLUDE_DIR}")
target_include_directories(Game PUBLIC "${SDL_INCLUDE_DIR}")
target_include_directories(Game PUBLIC "${GAME_SOURCE_DIR}")
target_include_directories(Game PUBLIC "${UPLAY_INCLUDE_DIR}")
target_include_directories(Game PUBLIC "${CMAKE_BINARY_DIR}/_cmrc/include") # Bundle resources
target_include_directories(Game PUBLIC "${MESSAGES_DIR}/build")

# Options
target_compile_options(Game PRIVATE "$<$<CONFIG:Debug>:/WX>") # Warning as error on debug (MSVC)
target_compile_definitions(Game PRIVATE GAME_GIT_HASH="${GAME_GIT_HASH}") # Version
