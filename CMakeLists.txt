cmake_minimum_required(VERSION 3.10)
project(LineSegmentClusterVoronoiDiagram)

set(CMAKE_CXX_STANDARD 17)

find_package(PostgreSQL REQUIRED)

#find_package(CGAL REQUIRED COMPONENTS Core) # If the dependency is required, use REQUIRED option - if it's not found CMake will issue an error
#include( ${CGAL_USE_FILE} )

set(BOOST_ROOT "/usr/include/boost")
set(BOOST_LIBRARYDIR "/usr/lib/x86_64-linux-gnu")
find_package(Boost REQUIRED COMPONENTS thread timer chrono system)
include_directories(${BOOST_ROOT})
link_directories(${BOOST_LIBRARYDIR})

add_executable(LineSegmentClusterVoronoiDiagram
        #main.cpp
        PostgreSQLToBoostLoader.cpp
        PostgreSQLToBoostLoader.h
        tests/main.cpp
        #tests/PostgreSQLToBoostLoader.cpp
        BoostVoronoiDiagram.cpp
        BoostVoronoiDiagram.h
        #tests/BoostVoronoiDiagramTest.cpp
        BoostToPostgreSQLSaver.cpp
        BoostToPostgreSQLSaver.h
        voronoi_visual_utils.h
        BoostVoronoiVisualizer.cpp
        BoostVoronoiVisualizer.h
        #tests/BoostToPostgreSQLSaverTest.cpp
        Grid.cpp
        Grid.h
        #tests/GridTest.cpp
        #tests/SimpleTest.cpp
        tests/Benchmark.cpp
        VoronoiMerger.cpp VoronoiMerger.h tests/MergerTest.cpp)

target_include_directories(LineSegmentClusterVoronoiDiagram PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries( LineSegmentClusterVoronoiDiagram PUBLIC ${Boost_LIBRARIES}) # ${CGAL_LIBRARIES})

# Add include directories to your target. PRIVATE is useful with multi-target projects - see documentation of target_include_directories for more info
target_include_directories(LineSegmentClusterVoronoiDiagram PRIVATE ${PostgreSQL_INCLUDE_DIRS})

# Add libraries to link your target againts. Again, PRIVATE is important for multi-target projects
target_link_libraries(LineSegmentClusterVoronoiDiagram PRIVATE ${PostgreSQL_LIBRARIES})