//
// Created by Michel Kunkler on 18.05.18.
//

#include "BoostVoronoiDiagram.h"

void BoostVoronoiDiagram::cleanSegmentSet(LineSegmentVector *lsv) {
    typedef int32 Unit;
    typedef boost::polygon::scanline_base<Unit>::Point Point;
    typedef boost::polygon::scanline_base<Unit>::half_edge half_edge;
    typedef int segment_id;
    std::vector<std::pair<half_edge, segment_id> > half_edges;
    std::vector<std::pair<half_edge, segment_id> > half_edges_out;
    segment_id id = 0;
    half_edges.reserve(lsv->size());
    for (auto line_segment = lsv->begin();
         line_segment != lsv->end(); ++line_segment) {
        Point l = line_segment->line_segment_data.low();
        Point h = line_segment->line_segment_data.high();
        half_edges.emplace_back(std::make_pair(half_edge(l, h), id++));
    }
    half_edges_out.reserve(half_edges.size());

    // Apparently no need to pre-sort data when calling validate_scan.
    boost::polygon::line_intersection<Unit>::validate_scan(
            half_edges_out, half_edges.begin(), half_edges.end());

    LineSegmentVector result;
    result.reserve(half_edges_out.size());
    for (std::size_t i = 0; i < half_edges_out.size(); ++i) {
        id = half_edges_out[i].second;
        Point l = half_edges_out[i].first.first;
        Point h = half_edges_out[i].first.second;
        LineSegment orig_seg = lsv->at(id);
        if (orig_seg.line_segment_data.high() < orig_seg.line_segment_data.low())
            std::swap(l, h);
        LineSegment new_seg = {
                LineSegmentData(l, h), // line_segment_data
                orig_seg.dclass, // dclass
        };
        result.emplace_back(new_seg);
    }
    std::swap(result, *lsv);
}

void BoostVoronoiDiagram::create(LineSegmentVector *lsv) {
    cleanSegmentSet(lsv);

    std::vector<LineSegmentData> ssd = std::vector<LineSegmentData>();
    for(auto ls = lsv->begin(); ls != lsv->end(); ++ls) {
        ssd.push_back(ls->line_segment_data);
    }
    boost::polygon::construct_voronoi(ssd.begin(), ssd.end(), &vd);
}
