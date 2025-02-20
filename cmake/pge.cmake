# OpenGL
set(OpenGL_GL_PREFERENCE LEGACY)
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${OpenGL_LIBRARIES} OpenGL::GL)

# if (NOT HAS_TERMINAL)
# target_link_libraries(${PROJECT_NAME} -mwindows -municode)
# endif (NOT HAS_TERMINAL)

# GDI+
set(GDIPLUS_LIBRARY gdiplus)
target_link_libraries(${PROJECT_NAME} ${GDIPLUS_LIBRARY})

# Shlwapi
set(SHLWAPI_LIBRARY shlwapi)
target_link_libraries(${PROJECT_NAME} ${SHLWAPI_LIBRARY})

# Dwmapi
set(DWMAPI_LIBRARY dwmapi)
target_link_libraries(${PROJECT_NAME} ${DWMAPI_LIBRARY})

if(NOT USE_SDL2_MIXER)
# winmm
set(WINMM_LIBRARY winmm)
target_link_libraries(${PROJECT_NAME} ${WINMM_LIBRARY})
endif() # NOT USE_SDL2_MIXER


# To copy assets/

# set(SOURCE_ASSET_DIR ${PROJECT_SOURCE_DIR}/assets)
# set(ASSET_OUTPUT_DIR ${CMAKE_BINARY_DIR}/bin/assets)

# file(GLOB_RECURSE src_asset_files RELATIVE ${SOURCE_ASSET_DIR}/ "${SOURCE_ASSET_DIR}/*.*" "${SOURCE_ASSET_DIR}/*")
# foreach(fn ${src_asset_files})
#   add_custom_command(
#     OUTPUT ${ASSET_OUTPUT_DIR}/${fn}
#     COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_ASSET_DIR}/${fn} ${ASSET_OUTPUT_DIR}/${fn}
#     MAIN_DEPENDENCY ${SOURCE_ASSET_DIR}/${fn})
#   list(APPEND out_asset_files ${ASSET_OUTPUT_DIR}/${fn})
# endforeach()

add_custom_command(
  TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_SOURCE_DIR}/assets
          ${CMAKE_BINARY_DIR}/bin/assets)

# To copy data/

# set(SOURCE_DATA_DIR ${PROJECT_SOURCE_DIR}/data)
# set(DATA_OUTPUT_DIR ${CMAKE_BINARY_DIR}/bin/data)

# file(GLOB_RECURSE src_data_files RELATIVE ${SOURCE_DATA_DIR}/ "${SOURCE_DATA_DIR}/*.*" "${SOURCE_DATA_DIR}/*")
# foreach(fn ${src_data_files})
#   add_custom_command(
#     OUTPUT ${DATA_OUTPUT_DIR}/${fn}
#     COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_DATA_DIR}/${fn} ${DATA_OUTPUT_DIR}/${fn}
#     MAIN_DEPENDENCY ${SOURCE_DATA_DIR}/${fn})
#   list(APPEND out_data_files ${DATA_OUTPUT_DIR}/${fn})
# endforeach()

add_custom_command(
  TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_SOURCE_DIR}/data
          ${CMAKE_BINARY_DIR}/bin/data)

target_link_libraries(${PROJECT_NAME} stdc++fs)