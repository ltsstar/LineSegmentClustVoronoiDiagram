//
// Created by Michel Kunkler on 24.04.18.
//

#ifndef PARALLEL_DIJKSTRA_POSTGRESQLLOADER_H
#define PARALLEL_DIJKSTRA_POSTGRESQLLOADER_H

#include <array>
#include <vector>
#include "libpq/libpq-fs.h"
#include "libpq-fe.h"
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/polygon/voronoi.hpp>
#include "PostgreSQLToBoostLoader.h"
#include <boost/polygon/polygon.hpp>

typedef boost::int32_t int32;

typedef boost::polygon::voronoi_diagram<double> VoronoiDiagramBoost;

typedef boost::polygon::point_data<double> Point;
typedef boost::polygon::segment_data<double> LineSegmentData;
typedef struct LineSegment {
    LineSegmentData line_segment_data;
    int dclass;
    bool operator==(const LineSegment &A) const;
} LineSegment;

typedef std::vector<LineSegment> LineSegmentVector;


class PostgreSQLToBoostLoader {
private:
    static std::string db_name;
    static std::string user_name;

    PGconn     *conn;

    int32_t wgsToInt(double wgs);
public:
    PostgreSQLToBoostLoader ();
    ~PostgreSQLToBoostLoader();
    void loadLineSegments(LineSegmentVector *lsv);
    std::vector<std::array<double, 2>> getCoordinatesFromLineString(std::string line_string);
};
#endif //PARALLEL_DIJKSTRA_POSTGRESQLLOADER_H
