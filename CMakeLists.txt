cmake_minimum_required(VERSION 3.24)
project(cpp)

aux_source_directory(source SOURCE_DIR)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "/home/zhb15/newDisk/SoftWare/Cxx_Lib/Qt/5.15.2/gcc_64/lib/cmake/")

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -std=gnu++20 -Werror -W -O2 -ftemplate-depth=4096 -lgomp -lpthread -fmodules-ts")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp -Wall -W -O2")

find_package(Qt5 COMPONENTS Core Widgets Sql PrintSupport Network REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${BOOST_LIBRARY_DIRS})
add_definitions(-DBOOST_ALL_DYN_LINK)

add_executable(cpp ${SOURCE_DIR} source/TcpServer.h source/TcpClient.h)
target_link_libraries(cpp Qt::Core Qt::Widgets Qt::Sql Qt::PrintSupport Qt::Network)

