cmake_minimum_required(VERSION 3.19)
project(hw1)

set(CMAKE_CXX_STANDARD 11)


add_executable(hw1 avl.h Game.cpp Game.h library1.cpp library1.h exceptions.h avlTest.cpp playerLevelTest.cpp GameInsertTest.cpp BestPlayerByGroup.h Players.h)
add_executable(avlTest avl.h avlTest.cpp)
add_executable(playerLevelTest playerLevelTest.cpp avl.h)
add_executable(GameTest GameInsertTest.cpp avl.h Game.cpp Game.h exceptions.h BestPlayerByGroup.h Players.h)
add_executable(StuffTest main1.cpp library1.h library1.cpp Game.h Game.cpp avl.h Players.h BestPlayerByGroup.h exceptions.h)