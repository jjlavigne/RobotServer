cmake_minimum_required(VERSION 3.23)

project(ROBOTSERVER)

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
)