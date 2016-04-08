#ifndef VERTEX_H_
#define VERTEX_H_

#include "mesh.h"
#include "point.h"
#include "matrix.h"
#include <vector>

template< typename T, int nDimensions >
class Mesh::Vertex
{
private:
	typedef Point<T, nDimensions> curPoint;
	typedef Vertex* Pvertex;
	typedef std::vector< Pvertex > curVector;
	typedef Matrix< T, 4, 4 > curMatrix;
	curMatrix Q;
	curPoint point;
	curVector adjacency;
	
public:
	static int total;
	double figurePoint()
	{
		double D, a, b, c, d, l, m, n, k, p, q, r, s, t, o, u, v, x, y, z;
		//	x0 = x	x1 = y	x2 = z	x3 = 1
		a = Q[0][0];b = Q[0][1];c = Q[0][2];d = Q[0][3];
		l = Q[1][0];m = Q[1][1];n = Q[1][2];k = Q[1][3];
		p = Q[2][0];q = Q[2][1];r = Q[2][2];s = Q[2][3];
		t = Q[3][0];o = Q[3][1];u = Q[3][2];v = Q[3][3];
		
		//	solve a 3 Variable Linear Equation
		//	ax+by+cz+d=0
		//	lx+my+nz+k=0
		//	px+qy+rz+s=0
		D = (a*m*r+b*p*n+c*l*q)-(a*n*q+b*l*r+c*m*p);
		// const double CD = 1000000;
		// D = D * CD;
		// x = ((b*r*k+c*m*s+d*n*q)-(b*n*s+c*q*k+d*m*r))/D*CD;
		// y = ((a*n*s+c*p*k+d*l*r)-(a*r*k+c*l*s+d*n*p))/D*CD;
		// z = ((a*q*k+b*l*s+d*m*p)-(a*m*s+b*p*k+d*l*q))/D*CD;
		
		x = ((b*r*k+c*m*s+d*n*q)-(b*n*s+c*q*k+d*m*r))/D;
		y = ((a*n*s+c*p*k+d*l*r)-(a*r*k+c*l*s+d*n*p))/D;
		z = ((a*q*k+b*l*s+d*m*p)-(a*m*s+b*p*k+d*l*q))/D;
		point[0] = x;
		point[1] = y;
		point[2] = z;
		return a * x * x + b * x * y + c * x * z + d * x +
				l * y * x + m * y * y + n * y * z + k * y +
				p * z * x + q * z * y + r * z * z + s * z +
				t * x + o * y + u * z + v;
	}
	
	const curPoint& getPoint() const
	{
		return point;
	}
	
	curPoint& holdPoint()
	{
		return point;
	}
	
	const curMatrix& getMat() const
	{
		return Q;
	}
	
	curMatrix& holdMat()
	{
		return Q;
	}
	
	const curVector& getAdjacency() const
	{
		return adjacency;
	}
	
	curVector& holdAdjacency()
	{
		return adjacency;
	}
	
	int size() const
	{
		return adjacency.size();
	}
	
	void push_back(const Pvertex& other)
	{
		adjacency.push_back(other);
	}
	
	Pvertex& operator[](const int i)
    {
        return adjacency[i];
    }

    const Pvertex& operator[](const int i) const
    {
        return adjacency[i];
    }
	
	bool operator< (const Vertex& other) const
	{
		return point < other.point;
	}
	
	bool operator== (const Vertex& other) const
	{
		return point == other.point;
	}
	#ifndef DEBUG
	Vertex():point(){}
	Vertex(T x, T y):point(x, y){}
	Vertex(T x, T y, T z):point(x, y, z){}
	~Vertex(){total--;}
	#endif
	#ifdef DEBUG
	Vertex():point(){total++;}
	Vertex(T x, T y):point(x, y){total++;}
	Vertex(T x, T y, T z):point(x, y, z){total++;}
	~Vertex(){total--;}
	#endif
};
template< typename T, int nDimensions >
int Mesh::Vertex<T, nDimensions>::total = 0;
#endif