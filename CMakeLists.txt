cmake_minimum_required(VERSION 3.15)
project(Markov)

set(CMAKE_CXX_STANDARD 11)

add_executable(train markov_train.cpp  common.h)
add_executable(guess markov_guess.cpp common.h)
