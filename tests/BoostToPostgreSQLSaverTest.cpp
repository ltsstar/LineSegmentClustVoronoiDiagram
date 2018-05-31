//
// Created by Michel Kunkler on 18.05.18.
//

#include "catch.hpp"
#include "../BoostToPostgreSQLSaver.h"
#include "../BoostVoronoiDiagram.h"

TEST_CASE( "BoostToPostgreSQLSaver Test" ) {
    SECTION( "save calculated Voronoi Diagram" ) {
        PostgreSQLToBoostLoader postgreSQLToBoostLoader;
        LineSegmentVector line_segment_vector;
        postgreSQLToBoostLoader.loadLineSegments(&line_segment_vector);

        BoostVoronoiDiagram boostVoronoiDiagram;
        boostVoronoiDiagram.create(&line_segment_vector);


        BoostToPostgreSQLSaver boostToPostgreSQLSaver;
        boostToPostgreSQLSaver.saveVoronoiDiagram(&line_segment_vector, &boostVoronoiDiagram.vd);
    }
}