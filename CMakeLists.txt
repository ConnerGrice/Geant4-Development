#Checking for correct version of CMake
cmake_minimum_required(VERSION 3.8...3.18)
if(${CMAKE_VERSION} VERSION_LESS 3.12)
	cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

#Name project
project(Dev)

#Find Geant4 package
#If statement allows for the us of drivers, or not
option(WITH_GEANT4_UIVIS "Build program with Geatn4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
	#Gets all ui and visual drivers
	find_package(Geant4 REQUIRED ui_all vis_all)
else()
	find_package(Geant4 REQUIRED)
endif()

#Gets all the include files needed
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/classes)

#List the source and header files
file(GLOB sources ${PROJECT_SOURCE_DIR}/classes/*.cpp)
file(GLOB headers ${PROJECT_SOURCE_DIR}/classes/*.h)

#Defining exe file
add_executable(Dev Dev.cpp ${sources} ${headers})

#Linking Geant4 library files
target_link_libraries(Dev ${Geant4_LIBRARIES})