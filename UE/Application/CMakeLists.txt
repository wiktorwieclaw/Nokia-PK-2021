project(UeApplication)
cmake_minimum_required(VERSION 3.12)

aux_source_directory(. SRC_LIST)
aux_source_directory(Ports SRC_LIST)
aux_source_directory(States SRC_LIST)

include(FetchContent)

FetchContent_Declare(GSL
        GIT_REPOSITORY https://github.com/microsoft/GSL
        GIT_TAG main
        )

FetchContent_MakeAvailable(GSL)

add_library(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME}
        Common
        GSL
        )
