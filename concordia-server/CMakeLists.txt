add_executable(concordia_server_process
                  concordia_server_process.cpp
                  concordia_server.cpp
                  index_controller.cpp
                  searcher_controller.cpp
                  json_generator.cpp
                  unit_dao.cpp
              )
target_link_libraries(concordia_server_process fcgi fcgi++ pq concordia config++ log4cpp ${Boost_LIBRARIES} divsufsort utf8case)

