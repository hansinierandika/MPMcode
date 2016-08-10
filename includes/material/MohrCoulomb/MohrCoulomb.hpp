/***************************************************************************
                          Material Point Method
                           Shyamini Kularathna
                         Unversity Of Cambridge
File: MohrCoulomb.hpp
****************************************************************************/

#ifndef MPM_MATERIAL_MOHRCOULOMB_H
#define MPM_MATERIAL_MOHRCOULOMB_H

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
        class MohrCoulomb;
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class mpm::material::MohrCoulomb : public mpm::material::MaterialBase {

protected:
    static const unsigned dim = mpm::constants::DIM;
    static const unsigned numNodes = mpm::constants::NUMNODES;
    static const unsigned dof = 3 * (dim - 1);

    typedef Eigen::Matrix<double, 1, dof> VectorDDOF;
    typedef Eigen::Matrix<double, 1, 6>   VectorD6x1;

public:
    // CONSTRUCTOR
    MohrCoulomb();

    // CREATE THE MATERIAL
    static MaterialBase* create() {
        return new MohrCoulomb();
    }

    // COMPUTE STRESS
    void computeStress(const VectorDDOF& strain, VectorD6x1& stress, double& pressure, const double& dVolStrain) { };



    // GIVE DENSITY
    double giveDensity() {
        return density_;
    }


protected:
    double density_;
};

#include "MohrCoulomb.ipp"

#endif
