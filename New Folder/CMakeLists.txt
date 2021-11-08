cmake_minimum_required(VERSION 3.20)
project(Interlace)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")


add_executable(Interlace main.cpp Executer.h concurrent_queue.h Header.h Input.h InputFile.h Output.h thread_pool.h thread_pool_executor.h files/InputHelper.h files/thread.h files/threader.h subprocess.h)
