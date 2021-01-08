
cmake_minimum_required(VERSION 2.8.12)
project(face-trainer)

# find_package(OpenCV REQUIRED PATHS /usr/local/arm-opencv/install/share/OpenCV NO_DEFAULT_PATH)
find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED)

# set(toolchain /opt/EmbedSky/gcc-linaro-5.3-2016.02-x86_64_arm-linux-gnueabihf)
# option(CMAKE_C_COMPILER ${toolchain}/bin/arm-linux-gnueabihf-gcc)
# option(CMAKE_CXX_COMPILER ${toolchain}/bin/arm-linux-gnueabihf-g++)

if (NOT OpenCV_FOUND)
    message("OpenCV not found.")
elseif(NOT Eigen3_FOUND)
    message("Eigen3 not found.")
else()
    message("All packages are found.")
endif()

add_executable(train face_trainer.cpp)
target_link_libraries(train ${OpenCV_LIBS})


add_executable(capture face_capturer.cpp)
target_link_libraries(capture ${OpenCV_LIBS})


add_executable(collector face_collector.cpp)
target_link_libraries(collector ${OpenCV_LIBS})