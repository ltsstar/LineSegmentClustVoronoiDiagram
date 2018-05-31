//
// Created by Michel Kunkler on 28.05.18.
//

#include "catch.hpp"
#include "../Grid.h"
#include "../BoostVoronoiDiagram.h"

TEST_CASE("Grid Test", "[Grid]") {
    SECTION( "sorting" ) {
        PostgreSQLToBoostLoader postgreSQLToBoostLoader;
        LineSegmentVector lsv;
        LineSegmentVector cleaned_lsv;
        postgreSQLToBoostLoader.loadLineSegments(&lsv);

        std::cout << "Line segments before grid: " << lsv.size() << std::endl;
        Grid *grid = new Grid(&lsv);
        grid->sortIntoGrid();
        grid->cleanSegments();
        grid->saveTo(&cleaned_lsv);
        delete grid;

        std::cout << "Line segments after grid: " << cleaned_lsv.size() << std::endl;
        //BoostVoronoiDiagram boostVoronoiDiagram;
        //boostVoronoiDiagram.create(&(ssd_polygon.first));
    }
}