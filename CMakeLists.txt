cmake_minimum_required(VERSION 3.19)
project(hw1)

set(CMAKE_CXX_STANDARD 11)


add_executable(AvlTest avlTest.cpp avl.h)
add_executable(RecAVL testReplace.cpp avl.h game.cpp game.h PlayerLevel.h)
add_executable(hw1 avl.h avlTest.cpp game.cpp game.h library1.cpp library.h PlayerById.h group.h exceptions.cpp exceptions.h PlayerLevel.h testReplace.cpp)