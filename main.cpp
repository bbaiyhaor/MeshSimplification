#include "all.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <typeinfo>
using namespace std;

using Mesh::Vertex3Dd;
using Mesh::Edge3Dd;
using Mesh::Point3Dd;
using Mesh::Mat44d;

auto& dian = Mesh::dian;
auto& bian = Mesh::bian;
auto& face_edge = Mesh::face_edge;
auto& face_total = Mesh::face_total;

#define fec_(x, y) face_edge.count(make_pair(x, y))
#define fe_(x, y) face_edge[make_pair(x, y)]
#define gp_ getPoint()
#define hp_ holdPoint()

#define gl_ getLhsv()
#define hl_ holdLhsv()
#define rl_ reLhsv()

#define gr_ getRhsv()
#define hr_ holdRhsv()
#define rr_ reRhsv()

#define gm_ getMat()
#define hm_ holdMat()

#define ge_ getError()
#define he_ holdError()

ostream &operator<<(ostream &os, const Point3Dd &m) { 
    return os << m.x() << " " << m.y() << " " << m.z();
}

ostream &operator<<(ostream &os, const Mat44d &m) { 
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++)
			cout << m[i][j] << " ";
		cout << endl;
	}
}

void readin(char** argv)
{
	ifstream fin(argv[1]);
	Mesh::outfile = argv[2];
	double rate = atoi(argv[3]);
	
	while (! fin.eof()){
		char ch;
		fin >> ch;
		if (fin.eof())
			break;
		
		if (ch == '#'){	//	annotation
			fin >> noskipws;
			while (ch != '\n') fin >> ch;
			fin >> skipws;
		}
		else {		
			if (ch == 'v'){	//	vertex
				double x, y, z;
				fin >> x >> y >> z;
				// x = x * 10.0;
				// y = y * 10.0;
				// z = z * 10.0;
				dian.push_back(new Vertex3Dd(x, y, z));
			}
			else if (ch == 'f'){	//	face
				face_total++;
				int a, b, c;
				fin >> a >> b >> c;
				a--;
				b--;
				c--;
			
				//	a edge is shared by at most 2 triangles
				if (! Mesh::isExist(dian[a]->gp_, dian[b]->gp_)){//	e(a, b)
					dian[a]->push_back(dian[b]);	
					dian[b]->push_back(dian[a]);
					face_edge[make_pair(dian[a]->getPoint(), dian[b]->getPoint())] = new Edge3Dd(dian[a], dian[b]);
					bian.push_back(face_edge[make_pair(dian[a]->getPoint(), dian[b]->getPoint())]);
				}
								
				if (! Mesh::isExist(dian[c]->gp_, dian[b]->gp_)){	//	e(b, c)
					dian[b]->push_back(dian[c]);
					dian[c]->push_back(dian[b]);
					face_edge[make_pair(dian[b]->getPoint(), dian[c]->getPoint())] = new Edge3Dd(dian[b], dian[c]);
					bian.push_back(face_edge[make_pair(dian[b]->getPoint(), dian[c]->getPoint())]);
				}
				
				if (! Mesh::isExist(dian[a]->gp_, dian[c]->gp_)){	//	e(a, c)
					dian[a]->push_back(dian[c]);
					dian[c]->push_back(dian[a]);
					face_edge[make_pair(dian[a]->getPoint(), dian[c]->getPoint())] = new Edge3Dd(dian[a], dian[c]);
					bian.push_back(face_edge[make_pair(dian[a]->getPoint(), dian[c]->getPoint())]);
				}
			}
			else	//	error
			{
				cout << "There is an illegal char " << ch << " in " << argv[1] << endl;
				exit(-1);
			}
		}
	}
	fin.close();
	//Mesh::iterate_time = dian.size() * (1 - atof(argv[3]));
	Mesh::left_face = face_total * atof(argv[3]);
	//cout << "vertex number: " << dian.size() << " , edge number: " << bian.size() << " face number: " << face.size() 
		//<< " rate & iterate_time: " << Mesh::iterate_time << endl;
}

void process()
{
	// int size = dian.size();
	// for (int i = 0; i < size; i++){
		// int size1 = dian[i]->size();
		// for (int j = 0; j < size1; j++){
			// int size2 = (*dian[i])[j]->size();
			// int flag = 0;
			// for (int k = 0; k < size2; k++){
				// if ((*(*dian[i])[j])[k] == dian[i]){
					// flag = 1;
					// break;
				// }
			// }
			// if (! flag){
				// cout << "error at vertex " << i << ", " << j << endl;
			// }
		// }
	// }
	
	
	//	K = |a*a a*b a*c a*d|
	//		|a*b b*b b*c b*d|
	//		|a*c b*c c*c c*d|
	//		|a*d b*d c*d d*d|
	//	aX + bY +cZ + d = 0, a*a + b*b + c*c = 1
	//	Q = ∑ K
	
	//	for every vertex a
	for (Vertex3Dd*& elem : dian){
		Mat44d& Q = elem->holdMat();
		const Point3Dd& point = elem->getPoint();
		const vector< Vertex3Dd* >& adjacency = elem->getAdjacency();
		//	for its adjacency b
		for (Vertex3Dd* const& node : adjacency){
			const Point3Dd& point1 = node->getPoint();
			const vector< Vertex3Dd* >& adjacency1 = node->getAdjacency();
			//	for the adjacency of its adjacency c
			for (Vertex3Dd* const& node1 : adjacency1){
				const Point3Dd& point2 = node1->getPoint();
				if (Mesh::isExist(point2, point))
					Mesh::trianglePlane(Q, point, point1, point2);	
			}
		}
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				Q[i][j] /= 2;
	}
	// for (int i = 0; i < 10; i++)
		// cout << dian[i]->getMat() << endl;
}

int main(int argc, char** argv)
{
	if (argc != 4){
		cout << "Usage: main.exe filename.in filename.out rate" << endl;
		return -1;
	}
	readin(argv);
	process();
	Mesh::simplify();
	return 0;
}