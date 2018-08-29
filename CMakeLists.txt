cmake_minimum_required(VERSION 3.9)
project(roCORD)

set(CMAKE_CXX_STANDARD 14)  # enable C++14 standard
set (CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++1y -D TESTING -Wall") #

include_directories( /Users/normanziebal/websocketpp/ /usr/local/Cellar/openssl/1.0.2p/include/) #/usr/local/Cellar/boost@1.60/1.60.0/include)

set (Boost_USE_STATIC_LIBS ON)
set (Boost_USE_MULTITHREAD ON)
find_package (Boost 1.54.0 COMPONENTS REQUIRED chrono filesystem)
IF (Boost_FOUND)
    message(STATUS "Boost found!")
    include_directories(${Boost_INCLUDE_DIR})
    add_executable (roCORD main.cpp discord_bot.cpp discord_core.cpp discord_error.cpp discord_http.cpp discord_websocket.cpp clif_testing.cpp showmsg_testing.cpp)
    target_link_libraries (roCORD ${Boost_LIBRARIES} /usr/local/Cellar/openssl/1.0.2p/lib/libcrypto.a /usr/local/Cellar/openssl/1.0.2p/lib/libssl.a /usr/local/Cellar/curl/7.61.0/lib/libcurl.dylib)
ELSE()
    message(FATAL_ERROR "Boost not found!")
endif()


