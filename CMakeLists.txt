cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include(CTest)
Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.3.2 # or a different version
)

FetchContent_MakeAvailable(Catch2)

include_directories (${CMAKE_SOURCE_DIR}/include)

set (CMAKE_CXX_FLAGS "-fexceptions -Wall -O2 -Wpedantic")
add_library             ( Tuple lib/Tuple.cpp)
target_link_libraries   ( Tuple )

add_executable(TupleTest tests/TupleTest.cpp)
target_link_libraries(TupleTest PRIVATE Catch2::Catch2WithMain Tuple )
add_test(NAME TupleTest COMMAND TupleTest)

add_library             ( Canvas lib/Canvas.cpp)
target_link_libraries   ( Canvas Tuple )

add_executable(CanvasTest tests/CanvasTest.cpp)
target_link_libraries(CanvasTest PRIVATE Catch2::Catch2WithMain Canvas )
add_test(NAME CanvasTest COMMAND CanvasTest)

add_executable(MatrixTest tests/MatrixTest.cpp)
target_link_libraries(MatrixTest PRIVATE Catch2::Catch2WithMain Tuple )
add_test(NAME MatrixTest COMMAND MatrixTest )

add_library             ( Ray lib/Ray.cpp)
target_link_libraries   ( Ray Tuple )

add_library             ( Shape lib/Shape.cpp)
target_link_libraries   ( Shape Tuple )

add_library             ( Intersection lib/Intersection.cpp)
target_link_libraries   ( Intersection Ray Shape )

add_executable(RayTest tests/RayTest.cpp)
target_link_libraries(RayTest PRIVATE Catch2::Catch2WithMain Intersection )
add_test(NAME RayTest COMMAND RayTest)

add_executable          ( RT src/RT.cpp )
target_link_libraries   ( RT Tuple )