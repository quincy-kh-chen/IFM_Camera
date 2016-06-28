###################################################################################################
# This is the main CMakeLists.txt file for the O3D3xx plugin.
# Author : Jitesh Butala
# Email : jitesh.butala@ifm.com
###################################################################################################

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.9 FATAL_ERROR)

PROJECT(O3D3xx)

# Check if the build folder is a subfolder of the code folder
STRING(FIND "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" BUILD_SUBFOLDER_OF_CODE)

IF(BUILD_SUBFOLDER_OF_CODE GREATER -1)
	MESSAGE(FATAL_ERROR "Build folder cannot be a subfolder of the code folder. Please select another location.")
ENDIF()

# Set the Version Info
FIND_PACKAGE(Subversion)
SET(VERSION_MAJOR "1")
SET(VERSION_MINOR "11")
IF(Subversion_FOUND)
	GET_FILENAME_COMPONENT(ABS_FOLDER ${CMAKE_CURRENT_SOURCE_DIR} ABSOLUTE)
	EXECUTE_PROCESS(COMMAND ${Subversion_SVN_EXECUTABLE} info ${ABS_FOLDER}
					RESULT_VARIABLE INFO_RESULT
					OUTPUT_VARIABLE IGNORED
					ERROR_VARIABLE IGNORED)
	IF(INFO_RESULT EQUAL 0)
		# If SVN copy, take the latest version
		Subversion_WC_INFO(${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_NAME})
		
		FILE(WRITE "${CMAKE_CURRENT_SOURCE_DIR}/third_party/doxygen/svn_version.bin" "${${PROJECT_NAME}_WC_REVISION}")
		
		SET(VERSION_BUILD "${${PROJECT_NAME}_WC_REVISION}")
	ELSE()
		IF(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/third_party/doxygen/svn_version.bin")
			# If not SVN copy, use the version stored in the binary file
			FILE(READ "${CMAKE_CURRENT_SOURCE_DIR}/third_party/doxygen/svn_version.bin" VERSION_BUILD)
		ELSE()
			# If nothing present, then set the version to 999999999
			SET(VERSION_BUILD "999999999")
		ENDIF()
	ENDIF()
ELSE()
	IF(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/third_party/doxygen/svn_version.bin")
		# If not SVN copy, use the version stored in the binary file
		FILE(READ "${CMAKE_CURRENT_SOURCE_DIR}/third_party/doxygen/svn_version.bin" VERSION_BUILD)
	ELSE()
		# If nothing present, then set the version to 999999999
		SET(VERSION_BUILD "999999999")
	ENDIF()
ENDIF()

# Set the output directories
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
SET(BINARY_OUTPUT_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

# Set the common includes
SET(CXXSOURCES)

IF(${CMAKE_CL_64}) 
SET(PLATFORM "Windows64bit" )
SET(BITNESS "64")
ELSE()
SET(PLATFORM "Windows32bit" )
SET (BITNESS "32")
ENDIF()

SET(HXXSOURCES
		${CMAKE_CURRENT_SOURCE_DIR}/third_party/pmdsdk/${PLATFORM}/include/pmddatadescription.h
		${CMAKE_CURRENT_SOURCE_DIR}/third_party/pmdsdk/${PLATFORM}/include/pmdsdk2.h
		${CMAKE_CURRENT_SOURCE_DIR}/third_party/pmdsdk/${PLATFORM}/include/pmdsdk2common.h)
		
SET(INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/third_party/pmdsdk/${PLATFORM}/include)

# Give user option to build unit tests
OPTION(O3D3XX_UNIT_TESTS_BUILD_ALL "Build Unit Tests" OFF)

# Give user option to build sample codes
OPTION(O3D3XX_SAMPLE_CODES_BUILD_ALL "Build Sample codes" ON)

# Give user option to build Frame Rate test codes
OPTION(O3D3XX_FRAME_RATE_TESTS_BUILD_ALL "Build Frame Rate Tests" ON)

# Give user option to build the documentation
OPTION(O3D3XX_DOXYGEN_DOCUMENTATION_GENERATE "Generate Doxygen Documentation" OFF)

# Do we want the Release Package Option. not avaliable for customers
#OPTION(O3D3XX_GENERATE_RELEASE_PACKAGE "Create the project for installing the release" OFF)
SET(O3D3XX_GENERATE_RELEASE_PACKAGE FALSE)

# Find doxygen if documentation is selected
IF (${O3D3XX_DOXYGEN_DOCUMENTATION_GENERATE})
	FIND_PACKAGE(doxygen REQUIRED)
	FIND_PACKAGE(HTMLHelp REQUIRED)
ENDIF()


# Set the project to export the symbols
ADD_DEFINITIONS(-DDLL_EXPORTS)

# Set all the directories
# Add XML-RPC
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/third_party/xmlrpc)

# Add Source and Processing plugins
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/source_plugin)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/processing_plugin)

# Add sample codes projects if user has selected to build the sample codes
IF (${O3D3XX_SAMPLE_CODES_BUILD_ALL})
	#Add directory which contains sample code
	ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/samples)
ENDIF()

# Add Frame Rate Test
IF (${O3D3XX_FRAME_RATE_TESTS_BUILD_ALL})
	#Add directory which contains sample code
	ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/frame_rate_test)
ENDIF()

# Add unit testing frameowrk if user has selected to build unit tests
IF (${O3D3XX_UNIT_TESTS_BUILD_ALL})
	# Add GTest subdirectory so that it can also be built along with the project
	ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/third_party/gtest-1.7.0)

	# Set the GTest library & include directories
	SET(GTEST_LIB_DIRS
			${GTEST_LIBRARY}
			${GTEST_MAIN_LIBRARY})
	
	SET(GTEST_INCLUDE_DIRS
			${gtest_SOURCE_DIR}
			${gtest_SOURCE_DIR}/include)
			
	# Add Test subdirectory so that it can also be built along with the project
	ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/tests)
ENDIF()

# Set the include directories
INCLUDE_DIRECTORIES(${INCLUDE_DIRS} ${INCLUDE_DIRS_COMMON})



