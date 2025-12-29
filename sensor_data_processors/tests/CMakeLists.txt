cmake_minimum_required(VERSION 3.23)
project(ROBOTSERVER)
 
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../../googletest ${CMAKE_BINARY_DIR}/googletest)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../ sensor_data_processors)

add_executable(sensor_data_dispatcher_test SensorDataDispatcherTest.cpp)

target_link_libraries(sensor_data_dispatcher_test
  PRIVATE
    sensor_data_processors
    GTest::gtest
    GTest::gtest_main
)

add_test(
  NAME SensorDataDispatcherTest
  COMMAND sensor_data_dispatcher_test
)

enable_testing()