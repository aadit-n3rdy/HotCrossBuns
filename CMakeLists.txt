cmake_minimum_required(VERSION 3.14)

project(HotCrossBuns)

# Files in src/
add_library(hcb STATIC src/hcb.cpp src/comms/replica.cpp)
target_include_directories(hcb PUBLIC incl/)

# Files in app/
add_executable(hotcrossbuns app/main.cpp)
add_executable(server app/server.cpp)
add_executable(client app/client.cpp)

target_link_libraries(server hcb)
target_link_libraries(client hcb)
target_link_libraries(hotcrossbuns hcb)
