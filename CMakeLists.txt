cmake_minimum_required(VERSION 3.15)
project(rdysim)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)
find_package(READDY REQUIRED)
find_package(HDF5 COMPONENTS HL REQUIRED)

add_executable(rdysim main.cpp)
target_include_directories(rdysim PUBLIC ${READDY_INCLUDE_DIRS} ${HDF5_INCLUDE_DIRS} ${HDF5_HL_INCLUDE_DIR})
target_link_libraries(rdysim LINK_PUBLIC ${READDY_LIBRARIES} ${HDF5_LIBRARIES} ${HDF5_HL_LIBRARIES})