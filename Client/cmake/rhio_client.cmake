cmake_minimum_required(VERSION 2.8)
list(GET rhio_client_INCLUDE_DIRS 0 HEAD)
set(rhio_client_INCLUDE_DIRS 
    ${rhio_client_INCLUDE_DIRS}
    "${HEAD}/rhio_client"
)
