/***************************************************************************
                          Material Point Method
                           Shyamini Kularathna
                         Unversity Of Cambridge
File: ILE.hpp
****************************************************************************/

#ifndef MPM_MATERIAL_ILE_H
#define MPM_MATERIAL_ILE_H

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
        class ILE;
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class mpm::material::ILE : public mpm::material::MaterialBase {

protected:
    static const unsigned dim = mpm::constants::DIM;
    static const unsigned numNodes = mpm::constants::NUMNODES;
    static const unsigned dof = 3 * (dim - 1);

    typedef Eigen::Matrix<double, 1, dof> VectorDDOF;
    typedef Eigen::Matrix<double, 1, 6>   VectorD6x1;
    typedef Eigen::Matrix<double, 6, 6>   MatrixD6x6;

public:
    // CONSTRUCTOR
    ILE();

    // CREATE THE MATERIAL
    static MaterialBase* create() {
        return new ILE();
    }

    void computeElasticStiffnessMatrix();

    // COMPUTE STRESS
    void computeStress(const VectorDDOF& strain, VectorD6x1& stress, double& pressure, const double& dVolStrain);

    // GIVE DENSITY
    double giveDensity() {
        return density_;
    }


protected:
    double density_;
    double E_;
    double mu_;
    MatrixD6x6 De;
};

#include "ILE.ipp"

#endif
