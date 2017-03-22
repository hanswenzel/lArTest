#include <TF1.h>
Double_t ArScintillationSpectrum( const Double_t* k, Double_t *par)
{
  Double_t waveL; 
  Double_t kk=k[0];
  waveL =exp(-0.5*((kk-128.0)/(2.929))*((kk-128.0)/(2.929))); 
  return waveL;
}

// Calculates the dielectric constant of LAr from the Bideau-Sellmeier formula.
// See : A. Bideau-Mehu et al., "Measurement of refractive indices of Ne, Ar,
// Kr and Xe ...", J. Quant. Spectrosc. Radiat. Transfer, Vol. 25 (1981), 395
Double_t LArEpsilon(const Double_t lambda)
{
  Double_t epsilon;
  Double_t lowLambda=110.0; // lowLambda in  nanometer
  if (lambda < lowLambda) return 1.0e4; // lambda MUST be > 110.0 nm
  epsilon = lambda / 1000; // switch to micrometers
  epsilon = 1.0 / (epsilon * epsilon); // 1 / (lambda)^2
  epsilon = 1.2055e-2 * ( 0.2075/ (91.012 - epsilon) +
			  0.0415/ (87.892 - epsilon) +
			  4.3330 / (214.02 - epsilon) );
  epsilon *= (8./12.); // Bideau-Sellmeier -> Clausius-Mossotti
  Double_t GArRho= 0.001784; // density in g/cm3
  Double_t LArRho= 1.3954;    // density in g/cm3
  epsilon *= (LArRho / GArRho); // density correction (Ar gas -> LAr liquid)
  if (epsilon < 0.0 || epsilon > 0.999999) return 4.0e6 ;
  epsilon = (1.0 + 2.0* epsilon) / (1.0- epsilon); // solve Clausius-Mossotti
  return epsilon;
}

// Calculates the refractive index of LAr
Double_t LArRefIndex(const Double_t *lam, Double_t *par)
{
  Double_t lambda=lam[0];
  return ( sqrt(LArEpsilon(lambda)) ); // square root of dielectric constant
}

void lArRoot()
{
  TF1 *f1 = new TF1("myfunc",LArRefIndex,110.,1000.,0);
  // f1->Draw();
  TF1 *f2 = new TF1("myfunc2",ArScintillationSpectrum,100.,150.,0);
   f2->Draw();
}
