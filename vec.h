#ifndef VEC_H_
#define VEC_H_

#include "mesh.h"
#include <array>            // std::array

#define STATIC_ASSERT( e ) static_assert( e, "!(" #e ")" )

template< typename T, int nDimensions >
class Mesh::Vec
{
private:
    std::array< T, nDimensions > elements_;
	int sizen;
public:
	const int size() const
	{
		return sizen;
	}

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
	
	const T& w() const
	{
		STATIC_ASSERT(nDimensions > 3);
		return elements_[3];
	}
	
    void operator+=(const Vec& other);

    void operator-=(const Vec& other);
	
    friend Vec operator+(const Vec& lhs, const Vec& rhs)
    {
        Vec ret(lhs);

        ret += rhs;
        return ret;
    }

    friend Vec operator-(const Vec& lhs, const Vec& rhs)
    {
        Vec ret(lhs);

        ret -= rhs;
        return ret;
    }

    Vec(): elements_({}), sizen(nDimensions) {}  

    Vec(T x, T y, T z): sizen(nDimensions)
    {
        STATIC_ASSERT(nDimensions == 3);
        elements_[0] = x;
        elements_[1] = y;
        elements_[2] = z;
    }
	
	Vec(T x, T y, T z, T w): sizen(nDimensions)
    {
        STATIC_ASSERT(nDimensions == 4);
        elements_[0] = x;
        elements_[1] = y;
        elements_[2] = z;
		elements_[3] = w;
    }
};

template< typename T, int nDimensions >
void Mesh::Vec< T, nDimensions >::operator+=(const Vec& other)
{
	for(int i = 0; i < nDimensions; ++i){
		elements_[i] += other.elements_[i];
	}
}
	
template< typename T, int nDimensions >
void Mesh::Vec< T, nDimensions >::operator-=(const Vec& other)
{
	for( int i = 0; i < nDimensions; ++i ){
		elements_[i] -= other.elements_[i];
	}
}

#endif //POINT_H_

