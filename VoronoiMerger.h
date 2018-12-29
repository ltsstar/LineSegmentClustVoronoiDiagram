//
// Created by michel on 12/28/18.
//

#ifndef LINESEGMENTCLUSTERVORONOIDIAGRAM_VORONOIMERGER_H
#define LINESEGMENTCLUSTERVORONOIDIAGRAM_VORONOIMERGER_H


#include "PostgreSQLToBoostLoader.h"

class VoronoiMerger {
public:
    static void merge(LineSegmentVector *lsv, VoronoiDiagramBoost *vd);
};


#endif //LINESEGMENTCLUSTERVORONOIDIAGRAM_VORONOIMERGER_H
