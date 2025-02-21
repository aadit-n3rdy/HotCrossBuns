cmake_minimum_required(VERSION 3.14)

project(HotCrossBuns)

# Files in src/
add_library(hcb STATIC src/hcb.cpp)
target_include_directories(hcb PUBLIC incl/)

# Files in app/
add_executable(hotcrossbuns app/main.cpp)
target_link_libraries(hotcrossbuns hcb)
