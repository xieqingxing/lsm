cmake_minimum_required(VERSION 3.19)
project(lsm)

set(CMAKE_CXX_STANDARD 14)

add_executable(lsm main.cpp db.cpp db.h memtable.cpp memtable.h item.cpp item.h disk_writer.cpp disk_writer.h index.cpp index.h kv_data.cpp kv_data.h kv_search.cpp kv_search.h)