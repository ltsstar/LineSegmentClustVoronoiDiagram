//
// Created by michel on 12/28/18.
//

#include "VoronoiMerger.h"

void VoronoiMerger::merge(LineSegmentVector *lsv, VoronoiDiagramBoost *vd) {
  unsigned int cell_index = 0;
  for (VoronoiDiagramBoost::cell_iterator it = vd->cells().begin();
       it != vd->cells().end(); ++it) {
    VoronoiDiagramBoost::cell_type &cell = *it;
    VoronoiDiagramBoost::edge_type *current_edge = cell.incident_edge();
    if(current_edge == nullptr)
      continue;

    do {

      int cell_cluster = (*lsv)[cell.source_index()].dclass;
      if (true) {
        VoronoiDiagramBoost::cell_type * other_cell = current_edge->twin()->cell();
        int other_cluster = lsv->at(other_cell->source_index()).dclass;

        if( cell_cluster == other_cluster)
        {
          // merge cells
          current_edge->prev()->next( current_edge->twin()->next() );
          current_edge->next()->prev( current_edge->twin()->prev() );
          current_edge->twin()->next()->prev( current_edge->prev() );
          current_edge->twin()->prev()->next( current_edge->next() );
          other_cell->incident_edge(nullptr);
          if(cell.incident_edge() == current_edge) {
            cell.incident_edge(current_edge->next());
            current_edge = current_edge->next();
          }
          // delete condition
          if(current_edge->next() == current_edge->prev()) {
            cell.incident_edge(nullptr);
            current_edge->next(nullptr);
          }
        }
      }
      current_edge = current_edge->next();
    } while (current_edge != cell.incident_edge());
    ++cell_index;
  }
}
