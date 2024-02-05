cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 17)

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

add_library             ( Tuple lib/Tuple.cpp)
target_link_libraries   ( Tuple )

add_executable(TupleTest tests/TupleTest.cpp)
target_link_libraries(TupleTest PRIVATE Catch2::Catch2WithMain Tuple )
add_test(NAME TupleTest COMMAND TupleTest)

add_executable          ( RT src/RT.cpp )
target_link_libraries   ( RT Tuple )