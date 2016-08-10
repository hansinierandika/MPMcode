
#ifndef MPM_MATERIAL_MATERIALBASE_H
#define MPM_MATERIAL_MATERIALBASE_H

#include <iostream>
#include <vector>
#include <map>

#include "PropertyParse.hpp"
#include "Constants.hpp"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

namespace mpm {
    namespace material {
        class MaterialBase;
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class mpm::material::MaterialBase {

protected:
    static const unsigned dim = mpm::constants::DIM;
    static const unsigned numNodes = mpm::constants::NUMNODES;
    static const unsigned dof = 3 * (dim - 1);

    typedef Eigen::Matrix<double, 1, dof> VectorDDOF;
    typedef Eigen::Matrix<double, 1, 6>   VectorD6x1;

public:
    virtual double giveDensity() = 0;
    virtual void computeStress(const VectorDDOF& strain, VectorD6x1& stress, double& pressure, const double& dVolStrain) = 0;

    void setProperty(std::string propName, double& propValue) {
        propValue = mpm::misc::propertyList[propName];
    }

protected:
    double dt = mpm::misc::dt;

};

#endif
