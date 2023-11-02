# Find sources
set(UTILS_SOURCE_DIR "${PROJECT_SOURCE_DIR}/src/Utils")
file(GLOB_RECURSE UTILS_SOURCES_FILES ${UTILS_SOURCE_DIR}/*.c* ${UTILS_SOURCE_DIR}/*.h* ${UTILS_SOURCE_DIR}/*.txt)

# Create targets
add_library(Utils STATIC ${UTILS_SOURCES_FILES})

# Setup tab sources
foreach(_source IN ITEMS ${UTILS_SOURCES_FILES})
    get_filename_component(_source_path "${_source}" PATH)
    file(RELATIVE_PATH _source_path_rel "${UTILS_SOURCE_DIR}" "${_source_path}")
    string(REPLACE "/" "\\" _group_path "${_source_path_rel}")
    source_group("${_group_path}" FILES "${_source}")
endforeach()

# Properties
set_target_properties(Utils PROPERTIES FOLDER "Utils")
set_target_properties(Utils PROPERTIES OUTPUT_NAME "Utils")
set_target_properties(Utils PROPERTIES LINK_FLAGS "${IGNORE_WARNINGS}") # Missing pdb

# Includes
target_include_directories(Utils PUBLIC "${SOURCE_DIR}")
target_include_directories(Utils PUBLIC "${UTILS_SOURCE_DIR}")
target_include_directories(Utils PUBLIC "${UTILS_SOURCE_DIR}/glm")

# Options
target_compile_options(Utils PRIVATE "$<$<CONFIG:Debug>:/WX>") # Warning as error on debug (MSVC)
target_compile_definitions(Utils PRIVATE GAME_GIT_HASH="${GAME_GIT_HASH}") # Version
