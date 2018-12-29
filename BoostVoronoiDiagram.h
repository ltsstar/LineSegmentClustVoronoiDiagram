//
// Created by Michel Kunkler on 18.05.18.
//

#ifndef LINESEGMENTCLUSTERVORONOIDIAGRAM_BOOSTVORONOIDIAGRAM_H
#define LINESEGMENTCLUSTERVORONOIDIAGRAM_BOOSTVORONOIDIAGRAM_H

#include "PostgreSQLToBoostLoader.h"


class BoostVoronoiDiagram {
private:
public:
    VoronoiDiagramBoost vd;

    void create(LineSegmentVector *lsv);
    void cleanSegmentSet(LineSegmentVector *lsv);

    virtual ~BoostVoronoiDiagram();
};


#endif //LINESEGMENTCLUSTERVORONOIDIAGRAM_BOOSTVORONOIDIAGRAM_H
