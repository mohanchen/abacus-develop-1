//==========================================================
// AUTHOR : mohan
// DATE : 2021-02-01
//==========================================================
#include "run_lcao.h"
#include "src_pw/global.h"
#include "input.h"
#include "src_io/optical.h"
#include "src_io/cal_test.h"
#include "src_lcao/dftu.h"   //Quxin add for DFT+U on 20201029
#include "src_io/winput.h"
#include "src_global/sltk_atom_arrange.h"
#include "src_lcao/LOOP_cell.h"
#include "src_io/print_info.h"
#include "src_pw/symmetry.h"
#include "src_lcao/run_md.h"

Run_lcao::Run_lcao(){}
Run_lcao::~Run_lcao(){}


void Run_lcao::lcao_line(void)
{
    TITLE("Run_lcao","lcao_line");
	timer::tick("Run_lcao","lcao_line",'A');

    // Setup the unitcell.
    // improvement: a) separating the first reading of the atom_card and subsequent
    // cell relaxation. b) put NLOCAL and NBANDS as input parameters
    ucell.setup_cell( global_pseudo_dir , global_atom_card , ofs_running);

    //ucell.setup_cell( global_pseudo_dir , global_atom_card , ofs_running, NLOCAL, NBANDS);
    DONE(ofs_running, "SETUP UNITCELL");

    // symmetry analysis should be performed every time the cell is changed
    if (Symmetry::symm_flag)
    {
        symm.analy_sys();
        DONE(ofs_running, "SYMMETRY");
    }

    // Setup the k points according to symmetry.
    kv.set(symm, global_kpoint_card, NSPIN, ucell.G, ucell.latvec );
    DONE(ofs_running,"INIT K-POINTS");

    // print information
    // mohan add 2021-01-30
    Print_Info PI;
    PI.setup_parameters();

    // * reading the localized orbitals/projectors 
	// * construct the interpolation tables.
	hm.orb_con.set_orb_tables();

	// * allocate H and S matrices according to computational resources
	// * set the 'trace' between local H/S and global H/S
	LM.divide_HS_in_frag(GAMMA_ONLY_LOCAL, ParaO);

//--------------------------------------
// cell relaxation should begin here
//--------------------------------------

    // Initalize the plane wave basis set
    pw.gen_pw(ofs_running, ucell, kv);
    DONE(ofs_running,"INIT PLANEWAVE");
    cout << " UNIFORM GRID DIM     : " << pw.nx <<" * " << pw.ny <<" * "<< pw.nz << endl;
    cout << " UNIFORM GRID DIM(BIG): " << pw.nbx <<" * " << pw.nby <<" * "<< pw.nbz << endl;

    // the symmetry of a variety of systems.
    if(CALCULATION == "test")
    {
        Cal_Test::test_memory();
        QUIT();
    }

    // initialize the real-space uniform grid for FFT and parallel
    // distribution of plane waves
    Pgrid.init(pw.ncx, pw.ncy, pw.ncz, pw.nczp,
        pw.nrxx, pw.nbz, pw.bz); // mohan add 2010-07-22, update 2011-05-04


	// Inititlize the charge density.
    CHR.allocate(NSPIN, pw.nrxx, pw.ngmc);
    DONE(ofs_running,"INIT CHARGE");

	// Initializee the potential.
    pot.allocate(pw.nrxx);
    DONE(ofs_running,"INIT POTENTIAL");


	// Peize Lin add 2018-11-30
	if(CALCULATION=="nscf")
	{
		switch(exx_global.info.hybrid_type)
		{
			case Exx_Global::Hybrid_Type::HF:
			case Exx_Global::Hybrid_Type::PBE0:
			case Exx_Global::Hybrid_Type::HSE:
				exx_global.info.set_xcfunc(xcf);
				break;
		}
	}


	// Initialize the local wave functions.
	// npwx, eigenvalues, and weights
	wf.allocate_ekb_wg(kv.nks);

	// Initialize the FFT.
	UFFT.allocate();

	// Initialize the local part of
	// NC pseudopotentials
	ppcell.init_vloc();

	// Initialize the sum of all local potentials.
	// if ion_step==0, read in/initialize the potentials
	int ion_step=0;
	pot.init_pot(ion_step);


    if(CALCULATION=="md")
	{
		Run_MD run_md;
		run_md.opt_cell();
	}
	else
	{
		// Peize Lin 2016-12-03
		if (CALCULATION=="scf" || CALCULATION=="relax" || CALCULATION=="cell-relax")
		{
			switch(exx_global.info.hybrid_type)
			{
				case Exx_Global::Hybrid_Type::HF:
				case Exx_Global::Hybrid_Type::PBE0:
				case Exx_Global::Hybrid_Type::HSE:
					exx_lcao.init();
					break;
				case Exx_Global::Hybrid_Type::No:
				case Exx_Global::Hybrid_Type::Generate_Matrix:
					break;
				default:
					throw invalid_argument(TO_STRING(__FILE__)+TO_STRING(__LINE__));
			}
		}	

		// Quxin added for DFT+U
		if(INPUT.dft_plus_u) 
		{
			dftu.init();
		}

		LOOP_cell lc;
		lc.opt_cell();

		en.perform_dos();
	}


	timer::tick("Run_lcao","lcao_line",'A');
    return;
}
