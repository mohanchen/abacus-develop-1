#ifndef LOOP_IONS_H
#define LOOP_IONS_H

#include "LOOP_elec.h"
#include "../src_ions/ions_move_methods.h"
#include "../src_pw/charge_extra.h"
#include "../src_pw/md.h"
#include "../src_pw/mdNVT.h"
#include "../src_pw/mdNVE.h"
#include "../src_ions/lattice_change_methods.h"

class LOOP_ions
{

	public:

	LOOP_ions();
	~LOOP_ions();

	LOOP_elec LOE;

	void opt_ions(void);
	void output_HS_R(void); //LiuXh add 2019-07-15

	//2014-06-06, xiaohui
	mdnvt MDNVT ;
	mdNVE MDNVE ;

	private:

	Ions_Move_Methods IMM;

	//bool force_stress(void);
	Lattice_Change_Methods LCM;

	bool force_stress(const int &istep, int &force_step, int &stress_step);

	int istep;

	// electron charge density extropolation method
	Charge_Extra CE;

	//choose md ensemble, zheng daye
	int mdtype;

	void final_scf(void);

};

#endif
