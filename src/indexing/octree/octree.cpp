#include "fedes/indexing/octree/octree.hpp"

#include <span>
#include <stack>
#include <stdexcept>

#include "fedes/data/vector3.hpp"

namespace fedes {
	template <typename T>
	Octree<T>::Octree(const std::span<Vector3<T>>& points) {
		if (points.empty()) {
			throw std::length_error("Octree Constructor: Empty set of initial points sent");
		}
		Vector3<T> min;
		Vector3<T> max;
		for (auto& v : points) {
			if (v.x > max.x) {
				max.x = v.x;
			} else if (v.x < min.x) {
				min.x = v.x;
			}
			
			if (v.y > max.y) {
				max.y = v.y;
			} else if (v.y < min.y) {
				min.y = v.y;
			}

			if (v.z > max.z) {
				max.z = v.z;
			} else if (v.z < min.z) {
				min.z = v.z;
			}
		}
		Vector3<T> center = Vector3<T>((max.x + min.x) * 0.5, (max.y + min.y) * 0.5, (max.z + min.z) * 0.5);
		Vector3<T> extent = Vector3<T>((max.x - min.x), (max.y - min.y), (max.z - min.z));

		root_ = new Octant<T>(center, extent / 2);

		for (auto& v : points) {
			Insert(v);
		}
	}

	template<typename T>
	void Octree<T>::Insert(const Vector3<T>& point) {
		// @Todo: check for duplicate points before insertion
		InsertAtOctant(root_, point);
	}

	template<typename T>
	void Octree<T>::InsertAtOctant(Octant<T>* octant, const Vector3<T>& insertion_point) {

		// Handle leaf node case
		if (octant->IsLeaf()) {
			// If the node is a leaf and there's no data currently being stored, an insertion is valid
			if (octant->point == nullptr) {
				Vector3<T>* point = new Vector3<T>(insertion_point.x, insertion_point.y, insertion_point.z);
				octant->point = point;
				return;
			}
			// There's a point present at the leaf node already. Therefore a split should occur based on the current splitting criterion of 1 point per leaf.
			else {
				return Split(octant, insertion_point);
			}
		}
		// Handle interior node case - recursively insert into correct octant
		int oct = octant->DetermineChildOctant(insertion_point);
		Octant<T>* insert_octant = octant->children[oct];
		return InsertAtOctant(insert_octant, insertion_point);
	}

	// This function splits a leaf node into 8 children and inserts the new point alongside reinserting the old one
	template<typename T>
	void Octree<T>::Split(Octant<T>* octant, const Vector3<T>& insertion_point) {
		Vector3<T> current_point = *(octant->point);
		octant->point = nullptr;

		Vector3<T> split_origin;
		for (int i = 0; i < 8; i++) {
			split_origin.x = octant->extent.x * (i&4 ? 0.5 : -0.5);
			split_origin.y = octant->extent.y * (i&2 ? 0.5 : -0.5);
			split_origin.z = octant->extent.z * (i&1 ? 0.5 : -0.5);
			octant->children[i] = new Octant<T>(split_origin, octant->extent / 2);
		}

		// Insert current + new point in newly split octant directly
		InsertAtOctant(octant, current_point);
		InsertAtOctant(octant, insertion_point);
	}

	// Iterative post-order traversal for deletion of nodes, instead of a recursive `delete` approach through destructors
	template<typename T>
	void Octree<T>::Clear() {
		// @Todo: abstract this into some kind of iterator
		if (root_ == nullptr) {
			return;
		}
		std::stack<Octant<T>*>* stack = new std::stack<Octant<T>*>;
		std::stack<Octant<T>*>* output = new std::stack<Octant<T>*>;
		stack->push(root_);

		while (!stack->empty()) {
			Octant<T>* current = stack->top();
			stack->pop();
			output->push(current);
			if (!(current->IsLeaf())) {
				for (int i = 0; i < 8; i++) {
					stack->push(current->children[i]);
				}
			}
		}
		// Process output stack with a loop - free any memory that needs to be done with the guarantee of dealing with leaves first
		// ...
		// ...
		
		delete stack;
		delete output;
	}

	template <typename T>
	Octree<T>::~Octree() {
		Clear();
	}


	template class Octree<double>;
	template class Octree<float>;
}