#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>

const double pi= 3.141592654;

int main()
{
  double xmin=0.0005, xmax=0.099501, xintv=.001;
  double ymin=0.0005, ymax=0.099501, yintv=.001;
  double zmin=0.0, zmax=0.0, zintv=1;

  int xnum = (xmax - xmin) / xintv + 1;
  int ynum = (ymax - ymin) / yintv + 1;
  int znum = (zmax - zmin) / zintv + 1;
  
  int num = xnum * ynum* znum ;
  std::cout << "\n Total number of points = " << num <<std::endl;

  int n=0;

  std::ofstream fSoil, fStress;
  fSoil.open("particles.dat");
  fStress.open("initStress.dat");
  double x[num],y[num],z[num];

  double xinist[num],yinist[num],zinist[num];
  //For stresses 
  double phi=(pi*30.)/180.;
  double pd=1000;
  //double k0=1.0-sin(phi);
  double k0 = 1.0;
  double pymax=0.0;
  double yHeightMax = 0.;

  for (int k=0; k < znum; k++) {
    for (int j=0; j < ynum; j++) {
      for (int i=0; i < xnum; i++) {
	//3Dimension slump	 
	x[n] = xmin + xintv * i;
	y[n] = ymin + yintv * j;
	z[n] = zmin + zintv * k;
		  
	if (y[n] > yHeightMax) yHeightMax = y[n];
	       
	std::cout <<n<<"\t"<< x[n] << "\t" << y[n]<< "\t" << z[n] <<"\t" << std::endl;    
	n++;
      }
    }
  }

  fSoil << n << "\t"<< xintv << "\t" << yintv << std::endl;
  fStress << 0 << std::endl;

  for (int l = 0; l < n; l++) {
    // fSoil.precision(6);
    // fSoil.setf(std::ios::fixed, std::ios::floatfield);
    fSoil << x[l] << "\t" << y[l] << std::endl;
    
    xinist[l]=pd*k0*(yHeightMax - y[l])*9.81;
    yinist[l]=pd*(yHeightMax - y[l])*9.81;
    zinist[l]=pd*k0*(yHeightMax - y[l])*9.81;

    // fStress.precision(6);
    // fStress.setf(std::ios::fixed, std::ios::floatfield);
//    fStress << l << "\t" << -xinist[l] << "\t" << -yinist[l] << "\t" << -zinist[l] << "\t" << 0. << "\t" << 0.  << "\t" << 0. << std::endl;

  }
  fSoil.close();
  fStress.close();
  return 0;
}
