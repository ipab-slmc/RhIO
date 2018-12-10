cmake_minimum_required(VERSION 2.8)
list(GET rhio_server_INCLUDE_DIRS 0 HEAD)
set(rhio_server_INCLUDE_DIRS 
    ${rhio_server_INCLUDE_DIRS}
    "${HEAD}/rhio_server"
)
