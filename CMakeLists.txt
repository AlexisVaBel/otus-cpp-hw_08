cmake_minimum_required(VERSION 3.5)
set (CMAKE_CXX_STANDARD 14)

project(bayan)

if ($ENV{TRAVIS_BUILD_NUMBER})
 project(${PROJECT_NAME} VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else ($ENV{TRAVIS_BUILD_NUMBER})
 project(${PROJECT_NAME}  VERSION 0.0.1)
endif ($ENV{TRAVIS_BUILD_NUMBER})

#boost part here
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
find_package(Boost 1.45.0 COMPONENTS date_time filesystem REQUIRED)
#~boost part here



set(INC_DIRS  "." "samefs_searcher/" "hash/")
set(SRC_LIST main.cpp samefs_searcher/bayansearcher.cpp)


include_directories(${INC_DIRS})


add_executable(${PROJECT_NAME} ${SRC_LIST})

set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
        COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra"
        )

target_link_libraries(${PROJECT_NAME}  ${Boost_LIBRARIES})

#    target_link_libraries(${PROJECT_NAME}_test
#        ${Boost_LIBRARIES}
#        )


install (TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT alexisvabel@gmail.com)

set(CPACK_DEBIAN_PACKAGE_SHLIDEPS ON)
include(CPack)
