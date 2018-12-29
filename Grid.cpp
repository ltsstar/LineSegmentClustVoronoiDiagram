//
// Created by Michel Kunkler on 26.05.18.
//

#include "Grid.h"

GridLocation::GridLocation() {

}

GridLocation::GridLocation(int x, int y) : x(x), y(y) {

}

bool GridLocation::operator==(const GridLocation &A) const {
    return (A.x == x) && (A.y == y);
}

IntCoordinate WgsCoordinate::toInt() {
    return IntCoordinate( static_cast<uint32_t>(lon * 10000000), static_cast<uint32_t>(lat * 10000000) );
}

WgsCoordinate::WgsCoordinate(double lon, double lat) : lon(lon), lat(lat) {

}

WgsCoordinate::WgsCoordinate(Point point) {
    lon = static_cast<uint32_t>( point.x() / double(10000000) );
    lat = static_cast<uint32_t>( point.y() / double(10000000) );
}


WgsCoordinate IntCoordinate::toWgs() {
    return WgsCoordinate( x / double(10000000), y / double(10000000) );
}

IntCoordinate::IntCoordinate(uint32_t x, uint32_t y) : x(x), y(y) {

}

IntCoordinate::IntCoordinate(Point point) {
    x = static_cast<uint32_t>( point.x() );
    y = static_cast<uint32_t>( point.y() );
}

bool Square::is_empty() {
    return dijkstra_classes.empty();
}

bool Square::multiple_classes() {
    return dijkstra_classes.size() > 1;
}

void Square::add_dijkstra_class(int dijkstra_class) {
    if(!contains_dijkstra_class(dijkstra_class))
        dijkstra_classes.emplace_back(dijkstra_class);
}

bool Square::is_only_dijkstra_class(int dijkstra_class) {
    return (dijkstra_classes.size() == 1) && contains_dijkstra_class(dijkstra_class);
}

bool Square::contains_dijkstra_class(int dijkstra_class) {
    return std::find(dijkstra_classes.begin(), dijkstra_classes.end(), dijkstra_class) != dijkstra_classes.end();
}

void Square::add_part_segment(LineSegment line_segment) {
    partly_containing_segments.emplace_back(line_segment);
    add_dijkstra_class(line_segment.dclass);
}

void Square::add_full_segment(LineSegment line_segment) {
    fully_containing_segments.emplace_back(line_segment);
    add_dijkstra_class(line_segment.dclass);
}

int Square::get_first_dijkstra_class() {
    return dijkstra_classes[0];
}

void Square::clear() {
    fully_containing_segments.clear();
    partly_containing_segments.clear();
}

LineSegmentVector Square::get_fully_containing_line_segments() {
    return fully_containing_segments;
}

LineSegmentVector Square::get_partly_containing_line_segments() {
    return partly_containing_segments;
}

GridLocation Grid::findGridLocation(IntCoordinate int_coordinate) {
    return GridLocation(0,0);
}

GridLocation Grid::findGridLocation(POINT point) {
    int grid_x = static_cast<int>( (point.x() - extend_x.first) / square_size);
    int grid_y = static_cast<int>( (point.y() - extend_y.first) / square_size);
    return GridLocation(grid_x, grid_y);
}

/**
 * Bresenham modification
 *
 */
void Grid::findAllGridLocations(LineSegment *line_segment, GridLocationList * result)
{
    //make sure
    GridLocation g0 = findGridLocation(line_segment->line_segment_data.low());
    GridLocation g1 = findGridLocation(line_segment->line_segment_data.high());

    int32_t x0 = line_segment->line_segment_data.low().x(), x1 = line_segment->line_segment_data.high().x();
    int32_t y0 = line_segment->line_segment_data.low().y(), y1 = line_segment->line_segment_data.high().y();

    int dx = std::abs(g1.x - g0.x) * square_size, sx = (g0.x < g1.x ? 1 : -1) * square_size;
    int dy = -std::abs(g1.y - g0.y) * square_size, sy = (g0.y < g1.y ? 1 : -1) * square_size;
    int err = dx + dy, e2;

    for (;;) {
        POINT p0 = POINT(x0, y0);
        GridLocation g0 = findGridLocation(p0);
        result->emplace_back(g0);

        if (g0 == g1)
            return;

        e2 = 2 * err;

        // EITHER horizontal OR vertical step (but not both!)
        if (e2 > dy) {
            err += dy;
            x0 += sx;
        } else if (e2 < dx) { // <--- this "else" makes the difference
            err += dx;
            y0 += sy;
        }
    }
}

Grid::GridLocationList Grid::findGridLocations(LineSegment *line_segment) {
    GridLocationList result;
    GridLocation low = findGridLocation(line_segment->line_segment_data.low());
    GridLocation high = findGridLocation(line_segment->line_segment_data.high());
    if( high == low ) {
        result.emplace_back(low);
    } else {
        findAllGridLocations(line_segment, &result);
    }
    return result;
}

bool Grid::isInOneSquare(LineSegment * line_segment) {
    IntCoordinate low_point = IntCoordinate( line_segment->line_segment_data.low() );
    IntCoordinate high_point = IntCoordinate( line_segment->line_segment_data.high() );

    return findGridLocation( low_point ) == findGridLocation( high_point );
}

bool Grid::isCleanable(GridLocation square_location) {
    Square * central_square = getSquare(square_location);

    if( central_square == nullptr || central_square->is_empty() || central_square->multiple_classes() )
        return false;

    int central_square_dijkstra_class = central_square->get_first_dijkstra_class();

    GridLocation checking_square_location;
    Square * checking_square;
    // check that no other classes are within the 3 surrounding squares
    for(int i = -3; i <= 3; ++i) {
        for(int j = -3; j <= 3; ++j) {
            checking_square_location = GridLocation(square_location.x + i, square_location.y + j);
            checking_square = getSquare(checking_square_location);
            if( checking_square == nullptr ||
                !checking_square->is_empty() ||
                !checking_square->is_only_dijkstra_class(central_square_dijkstra_class) )
                return false;
        }
    }

    //left + right
    for(int i = -2; i <= 2; i += 4) {
        for(int j= -1; j <= 1; ++j) {
            checking_square_location = GridLocation(square_location.x + i, square_location.y + j);
            checking_square = getSquare(checking_square_location);
            if( checking_square == nullptr || checking_square->is_only_dijkstra_class(central_square_dijkstra_class) )
                return false;
        }
    }


    //bottom + top
    for(int i = -2; i <= 2; i += 4) {
        for(int j= -1; j <= 1; ++j) {
            checking_square_location = GridLocation(square_location.x + j, square_location.y + i);
            checking_square = getSquare(checking_square_location);
            if( checking_square == nullptr || checking_square->is_only_dijkstra_class(central_square_dijkstra_class) )
                return false;
        }
    }

    return true;
}

Square* Grid::getSquare(GridLocation grid_location) {
    if(grid_location.x < 0 || grid_location.y < 0)
        return nullptr;
    else if (grid_location.x >= grid_x || grid_location.y >= grid_y)
        return nullptr;

    return &(this->grid[grid_location.x][grid_location.y]);
}

void Grid::sortIntoGrid() {
    for(auto line_segment = lsv->begin();
            line_segment != lsv->end();
            ++line_segment) {
        Grid::GridLocationList grid_locations = findGridLocations( &(*line_segment) );
        if( grid_locations.size() == 1) {
            Square *square = getSquare(grid_locations[0]);
            square->add_full_segment( *line_segment );
        } else {
            for (auto grid_location = grid_locations.begin(); grid_location != grid_locations.end(); ++grid_location) {
                Square *square = getSquare(*grid_location);
                square->add_part_segment( *line_segment );
            }
        }
    }
}

void Grid::cleanSegments() {
    GridLocation grid_location = GridLocation(0, 0);
    for(auto x_it = grid.begin(); x_it != grid.end(); ++x_it) {
        grid_location.y = 0;
        for(auto y_it = x_it->begin(); y_it != x_it->end(); ++y_it) {
            if( isCleanable(grid_location) ) {
                getSquare(grid_location)->clear();
            }
            grid_location.y++;
        }
        grid_location.x++;
    }
}

void Grid::saveTo(LineSegmentVector *new_lsv) {
    for(auto x_it = grid.begin(); x_it != grid.end(); ++x_it) {
        for(auto y_it = x_it->begin(); y_it != x_it->end(); ++y_it) {
            LineSegmentVector full_ls = y_it->get_fully_containing_line_segments();
            for(auto line_seg_it = full_ls.begin(); line_seg_it != full_ls.end(); ++line_seg_it) {
                new_lsv->emplace_back( *line_seg_it );
            }
            LineSegmentVector part_ls = y_it->get_partly_containing_line_segments();
            // make sure the line segment gets only added in the first square
            for(auto line_seg_it = part_ls.begin(); line_seg_it != part_ls.end(); ++line_seg_it) {
                // find other squares of this LS
                GridLocationList gll = findGridLocations(&(*line_seg_it));
                for(auto grid_it = gll.begin(); grid_it != gll.end(); ++grid_it)
                {
                    Square * square = getSquare(*grid_it);
                    if(square == &(*y_it) ) {
                        new_lsv->emplace_back( *line_seg_it );
                        break;
                    }
                    else if(!square->is_empty()) {
                        break;
                    }
                }
            }
        }
    }
}

Grid::Grid(LineSegmentVector *lsv,
           std::pair<double, double> extend_x,
           std::pair<double, double> extend_y,
           double square_size) :
        lsv(lsv), extend_x(extend_x), extend_y(extend_y), square_size(square_size) {
    grid_x = static_cast<int>((extend_x.second - extend_x.first) / square_size + 1);
    grid_y = static_cast<int>((extend_y.second - extend_y.first) / square_size + 1);

    grid = std::vector<std::vector<Square>>(grid_x);
    for(auto it = grid.begin(); it != grid.end(); ++it) {
        *it = std::vector<Square>(grid_y);
    }
}

Grid::~Grid() {
}
