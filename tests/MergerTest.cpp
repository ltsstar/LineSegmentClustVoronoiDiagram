//
// Created by michel on 12/29/18.
//

#include "catch.hpp"
#include "../BoostVoronoiDiagram.h"
#include "../VoronoiMerger.h"
#include "../BoostToPostgreSQLSaver.h"

TEST_CASE( "Merger Test", "[Merger]" ) {
  SECTION( "Voronoi Diagram" ) {
    PostgreSQLToBoostLoader postgreSQLToBoostLoader;
    LineSegmentVector line_segment_vector;
    postgreSQLToBoostLoader.loadLineSegments(&line_segment_vector);

    BoostVoronoiDiagram boostVoronoiDiagram;
    boostVoronoiDiagram.create(&line_segment_vector);


    VoronoiMerger::merge(&line_segment_vector, &(boostVoronoiDiagram.vd));

    BoostToPostgreSQLSaver boostToPostgreSQLSaver = BoostToPostgreSQLSaver();
    boostToPostgreSQLSaver.saveVoronoiDiagram(&line_segment_vector, &boostVoronoiDiagram.vd);
  }

}