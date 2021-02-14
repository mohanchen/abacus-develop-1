#ifndef USE_HAMILT_MATRIX_H
#define USE_HAMILT_MATRIX_H

#include "../src_pw/tools.h"
#include "LCAO_gen_fixedH.h"
#include "gint_gamma.h"
#include "gint_k.h"
#include "grid_integral.h"

class Use_Hamilt_Matrix
{
	public:
	Use_Hamilt_Matrix();
	~Use_Hamilt_Matrix();

    void set_ion(void);
		
	// used fro k-dependent Hamiltonian matrix.
	void calculate_Hk( const int &ik);
	
	// used for Gamma only Hamiltonian matrix.
	void calculate_Hgamma( const int &ik );						// Peize Lin add ik 2016-12-03

    void calculate_STN_R(void); //LiuXh add 2019-07-15

	// used for gamma only algorithms.
	Gint_Gamma GG;

	// used for k-dependent grid integration.
	Gint_k GK;

	// use overlap matrix to generate fixed Hamiltonian
	LCAO_gen_fixedH genH;

	// init S (overlap matrix) flag.
    bool init_s;

	private:

	// used for gamma only algorithms.
	void calculate_STNR_gamma(void);

	void calculate_STNR_gamma_B(void); //mohan add 2012-04-14

	void calculate_STNR_k(void);

};

#endif