mpm::material::ILE::ILE() {
    setProperty("density", density_);
    setProperty("youngModulus", E_);
    setProperty("poissonRatio", mu_);
    this->computeElasticStiffnessMatrix();
}


void mpm::material::ILE::computeElasticStiffnessMatrix() {

 // Bulk and shear modulus
    double K, G;
    double a1, a2;

    K = E_ / (3.0 * (1. - 2. * mu_));
    G = E_ / (2.0 * (1. + mu_));

    a1 = K + (4.0 / 3.0) * G;
    a2 = K - (2.0 / 3.0) * G;

    // compute elasticityTensor
    De(0,0)=a1;
    De(0,1)=a2;
    De(0,2)=a2;
    De(0,3)=0;
    De(0,4)=0;
    De(0,5)=0;
    De(1,0)=a2;
    De(1,1)=a1;
    De(1,2)=a2;
    De(1,3)=0;
    De(1,4)=0;
    De(1,5)=0;
    De(2,0)=a2;
    De(2,1)=a2;
    De(2,2)=a1;
    De(2,3)=0;
    De(2,4)=0;
    De(2,5)=0;
    De(3,0)= 0;
    De(3,1)= 0;
    De(3,2)= 0;
    De(3,3)=0;
    De(3,4)=0;
    De(3,5)=0;
    De(4,0)= 0;
    De(4,1)= 0;
    De(4,2)= 0;
    De(4,3)=0;
    De(4,4)=0;
    De(4,5)=0;
    De(5,0)= 0;
    De(5,1)= 0;
    De(5,2)= 0;
    De(5,3)=0;
    De(5,4)=0;
    De(5,5)=0;
}


void mpm::material::ILE::computeStress(const VectorDDOF& strain, VectorD6x1& stress, double& pressure, const double& dVolStrain) {

    VectorD6x1 dStrain = VectorD6x1::Zero();
    VectorD6x1 dStress = VectorD6x1::Zero();
    if (dim == 2) {
        dStrain(0) = strain(0);
        dStrain(1) = strain(1);
        dStrain(3) = strain(2);
        dStress = De * dStrain.transpose();
    }
    //else if (dim == 3) {
    //   dStress = De * strain.transpose();
    // }
    stress += dStress;
}
