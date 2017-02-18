
cmake_minimum_required(VERSION 2.8)

project( examples )

find_package( OpenCV REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable( example_02-01 example_02-01.cpp )
add_executable( example_02-02 example_02-02.cpp )
add_executable( example_02-03 example_02-03.cpp )
add_executable( example_02-04 example_02-04.cpp )
add_executable( example_02-05 example_02-05.cpp )
add_executable( example_02-06 example_02-06.cpp )
add_executable( example_02-07 example_02-07.cpp )
add_executable( example_02-08 example_02-08.cpp )
add_executable( example_02-09 example_02-09.cpp )
add_executable( example_02-10 example_02-10.cpp )
add_executable( example_02-11 example_02-11.cpp )

target_link_libraries( example_02-01 ${OpenCV_LIBS} )
#target_link_libraries( example_02-01 opencv_core opencv_highgui opencv_imgcodecs )
target_link_libraries( example_02-02 ${OpenCV_LIBS} )
target_link_libraries( example_02-03 ${OpenCV_LIBS} )
target_link_libraries( example_02-04 ${OpenCV_LIBS} )
target_link_libraries( example_02-05 ${OpenCV_LIBS} )
target_link_libraries( example_02-06 ${OpenCV_LIBS} )
target_link_libraries( example_02-07 ${OpenCV_LIBS} )
target_link_libraries( example_02-08 ${OpenCV_LIBS} )
target_link_libraries( example_02-09 ${OpenCV_LIBS} )
target_link_libraries( example_02-10 ${OpenCV_LIBS} )
target_link_libraries( example_02-11 ${OpenCV_LIBS} )




