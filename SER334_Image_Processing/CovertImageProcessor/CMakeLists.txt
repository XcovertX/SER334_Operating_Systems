cmake_minimum_required(VERSION 3.21)
project(SER334_Image_Processing C)

set(CMAKE_C_STANDARD 99)

add_executable(SER334_Image_Processing CovertImageProcessor.c BMPHandler.c Image.c)
