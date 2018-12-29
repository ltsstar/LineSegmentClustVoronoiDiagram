//
// Created by Michel Kunkler on 26.05.18.
//

#ifndef LINESEGMENTCLUSTERVORONOIDIAGRAM_GRID_H
#define LINESEGMENTCLUSTERVORONOIDIAGRAM_GRID_H

#include <utility>
#include "PostgreSQLToBoostLoader.h"

typedef struct GridLocation {
    int x;
    int y;
    GridLocation();
    GridLocation(int x, int y);
    bool operator==(const GridLocation &A) const;
} GridLocation;

struct IntCoordinate;

typedef struct WgsCoordinate {
    double lon;
    double lat;

    IntCoordinate toInt();
    WgsCoordinate(double lon, double lat);
    WgsCoordinate(Point point);
} WgsCoordinate;

typedef struct IntCoordinate {
    uint32_t x;
    uint32_t y;

    WgsCoordinate toWgs();
    IntCoordinate(uint32_t x, uint32_t y);
    IntCoordinate(Point point);
} IntCoordinate;

class Square {
private:
    LineSegmentVector fully_containing_segments;
    LineSegmentVector partly_containing_segments;
    std::vector<int> dijkstra_classes;
    void add_dijkstra_class(int dijkstra_class);
public:
    LineSegmentVector get_fully_containing_line_segments();
    LineSegmentVector get_partly_containing_line_segments();
    bool is_empty();
    bool multiple_classes();
    bool contains_dijkstra_class(int dijkstra_class);
    bool is_only_dijkstra_class(int dijkstra_class);
    void add_part_segment(LineSegment line_segment);
    void add_full_segment(LineSegment line_segment);
    int get_first_dijkstra_class();
    void clear();
};

class Grid {
private:
    typedef std::vector<GridLocation> GridLocationList;
    typedef std::vector<Square*> SquareList;
    typedef std::vector<std::vector<Square>> GridMatrix;
    typedef boost::polygon::point_data<double> POINT;

    GridMatrix grid;
    LineSegmentVector *lsv;
    LineSegmentVector *lsv_non_grid;

    GridLocation findGridLocation(IntCoordinate int_coordinate);
    GridLocation findGridLocation(POINT point);
    void findAllGridLocations(LineSegment *line_segment, GridLocationList * result);
    GridLocationList findGridLocations(LineSegment * line_segment);
    bool isInOneSquare(LineSegment * line_segment);
    bool isCleanable(GridLocation square_location);
    Square * getSquare(GridLocation grid_location);

public:
    std::pair<double, double> extend_x;
    std::pair<double, double> extend_y;
    int grid_x;
    int grid_y;

    double square_size;

    void sortIntoGrid();
    void cleanSegments();
    void saveTo(LineSegmentVector *new_lsv);

    explicit Grid(LineSegmentVector *lsv,
                  std::pair<double, double> extend_x,
                  std::pair<double, double> extend_y,
                  double square_size);
    ~Grid();
};


#endif //LINESEGMENTCLUSTERVORONOIDIAGRAM_GRID_H
