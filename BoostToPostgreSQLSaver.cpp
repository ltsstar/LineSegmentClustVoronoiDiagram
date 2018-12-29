//
// Created by Michel Kunkler on 18.05.18.
//

#include <string>

#include "BoostToPostgreSQLSaver.h"
#include "voronoi_visual_utils.h"

#include "boost/polygon/voronoi.hpp"
#include "BoostVoronoiVisualizer.h"
#include "tests/catch.hpp"
#include <sstream>

using boost::polygon::voronoi_builder;
using boost::polygon::voronoi_diagram;
using boost::polygon::voronoi_edge;
using boost::polygon::point_data;
using boost::polygon::segment_data;

std::string BoostToPostgreSQLSaver::db_name = "gis_artificial";
std::string BoostToPostgreSQLSaver::user_name = "michel";

BoostToPostgreSQLSaver::BoostToPostgreSQLSaver() {
    char conninfo[200];
    sprintf(conninfo, "dbname = %s user = %s", db_name.c_str(), user_name.c_str());

    conn = PQconnectdb(conninfo);
    if (PQstatus(conn) != CONNECTION_OK) {
        std::cerr << "Connection to database failed: "
                  << PQerrorMessage(conn);
        PQfinish(conn);
    }
}

BoostToPostgreSQLSaver::~BoostToPostgreSQLSaver() {
    PQfinish(conn);
}


double BoostToPostgreSQLSaver::intToWgs(int32_t wgs) {
    return wgs / double(10000000);
}

void BoostToPostgreSQLSaver::saveVoronoiDiagram(LineSegmentVector *lsv, VoronoiDiagramBoost *vd) {
    std::vector<LineSegmentData> ssd = std::vector<LineSegmentData>();
    for(auto line_segment = lsv->begin(); line_segment != lsv->end(); ++line_segment) {
        LineSegmentData lsdata = line_segment->line_segment_data;
        ssd.push_back(lsdata);
    }
    BoostVoronoiVisualizer boostVoronoiVisualizer(ssd);
    int64_t i = 0;
    for (auto it = vd->cells().begin();
            it != vd->cells().end();
            ++it) {
        if(it->incident_edge() == nullptr)
            continue;
        i++;
        std::ostringstream query;
        query << "INSERT INTO voronoi_cells VALUES (";
        query << i << ", 0, 0, ";
        query << "ST_MakePolygon(ST_GeomFromText('LINESTRING(";

        voronoi_edge<double> *edge = it->incident_edge();
        do {
            std::string edge_string;
            if(edge->is_finite()) {
                if (edge->is_curved()) {
                    std::vector<point_data<double>> sampled_edge;
                    point_data<double> p1 (edge->vertex0()->x(), edge->vertex0()->y());
                    sampled_edge.push_back(p1);
                    point_data<double> p2 (edge->vertex1()->x(), edge->vertex1()->y());
                    sampled_edge.push_back(p2);
                    boostVoronoiVisualizer.sampleCurvedEdge(*edge, &sampled_edge);
                    this->addEdgeToString(&sampled_edge, query);
                } else {
                    std::vector<point_data<double>> sampled_edge;
                    point_data<double> p1 (edge->vertex0()->x(), edge->vertex0()->y());
                    sampled_edge.push_back(p1);
                    point_data<double> p2 (edge->vertex1()->x(), edge->vertex1()->y());
                    sampled_edge.push_back(p2);

                    this->addEdgeToString(&sampled_edge, query);
                    //delete sampled_edge;
                }
            }
            edge = edge->next();
            // Do smth. with edge.
        } while (edge != it->incident_edge() && edge != nullptr);
        std::string query_str = query.str();
        query_str.pop_back();
        query_str.pop_back();
        query_str += ")', 4326)))";

        PGresult *res;
        char * query_ch = new char [query_str.length()+1];
        strcpy (query_ch, query_str.c_str());
        res = PQexec(this->conn, query_ch);
        if (!res)
        {
            fprintf(stderr, "Send query failed: %s", PQresultStatus(res));
            PQfinish(conn);
            exit(1);
        }
        delete query_ch;
    }
}

void BoostToPostgreSQLSaver::addEdgeToString(std::vector<point_data<double>> *edge, std::ostringstream& resultStream) {
    for( auto const &sample : *edge) {
        resultStream << this->intToWgs(static_cast<int32>(sample.x()));
        resultStream << " ";
        resultStream << this->intToWgs(static_cast<int32>(sample.y()));
        resultStream << ", ";
    }
}
