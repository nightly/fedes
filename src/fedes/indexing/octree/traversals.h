#pragma once 

#include <stack>
#include <memory>
#include <iterator>

#include "fedes/indexing/octree/octant.h"

namespace fedes {

	template<typename Octree>
	class OctreePostOrderIterator {
	public:
		using Octant = typename Octree::Octant;

		using iterator_catergory = std::input_iterator_tag;
		using value_type = Octant;
		using pointer_type = value_type*;
		using reference_type = value_type&;
	private:
		std::stack<pointer_type> visit_stack_;
		std::stack<pointer_type> ordered_stack_;
	public:

		/* 
		 *@brief Octree Post-Order Iterator constructor, with the ordered stack being finalised at time of Iterator construction
		 * 
		 * Empty octants (i.e. empty leaf) are present for completeness to facilitate memory deallocation
		 */
		OctreePostOrderIterator(pointer_type root_octant) {
			if (root_octant == nullptr) {
				return;
			}
			visit_stack_.push(root_octant);
			while (!visit_stack_.empty()) {
				pointer_type oct = visit_stack_.top();
				visit_stack_.pop();
				ordered_stack_.push(oct);
				if (!oct->IsLeaf()) {
					for (uint_fast8_t i = 0; i < 8; i++) {
						visit_stack_.push(oct->child[i]);
					}
				}
			}
		}


		/*
		* @brief Copy constructor
		*/
		OctreePostOrderIterator(const OctreePostOrderIterator& other) noexcept {
			visit_stack_ = other.visit_stack_;
			ordered_stack_ = other.ordered_stack_;
		}
		
		/*
		* @brief Move constructor
		*/
		OctreePostOrderIterator(OctreePostOrderIterator&& other) noexcept {
			visit_stack_ = std::move(other.visit_stack_);
			ordered_stack_ = std::move(other.ordered_stack_);
		}

		/*
		* @brief Copy assignment
		*/
		OctreePostOrderIterator& operator=(const OctreePostOrderIterator& other) noexcept {
			visit_stack_ = std::move(other.visit_stack_);
			ordered_stack_ = std::move(other.ordered_stack_);
			return *this;
		}
		

		/*
		* @brief Move assignment
		*/
		OctreePostOrderIterator& operator=(OctreePostOrderIterator&& other) noexcept {
			visit_stack_ = std::move(other.visit_stack_);
			ordered_stack_ = std::move(other.ordered_stack_);
			return *this;
		}

		/*
		* @brief Prefix increment: pops the top node from the ordered stack
		*/
		OctreePostOrderIterator& operator++() {
			ordered_stack_.pop();
			return *this;
		}

		/*
		* @brief: Postfix increment
		* @Performance: N.B. involves an expensive copy
		*/
		OctreePostOrderIterator operator++(int) {
			OctreePostOrderIterator<Octree> iterator = *this;
			++(*this);
			return *this;
		}

		/*
		* @brief Pointer type operator
		*/
		pointer_type operator->() {
			Octant* top = ordered_stack_.top();
			return top;
		}
		
		/*
		* @brief Reference type operator
		*/
		reference_type operator*() {
			Octant* top = ordered_stack_.top();
			return *top;
		}

		/*
		* @brief Equality type operator
		*/
		bool operator==(const OctreePostOrderIterator& other) const {
			return ordered_stack_ == other.ordered_stack_;
		}
		
		/*
		* @brief Inequality type operator
		*/
		bool operator!=(const OctreePostOrderIterator& other) const {
			return ordered_stack_ != other.ordered_stack_;
		}
	};
}

#include "fedes/indexing/octree/octree.h"