#	opendatacon
 #
 #	Copyright (c) 2016:
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
project (opendatacon_installer)
cmake_minimum_required (VERSION 2.8)
 
set(CPACK_PACKAGE_NAME "opendatacon")
set(CPACK_PACKAGE_VENDOR "opendatacon.net")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "opendatacon - Installation")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/RELEASE_NOTES")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE_HEADER.txt")
set(CPACK_PACKAGE_VERSION ${ODC_VERSION})
set(CPACK_PACKAGE_VERSION_MAJOR ${ODC_MAJOR_VERSION})
set(CPACK_PACKAGE_VERSION_MINOR ${ODC_MINOR_VERSION})
set(CPACK_PACKAGE_VERSION_PATCH ${ODC_MICRO_VERSION})
set(CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_NAME})

string(REGEX MATCH "/([^/]*-)" TOOLCHAIN_MATCH ${CMAKE_TOOLCHAIN_FILE} "")
set(cross_detect ${CMAKE_MATCH_1})
message("Cross toolchain detect: ${cross_detect}")
string(REGEX MATCH "(-m32)" BITNESS_MATCH ${CMAKE_CXX_FLAGS} "")
set(bit_detect ${CMAKE_MATCH_1})
message("Compile bitness detect: ${bit_detect}")
set(CPACK_PACKAGE_FILE_NAME "opendatacon-${GIT_DESCRIBE}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}${bit_detect}-${cross_detect}${CMAKE_BUILD_TYPE}")

# Mac OSX bundle specific variables
set(CPACK_BUNDLE_NAME opendatacon)
set(CPACK_BUNDLE_PLIST ${CMAKE_SOURCE_DIR}/install/MacOS/opendatacon.plist)
set(CPACK_BUNDLE_ICON ${CMAKE_SOURCE_DIR}/install/MacOS/AplicationIcon.icns)

#RPM Specific variables
#set(CPACK_GENERATOR "RPM")
#set(CPACK_PACKAGE_RELEASE 1)
#set(CPACK_PACKAGE_CONTACT "opendatacon.net")
#set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
#set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_PACKAGE_RELEASE}.${CMAKE_SYSTEM_PROCESSOR}")
include(CPack)
