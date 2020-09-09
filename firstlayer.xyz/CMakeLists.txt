cmake_minimum_required (VERSION 3.9)
project (firstlayer_tools)


# add_custom_command(OUTPUT ../lib/liblibslic3r.a COMMAND ./build_slic3r.sh)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)




include_directories("${PROJECT_SOURCE_DIR}/../xs/src")
link_directories("${PROJECT_SOURCE_DIR}/../build/")

add_executable(modelinfo modelinfo.cpp)
target_link_libraries(modelinfo libslic3r
    expat
    BSpline
    ZipArchive
    polypartition
    clipper
    poly2tri
    admesh
    pthread
    boost_thread
    boost_system
    boost_filesystem)


add_executable(modeljson modeljson.cpp)
target_link_libraries(modeljson libslic3r
    expat
    BSpline
    ZipArchive
    polypartition
    clipper
    poly2tri
    admesh
    pthread
    boost_thread
    boost_system
    boost_filesystem)
