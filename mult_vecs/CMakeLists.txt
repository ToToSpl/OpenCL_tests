cmake_minimum_required(VERSION 3.10.2)

if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(mult_vecs)

if(NOT MSVC)
    add_definitions("-Wall -Wextra")
else()
    add_definitions("-WX -W2")
endif()

find_package(OpenCL REQUIRED)

add_executable(mult_vecs
    main.cpp    
)

if (LINUX)
    target_link_libraries(mult_vecs
        OpenCL
    )
endif()

if(APPLE)
    target_link_libraries(mult_vecs
        "-framework OpenCL"
    )
endif()