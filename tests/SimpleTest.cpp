//
// Created by Michel Kunkler on 18.05.18.
//

#include "catch.hpp"
#include "../BoostVoronoiDiagram.h"

TEST_CASE( "Simple Test", "[SimpleTest]" ) {
  SECTION( "Voronoi Diagram" ) {
    LineSegmentVector * line_segment_vector = new LineSegmentVector();
    LineSegment ls1 = LineSegment { LineSegmentData(Point(0, 0), Point(-1, 0)), 1};
    LineSegment ls2 = LineSegment { LineSegmentData(Point(-2, -2), Point(-2, -1)), 1 };
    LineSegment ls3 = LineSegment { LineSegmentData(Point(-2, 1), Point(-2, 2)), 1 };
    line_segment_vector->push_back(ls1);
    line_segment_vector->push_back(ls2);
    line_segment_vector->push_back(ls3);

    BoostVoronoiDiagram boostVoronoiDiagram;
    boostVoronoiDiagram.create(line_segment_vector);
  }

}