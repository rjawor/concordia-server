file(GLOB main_sources "*.cpp")

add_executable(concordia_server_process
                  ${main_sources}
               )
target_link_libraries(concordia_server_process fcgi fcgi++ pq concordia config++ log4cpp ${Boost_LIBRARIES} divsufsort utf8case icuuc)
