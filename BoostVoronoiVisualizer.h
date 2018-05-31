//
// Created by Michel Kunkler on 18.05.18.
//

#ifndef LINESEGMENTCLUSTERVORONOIDIAGRAM_BOOSTVORONOIVISUALIZER_H
#define LINESEGMENTCLUSTERVORONOIDIAGRAM_BOOSTVORONOIVISUALIZER_H

#include <boost/polygon/polygon.hpp>
#include <boost/polygon/voronoi.hpp>
using namespace boost::polygon;

#include "voronoi_visual_utils.h"


class BoostVoronoiVisualizer {
private:
    typedef double coordinate_type;
    typedef point_data<coordinate_type> point_type;
    typedef segment_data<coordinate_type> segment_type;
    typedef rectangle_data<coordinate_type> rect_type;
    typedef voronoi_builder<int> VB;
    typedef voronoi_diagram<coordinate_type> VD;
    typedef VD::cell_type cell_type;
    typedef VD::cell_type::source_index_type source_index_type;
    typedef VD::cell_type::source_category_type source_category_type;
    typedef VD::edge_type edge_type;
    typedef VD::cell_container_type cell_container_type;
    typedef VD::cell_container_type vertex_container_type;
    typedef VD::edge_container_type edge_container_type;
    typedef VD::const_cell_iterator const_cell_iterator;
    typedef VD::const_vertex_iterator const_vertex_iterator;
    typedef VD::const_edge_iterator const_edge_iterator;

    typedef std::vector<segment_type> SSD_POLYGON;

    coordinate_type max_dist = 1000.0;
    SSD_POLYGON *segment_data_;

    point_type retrievePoint(const cell_type& cell);
    segment_type retrieveSegment(const cell_type& cell);

public:
    void sampleCurvedEdge(const edge_type& edge, std::vector<point_type>* sampled_edge);
    explicit BoostVoronoiVisualizer(SSD_POLYGON &segment_data);
};


#endif //LINESEGMENTCLUSTERVORONOIDIAGRAM_BOOSTVORONOIVISUALIZER_H
