//   
//   Copyright (c) 2007 David Moore, James Mooney, All Rights Reserved.
//   
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the GNU General Public License as
//   published by the Free Software Foundation; either version 2 of
//   the License, or (at your option) any later version.
//   
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//   GNU General Public License for more details.
//   
//   You should have received a copy of the GNU General Public License
//   along with this program; if not, write to the Free Software
//   Foundation, Inc., 59 Temple Place, Suite 330, Boston,
//   MA 02111-1307 USA
//   
//-------------------------------------------------------------------
// Copyright (c) David Moore 30/02/2005 All Rights Reserved.
//-------------------------------------------------------------------
// modified from original version in 2003

#ifndef __ARRAY2__
#define __ARRAY2__

// 2d array template class
template < class T >
class Array
{
protected:
	T* array;
	int size;
	T nullReturn;
public:
	Array()
	{
		array = 0;
		size = 0;
	};

	Array(int _size)
	{
		array = 0;
		if(_size < 0)
			return;
		array = new T[_size];
		size = _size;

	};

	Array(Array<T> &a)
	{
		array = new T[a.size];
		size = a.size;
		for(int n = 0; n < size; n++) {
			array[n] = a.array[n];
		}
	};

	~Array()
	{
		Destroy();
	};

	void Destroy()
	{
		if(array) {
			delete [] array;
			array = 0;
			size = 0;
		}
	};
	void Create(int _size)
	{
		if(_size <= 0)
			return;
		Destroy();
		array = new T[_size];
		size = _size;
	};
	void Create(int _size, T init)
	{
		if(_size <= 0)
			return;

		Destroy();
		array = new T[_size];
		size = _size;
		for(int n = 0; n < size; n++) {
			array[n] = init;
		}
	};
	T& operator [](int index)
	{
		if(size > 0 && index >= 0 && index < size) {
			return array[index];
		}
		return nullReturn;
	}
	operator T*()
	{
		return array;
	};

};

template < class T >
class Array2
{
protected:
	int size;
	int sizeY;
	int sizeX;
	T* array;
	T nullReturn;
public:

	int Size()
	{
		return size;
	};
	int SizeX()
	{
		return sizeX;
	};
	int SizeY()
	{
		return sizeY;
	};

	Array2()
	{
		array = 0;
		size = 0;
		sizeY = 0;
		sizeX = 0;

	};

	Array2(int x, int y)
	{
		array = 0;
		Create(x,y);
	};

	// Copy constructor
	Array2(Array2<T> &a)
	{
		array = 0;
		Create(a.sizeX,a.sizeY);
		for(int n = 0; n < a.size; n++) {
			array[n] = a.array[n];
		}
	};

	void Create(int x, int y)
	{
		if( x > 0 && y > 0 ) {
			Destroy();

			sizeY = y;
			sizeX = x;
			size = x * y;
			array = new T[size];
		}
	};

	void Create(int x, int y, T init)
	{
		if( x > 0 && y > 0 ) {
			Destroy();

			sizeY = y;
			sizeX = x;
			size = x * y;
			array = new T[size];
			for(int n = 0; n < size; n++) {
				array[n] = init;
			}
		}
	};

	~Array2()
	{
		Destroy();
	};
	void Destroy()
	{
		if(array) {
			delete [] array;
			array = 0;
			size = 0;
		}
	};
	T& Index(int x, int y) // bounds check and returns a null
	{

		if(size > 0 && x >= 0 && x < sizeX && y >= 0 && y < sizeY) // bounds check
		{
			return *(array + ((x * sizeY) + y));
		}

		return nullReturn;
	};

	T& IndexNoBounds(int x, int y) // no bounds checking // for speed
	{
		return *(array + ((x * sizeY) + y));
	};

	T* operator[](int x) // returns a pointer to a row
	{
		if(x >= 0 && x < sizeX)
		{
			return array + (x * sizeY);
		}
		return 0;
	};

};


#endif
