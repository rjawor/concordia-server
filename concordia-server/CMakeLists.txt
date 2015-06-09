add_executable(hello_world hello_world.cpp)
target_link_libraries(hello_world fcgi fcgi++)

add_executable(echo-cpp echo-cpp.cpp)
target_link_libraries(echo-cpp fcgi fcgi++)

add_executable(concordia_server_process concordia_server_process.cpp concordia_server.cpp)
target_link_libraries(concordia_server_process fcgi fcgi++ concordia config++ log4cpp ${Boost_LIBRARIES} divsufsort utf8case)

