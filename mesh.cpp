#include "all.h"
#include <vector>
#include <map>
#include <queue>
#include <utility>
#include <iostream>
using namespace std;
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

#define gc_ getCdd()
#define hc_ holdCdd()
#define rc_ reCdd()

#define gm_ getMat()
#define hm_ holdMat()

#define ge_ getError()
#define he_ holdError()


//#define DEBUG
//#define RAW
//#define INFO
namespace Mesh
{
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
	
	class EdgeCompare
	{
	public:
		bool operator()(Edge3Dd * const &lhs, Edge3Dd * const &rhs)
		{
			return lhs->getError() > rhs->getError();
		}
	};
	using std::vector;
	using std::map;
	using std::pair;
	//alias template
	template<typename T>
	using dui = std::priority_queue<T, vector<T>, EdgeCompare >;
	
	
	
	const char* outfile;
	vector<Vertex3Dd*> dian;
	vector<Edge3Dd*> bian;
	map< pair<Point3Dd, Point3Dd>, Edge3Dd* > face_edge;
	dui<Edge3Dd*> graph;
	int iterate_time;
	
	
	bool isExist(const Point3Dd& lhs, const Point3Dd& rhs)
	{
		if (fec_(lhs, rhs) > 0 || fec_(rhs, lhs) > 0)
			return true;
		return false;
	}
	
	//	K = |a*a a*b a*c a*d|
	//		|a*b b*b b*c b*d|
	//		|a*c b*c c*c c*d|
	//		|a*d b*d c*d d*d|
	//	aX + bY +cZ + d = 0, a*a + b*b + c*c = 1
	//	Q = ∑ K
	void trianglePlane(Mat44d& Q, const Point3Dd& A, const Point3Dd& B, const Point3Dd& C, int cel)
	{
		Point3Dd alpha = B - A;
		Point3Dd beta = C - A;
		double a = alpha.y() * beta.z() - alpha.z() * beta.y();
		//double b = -(alpha.x() * beta.z() - alpha.z() * beta.x());
		double b = alpha.z() * beta.x() - alpha.x() * beta.z();
		double c = alpha.x() * beta.y() - alpha.y() * beta.x();
		double d = -(A.x() * (alpha.y() * beta.z() - alpha.z() * beta.y()) 
			- A.y() * (alpha.x() * beta.z() - alpha.z() * beta.x()) 
			+ A.z() * (alpha.x() * beta.y() - alpha.y() * beta.x()));
		double length = sqrt(a * a + b * b + c * c);
		a /= length;
		b /= length;
		c /= length;
		d /= length;
		Q[0][0] += (cel * a * a); Q[0][1] += (cel * a * b); Q[0][2] += (cel * a * c); Q[0][3] += (cel * a * d);
		Q[1][0] += (cel * a * b); Q[1][1] += (cel * b * b); Q[1][2] += (cel * b * c); Q[1][3] += (cel * b * d);
		Q[2][0] += (cel * a * c); Q[2][1] += (cel * b * c); Q[2][2] += (cel * c * c); Q[2][3] += (cel * c * d);
		Q[3][0] += (cel * a * d); Q[3][1] += (cel * b * d); Q[3][2] += (cel * c * d); Q[3][3] += (cel * d * d);
	
	}
	
	void seekCdd(Edge3Dd* cur)
	{
		cur->newCdd();
		Vertex3Dd& candidate = cur->holdCdd();
		candidate.holdMat() = cur->getLhsv().getMat() + cur->getRhsv().getMat();
		cur->holdError() = candidate.figurePoint();					
	}

	void build_priority()
	{
		int bian_size = bian.size();
		for (int i = 0; i < bian_size; i++){
			seekCdd(bian[i]);
			graph.push(bian[i]);
			#ifdef RAW
			cout << bian[i]->getLhsv().getPoint() << "     " << bian[i]->getRhsv().getPoint() << endl;
			cout << bian[i]->getError() << " " << bian[i]->gc_.gp_ << endl << endl;
			#endif
		}
		dian.clear();
		bian.clear();
	}
	
	Edge3Dd* newEdge3Dd(Vertex3Dd* lhs, Vertex3Dd* rhs)
	{
		Edge3Dd* re = new Edge3Dd(lhs, rhs);
		seekCdd(re);
		return re;
	}
	
	void addEdge(Edge3Dd* cur)
	{
		Vertex3Dd *lhs = cur->reLhsv();
		Vertex3Dd *rhs = cur->reRhsv();
		Vertex3Dd *cdd = cur->reCdd();
		const Point3Dd& cddp = cdd->gp_;
		
		//	for l
		#ifdef DEBUG
		int flag = 0;
		#endif
		vector< Vertex3Dd* >& adjacency = lhs->holdAdjacency();			
		for (Vertex3Dd*& elem : adjacency){
			if (elem == rhs){
				#ifdef DEBUG
				flag = 1;
				#endif
				continue;
			}
			const Point3Dd& point = elem->getPoint();
			if (! isExist(cddp, point)){
				fe_(cddp, point) = newEdge3Dd(elem, cdd);
				graph.push(fe_(cddp, point));
				elem->push_back(cdd);
				cdd->push_back(elem);
			}
		}

		#ifdef DEBUG
		if (! flag){
			cout << "nidiebi	L" << endl;
			exit(-1);
		}
		flag = 0;
		#endif
		
		
		//	for r
		vector< Vertex3Dd* >& adjacency1 = rhs->holdAdjacency();			
		for (Vertex3Dd*& elem : adjacency1){
			if (elem == lhs){
				#ifdef DEBUG
				flag = 1;
				#endif
				continue;
			}
			const Point3Dd& point = elem->getPoint();
			if (! isExist(cddp, point)){
				fe_(cddp, point) = newEdge3Dd(elem, cdd);
				graph.push(fe_(cddp, point));
				elem->push_back(cdd);
				cdd->push_back(elem);
			}
		}
		#ifdef DEBUG
		if (! flag){
			cout << "nidiebi	R" << endl;
			exit(-1);
		}
		flag = 0;
		#endif		
	}
	
	void remove(Vertex3Dd* node, Vertex3Dd* one)
	{
		vector<Vertex3Dd*>& a = node->holdAdjacency();
		for (int i = 0, size = a.size(); i < size; i++)
			if (a[i] == one){
				swap(a[i], a.back());
				a.pop_back();
				return;
			}
		cout << "bug" << endl;
	}
	
	void delEdge(Edge3Dd* cur)
	{
		Vertex3Dd *lhs = cur->reLhsv();
		const Point3Dd& lhsp = lhs->getPoint();
		Vertex3Dd *rhs = cur->reRhsv();
		const Point3Dd& rhsp = rhs->getPoint();
		
		//	for l
		#ifdef DEBUG
		int flag = 0;
		#endif
		vector< Vertex3Dd* >& adjacency = lhs->holdAdjacency();			
		for (Vertex3Dd*& elem : adjacency){
			if (elem == rhs){
				#ifdef DEBUG
				flag = 1;
				#endif
				continue;
			}
			remove(elem, lhs);
			const Point3Dd& point = elem->getPoint();
			if (fec_(lhsp, point) > 0){
				fe_(lhsp, point)->delCdd();
				fe_(lhsp, point)->exist = false;
				face_edge.erase(make_pair(lhsp, point));
			}
			else if (fec_(point, lhsp) > 0){
				fe_(point, lhsp)->delCdd();
				fe_(point, lhsp)->exist = false;
				face_edge.erase(make_pair(point, lhsp));	
			}
			else
				cout << "error del L" << endl;
		}

		#ifdef DEBUG
		if (! flag){
			cout << "nimabi	L" << endl;
			exit(-1);
		}
		flag = 0;
		#endif
		//	for r
		vector< Vertex3Dd* >& adjacency1 = rhs->holdAdjacency();			
		for (Vertex3Dd*& elem : adjacency1){
			if (elem == lhs){
				#ifdef DEBUG
				flag = 1;
				#endif
				continue;
			}
			remove(elem, rhs);
			const Point3Dd& point = elem->getPoint();
			if (fec_(rhsp, point) > 0){
				fe_(rhsp, point)->delCdd();
				fe_(rhsp, point)->exist = false;
				face_edge.erase(make_pair(rhsp, point));
			}
			else if (fec_(point, rhsp) > 0){
				fe_(point, rhsp)->delCdd();
				fe_(point, rhsp)->exist = false;
				face_edge.erase(make_pair(point, rhsp));	
			}
			else
				cout << "error del R" << endl;
		}
		#ifdef DEBUG
		if (! flag){
			cout << "nimabi	R" << endl;
			exit(-1);
		}
		flag = 0;
		#endif
		
		if (fec_(lhsp, rhsp) > 0){
			fe_(lhsp, rhsp)->exist = false;
			face_edge.erase(make_pair(lhsp, rhsp));
		}
		else if (fec_(rhsp, lhsp) > 0){
			fe_(rhsp, lhsp)->exist = false;
			face_edge.erase(make_pair(rhsp, lhsp));	
		}
		else
			cout << "wocao" << endl;
		cur->delLhsv();
		cur->delRhsv();
		delete cur;
	}
	
	void iterate_update()
	{
		#ifdef INFO
		cout << "total vertex: " << Vertex3Dd::total << " begin edge: " << face_edge.size() << endl;
		#endif
		int stop_flag = 1;
		while (stop_flag){
			Edge3Dd* first = graph.top();
			graph.pop();
			if (first->exist){
					stop_flag = 0;
					addEdge(first);
					delEdge(first);
			}
			else 
			{
				delete first;
			}			
		}
	}
	
	void print()
	{
		FILE* of = fopen(outfile, "w");
		map<Vertex3Dd*, int> order;
		int number = 1;
		for (auto& elem : face_edge){
			//if (elem.second->exist == false)
				//continue;
			if (! order.count(elem.second->rl_)){
				order[elem.second->rl_] = number++;
				dian.push_back(elem.second->rl_);
				fprintf(of, "v %.12f %.12f %.12f\n", elem.second->gl_.gp_.x(), elem.second->gl_.gp_.y(), elem.second->gl_.gp_.z());
			}
				
			if (! order.count(elem.second->rr_)){
				order[elem.second->rr_] = number++;
				dian.push_back(elem.second->rr_);
				fprintf(of, "v %.12f %.12f %.12f\n", elem.second->gr_.gp_.x(), elem.second->gr_.gp_.y(), elem.second->gr_.gp_.z());
			}
				
		}
		for (auto& elem : dian){
			const Point3Dd& point = elem->getPoint();
			const vector< Vertex3Dd* >& adjacency = elem->getAdjacency();
			for (Vertex3Dd* const& node : adjacency){
				const vector< Vertex3Dd* >& adjacency1 = node->getAdjacency();
				for (Vertex3Dd* const& node1 : adjacency1){
					const Point3Dd& point2 = node1->getPoint();
					if (isExist(point2, point))
						fprintf(of, "f %d %d %d\n", order[elem], order[node], order[node1]);
				}	
			}
		}
	}
	
	void simplify()
	{
		build_priority();
		while (iterate_time--)
			iterate_update();
		print();
	}
}