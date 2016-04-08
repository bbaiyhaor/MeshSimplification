#ifndef MATRIX_H_
#define MATRIX_H_

#include "mesh.h"
#include "vec.h"
#include <array>            // std::array

#define STATIC_ASSERT( e ) static_assert( e, "!(" #e ")" )

template< typename T, int r, int c >
class Mesh::Matrix
{
private:
    std::array< Vec<T, c>, r > elements_;
	int rows, cols;
public:
	const int row() const
	{
		return rows;
	}
	
	const int col() const
	{
		return cols;
	}
	
    Vec<T, c>& operator[](const int i)
    {
        return elements_[i];
    }

    const Vec<T, c>& operator[](const int i) const
    {
        return elements_[i];
    }
	
	void operator+=(const Matrix& other);
	void operator-=(const Matrix& other);
	
    friend Matrix operator+(const Matrix& lhs, const Matrix& rhs)
    {
        Matrix ret(lhs);

        ret += rhs;
        return ret;
    }

    friend Matrix operator-(const Matrix& lhs, const Matrix& rhs)
    {
        Matrix ret(lhs);

        ret -= rhs;
        return ret;
    }

    Matrix(): elements_({}), rows(r), cols(c) {}  

};

template< typename T, int r, int c >
void Mesh::Matrix< T, r, c >::operator+=(const Matrix& other)
{
	for(int i = 0; i < rows; ++i){
		elements_[i] += other.elements_[i];
	}
}

template< typename T, int r, int c >
void Mesh::Matrix< T, r, c >::operator-=(const Matrix& other)
{
	for( int i = 0; i < rows; ++i ){
		elements_[i] -= other.elements_[i];
	}
}
	

#endif //POINT_H_

