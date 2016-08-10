/*****************************************************************************
                        Material Point Method
                         Shyamini Kularathna
                       University of Cambridge
FILE: MpmParticle.hpp
*****************************************************************************/
#ifndef MPM_MPMPARTICLE_H
#define MPM_MPMPARTICLE_H

// c++ header files
#include <vector>
#include <fstream>
#include <iostream>

// eigen header files
#include <Eigen/Dense>

// mpm header files
#include "Constants.hpp"
#include "Particle.hpp"
#include "MaterialBase.hpp"

namespace mpm {
    class MpmParticle;
}

class mpm::MpmParticle {

protected:
    static const unsigned dim = mpm::constants::DIM;
    static const unsigned numNodes = mpm::constants::NUMNODES;
    static const unsigned dof = 3*(dim-1);

    typedef mpm::Particle* ParticlePtr;
    typedef mpm::material::MaterialBase* MaterialBasePtr;

    typedef std::vector<MaterialBasePtr> VecOfMaterialBasePtr;
    typedef Eigen::Matrix<double, 1, dim> VectorDDIM;
    typedef Eigen::Matrix<double, 1, dof> VectorDDOF; 
    typedef Eigen::Matrix<double, 1, 6>   VectorD6x1;

public:
    // constructor
    MpmParticle();

    // Destructor
    //! Free all dynamically allocated memory within this class
    ~MpmParticle();

    // read particles
    void read_particles(std::ifstream& pFile, std::ifstream& sFile);

    // Read traction at particles
    //! param[in] tractionFile input file for traction force at particles
    void read_traction(std::ifstream& tractionFile);

    // assign material to particle
    void assign_material_to_particles(VecOfMaterialBasePtr& materialPtrs);

    // Iterate over particles
    template<typename FP>
    void iterate_over_particles(FP function);

    // give total number of particles
    unsigned number_of_particles() {
        return particles_.size();
    }

    // give pointer to the particle of given id
    ParticlePtr pointer_to_particle(const unsigned& id) {
        return particles_.at(id);
    }

    // give coordinates of the particle of given id
    VectorDDIM particle_coordinates(const unsigned& id) {
        VectorDDIM pCoord = particles_.at(id) -> give_coordinates();
        return pCoord;
    }

    // WRITE PARTICLE VELOCITY DATA
    void write_particle_velocity_data_to_file(std::ostream& outFile);

    // WRITE PARTICLE PRESSURE DATA
    void write_particle_pressure_data_to_file(std::ostream& outFile);

    // WRITE PARTICLE STRESS DATA
    void write_particle_stress_data_to_file(std::ostream& outFile);

    // WRITE PARTICLE STRAIN DATA
    void write_particle_strain_data_to_file(std::ostream& outFile);


protected:
    // 1. Pointers to the all particles
    std::vector<mpm::Particle*> particles_;
};

#include "MpmParticle.ipp"

#endif
