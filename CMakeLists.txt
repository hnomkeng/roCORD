message (STATUS "Looking for libs for Discord!")
cmake_minimum_required (VERSION 3.11)

set (CMAKE_CXX_STANDARD 11)  # enable C++11 standard

if (NOT (DEFINED JSON AND DEFINED WEBSOCKETPP) AND NOT DEFINED TRAVIS)
	message (FATAL_ERROR "You did not provide JSON or WEBSOCKETPP Path")
endif()


set (CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++1y -Wall -pthread")
if (DEFINED TRAVIS OR DEFINED STAND_ALONE)
	project (roCORD)
	set (CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -D TESTING")
	set (roCORD_src src/main.cpp src/clif_testing.cpp src/showmsg_testing.cpp src/discord_bot.cpp src/discord_core.cpp src/discord_error.cpp src/discord_http.cpp src/discord_websocket.cpp)

	if (DEFINED STAND_ALONE)
		message (STATUS "Using stand_alone setup!")
		include_directories(${JSON} ${WEBSOCKETPP}) 
	else ()
		message (STATUS "Using travis setup!")
		include_directories(cget/cget/pkg/nlohmann__json/install/include/ websocketpp/) 
	endif ()
	
else () #RELEASE
	message (STATUS "Using release setup!")
	set(roCORD_src discord_bot.cpp discord_core.cpp discord_error.cpp discord_http.cpp discord_websocket.cpp)
	include_directories (${JSON} ${WEBSOCKETPP})
endif ()

set (Boost_USE_STATIC_LIBS ON)
set (Boost_USE_MULTITHREAD ON)
find_package (Boost 1.54.0 COMPONENTS REQUIRED system chrono filesystem locale)
find_package (Iconv REQUIRED)
find_package (OpenSSL REQUIRED)
find_package (CURL REQUIRED)
if(NOT APPLE)
	find_package (ICU COMPONENTS REQUIRED data i18n io le lx test tu uc)
endif()
include_directories (${Boost_INCLUDE_DIR} ${ICU_INCLUDE_DIR} ${Iconv_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR} ${CURL_INCLUDE_DIRS})

if (DEFINED TRAVIS OR DEFINED STAND_ALONE)
	add_executable (roCORD ${roCORD_src})
else ()
	add_library (roCORD ${roCORD_src})
endif ()


target_link_libraries (roCORD ${Boost_LIBRARIES} ${ICU_LIBRARIES} ${Iconv_LIBRARIES} ${OPENSSL_LIBRARIES} ${CURL_LIBRARIES})

