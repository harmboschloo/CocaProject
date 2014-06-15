// Copyright (C) 2002-2009 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine" and the "irrXML" project.
// For conditions of distribution and use, see copyright notice in irrlicht.h and irrXML.h

#ifndef __IRR_ARRAY_H_INCLUDED__
#define __IRR_ARRAY_H_INCLUDED__

#include "irrTypes.h"
#include "heapsort.h"
#include "irrAllocator.h"

namespace irr
{
namespace core
{

//! Self reallocating template array (like stl vector) with additional features.
/** Some features are: Heap sorting, binary search methods, easier debugging.
*/
template <class T, typename TAlloc = irrAllocator<T> >
class array
{

public:

	//! Default constructor for empty array.
	array()
		: data(0), allocated(0), used(0),
			strategy(ALLOC_STRATEGY_DOUBLE), free_when_destroyed(true), is_sorted(true)
	{
	}

	//! Constructs an array and allocates an initial chunk of memory.
	/** \param start_count Amount of elements to pre-allocate. */
	array(u32 start_count)
      : data(0), allocated(0), used(0),
        strategy(ALLOC_STRATEGY_DOUBLE), free_when_destroyed(true), is_sorted(true)
	{
		reallocate(start_count);
	}


	//! Copy constructor
	array(const array<T>& other)
		: data(0)
	{
		*this = other;
	}



	//! Destructor.
	/** Frees allocated memory, if set_free_when_destroyed was not set to
	false by the user before. */
	~array()
	{
		if (free_when_destroyed)
		{
			for (u32 i=0; i<used; ++i)
				allocator.destruct(&data[i]);

			allocator.deallocate(data);
		}
	}


	//! Reallocates the array, make it bigger or smaller.
	/** \param new_size New size of array. */
	void reallocate(u32 new_size)
	{
		T* old_data = data;

		data = allocator.allocate(new_size); //new T[new_size];
		allocated = new_size;

		// copy old data
		s32 end = used < new_size ? used : new_size;

		for (s32 i=0; i<end; ++i)
		{
			// data[i] = old_data[i];
			allocator.construct(&data[i], old_data[i]);
		}

		// destruct old data
		for (u32 j=0; j<used; ++j)
			allocator.destruct(&old_data[j]);

		if (allocated < used)
			used = allocated;

		allocator.deallocate(old_data); //delete [] old_data;
	}


	//! set a new allocation strategy
	/** if the maximum size of the array is unknown, you can define how big the
	allocation should happen.
	\param element: newStratgey to applay to this array. */
	void setAllocStrategy ( eAllocStrategy newStrategy = ALLOC_STRATEGY_DOUBLE )
	{
		strategy = newStrategy;
	}

	//! Adds an element at back of array.
	/** If the array is too small to add this new element it is made bigger.
	\param element: Element to add at the back of the array. */
	void push_back(const T& element)
	{
		if (used + 1 > allocated)
		{
			// this doesn't work if the element is in the same array. So
			// we'll copy the element first to be sure we'll get no data
			// corruption

			T e(element);
			//reallocate(used * 2 +1); // increase data block
			// TA: okt, 2008. it's only allowed to alloc one element, if
			// default constructor has to be called

			// increase data block
			u32 newAlloc;
			switch ( strategy )
			{
				case ALLOC_STRATEGY_DOUBLE:
					newAlloc = used + 1 + (allocated < 500 ?
							(allocated < 5 ? 5 : used) : used >> 2);
					break;
				default:
				case ALLOC_STRATEGY_SAFE:
					newAlloc = used + 1;
					break;
			}
			reallocate( newAlloc);
			// construct new element
			// Attention!. in missing default constructors for faster alloc methods
			allocator.construct(&data[used++], e); // data[used++] = e; // push_back
		}
		else
		{
			//data[used++] = element;
			// instead of using this here, we copy it the safe way:
			allocator.construct(&data[used++], element);
		}

		is_sorted = false;
	}


	//! Adds an element at the front of the array.
	/** If the array is to small to add this new element, the array is
	made bigger. Please note that this is slow, because the whole array
	needs to be copied for this.
	\param element Element to add at the back of the array. */
	void push_front(const T& element)
	{
		insert(element);
	}


	//! Insert item into array at specified position.
	/** Please use this only if you know what you are doing (possible
	performance loss). The preferred method of adding elements should be
	push_back().
	\param element: Element to be inserted
	\param index: Where position to insert the new element. */
	void insert(const T& element, u32 index=0)
	{
		_IRR_DEBUG_BREAK_IF(index>used) // access violation

		if (used + 1 > allocated)
			reallocate(used +1);

		for (u32 i=used; i>index; --i)
		{
			if (i<used)
				allocator.destruct(&data[i]);
			allocator.construct(&data[i], data[i-1]); // data[i] = data[i-1];
		}

		if (used > index)
			allocator.destruct(&data[index]);
		allocator.construct(&data[index], element); // data[index] = element;
		is_sorted = false;
		++used;
	}


	//! Clears the array and deletes all allocated memory.
	void clear()
	{
		for (u32 i=0; i<used; ++i)
			allocator.destruct(&data[i]);

		allocator.deallocate(data); // delete [] data;
		data = 0;
		used = 0;
		allocated = 0;
		is_sorted = true;
	}


	//! Sets pointer to new array, using this as new workspace.
	/** \param newPointer: Pointer to new array of elements.
	\param size: Size of the new array. */
	void set_pointer(T* newPointer, u32 size)
	{
		for (u32 i=0; i<used; ++i)
			allocator.destruct(&data[i]);

		allocator.deallocate(data); // delete [] data;
		data = newPointer;
		allocated = size;
		used = size;
		is_sorted = false;
	}


	//! Sets if the array should delete the memory it uses upon destruction.
	/** \param f If true, the array frees the allocated memory in its
	destructor, otherwise not. The default is true. */
	void set_free_when_destroyed(bool f)
	{
		free_when_destroyed = f;
	}


	//! Sets the size of the array and allocates new elements if necessary.
	/** Please note: This is only secure when using it with simple types,
	because no default constructor will be called for the added elements.
	\param usedNow Amount of elements now used. */
	void set_used(u32 usedNow)
	{
		if (allocated < usedNow)
			reallocate(usedNow);

		used = usedNow;
	}


	//! Assignment operator
	void operator=(const array<T>& other)
	{
		strategy = other.strategy;

		if (data)
		{
			for (u32 i=0; i<used; ++i)
				allocator.destruct(&data[i]);

			allocator.deallocate(data); // delete [] data;
		}

		//if (allocated < other.allocated)
		if (other.allocated == 0)
			data = 0;
		else
			data = allocator.allocate(other.allocated); // new T[other.allocated];

		used = other.used;
		free_when_destroyed = other.free_when_destroyed;
		is_sorted = other.is_sorted;
		allocated = other.allocated;

		for (u32 i=0; i<other.used; ++i)
			allocator.construct(&data[i], other.data[i]); // data[i] = other.data[i];
	}


	//! Equality operator
	bool operator == (const array<T>& other) const
	{
		if (used != other.used)
			return false;

		for (u32 i=0; i<other.used; ++i)
			if (data[i] != other[i])
				return false;
		return true;
	}

	//! Inequality operator
	bool operator != (const array<T>& other) const
	{
		return !(*this==other);
	}


	//! Direct access operator
	T& operator [](u32 index)
	{
		_IRR_DEBUG_BREAK_IF(index>=used) // access violation

		return data[index];
	}


	//! Direct const access operator
	const T& operator [](u32 index) const
	{
		_IRR_DEBUG_BREAK_IF(index>=used) // access violation

		return data[index];
	}


	//! Gets last element.
	T& getLast()
	{
		_IRR_DEBUG_BREAK_IF(!used) // access violation

		return data[used-1];
	}


	//! Gets last element
	const T& getLast() const
	{
		_IRR_DEBUG_BREAK_IF(!used) // access violation

		return data[used-1];
	}


	//! Gets a pointer to the array.
	/** \return Pointer to the array. */
	T* pointer()
	{
		return data;
	}


	//! Gets a const pointer to the array.
	/** \return Pointer to the array. */
	const T* const_pointer() const
	{
		return data;
	}


	//! Get size of array.
	/** \return Size of elements used in the array. */
	u32 size() const
	{
		return used;
	}


	//! Get amount of memory allocated.
	/** \return Amount of memory allocated. The amount of bytes
	allocated would be allocated_size() * sizeof(ElementsUsed); */
	u32 allocated_size() const
	{
		return allocated;
	}


	//! Check if array is empty.
	/** \return True if the array is empty false if not. */
	bool empty() const
	{
		return used == 0;
	}


	//! Sorts the array using heapsort.
	/** There is no additional memory waste and the algorithm performs
	O(n*log n) in worst case. */
	void sort()
	{
		if (is_sorted || used<2)
			return;

		heapsort(data, used);
		is_sorted = true;
	}


	//! Performs a binary search for an element, returns -1 if not found.
	/** The array will be sorted before the binary search if it is not
	already sorted.
	\param element Element to search for.
	\return Position of the searched element if it was found,
	otherwise -1 is returned. */
	s32 binary_search(const T& element)
	{
		sort();
		return binary_search(element, 0, used-1);
	}


	//! Performs a binary search for an element, returns -1 if not found.
	/** \param element: Element to search for.
	\param left First left index
	\param right Last right index.
	\return Position of the searched element if it was found, otherwise -1
	is returned. */
	s32 binary_search(const T& element, s32 left, s32 right) const
	{
		if (!used)
			return -1;

		s32 m;

		do
		{
			m = (left+right)>>1;

			if (element < data[m])
				right = m - 1;
			else
				left = m + 1;

		} while((element < data[m] || data[m] < element) && left<=right);
		// this last line equals to:
		// " while((element != array[m]) && left<=right);"
		// but we only want to use the '<' operator.
		// the same in next line, it is "(element == array[m])"


		if (!(element < data[m]) && !(data[m] < element))
			return m;

		return -1;
	}


	//! Performs a binary search for an element, returns -1 if not found.
	//! it is used for searching a multiset
	/** The array will be sorted before the binary search if it is not
	already sorted.
	\param element	Element to search for.
	\param &last	return lastIndex of equal elements
	\return Position of the first searched element if it was found,
	otherwise -1 is returned. */
	s32 binary_search_multi(const T& element, s32 &last)
	{
		sort();
		s32 index = binary_search(element, 0, used-1);
		if ( index < 0 )
			return index;

		// The search can be somewhere in the middle of the set
		// look linear previous and past the index
		last = index;

		while ( index > 0 && !(element < data[index - 1]) && !(data[index - 1] < element) )
		{
			index -= 1;
		}
		// look linear up
		while ( last < (s32) used - 1 && !(element < data[last + 1]) && !(data[last + 1] < element) )
		{
			last += 1;
		}

		return index;
	}

	//! Finds an element in linear time, which is very slow.
	/** Use binary_search for faster finding. Only works if ==operator is
	implemented.
	\param element Element to search for.
	\return Position of the searched element if it was found, otherwise -1
	is returned. */
	s32 linear_search(const T& element) const
	{
		for (u32 i=0; i<used; ++i)
			if (element == data[i])
				return (s32)i;

		return -1;
	}


	//! Finds an element in linear time, which is very slow.
	/** Use binary_search for faster finding. Only works if ==operator is
	implemented.
	\param element: Element to search for.
	\return Position of the searched element if it was found, otherwise -1
	is returned. */
	s32 linear_reverse_search(const T& element) const
	{
		for (s32 i=used-1; i>=0; --i)
			if (data[i] == element)
				return i;

		return -1;
	}


	//! Erases an element from the array.
	/** May be slow, because all elements following after the erased
	element have to be copied.
	\param index: Index of element to be erased. */
	void erase(u32 index)
	{
		_IRR_DEBUG_BREAK_IF(index>=used) // access violation

		for (u32 i=index+1; i<used; ++i)
		{
			allocator.destruct(&data[i-1]);
			allocator.construct(&data[i-1], data[i]); // data[i-1] = data[i];
		}

		allocator.destruct(&data[used-1]);

		--used;
	}


	//! Erases some elements from the array.
	/** May be slow, because all elements following after the erased
	element have to be copied.
	\param index: Index of the first element to be erased.
	\param count: Amount of elements to be erased. */
	void erase(u32 index, s32 count)
	{
		_IRR_DEBUG_BREAK_IF(index>=used || count<1 || index+count>used) // access violation

		u32 i;
		for (i=index; i<index+count; ++i)
			allocator.destruct(&data[i]);

		for (i=index+count; i<used; ++i)
		{
			if (i > index+count)
				allocator.destruct(&data[i-count]);

			allocator.construct(&data[i-count], data[i]); // data[i-count] = data[i];

			if (i >= used-count)
				allocator.destruct(&data[i]);
		}

		used-= count;
	}


	//! Sets if the array is sorted
	void set_sorted(bool _is_sorted)
	{
		is_sorted = _is_sorted;
	}

	private:

		T* data;
		TAlloc allocator;
		u32 allocated;
		u32 used;
		eAllocStrategy strategy;
		bool free_when_destroyed:1;
		bool is_sorted:1;
};


} // end namespace core
} // end namespace irr


#endif

