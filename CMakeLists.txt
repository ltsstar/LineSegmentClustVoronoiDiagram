cmake_minimum_required(VERSION 3.10)
project(LineSegmentClusterVoronoiDiagram)

set(CMAKE_CXX_STANDARD 17)

find_package(PostgreSQL REQUIRED)

find_package(CGAL REQUIRED COMPONENTS Core) # If the dependency is required, use REQUIRED option - if it's not found CMake will issue an error
include( ${CGAL_USE_FILE} )

set(Boost_INCLUDE_DIR /usr/local/opt/boost/include)
set(Boost_LIBRARY_DIR /usr/local/opt/boost/lib)
find_package(Boost REQUIRED COMPONENTS thread timer chrono system)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

add_executable(LineSegmentClusterVoronoiDiagram #main.cpp
        PostgreSQLToBoostLoader.cpp PostgreSQLToBoostLoader.h tests/main.cpp tests/PostgreSQLToBoostLoader.cpp BoostVoronoiDiagram.cpp BoostVoronoiDiagram.h tests/BoostVoronoiDiagramTest.cpp BoostToPostgreSQLSaver.cpp BoostToPostgreSQLSaver.h voronoi_visual_utils.h BoostVoronoiVisualizer.cpp BoostVoronoiVisualizer.h tests/BoostToPostgreSQLSaverTest.cpp Grid.cpp Grid.h tests/GridTest.cpp)

target_include_directories(LineSegmentClusterVoronoiDiagram PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries( LineSegmentClusterVoronoiDiagram PUBLIC ${Boost_LIBRARIES} ${CGAL_LIBRARIES})

# Add include directories to your target. PRIVATE is useful with multi-target projects - see documentation of target_include_directories for more info
target_include_directories(LineSegmentClusterVoronoiDiagram PRIVATE ${PostgreSQL_INCLUDE_DIRS})

# Add libraries to link your target againts. Again, PRIVATE is important for multi-target projects
target_link_libraries(LineSegmentClusterVoronoiDiagram PRIVATE ${PostgreSQL_LIBRARIES})