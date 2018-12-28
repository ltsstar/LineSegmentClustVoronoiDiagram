//
// Created by Michel Kunkler on 24.04.18.
//

#include "PostgreSQLToBoostLoader.h"


std::string PostgreSQLToBoostLoader::db_name = "gis_denmark";
std::string PostgreSQLToBoostLoader::user_name = "michel";


bool LineSegment::operator==(const LineSegment &A) const {
    return line_segment_data == A.line_segment_data;
}

PostgreSQLToBoostLoader::PostgreSQLToBoostLoader() {
    char conninfo[200];
    sprintf(conninfo, "dbname = %s user = %s", db_name.c_str(), user_name.c_str());

    conn = PQconnectdb(conninfo);
    if (PQstatus(conn) != CONNECTION_OK) {
        std::cerr << "Connection to database failed: "
                    << PQerrorMessage(conn);
        PQfinish(conn);
    }
}

PostgreSQLToBoostLoader::~PostgreSQLToBoostLoader() {
    PQfinish(conn);
}

void PostgreSQLToBoostLoader::loadLineSegments(LineSegmentVector *lsv) {

    PGresult *res;
    if (!PQsendQuery(conn,
                     "select osm_id, sub_id, class, st_astext(st_transform(line, 4326)) from dijkstra_lines"))
    {
        fprintf(stderr, "Send query failed: %s", PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }

    int64_t osm_id;
    int32_t sub_id;
    int32_t dijkstra_class;
    char* line_segment;
    res = PQgetResult(conn);
    while(res != nullptr) {
        for (int64_t i= 0; i < PQntuples(res); i++) {
            osm_id = std::stoull(PQgetvalue(res, i, 0));
            sub_id = std::stoi(PQgetvalue(res, i, 1));
            dijkstra_class = std::stoi(PQgetvalue(res, i, 2));
            line_segment = PQgetvalue(res, i, 3);
            std::vector<std::array<double, 2>> coordinates = this->getCoordinatesFromLineString(
                    std::string(line_segment));

            LineSegment ls = {
                    LineSegmentData(
                            Point(wgsToInt(coordinates[0][0]), wgsToInt(coordinates[0][1])),
                            Point(wgsToInt(coordinates[1][0]), wgsToInt(coordinates[1][1]))
                    ), // line_segment_data
                    dijkstra_class, // dclass
            };

            lsv->emplace_back(ls);
        }
        PQclear(res);
        res = PQgetResult(conn);
    }
}

std::vector<std::array<double, 2>> PostgreSQLToBoostLoader::getCoordinatesFromLineString(std::string line_string) {
    std::vector<std::array<double, 2>> coordinate_list;

    line_string.pop_back();
    line_string = line_string.substr(11); // remove "LINESTRING(" and ")"

    std::vector<std::string> splits;
    boost::split(splits, line_string, boost::is_any_of(","));

    for(std::vector<std::string>::iterator it = splits.begin();
            it != splits.end();
            ++it) {
        std::vector<std::string> coordinates;
        boost::split(coordinates, *it, boost::is_any_of(" "));
        coordinate_list.push_back({stod(coordinates[0]), stod(coordinates[1])});
    }
    return coordinate_list;
}

int32_t PostgreSQLToBoostLoader::wgsToInt(double wgs) {
    return static_cast<int32>(wgs * 10000000 + 0.5); // lat * 10^7 and rounding
}