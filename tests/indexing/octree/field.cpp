#include <gtest/gtest.h>
#include "fedes/indexing/octree/traversals.h"

#include <vector>

#include "fedes/indexing/octree/octree.h"
#include "fedes/indexing/octree/octant.h"
#include "fedes/maths/random.h"
#include "fedes/maths/vector3.h"

/*
*		3------7.
*		|`.    | `.
*		|  `2--+---6
*		|   |  |   |
*		1---+--5.  |
*		 `. |    `.|
*		   `0------4
*/

class OctreeFieldTest : public ::testing::Test {
protected:
	std::unique_ptr<fedes::Octree<double>> octree_;
	std::vector<fedes::Vector3<double>> points_;

	void SetUp() override {
		points_ = fedes::GenerateRandomArray<double>(-1.00, 1.00, 48);
		points_.emplace_back(-1.00, -1.00, -1.00);
		points_.emplace_back(1.00, 1.00, 1.00);
		octree_ = std::make_unique<fedes::Octree<double>>(points_, 10, 0);
	};

	fedes::Vector3<double> BestForDir(uint_fast8_t dir) {
		double best_dist = std::numeric_limits<double>::max();
		for (const auto& p : points_) {
			if (fedes::DetermineDirection(fedes::Vector3<double>(0.0, 0.0, 0.0), p) == dir) {
				double current_dist = fedes::DistanceSquared(p, fedes::Vector3<double>(0.0, 0.0, 0.0));
				if (current_dist < best_dist) {
					best_dist = current_dist;
				}
			}
		}
	}
};


TEST_F(OctreeFieldTest, Direction2) {
	
}