//
// Created by michel on 12/28/18.
//

#include "catch.hpp"
#include "../Grid.h"
#include "../BoostVoronoiDiagram.h"
#include "../VoronoiMerger.h"
#include <chrono>

TEST_CASE("Benchmark", "[Benchmark]") {
  SECTION( "Benchmark" ) {
    int iterations = 1;
    int square_size_double_min = 2;
    int square_size_double_dec = 2;

    PostgreSQLToBoostLoader postgreSQLToBoostLoader;
    LineSegmentVector lsv;
    postgreSQLToBoostLoader.loadLineSegments(&lsv);

    IntCoordinate low = WgsCoordinate(7.8, 54.1).toInt();
    IntCoordinate high = WgsCoordinate(13, 57.8).toInt();
    std::pair<int32_t, int32_t> extend_x = std::pair<int32_t, int32_t>(low.x, high.x);
    std::pair<int32_t, int32_t> extend_y = std::pair<int32_t, int32_t>(low.y, high.y);

    std::cout << "Line segments before grid: " << lsv.size() << std::endl;


    std::map<int, std::vector<std::tuple<double, int, double, double>>> results;
    for(int i = 0; i < iterations; ++i) {
      for (int square_size_double = 100; square_size_double >= square_size_double_min;
           square_size_double -= square_size_double_dec) {

        if(i == 0) {
          results[square_size_double] = std::vector<
                  std::tuple<double, int, double, double>>();
        }

        auto start = std::chrono::system_clock::now();

        int32_t square_size = static_cast<int32_t>(square_size_double * 10000000 / 1000);
        Grid grid = Grid(&lsv, extend_x, extend_y, square_size);
        grid.sortIntoGrid();
        grid.cleanSegments();
        LineSegmentVector cleaned_lsv;
        grid.saveTo(&cleaned_lsv);

        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> diff1 = end-start;
        start = std::chrono::system_clock::now();


        BoostVoronoiDiagram boostVoronoiDiagram;
        boostVoronoiDiagram.create(&cleaned_lsv);


        end = std::chrono::system_clock::now();
        std::chrono::duration<double> diff2 = end-start;
        start = std::chrono::system_clock::now();


        VoronoiMerger::merge(&cleaned_lsv, &boostVoronoiDiagram.vd);

        end = std::chrono::system_clock::now();
        std::chrono::duration<double> diff3 = end-start;

        results[square_size_double].push_back(
                std::make_tuple(diff1.count(), cleaned_lsv.size(), diff2.count(), diff3.count())
        );
      }
    }

    for (int square_size_double = 100; square_size_double >= square_size_double_min; square_size_double -= square_size_double_dec) {
      std::cout << "--------------------------------------------" << std::endl;
      std::cout << "Square size: " << square_size_double << std::endl;
      for(int i = 0; i < iterations; ++i) {
        std::cout << std::get<0>(results[square_size_double][i]) << ", ";
      }
      std::cout << std::endl << "########################" << std::endl;
      for(int i = 0; i < iterations; ++i) {
        std::cout << std::get<2>(results[square_size_double][i]) << ", ";
      }
      std::cout << std::endl << "########################" << std::endl;
      for(int i = 0; i < iterations; ++i) {
        std::cout << std::get<3>(results[square_size_double][i]) << ", ";
      }
      std::cout << std::endl << "########################" << std::endl;
      for(int i = 0; i < iterations; ++i) {
        std::cout << std::get<1>(results[square_size_double][i]) << ", ";
      }
      std::cout << std::endl << "---------------------------------------" << std::endl;
    }
  }
}