/***************************************************************************
                          Material Point Method
                           Shyamini Kularathna
                         Unversity Of Cambridge
File: Newtonian.hpp
****************************************************************************/

#ifndef MPM_MATERIAL_NEWTONIAN_H
#define MPM_MATERIAL_NEWTONIAN_H

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// c++ header files
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

// mpm header files
#include "MaterialBase.hpp"
#include "PropertyParse.hpp"
#include "Constants.hpp" 

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

namespace mpm {
    namespace material {
        class Newtonian;
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class mpm::material::Newtonian : public mpm::material::MaterialBase {

protected:
    static const unsigned dim = mpm::constants::DIM;
    static const unsigned numNodes = mpm::constants::NUMNODES;
    static const unsigned dof = 3 * (dim - 1);

    typedef Eigen::Matrix<double, 1, dof> VectorDDOF;
    typedef Eigen::Matrix<double, 1, 6>   VectorD6x1;

public:

    // CONSTRUCTOR
    Newtonian();

    // CREATE THE MATERIAL
    static MaterialBase* create() {
        return new Newtonian();
    }

    // COMPUTE STRESS
    void computeStress(const VectorDDOF& strain, VectorD6x1& stress, double& pressure, const double& dVolStrain);



    // GIVE DENSITY
    double giveDensity() {
        return density_;
    }

protected:
    double density_;
    double viscosity_;
    double bulkModulus_;
    double dt_;
};

#include "Newtonian.ipp"

#endif
