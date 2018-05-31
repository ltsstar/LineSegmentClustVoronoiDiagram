//
// Created by Michel Kunkler on 18.05.18.
//

#ifndef LINESEGMENTCLUSTERVORONOIDIAGRAM_BOOSTTOPOSTGRESQLSAVER_H
#define LINESEGMENTCLUSTERVORONOIDIAGRAM_BOOSTTOPOSTGRESQLSAVER_H

#include <string>

#include <boost/polygon/polygon.hpp>
#include <boost/polygon/voronoi.hpp>
using namespace boost::polygon;


#include "PostgreSQLToBoostLoader.h"

class BoostToPostgreSQLSaver {
private:
    static std::string db_name;
    static std::string user_name;

    PGconn     *conn;

    double intToWgs(int32_t wgs);
    void addEdgeToString(std::vector<point_data<double>> *edge, std::ostringstream& resultStream);

public:
    BoostToPostgreSQLSaver();
    ~BoostToPostgreSQLSaver();
    void saveVoronoiDiagram(LineSegmentVector *lsv, VoronoiDiagramBoost *vd);
};


#endif //LINESEGMENTCLUSTERVORONOIDIAGRAM_BOOSTTOPOSTGRESQLSAVER_H
