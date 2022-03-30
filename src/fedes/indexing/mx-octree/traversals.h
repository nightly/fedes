#pragma once 

#include <stack>
#include <memory>
#include <iterator>

#include "fedes/indexing/mx-octree/octant.h"

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
		std::unique_ptr<std::stack<pointer_type>> process_stack_;
		std::unique_ptr<std::stack<pointer_type>> ordered_stack_;
	public:
		OctreePostOrderIterator(pointer_type root_octant);
		OctreePostOrderIterator(const OctreePostOrderIterator& other) noexcept;
		OctreePostOrderIterator(OctreePostOrderIterator&& other) noexcept;
		OctreePostOrderIterator& operator=(const OctreePostOrderIterator& other) noexcept;
		OctreePostOrderIterator& operator=(OctreePostOrderIterator&& other) noexcept;

		OctreePostOrderIterator& operator++();
		OctreePostOrderIterator operator++(int);
		pointer_type operator->();
		reference_type operator*();
		bool operator==(const OctreePostOrderIterator& other) const;
		bool operator!=(const OctreePostOrderIterator& other) const;
	};
}

#include "fedes/indexing/mx-octree/octree.h"