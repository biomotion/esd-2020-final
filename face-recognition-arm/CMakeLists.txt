
cmake_minimum_required(VERSION 2.8.12)
project(face-recognition)

set(CMAKE_BUILD_TYPE Release)
# find_package(OpenCV REQUIRED PATHS /usr/local/arm-opencv/install/share/OpenCV NO_DEFAULT_PATH)
find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED)

# set(toolchain /opt/EmbedSky/gcc-linaro-5.3-2016.02-x86_64_arm-linux-gnueabihf)
# set(CMAKE_C_COMPILER ${toolchain}/bin/arm-linux-gnueabihf-gcc)
# set(CMAKE_CXX_COMPILER ${toolchain}/bin/arm-linux-gnueabihf-g++)

# include_directories(${toolchain}/include/)
# link_directories(
#     ${toolchain}/arm-linux-gnueabihf/libc/lib/
#     ${toolchain}/lib/
#     ${toolchain}/qt5.5/rootfs_imx6q_V3_qt5.5_env/lib/
#     ${toolchain}/qt5.5/rootfs_imx6q_V3_qt5.5_env/qt5.5_env/lib/
#     ${toolchain}/qt5.5/rootfs_imx6q_V3_qt5.5_env/usr/lib/)

if (NOT OpenCV_FOUND)
    message("OpenCV not found.")
elseif(NOT Eigen3_FOUND)
    message("Eigen3 not found.")
else()
    message("All packages are found.")
endif()

add_executable(recognition face_recognition.cpp)
target_link_libraries(recognition ${OpenCV_LIBS})

