#ifndef POINT_H_
#define POINT_H_

#include "mesh.h"
#include <array>            // std::array

#define STATIC_ASSERT( e ) static_assert( e, "!(" #e ")" )

template< typename T, int nDimensions >
class Mesh::Point
{
private:
    std::array< T, nDimensions > elements_;

public:
    T& operator[](const int i)
    {
        return elements_[i];
    }

    const T& operator[](const int i) const
    {
        return elements_[i];
    }

	const T& x() const
	{
		return elements_[0];
	}
	
	const T& y() const
	{
		STATIC_ASSERT(nDimensions > 1);
		return elements_[1];
	}
	
	const T& z() const
	{
		STATIC_ASSERT(nDimensions > 2);
		return elements_[2];
	}
	
	bool operator<(const Point& other)const;
	bool operator==(const Point& other)const;
	void operator+=(const Point& other);
	void operator-=(const Point& other);
    friend Point operator+(const Point& lhs, const Point& rhs)
    {
        Point ret(lhs);

        ret += rhs;
        return ret;
    }

    friend Point operator-(const Point& lhs, const Point& rhs)
    {
        Point ret( lhs );

        ret -= rhs;
        return ret;
    }

    Point(): elements_({}) {}

    Point(T x, T y)
    {
        STATIC_ASSERT( nDimensions == 2 );
        elements_[0] = x;
        elements_[1] = y;
    }

    Point(T x, T y, T z)
    {
	    STATIC_ASSERT( nDimensions == 3 );
        elements_[0] = x;
        elements_[1] = y;
        elements_[2] = z;
	}
};

template< typename T, int nDimensions >
bool Mesh::Point< T, nDimensions >::operator<(const Point& other) const
{
	for (int i = 0; i < nDimensions; i++){
		if (elements_[i] != other.elements_[i])
			return elements_[i] < other.elements_[i];
	}
}

template< typename T, int nDimensions >
void Mesh::Point< T, nDimensions >::operator+=(const Point& other)
{
	for(int i = 0; i < nDimensions; ++i){
		elements_[i] += other.elements_[i];
	}
}

template< typename T, int nDimensions >
void Mesh::Point< T, nDimensions >::operator-=(const Point& other)
{
	for( int i = 0; i < nDimensions; ++i ){
		elements_[i] -= other.elements_[i];
	}
}

template< typename T, int nDimensions >
bool Mesh::Point< T, nDimensions >::operator==(const Point& other)const
{
	for (int i = 0; i < nDimensions; i++){
		//if (elements_[i] - other.elements_[i] > 1e-6 ||
		//elements_[i] - other.elements_[i] < -1e-6)
		if (elements_[i] != other.elements_[i])
			return false;
	}
	return true;
}

#endif //POINT_H_

