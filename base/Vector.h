#pragma once

#include <cassert>
#include <vector>
#include <algorithm>
#include <functional>

#include "base/Ref.h"


template <class T>
class Vector
{

public:

	typedef typename std::vector<T>::iterator iterator;
	typedef typename std::vector<T>::const_iterator const_iterator;

	typedef typename std::vector<T>::reverse_iterator reverse_iterator;
	typedef typename std::vector<T>::const_reverse_iterator const_reverse_iterator;

	iterator begin() { return _data.begin(); }
	const_iterator begin() const { return _data.begin(); }

	iterator end() { return _data.end(); }
	const_iterator end() const { return  _data.end(); }

	const_iterator cbegin() const { return _data.cbegin(); }
	const_iterator cend() const { return _data.cend(); }

	reverse_iterator rbegin() { return _data.rbegin(); }
	const_reverse_iterator rbegin() const { return _data.rbegin(); }

	reverse_iterator rend() { return _data.rend(); }
	const_reverse_iterator rend() const { return _data.rend(); }

	const_reverse_iterator crbegin() const { return _data.crbegin(); }
	const_reverse_iterator crend() const { return _data.crend(); }

	Vector<T>() : _data()
	{
		static_assert(std::is_convertible<T, Ref*>::value, "Invalid Type for Vector<T>!");
	}

	explicit Vector<T>(size_t capacity) : _data()
	{
		static_assert(std::is_convertible<T, Ref*>::value, "Invalid Type for Vector<T>!");
		reserve(capacity);
	}

	~Vector<T>()
	{
		clear();
	}

	Vector<T>(const Vector<T>& other)
	{
		static_assert(std::is_convertible<T, Ref*>::value, "Invalid Type for Vector<T>!");
		_data = other._data;
		addRefForAllObjects();
	}

	Vector<T>(Vector<T>&& other)
	{
		static_assert(std::is_convertible<T, Ref*>::value, "Invalid Type for Vector<T>!");
		_data = std::move(other._data);
	}

	Vector<T>& operator=(const Vector<T>& other)
	{
		if (this != &other)
		{
			clear();
			_data = other._data;
			addRefForAllObjects();
		}
		return *this;
	}

	Vector<T>& operator=(Vector<T>&& other)
	{
		if (this != &other)
		{
			clear();
			_data = std::move(other._data);
		}
		return *this;
	}

	void reserve(size_t n)
	{
		_data.reserve(n);
	}

	size_t capacity() const
	{
		return _data._capacity();
	}

	size_t size() const
	{
		return _data.size();
	}

	bool empty() const
	{
		return _data.empty();
	}
	
	size_t maxSize() const
	{
		return _data.max_size();
	}

	size_t getIndex(T object) const
	{
		auto iter = std::find(std::begin(_data), std::end(_data), object);
		if (iter != _data.end())
			return iter - _data.begin();
		return -1;
	}

	const_iterator find(T object) const
	{
		return std::find(std::begin(_data), std::end(_data), object);
	}

	iterator find(T object)
	{
		return std::find(_data.begin(), _data.end(), object);
	}

	T at(size_t index) const
	{
		assert(index >= 0 && index < size(), "index out of range");
		return _data[index];
	}

	T front() const
	{
		return _data.front();
	}

	T back() const
	{
		return _data.back();
	}

	bool contains(T object) const
	{
		return (std::find(std::begin(_data), std::end(_data), object) != _data.end());
	}

	bool equals(const Vector<T>& other)
	{
		size_t s = this->size();
		if ( s != other.size())
			return false;

		for (auto i = 0; i < s; ++i)
		{
			if (this->at(i) != other.at(i))
				return false;
		}
		return true;
	}

	void pushBack(T object)
	{
		assert(object, "The object should not be nullptr");
		_data.push_back(object);
		object->retain();
	}

	void pushBack(const Vector<T>& other)
	{
		for (const auto& obj : other)
		{
			_data.push_back(obj);
			obj->retain();
		}
	}

	void insert(size_t index, T object)
	{
		assert(index >= 0 && index < size(), "invalid index");
		assert(object, "The object should not be nullptr");
		_data.insert(std::begin(_data) + index, object);
		object->retain();
	}

	void popBack()
	{
		assert(!_data.empty(), "no objects added!");
		auto last = _data.back();
		_data.pop_back();
		last->release();
	}

	void eraseObject(T object, bool removeAll = false)
	{
		assert(object, "The object should not be nullptr");

		if (removeAll)
		{
			for (auto iter = _data.begin(); iter != _data.end(); )
			{
				if ((*iter) == object)
				{
					iter = _data.erase(iter);
					object->release();
				}
				else
				{
					++iter;
				}
			}
		}
		else
		{
			auto iter = std::find(std::begin(_data), std::end(_data), object);
			if (iter != _data.end())
			{
				_data.erase(iter);
				object->release();
			}
		}
	}

	iterator erase(iterator position)
	{
		assert(position >= _data.begin() && position < _data.end(), "invalid position!");
		(*position)->release();
		return _data.erase(position);
	}

	iterator erase(iterator first, iterator last)
	{
		for (auto iter = first; iter != last; ++iter)
		{
			(*iter)->release();
		}
		return _data.erase(first, last);
	}

	iterator erase(size_t index)
	{
		assert(!_data.empty() && index >= 0 && index < size(), "invalid index!");
		auto it = std::next(begin(), index);
		(*it)->release();
		return _data.erase(it);
	}

	void clear()
	{
		for (auto it = std::begin(_data); it != std::end(_data); ++it)
			(*it)->release();
		_data.clear();
	}

	void swap(T object1, T object2)
	{
		auto idx1 = getIndex(object1);
		auto idx2 = getIndex(object2);
		
		assert(idx1 >= 0 && idx2 >= 0, "invalid object index");
		std::swap(_data[idx1], _data[idx2]);
	}

	void swap(size_t index1, size_t index2)
	{
		assert(index1 > 0 && index2 >= 0 " invalid object index");
		std::swap(_data[index1], _data[index2]);
	}

	void replace(size_t index, T object)
	{
		assert(index >= 0 && index < size(), "invalid index!");
		assert(object, "the object should not be nullptr");

		_data[index]->release();
		_data[index] = object;
		object->retain();
	}

	void reverse()
	{
		std::reverse(std::begin(_data), std::end(_data));
	}

	void shrinkToFit()
	{
		_data.shrink_to_fit();
	}

protected:

	void addRefForAllObjects()
	{
		for (const auto& obj : _data)
			obj->retain();
	}

	std::vector<T> _data;

};