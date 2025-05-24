cmake_minimum_required(VERSION 3.6)

project(TetrisMain LANGUAGES CXX)

#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT TARGET SDL3::SDL3)
    find_package(SDL3 CONFIG)
endif()
#if(NOT TARGET SDL3_image::SDL3_image)
#    find_package(SDL3_image CONFIG)
#endif ()

if(NOT TARGET SDL3::SDL3)
    find_library(SDL3_LIBRARY NAMES "SDL3")
    find_path(SDL3_INCLUDE_DIR NAMES "SDL3/SDL.h" REQUIRED)
    add_library(SDL3::SDL3 UNKNOWN IMPORTED)
    set_property(TARGET SDL3::SDL3 PROPERTY IMPORTED_LOCATION "${SDL3_LIBRARY}")
    set_property(TARGET SDL3::SDL3 PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${SDL3_INCLUDE_DIR}")

    find_library(SDL3_IMAGE_LIBRARY NAMES "SDL3_image")
    find_path(SDL3_IMAGE_INCLUDE_DIR NAMES "SDL3_image/SDL_image.h" REQUIRED)
    add_library(SDL3_image::SDL3_image UNKNOWN IMPORTED)
    set_property(TARGET SDL3_image::SDL3_image PROPERTY IMPORTED_LOCATION "${SDL3_IMAGE_LIBRARY}")
    set_property(TARGET SDL3_image::SDL3_image PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${SDL3_IMAGE_INCLUDE_DIR}")
endif()

if(NOT TARGET SDL3::SDL3)
    message(FATAL_ERROR "Cannot find SDL3.

Possible ways to fix this:
- Use a SDL3 Android aar archive, and configure gradle to use it: prefab is required.
- Add add_subdirectory(path/to/SDL) to your CMake script, and make sure a vendored SDL is present there.
")
endif()

# 以下增加SDL3_image的配置
if(NOT TARGET SDL3_image::SDL3_image)
    find_package(SDL3_image CONFIG)
endif ()

if(NOT TARGET SDL3_image::SDL3_image)
    find_library(SDL3_IMAGE_LIBRARY NAMES "SDL3_image")
    find_path(SDL3_IMAGE_INCLUDE_DIR NAMES "SDL3_image/SDL_image.h" REQUIRED)
    add_library(SDL3_image::SDL3_image UNKNOWN IMPORTED)
    set_property(TARGET SDL3_image::SDL3_image PROPERTY IMPORTED_LOCATION "${SDL3_IMAGE_LIBRARY}")
    set_property(TARGET SDL3_image::SDL3_image PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${SDL3_IMAGE_INCLUDE_DIR}")
endif()

if(NOT TARGET SDL3_image::SDL3_image)
    message(FATAL_ERROR "Cannot find SDL3_image.")
endif()

# 以下增加SDL3_ttf的配置
if(NOT TARGET SDL3_ttf::SDL3_ttf)
    find_package(SDL3_ttf CONFIG)
endif ()

if(NOT TARGET SDL3_ttf::SDL3_ttf)
    find_library(SDL3_TTF_LIBRARY NAMES "SDL3_ttf")
    find_path(SDL3_TTF_INCLUDE_DIR NAMES "SDL3_ttf/SDL_ttf.h" REQUIRED)
    add_library(SDL3_ttf::SDL3_ttf UNKNOWN IMPORTED)
    set_property(TARGET SDL3_ttf::SDL3_ttf PROPERTY IMPORTED_LOCATION "${SDL3_TTF_LIBRARY}")
    set_property(TARGET SDL3_ttf::SDL3_ttf PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${SDL3_TTF_INCLUDE_DIR}")
endif()

if(NOT TARGET SDL3_ttf::SDL3_ttf)
    message(FATAL_ERROR "Cannot find SDL3_ttf.")
endif()

find_library(ANDROID_LOG_LIB log)#NAMES "log")

#set(ASSETS_SOURCE_FOLDER "../../../../../core/assets")
#set(ASSETS_DESTINATION_FOLDER "../../assets")
set(NLOHMANN_INCLUDE_PATH "../../../../../core/nlohmann")
set(TETRIS_CORE_INCLUDE_PATH "../../../../../core")
set(TETRIS_CORE_SRC_LIST
        ../../../../../core/Actor.cpp
        ../../../../../core/Animation.cpp
        ../../../../../core/Arena.cpp
        ../../../../../core/AudioSuite.cpp
        ../../../../../core/BlockGroup.cpp
        ../../../../../core/BlockMoveTracker.cpp
        ../../../../../core/BrickPool.cpp
        ../../../../../core/Button.cpp
        ../../../../../core/ConstDef.cpp
        ../../../../../core/Control.cpp
        #../../../../../core/Cover.cpp
        #../../../../../core/DebugTrace.cpp
        ../../../../../core/Dialog.cpp
        ../../../../../core/EventQueue.cpp
        ../../../../../core/FadeTracker.cpp
        ../../../../../core/Label.cpp
        ../../../../../core/main.cpp
        ../../../../../core/Material.cpp
        ../../../../../core/MusicCarousel.cpp
        ../../../../../core/Panel.cpp
        ../../../../../core/PhotoCarousel.cpp
        ../../../../../core/Screen.cpp
        ../../../../../core/Timer.cpp
        ../../../../../core/TinyFS.cpp)

#拷贝assets目录到app目录下
#file(MAKE_DIRECTORY ${ASSETS_DESTINATION_FOLDER})
#file(COPY ${ASSETS_SOURCE_FOLDER}/ DESTINATION ${ASSETS_DESTINATION_FOLDER})
#message("copy file from ${ASSETS_SOURCE_FOLDER}/ to ${ASSETS_DESTINATION_FOLDER} finished")

add_library(main SHARED ${TETRIS_CORE_SRC_LIST})
#target_include_directories(main PRIVATE ${NLOHMANN_INCLUDE_PATH} ${TETRIS_CORE_INCLUDE_PATH})
#target_compile_features(main PRIVATE cxx_std_17)
target_compile_definitions(main PRIVATE SDL_PLATFORM_ANDROID=1)

#修改这里，增加SDL3_image库
target_link_libraries(main PRIVATE SDL3::SDL3 SDL3_image::SDL3_image SDL3_ttf::SDL3_ttf ${ANDROID_LOG_LIB})
