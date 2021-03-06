#	opendatacon
 #
 #	Copyright (c) 2014:
 #
 #		DCrip3fJguWgVCLrZFfA7sIGgvx1Ou3fHfCxnrz4svAi
 #		yxeOtDhDCXf1Z4ApgXvX5ahqQmzRfJ2DoX8S05SqHA==
 #	
 #	Licensed under the Apache License, Version 2.0 (the "License");
 #	you may not use this file except in compliance with the License.
 #	You may obtain a copy of the License at
 #	
 #		http://www.apache.org/licenses/LICENSE-2.0
 #
 #	Unless required by applicable law or agreed to in writing, software
 #	distributed under the License is distributed on an "AS IS" BASIS,
 #	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 #	See the License for the specific language governing permissions and
 #	limitations under the License.
 # 
project(WebUI)
cmake_minimum_required(VERSION 2.8)
file(GLOB ${PROJECT_NAME}_SRC *.cpp *.h *.def)

set(MICROHTTPD_HOME_INSTRUCTIONS "Choose the location of libmicrohttpd: where it was installed - there should be lib and include directories within")
if(DEFINED MICROHTTPD_HOME)
	set(MICROHTTPD_HOME ${MICROHTTPD_HOME} CACHE PATH ${MICROHTTPD_HOME_INSTRUCTIONS})
else()
	set(MICROHTTPD_HOME "/usr/local" CACHE PATH ${MICROHTTPD_HOME_INSTRUCTIONS})
endif()

include_directories(
	../include
	../JSON
	${MICROHTTPD_HOME}/include
)

#import the microhttpd lib
find_library(HTTPD_LIB NAMES microhttpd libmicrohttpd
	PATHS ${MICROHTTPD_HOME}/lib
	PATH_SUFFIXES ${CMAKE_LIBRARY_ARCHITECTURE}
	NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH)
add_library(httpd_target UNKNOWN IMPORTED)
set_property(TARGET httpd_target PROPERTY IMPORTED_LOCATION "${HTTPD_LIB}")

#hide (in non-advanced mode) the library path in cmake guis since it's derrived
mark_as_advanced(FORCE HTTPD_LIB)

#include in install/packaging
get_filename_component(_resolvedFile "${HTTPD_LIB}" REALPATH) #check for symolic link
if(${_resolvedFile} STREQUAL ${HTTPD_LIB})
	#not a link - just install file
	install(FILES "${HTTPD_LIB}" DESTINATION ${INSTALLDIR_LIBS})
else()
	#is a link -install link and file
	install(FILES "${HTTPD_LIB}" "${_resolvedFile}" DESTINATION ${INSTALLDIR_LIBS})
endif()

link_directories("${DNP3_HOME}/lib")

add_library(${PROJECT_NAME} MODULE ${${PROJECT_NAME}_SRC})
target_link_libraries(${PROJECT_NAME} ODC httpd_target asiopal)

install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${INSTALLDIR_MODULES})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER plugins)
