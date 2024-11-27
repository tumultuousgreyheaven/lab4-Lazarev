cmake_minimum_required(VERSION 3.16)

project(CAL)

find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})

include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB sources "src/*.cc")

file(GLOB MACRO_FILES "*.mac")
file(COPY ${MACRO_FILES} DESTINATION ${PROJECT_BINARY_DIR})

add_executable(CAL main.cc ${sources})

target_link_libraries(CAL ${Geant4_LIBRARIES})
