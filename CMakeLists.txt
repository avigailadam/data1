cmake_minimum_required(VERSION 3.19)
project(hw1)

set(CMAKE_CXX_STANDARD 11)


add_executable(hw1 avl.h game.cpp game.h library1.cpp library.h PlayerById.h group.h exceptions.cpp exceptions.h PlayerLevel.h avlTest.cpp playerLevelTest.cpp)
add_executable(avlTest avl.h avlTest.cpp)
add_executable(playerLevelTest playerLevelTest.cpp avl.h PlayerLevel.h)