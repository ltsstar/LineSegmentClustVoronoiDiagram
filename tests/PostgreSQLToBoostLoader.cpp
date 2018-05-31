//
// Created by Michel Kunkler on 17.05.18.
//

#include "catch.hpp"
#include "../PostgreSQLToBoostLoader.h"

TEST_CASE( "PostgreSQLToBoostLoader Test" ) {
    SECTION( "return data not empty" ) {
        PostgreSQLToBoostLoader postgreSQLToBoostLoader;
        LineSegmentVector line_segment_vector;
        postgreSQLToBoostLoader.loadLineSegments(&line_segment_vector);

        REQUIRE(line_segment_vector.size() > 0);

        REQUIRE(line_segment_vector[0].line_segment_data.low().x() != 0);
        REQUIRE(line_segment_vector[0].line_segment_data.low().y() != 0);
    }

}