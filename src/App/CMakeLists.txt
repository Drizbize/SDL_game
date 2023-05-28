# Find sources
set(APP_SOURCE_DIR "${PROJECT_SOURCE_DIR}/src/App")
file(GLOB_RECURSE APP_SOURCES_FILES ${APP_SOURCE_DIR}/*.c* ${APP_SOURCE_DIR}/*.h* ${APP_SOURCE_DIR}/*.txt)

# Create targets
add_executable(App ${APP_SOURCES_FILES})

# Dependencies
add_dependencies(App Game)
add_dependencies(App Utils)

# Setup tab sources
foreach(_source IN ITEMS ${APP_SOURCES_FILES})
    get_filename_component(_source_path "${_source}" PATH)
    file(RELATIVE_PATH _source_path_rel "${APP_SOURCE_DIR}/" "${_source_path}")
    string(REPLACE "/" "\\" _group_path "${_source_path_rel}")
    source_group("${_group_path}" FILES "${_source}")
endforeach()

# Properties
set_target_properties(App PROPERTIES FOLDER "App")
set_target_properties(App PROPERTIES OUTPUT_NAME "App")
set_target_properties(App PROPERTIES LINK_FLAGS "${IGNORE_WARNINGS}") # Missing pdb

# Includes
target_include_directories(App PUBLIC "${PROJECT_SOURCE_DIR}/src/Game")
target_include_directories(App PUBLIC "${PROJECT_SOURCE_DIR}/src/Utils")
target_include_directories(App PUBLIC "${APP_SOURCE_DIR}")

# Links
target_link_directories(
	App PUBLIC 
	${CMAKE_LIBRARY_OUTPUT_DIRECTORY} 
)

target_link_libraries(App Utils Game)
target_link_libraries(App ${SDL_LIB_DIR}/SDL2.lib)

add_custom_command(
        TARGET App POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${GAME_RUNTIME}/bin/SDL2.dll
                ${GAME_OUTPUT_PREFIX}/bin/SDL2.dll)