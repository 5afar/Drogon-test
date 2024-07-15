cmake_minimum_required(VERSION 3.29)
project(zhcopy)

set(CMAKE_CXX_STANDARD 17)

set(SRC ./src/main.cxx)

add_executable(zhcopy ${SRC})

find_package(Drogon REQUIRED)
target_link_libraries(zhcopy PRIVATE Drogon::Drogon)


aux_source_directory(controllers CTL_SRC)
aux_source_directory(filters FILTER_SRC)
aux_source_directory(plugins PLUGIN_SRC)
aux_source_directory(model MODEL_SRC)

target_include_directories(${PROJECT_NAME}
                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
                                   ${CMAKE_CURRENT_SOURCE_DIR}/models)
target_sources(${PROJECT_NAME}
               PRIVATE
               ${SRC_DIR}
               ${CTL_SRC}
               ${FILTER_SRC}
               ${PLUGIN_SRC}
               ${MODEL_SRC})