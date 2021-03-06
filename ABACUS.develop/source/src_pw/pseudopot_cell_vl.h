//==========================================================
// AUTHOR : Lixin He,mohan
// DATE : 2008-11-6
//==========================================================
#ifndef pseudopot_cell_vl_H
#define pseudopot_cell_vl_H

#include "tools.h"

class pseudopot_cell_vl
{
public:

	pseudopot_cell_vl();
	~pseudopot_cell_vl();

	void init_vloc(void);

	matrix vloc;   //(ntype,ngl),the local potential for each atom type(ntype,ngl)
	bool *numeric; //[ntype], =true

private:

	double *zp;   // (npsx),the charge of the pseudopotential

	void allocate(void);

	// generate vloc for a particular atom type.
	void vloc_of_g( const int &msh, const double *rab, const double *r, const double *vloc_at,
	               const double &zp, double *vloc) const;

	void print_vloc(void) const;

};

#endif // PSEUDOPOT_CELL_VL_H
