message (STATUS "Looking for libs for Discord!")
cmake_minimum_required (VERSION 3.9)

set (CMAKE_MODULE_PATH "config/cmake_modules/;${CMAKE_MODULE_PATH}")
set (CMAKE_CXX_STANDARD 11)  # enable C++11 standard

if (NOT (DEFINED JSON AND DEFINED WEBSOCKETPP) AND NOT DEFINED TRAVIS)
	message (FATAL_ERROR "You did not provide JSON or WEBSOCKETPP Path")
endif()

if (DEFINED TRAVIS OR DEFINED STAND_ALONE)
	project (roCORD)
	set (CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++1y -D TESTING -Wall -pthread -liconv")
	set (roCORD_src src/main.cpp src/clif_testing.cpp src/showmsg_testing.cpp src/discord_bot.cpp src/discord_core.cpp src/discord_error.cpp src/discord_http.cpp src/discord_websocket.cpp)

	if (DEFINED STAND_ALONE)
		message (STATUS "Using stand_alone setup!")
		include_directories(${JSON} ${WEBSOCKETPP}) 
	else ()
		message (STATUS "Using travis setup!")
		include_directories(/home/travis/build/Normynator/roCORD/cget/cget/pkg/nlohmann__json/install/include/ /home/travis/build/Normynator/roCORD/websocketpp/) 
	endif ()
	
else () #RELEASE
	message (STATUS "Using release setup!")
	set(roCORD_src discord_bot.cpp discord_core.cpp discord_error.cpp discord_http.cpp discord_websocket.cpp)
	include_directories (${JSON} ${WEBSOCKETPP})
endif ()

set (Boost_USE_STATIC_LIBS ON)
set (Boost_USE_MULTITHREAD ON)
find_package (Boost 1.54.0 COMPONENTS REQUIRED chrono filesystem locale)
find_package (iconv REQUIRED)
find_package (OpenSSL REQUIRED)
find_package (CURL REQUIRED)
include_directories (${Boost_INCLUDE_DIR} ${Iconv_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR} ${CURL_INCLUDE_DIRS})

if (DEFINED TRAVIS OR DEFINED STAND_ALONE)
	add_executable (roCORD ${roCORD_src})
else ()
	add_library (roCORD ${roCORD_src})
endif ()


target_link_libraries (roCORD ${Boost_LIBRARIES} ${Iconv_LIBRARIES} ${OPENSSL_LIBRARIES} ${CURL_LIBRARIES})

