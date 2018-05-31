//
// Created by Michel Kunkler on 18.05.18.
//

#include "catch.hpp"
#include "../BoostVoronoiDiagram.h"

TEST_CASE( "BoostVoronoiDiagram Test" ) {
    SECTION( "calculate Voronoi Diagram" ) {
        PostgreSQLToBoostLoader postgreSQLToBoostLoader;
        LineSegmentVector line_segment_vector;
        postgreSQLToBoostLoader.loadLineSegments(&line_segment_vector);

        BoostVoronoiDiagram boostVoronoiDiagram;
        boostVoronoiDiagram.create(&line_segment_vector);
    }

}