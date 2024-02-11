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

add_compile_options(-fexceptions -Wall -O2 -Wpedantic -g)

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
target_link_libraries   ( Shape Tuple Ray Pattern )


add_library             ( Light lib/Light.cpp)
target_link_libraries   ( Light Tuple )

add_library             ( World lib/World.cpp)
target_link_libraries   ( World Shape Light )

add_library             ( Camera lib/Camera.cpp)
target_link_libraries   ( Camera Ray Canvas World )

add_library             ( Pattern lib/Pattern.cpp)
target_link_libraries   ( Pattern Tuple )

add_executable(RayTest tests/RayTest.cpp)
target_link_libraries(RayTest PRIVATE Catch2::Catch2WithMain Shape )
add_test(NAME RayTest COMMAND RayTest)

add_executable(ShapeTest tests/ShapeTest.cpp)
target_link_libraries(ShapeTest PRIVATE Catch2::Catch2WithMain Shape Light )
add_test(NAME ShapeTest COMMAND ShapeTest)

add_executable(WorldTest tests/WorldTest.cpp)
target_link_libraries(WorldTest PRIVATE Catch2::Catch2WithMain World )
add_test(NAME WorldTest COMMAND WorldTest)

add_executable(CameraTest tests/CameraTest.cpp)
target_link_libraries(CameraTest PRIVATE Catch2::Catch2WithMain Camera )
add_test(NAME CameraTest COMMAND CameraTest)

add_executable(PatternTest tests/PatternTest.cpp)
target_link_libraries(PatternTest PRIVATE Catch2::Catch2WithMain Shape Light )
add_test(NAME PatternTest COMMAND PatternTest)

add_executable          ( RT src/RT.cpp )
target_link_libraries   ( RT Camera )