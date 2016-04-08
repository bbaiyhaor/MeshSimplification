#ifndef MESH_H_
#define MESH_H_

#include <vector>
#include <map>
#include <utility>
namespace Mesh
{
	template< typename T, int nDimensions = 2 >
	class Point;
	template< typename T, int r = 4, int c = 4 >
	class Matrix;
	template< typename T, int nDimensions = 4 >
	class Vec;
	template< typename T, int nDimensions = 3>
	class Edge;
	template< typename T, int nDimensions = 3 >
	class Vertex;
	
	// typedef Point< int, 2 > Point2D;
	// typedef Point< int, 3 > Point3D;
	// typedef Point< float, 2> Point2Df;
	// typedef Point< float, 3> Point3Df;
	// typedef Point< double, 2> Point2Dd;
	typedef Point< double, 3> Point3Dd;

	// typedef Vec< int, 3 > Vec3D;
	// typedef Vec< int, 4 > Vec4D;
	// typedef Vec< float, 3> Vec3Df;
	// typedef Vec< float, 4> Vec4Df;
	// typedef Vec< double, 3> Vec3Dd;
	// typedef Vec< double, 4> Vec4Dd;
	
	// typedef Vertex<int> Vertex3D;
	// typedef Vertex<float> Vertex3Df;
	typedef Vertex<double> Vertex3Dd;
	
	// typedef Matrix< int, 4, 4 > Mat44;
	// typedef Matrix< float, 4, 4 > Mat44f;
	typedef Matrix< double, 4, 4 > Mat44d;
	
	// typedef Edge< int, 3> Edge3D;
	// typedef Edge< float, 3> Edge3Df;
	typedef Edge< double, 3> Edge3Dd;
	
	extern const char* outfile;
	extern int iterate_time;
	extern std::vector<Vertex3Dd*> dian;
	extern std::vector<Edge3Dd*> bian;
	extern std::map< std::pair<Point3Dd, Point3Dd>, Edge3Dd* > face_edge;
	void simplify();
	void trianglePlane(Mat44d& Q, const Point3Dd& A, const Point3Dd& B, const Point3Dd& C, int cel = 1);
	bool isExist(const Point3Dd& lhs, const Point3Dd& rhs);
}
#endif