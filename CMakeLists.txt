cmake_minimum_required(VERSION 3.23)

project(ROBOTSERVER)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(sensor_data_processors)

add_executable(ROBOTSERVER)

add_subdirectory(sensor_data_processors)

target_sources(ROBOTSERVER
  PRIVATE
    main.cpp
)

target_link_libraries(ROBOTSERVER
  PRIVATE
    sensor_data_processors
    SDL2        # Links standard SDL2
    SDL2_image  # Links the image extension
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
