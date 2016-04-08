#ifndef EDGE_H_
#define EDGE_H_

#include "mesh.h"
#include "vertex.h"
#include <algorithm>
#include <assert.h>

template< typename T, int nDimensions>
class Mesh::Edge
{
private:
	typedef Vertex<T, nDimensions> curVertex;
	curVertex *lhsv, *rhsv, *cdd;
	double err;
public:
	bool exist;
	curVertex* reLhsv()
	{
		return lhsv;
	}
	
	const curVertex& getLhsv() const
	{
		assert( lhsv != 0 );
		return *lhsv;
	}
	
	curVertex& holdLhsv()
	{
		assert( lhsv != 0 );
		return *lhsv;
	}
	
	void delLhsv()
	{
		if (lhsv != 0){
			delete lhsv;
			lhsv = 0;
		}
	}
	
	void newLhsv()
	{
		delLhsv();
		lhsv = new curVertex();
	}
	
	curVertex* reRhsv()
	{
		return rhsv;
	}
	
	const curVertex& getRhsv() const
	{
		assert( rhsv != 0 );
		return *rhsv;
	}
	
	curVertex& holdRhsv()
	{
		assert( rhsv != 0 );
		return *rhsv;
	}
	
	void delRhsv()
	{
		if (rhsv != 0){
			delete rhsv;
			rhsv = 0;
		}
	}
	
	void newRhsv()
	{
		delRhsv();
		rhsv = new curVertex();
	}
	
	curVertex* reCdd()
	{
		return cdd;
	}
	
	const curVertex& getCdd() const
	{
		assert( cdd != 0 );
		return *cdd;
	}
	
	curVertex& holdCdd()
	{	
		assert( cdd != 0 );
		return *cdd;
	}
	
	void delCdd()
	{	
		if (cdd != 0){
			delete cdd;
			cdd = 0;
		}
	}
	
	void newCdd()
	{
		delCdd();
		cdd = new curVertex();
	}
	
	const double getError() const
	{
		return err;
	}
	
	double& holdError()
	{
		return err;
	}
	
	Edge(curVertex *lhsv, curVertex *rhsv, curVertex *cdd = 0)
	:lhsv(lhsv), rhsv(rhsv), cdd(cdd), err(0), exist(true)
	{	
		swap();
	}
	
private:
	void swap()
	{
		if ((lhsv != 0) && (rhsv != 0) && (! ((*lhsv) < (*rhsv))))
			std::swap(lhsv, rhsv);
	}
};

#endif