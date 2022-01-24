#include "fedes/indexing/octree/traversals.hpp"

#include <stack>
#include <iterator>
#include <memory>

#include "fedes/indexing/octree/octant.hpp"
#include "fedes/indexing/octree/octree.hpp"

namespace fedes {

	/* @brief Octree Post-Order Iterator constructor, with the ordered stack being finalised at time of Iterator construction
	* 
	* Empty octants (i.e. empty leaf) are present for completeness to facilitate memory deallocation
	* 
	* @tparam Octree: the Octree type, e.g. Octree<double>
	* @param root_octant: pointer to the top-level Octant to post-order traverse from
	*/
	template <typename Octree>
	OctreePostOrderIterator<Octree>::OctreePostOrderIterator(pointer_type root_octant) {
		process_stack_ = std::make_unique<std::stack<pointer_type>>();
		ordered_stack_ = std::make_unique<std::stack<pointer_type>>();
		if (root_octant == nullptr) {
			return;
		}
		process_stack_->push(root_octant);
		while (!process_stack_->empty()) {
			Octant* current = process_stack_->top();
			process_stack_->pop();
			ordered_stack_->push(current);
			if (!(current->IsLeaf())) {
				for (int i = 0; i < 8; i++) {
					process_stack_->push(current->child[i]);
				}
			}
		}
	}

	/*
	* @brief Copy constructor
	*/
	template <typename Octree>
	OctreePostOrderIterator<Octree>::OctreePostOrderIterator(const OctreePostOrderIterator& other) noexcept {
		*process_stack_ = *(other.process_stack_);
		*ordered_stack_ = *(other.ordered_stack_);
	}

	/*
	* @brief Move constructor
	*/
	template <typename Octree>
	OctreePostOrderIterator<Octree>::OctreePostOrderIterator(OctreePostOrderIterator&& other) noexcept {
		*process_stack_ = *(other.process_stack_);
		*ordered_stack_ = *(other.ordered_stack_);
		other.process_stack_ = nullptr;
		other.ordered_stack_ = nullptr;
	}

	/*
	* @brief Copy assignment
	*/
	template <typename Octree>
	OctreePostOrderIterator<Octree>& OctreePostOrderIterator<Octree>::operator=(const OctreePostOrderIterator& other) noexcept {
		*process_stack_ = *(other.process_stack_);
		*ordered_stack_ = *(other.ordered_stack_);
		return *this;
	}

	/*
	* @brief Move assignment
	*/
	template <typename Octree>
	OctreePostOrderIterator<Octree>& OctreePostOrderIterator<Octree>::operator=(OctreePostOrderIterator&& other) noexcept {
		*process_stack_ = *(other.ordered_stack_);
		*ordered_stack_ = *(other.ordered_stack_);
		other.process_stack_ = nullptr;
		other.ordered_stack_ = nullptr;
		return *this;
	}

	/*
	* @brief Prefix increment: pops the top node from the ordered stack
	*/
	template <typename Octree>
	OctreePostOrderIterator<Octree>& OctreePostOrderIterator<Octree>::operator++() {
		ordered_stack_->pop();
		return *this;
	}

	/*
	* @brief Postfix increment: N.B. involves an expensive copy
	*/
	template <typename Octree>
	OctreePostOrderIterator<Octree> OctreePostOrderIterator<Octree>::operator++(int) {
		OctreePostOrderIterator<Octree> iterator = *this;
		++(*this);
		return *this;
	}
	
	/*
	* @brief Pointer type operator
	*/
	template <typename Octree>
	OctreePostOrderIterator<Octree>::pointer_type OctreePostOrderIterator<Octree>::operator->() {
		Octant* top =ordered_stack_->top();
		return top;
	}

	/*
	* @brief Reference type operator
	*/
	template <typename Octree>
	OctreePostOrderIterator<Octree>::reference_type OctreePostOrderIterator<Octree>::operator*() {
		Octant* top = ordered_stack_->top();
		return *top;
	}
	
	/*
	* @brief Equality type operator
	*/
	template <typename Octree>
	bool OctreePostOrderIterator<Octree>::operator==(const OctreePostOrderIterator& other) const {
		return *ordered_stack_ == *(other.ordered_stack_);
	}
	
	/*
	* @brief Not equal operator
	*/
	template <typename Octree>
	bool OctreePostOrderIterator<Octree>::operator!=(const OctreePostOrderIterator& other) const {
		return *ordered_stack_ != *(other.ordered_stack_);
	}

	/* Explicit template instantiations */
	template class OctreePostOrderIterator<Octree<double>>;
	template class OctreePostOrderIterator<Octree<float>>;
}