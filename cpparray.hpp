#include <stddef.h>
#ifndef ARRAY
#define ARRAY

///
///Array is a dynamic array container
///Any out of range indexing causes undefined behaviour
///Poping nonexistent member causes undefined behaviour
///
template <typename T>
class Array
{
private:
	T * pointer;
	size_t capacity;
	size_t numberOfElements;

	void copyFrom(const Array<T> & other)
	{
		pointer = (T*)operator new (sizeof(T)*(other.numberOfElements));
		for (size_t i = 0; i< other.numberOfElements; ++i)
		{
			push_back(other[i]);
		}
	}

	void appendFrom(const Array<T> & other)
	{
		for (size_t i = 0; i< other.numberOfElements; i++)
		{
			push_back(other[i]);
		}
	}

	void reallocate(const size_t & length)
	{
		T* tmp = pointer;
		size_t tmpLen = numberOfElements;
		pointer = (T*)operator new (sizeof(T)*length); 
													 
		for (size_t i = 0; i < numberOfElements; ++i)
		{
			new (pointer + i) T(tmp[i]);
		}
		for (size_t i = numberOfElements; i < length; ++i)
		{
			new (pointer + i) T();
		}
		capacity = length;
		dealoc(tmp, tmpLen);
	}

	void dealoc(T* & ptr, const size_t & len)
	{
		for (size_t i = 0; i<len; ++i)
		{
			ptr[i].~T();
		}
		operator delete(ptr);
	}

public:

	Array()
	{
		pointer = nullptr;
		capacity = 0;
		numberOfElements = 0;
	}

	Array(const Array<T> &other)
	{
		capacity = other.numberOfElements;
		numberOfElements = 0;
		copyFrom(other);
	}
	
	Array(Array<T> && other) 
	{
		pointer = other.pointer;
		capacity = other.capacity;
		numberOfElements = other.numberOfElements;
		other.pointer = nullptr;
		other.capacity = 0;
		other.numberOfElements = 0;

	}

	Array(const size_t & numberElem) : capacity(numberElem), numberOfElements(numberElem)
	{
		pointer = (T*)operator new (sizeof(T)*numberElem);
	}

	~Array()
	{
		dealoc(pointer, numberOfElements);
	}

	///
	///Copy assignment
	///
	Array<T>& operator = (const Array<T> &other)
	{
		Array<T> tmp(other);
		swap(tmp);
		return *this;
	}

	///
	///Move assignment
	///
	Array<T>& operator = (Array<T> && other)
	{
		swap(other);
		return *this;
	}

	///
	///Returns preallocated size of Array
	///
	size_t allocSize() const
	{
		return capacity;
	}

	///
	///Returns the size of Array
	///
	size_t size() const
	{
		return numberOfElements;
	}

	///
	///Checks if Array is empty
	///Returns true if empty. Else false
	///
	bool empty() const
	{
		return !numberOfElements;
	}

	///
	///Adds an element to the end of the Array
	///Argument val is the value of the element added
	///
	void push_back(const T & val)
	{
		if (numberOfElements >= capacity)
		{
			reallocate(numberOfElements + 1);
		}

		new (pointer + numberOfElements) T(val);
		++numberOfElements;
	}

	///
	///Deletes the last member of Array
	///Poping non-existent member causes undefined behaviour
	///
	void pop_back()
	{
		pointer[numberOfElements - 1].~T();
		--numberOfElements;
	}

	///
	///Returns the value of first member in Array
	///
	T& front()
	{
		return pointer[0];
	}

	///
	///Returns the value of first member in Array
	///
	const T& front() const
	{
		return pointer[0];
	}

	///
	///Return the value of last member in Array
	///
	T& back()
	{
		return pointer[numberOfElements - 1];
	}

	///
	///Return the value of last member in Array
	///
	const T& back() const
	{
		return pointer[numberOfElements - 1];
	}

	///
	///Swaps the contents of Arrays - this, other
	///
	void swap(Array<T> & other)
	{
		T* tmp = pointer;
		pointer = other.pointer;
		other.pointer = tmp;

		size_t tmpE = other.numberOfElements;
		other.numberOfElements = numberOfElements;
		numberOfElements = tmpE;

		tmpE = other.capacity;
		other.capacity = capacity;
		capacity = tmpE;
	}

	///
	///Overloads operator [] so certain members can be reached using []
	///and their value changed or written out
	///
	T & operator[] (const size_t & position)
	{
		return pointer[position];
	}

	///
	///Overloads operator [] so certain members can be reached using []
	///
	const T & operator[] (const size_t & position) const
	{
		return pointer[position];
	}


	///
	///Resize Array to an Array of size of argument length
	///If length > capacity then allocates new Array with #memb = length and capacity= length
	///If size=#memb < length < capacity then initializes members from index size to length, #memb = length
	///If length < size then deconstructs members from index length to size
	///
	void resize(const size_t & length)
	{
		if (length > capacity)
		{
			reallocate(length);
			numberOfElements = length;
		}
		else if (length > numberOfElements)
		{
			for (size_t i = numberOfElements; i < length; ++i)
			{
				new (pointer + i) T();
			}
			numberOfElements = length;
		}
		else if (length < numberOfElements)
		{
			for (size_t i = length; i < numberOfElements; ++i)
			{
				pointer[i].~T();
			}
			numberOfElements = length;
		}
	}

	///
	///Prealocates memory for content
	///Argument length is the number of members that can be added using prealoc memory
	///
	void reserve(const size_t & length)
	{
		size_t tmpLen = numberOfElements;
		T* tmp = pointer;

		pointer = (T*)operator new (sizeof(T)*(length + capacity));
		for (size_t i = 0; i < numberOfElements; ++i)
		{
			new (pointer + i) T(tmp[i]);
		}
		capacity = length;
		dealoc(tmp, tmpLen);
	}

	///
	///Appends contents from Array other in argument to content of this
	///If capacity of instance this is greater than the sum of numbers of elements of both Arrays
	///then no reallocation happens and content from Array other is inserted.
	///else Array is reaalocated to the size of the sum of lengths of both arrays
	///and both contents are inserted.
	///
	void append(const Array<T> & other)
	{
		size_t tmpSize = other.numberOfElements + numberOfElements;
		if (capacity > tmpSize)
		{

		}
		else
			reallocate(tmpSize);

		appendFrom(other);
	}

	///
	///Clears the Array of all members
	///
	void clear()
	{
		for (size_t i = 0; i < numberOfElements; ++i)
		{
			pointer[i].~T();
		}
		numberOfElements = 0;
	}

	class iterator
	{
	public:
		T * curr;
		iterator(T* pointer)
		{
			curr = pointer;
		}

		iterator & operator++()
		{
			++curr;
			return *this;
		}

		T & operator * ()
		{
			return *curr;
		}

		bool operator==(const iterator &other) const
		{
			return curr == other.curr;
		}

		bool operator!=(const iterator &other) const
		{
			return curr != other.curr;
		}

	};

	iterator begin()
	{
		return iterator(pointer);
	}

	iterator end()
	{
		return iterator(&(pointer[numberOfElements]));
	}

	class const_iterator
	{
	public:
		T * curr;
		const_iterator(T* pointer)
		{
			curr = pointer;
		}

		const_iterator & operator++()
		{
			++curr;
			return *this;
		}

		const T & operator * () const
		{
			return *curr;
		}

		bool operator==(const const_iterator &other) const
		{
			return curr == other.curr;
		}

		bool operator!=(const const_iterator &other) const
		{
			return curr != other.curr;
		}

	};

	const_iterator begin() const
	{
		return const_iterator(pointer);
	}

	const_iterator end() const
	{
		return const_iterator(&(pointer[numberOfElements]));
	}

};

#endif

