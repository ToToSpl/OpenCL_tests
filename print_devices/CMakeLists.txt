cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(print_devs)

if(NOT MSVC)
    add_definitions("-Wall -Wextra")
else()
    add_definitions("-WX -W2")
endif()

find_package(OpenCL REQUIRED)

add_executable(print_devs
    main.cpp    
)

target_link_libraries(print_devs
   LINK_PRIVATE OpenCL
)