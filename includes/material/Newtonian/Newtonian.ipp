mpm::material::Newtonian::Newtonian() {
    setProperty("density", density_);
    setProperty("viscosity", viscosity_);
    setProperty("bulkModulus", bulkModulus_);
    dt_ = mpm::misc::dt;
}


void mpm::material::Newtonian::computeStress(const VectorDDOF& strain, VectorD6x1& stress, double& pressure, const double& dVolStrain) {

    double dPressure = -(bulkModulus_ * dVolStrain);
    pressure += dPressure;

    if (dim == 2) {
        stress(0) = -pressure + (2 * viscosity_ * strain(0)) - (2 * viscosity_ * (strain(0) + strain(1)) / 3);
        stress(1) = -pressure + (2 * viscosity_ * strain(1)) - (2 * viscosity_ * (strain(0) + strain(1)) / 3);
        stress(2) = - pressure - (2 * viscosity_ * (strain(0) + strain(1)) / 3);
        stress(3) = viscosity_ * strain(2);
        stress(4) = 0.0;
        stress(5) = 0.0;
    } 

}
