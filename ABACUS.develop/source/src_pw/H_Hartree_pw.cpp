#include "H_Hartree_pw.h"

double H_Hartree_pw::hartree_energy=0.0;

//--------------------------------------------------------------------
// Transform charge density to hartree potential.
//--------------------------------------------------------------------
void H_Hartree_pw::v_hartree(
	const UnitCell &cell, 
	PW_Basis &pwb, 
	const Use_FFT &ufft,
	const int &nspin, 
	matrix &v, 
	double** rho)
{
    TITLE("H_Hartree_pw","v_hartree");
    timer::tick("H_Hartree_pw","v_hartree");

    complex<double> *Porter = ufft.porter;

    //  Hartree potential VH(r) from n(r)
    ZEROS( Porter, pwb.nrxx );
    int nspin0 = 1;
    if(nspin==2) nspin0 = nspin;
    for(int is=0; is<nspin0; is++)
    {
        for (int ir=0; ir<pwb.nrxx; ir++) 
        {
            Porter[ir] += complex<double>( rho[is][ir], 0.0 );
        }
    }

    //=============================
    //  bring rho (aux) to G space
    //=============================
    pwb.FFT_chg.FFT3D(Porter, -1);

    //double charge;
    //if (pwb.gstart == 1)
    //{
    //    charge = cell.omega * Porter[pwb.ig2fftc[0]].real();
    //}
    //OUT(ofs_running, "v_h charge", charge);

    //=======================================================
    // calculate hartree potential in G-space (NB: V(G=0)=0 )
    //=======================================================

	double ehart = 0.0;

    complex<double> *vh_g  = new complex<double>[pwb.ngmc];
    ZEROS(vh_g, pwb.ngmc);

    for (int ig = pwb.gstart; ig<pwb.ngmc; ig++)
    {
        const int j = pwb.ig2fftc[ig];
        if(pwb.gg[ig] >= 1.0e-12) //LiuXh 20180410
        {
            const double fac = e2 * FOUR_PI / (cell.tpiba2 * pwb.gg [ig]);

            ehart += ( conj( Porter[j] ) * Porter[j] ).real() * fac;
            vh_g[ig] = fac * Porter[j];
        }
    }

    Parallel_Reduce::reduce_double_pool( ehart );
    ehart *= 0.5 * cell.omega;

    //cout << " ehart=" << ehart << endl;

    H_Hartree_pw::hartree_energy = ehart;



    ZEROS(Porter, pwb.nrxx);

    for (int ig = 0;ig < pwb.ngmc;ig++)
    {
        Porter[pwb.ig2fftc[ig]] = vh_g[ig];
    }

    //==========================================
    //transform hartree potential to real space
    //==========================================
    pwb.FFT_chg.FFT3D(Porter, 1);
    //==========================================
    //Add hartree potential to the xc potential
    //==========================================

	if(nspin==4)
	{
		for (int ir = 0;ir < pwb.nrxx;ir++)
		{
			v(0, ir) += Porter[ir].real();
		}
	}
	else
	{
		for (int is = 0;is < nspin;is++)
		{
			for (int ir = 0;ir < pwb.nrxx;ir++)
			{
				v(is, ir) += Porter[ir].real();
			}
		}
	}


//-----------------------------------------------------------
// we need to add this out_potential funciton back 
// in near future, 2021-02-25
//-----------------------------------------------------------

	//-------------------------------------------
	// output the Hartree potential into a file.
	//-------------------------------------------
/*
	if(out_potential==-2)
	{
		cout << " output VH" << endl;
		int is = 0;
		int iter = 0;
		int precision = 3;
		string fn = "VH.dat";
		stringstream ss;
		ss << global_out_dir << fn;
		matrix v;
		v.create(1,pwb.nrxx);
		for(int ir=0; ir<pwb.nrxx; ++ir)
		{
			v(0,ir) = Porter[ir].real();
		}
		this->write_potential( is, iter, ss.str(), v, precision, 1 );
	}
*/

    timer::tick("H_Hartree_pw","v_hartree");

    delete[] vh_g;

    return;
} // end subroutine v_h
