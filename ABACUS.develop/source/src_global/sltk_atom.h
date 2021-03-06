//==========================================================
// AUTHOR : sltk, ywcui, mohan
// START DATE : 2007-04-07
// LAST DATE : 2008-11-22
//==========================================================
#ifndef INCLUDE_FATOM
#define INCLUDE_FATOM

#include "sltk_util.h"
#include "sltk_adjacent_set.h"

class AdjacentSet;

// a class contains the atom position, 
// the type and the index,
class FAtom
{
private:
	double d_x;
	double d_y;
	double d_z;
	AdjacentSet* as;

	int type;
	int natom;

public:
//==========================================================
// Default Constructor and deconstructor
//==========================================================

	FAtom();
	~FAtom();
//2015-05-07
	void delete_vector(void);

//	static int count1;
//	static int count2;

//==========================================================
// MEMBER FUNCTION :
// NAME : setAdjacent
// Dangerous but high performance interface function!
// no exception test.
//
// NAME : getAdjacentSet
//
// NAME : setAdjacentSet
//==========================================================
	void setAdjacent(const int x_in, const int y_in, const int z_in, const int offset)
	{ this->as->set(x_in, y_in, z_in, offset); }

	AdjacentSet* getAdjacentSet() const
	{ return this->as; }

	void setAdjacentSet(AdjacentSet* const pointAdjacentSet)
	{ this->as = pointAdjacentSet; }

	void allocate_AdjacentSet(void)
	{ this->as = new AdjacentSet; allocate = true; }
	bool allocate;

//==========================================================
// MEMBER FUNCTION :
// EXPLAIN : get value
//==========================================================
	const double& x() const { return d_x; }
	const double& y() const { return d_y; }
	const double& z() const { return d_z; }
	const int& getType() const { return type;}
	const int& getNatom() const { return natom;}

//==========================================================
// MEMBER FUNCTION :
// EXPLAIN : set value
//==========================================================
	void setX(const double& r) { d_x = r; }
	void setY(const double& r) { d_y = r; }
	void setZ(const double& r) { d_z = r; }
	void setType(const int ntype) {type = ntype;}
	void setNatom(const int atom) {natom = atom;}
};

#endif // INCLUDE_FATOM
