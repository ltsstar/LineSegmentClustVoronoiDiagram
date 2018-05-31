//
// Created by Michel Kunkler on 18.05.18.
//

#include "BoostVoronoiVisualizer.h"

void BoostVoronoiVisualizer::sampleCurvedEdge(const BoostVoronoiVisualizer::edge_type &edge,
                                              std::vector<BoostVoronoiVisualizer::point_type> *sampled_edge) {
    point_type point = edge.cell()->contains_point() ?
                       retrievePoint(*edge.cell()) :
                       retrievePoint(*edge.twin()->cell());
    segment_type segment = edge.cell()->contains_point() ?
                           retrieveSegment(*edge.twin()->cell()) :
                           retrieveSegment(*edge.cell());
    voronoi_visual_utils<coordinate_type>::discretize(
            point, segment, max_dist, sampled_edge);

}

BoostVoronoiVisualizer::point_type BoostVoronoiVisualizer::retrievePoint(
        const BoostVoronoiVisualizer::cell_type &cell
) {
    source_index_type index = cell.source_index();
    source_category_type category = cell.source_category();

    if (category == SOURCE_CATEGORY_SEGMENT_START_POINT) {
        return (*segment_data_)[index].low();
    } else {
        return (*segment_data_)[index].high();
    }

}

BoostVoronoiVisualizer::segment_type BoostVoronoiVisualizer::retrieveSegment(
        const BoostVoronoiVisualizer::cell_type &cell
) {
    source_index_type index = cell.source_index();
    return (*segment_data_)[index];
}

BoostVoronoiVisualizer::BoostVoronoiVisualizer(BoostVoronoiVisualizer::SSD_POLYGON &segment_data)
        : segment_data_(&segment_data) {
}
