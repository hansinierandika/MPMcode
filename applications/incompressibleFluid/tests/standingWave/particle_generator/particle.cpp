#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <vector>

const double pi= 3.141592654;

int main()
{

    std::cout << "\n \t \t \t 2-D PARTICLE GENERATOR FOR \n";
    std::cout << "\t \t \t COLLAPSE OF WATER COLUMN TEST \n \n";

  double xmin=0.0, xmax=0.0, xintv=.0;
  double ymin=0.0, ymax=0.0, yintv=.0;
  double zmin=0.0, zmax=0.0, zintv=1;
  unsigned matId = 0;
  double A_ = 0.;

  std::cout << "Enter x-min, x-max, x-spacing: ";
  std::cin >> xmin >> xmax >> xintv;
  std::cout << "Enter y-min, y-max- y-spacing: ";
  std::cin >> ymin >> ymax >> yintv;
  std::cout << "Enter material ID: ";
  std::cin >> matId;
  std::cout << "Enter amplitude of the wave: ";
  std::cin >> A_;

  xmin = xmin + (xintv /2.0);
  xmax = xmax - (xintv/2.0) + 0.00000001;
  ymin = ymin + (yintv/2.);
  ymax = ymax - (yintv/2.0) + 0.00000001;

  int xnum = (xmax - xmin) / xintv + 1;
  int ynum = (ymax - ymin) / yintv + 1;
  int znum = (zmax - zmin) / zintv + 1;
  
  int num = xnum * ynum* znum ;

  int n=0;

  std::ofstream fSoil, fStress;
  fSoil.open("particles.dat");
  fStress.open("initStress.dat");
  // double x[num],y[num],z[num];

  std::vector<double> xcoord, ycoord;
  double xinist[num],yinist[num],zinist[num];

  //For stresses 
  double phi=(pi*30.)/180.;
  double pd=1000;
  //double k0=1.0-sin(phi);
  double k0 = 1.0;
  double pymax=0.0;
  double yHeightMax = 0.;

  xcoord.clear();
  ycoord.clear();
  unsigned totalNum = 0;
  for (unsigned i = 0; i < xnum; i++) {
      double x_ = xmin + (xintv * i);
      double height = (ymax - A_) +(A_ * cos (pi* x_ / xmax));
      for (unsigned j = 0; j < ynum; j++) {
          double y_ = ymin + (yintv * j);
          if (y_ < height) {
              xcoord.push_back(x_);
              ycoord.push_back(y_);
              totalNum++;
          } 
      }
  }

  fSoil << totalNum << "\t" << matId << "\n";
  fSoil << xintv << "\t" << yintv << "\n";
  for (unsigned i = 0; i < totalNum; i++) {
      fSoil << xcoord.at(i) << "\t" << ycoord.at(i) << "\n"; 
  }
  fStress << totalNum << "\n";

  for (int l = 0; l < totalNum; l++) {
      double coordX = xcoord.at(l);
      double coordY = ycoord.at(l);
      double yHeight = (ymax - A_) +(A_ * cos (pi* coordX / xmax));
      double xstress = -pd * (yHeight - coordY) * 9.81;
      double ystress = -pd * (yHeight - coordY) * 9.81;
      double zstress = -pd * (yHeight - coordY) * 9.81;
      fStress << l << "\t" << xstress << "\t" << ystress << "\t" << zstress << "\t" << 0. << "\t" << 0. << "\t" << 0. <<"\n";    

    // fStress.precision(6);
    // fStress.setf(std::ios::fixed, std::ios::floatfield);
    // fStress << l << "\t" << -xinist[l] << "\t" << -yinist[l] << "\t" << 0. << std::endl;

  }
  fSoil.close();
  fStress.close();
  return 0;
}
