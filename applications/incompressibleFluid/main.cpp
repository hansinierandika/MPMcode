/*************************************************************************
                        2D-Material Point Method
                       Author: Shyamini Kularathna
                         University of Cambridge
           
Description: This file contains the incompressible fluid flow solver based on 
             the Chorin's Projection method. The incompressible Navier-Stokes 
             equations are solved using a semi-implicit approach in three steps.
             There are weveral options to be chosen by the user.
                  1. Gravity is known at nodes or particles
                  2. Consistent mass matrix or lumped mass matrix
**************************************************************************/
                                                                          
// c++ header files
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <functional>

// header files
#include "FileHandle.hpp"
#include "PropertyParse.hpp"
#include "Mesh.hpp"
#include "MpmParticle.hpp"
#include "Element.hpp"
#include "Node.hpp"
#include "Particle.hpp"


int main (int argc, char* argv[]) {

    std::cout << "\n \n \t \t Single Phase Material Point Method \n";
    std::cout << "\t \t \t Univerisy Of Cambridge \n";

    if (argc != 2)
        std::cerr << "ERROR: in arguments" << "\n";

    boost::filesystem::path p (argv[1]);
    if (boost::filesystem::exists(p))
    {
        if (boost::filesystem::is_directory(p))
            std::cout << "\n \t Input data is from " << p << "\n";
        else
            std::cerr << "ERROR: " << p << " is not a directory \n";
    }
    else
        std::cerr << "ERROR: " << p << " does not exist" << "\n";


    mpm::FileHandle fileHandle_(p);
    mpm::Mesh* mesh_ = fileHandle_.read_mesh();
    mesh_->iterate_over_elements(std::bind(&mpm::Element::compute_centre_coordinates, std::placeholders::_1));

    mpm::MpmParticle* particles_ = fileHandle_.read_particles();

    double dt_ = mpm::misc::dt;
    unsigned TotalSteps = mpm::misc::numOfTotalSteps;
    unsigned subSteps = mpm::misc::numOfSubSteps;
    std::cout << " Read All InputFiles" << "\n \n";

    unsigned writeSteps = 0;
    for (unsigned i = 0; i < TotalSteps; i++) {
        mesh_->initialise_mesh();
        particles_->iterate_over_particles(std::bind(&mpm::Particle::initialise_particle, std::placeholders::_1));

        if ( i == writeSteps * subSteps) {
            std::cout << "\nStep: " << i << "/" << TotalSteps << "\n";
            fileHandle_.WriteData(i, particles_);
            writeSteps++;
        } 

        mesh_->locate_particles_in_mesh(particles_);
        particles_->iterate_over_particles(std::bind(&mpm::Particle::compute_local_coordinates, std::placeholders::_1));
        particles_->iterate_over_particles(std::bind(&mpm::Particle::compute_shape_functions, std::placeholders::_1));
        particles_->iterate_over_particles(std::bind(&mpm::Particle::compute_global_derivatives_shape_functions, std::placeholders::_1));
        particles_->iterate_over_particles(std::bind(&mpm::Particle::compute_global_derivatives_shape_functions_at_centre, std::placeholders::_1));
        particles_->iterate_over_particles(std::bind(&mpm::Particle::compute_B_matrix, std::placeholders::_1));
        particles_->iterate_over_particles(std::bind(&mpm::Particle::compute_BBar_matrix, std::placeholders::_1));
        particles_->iterate_over_particles(std::bind(&mpm::Particle::compute_B_matrix_at_centre, std::placeholders::_1));

        particles_->iterate_over_particles(std::bind(&mpm::Particle::map_mass_to_nodes, std::placeholders::_1));
        particles_->iterate_over_particles(std::bind(&mpm::Particle::map_momentum_to_nodes, std::placeholders::_1));
        mesh_->iterate_over_nodes_of_p(std::bind(&mpm::Node::compute_nodal_velocity_from_momentum, std::placeholders::_1));

        particles_->iterate_over_particles(std::bind(&mpm::Particle::assign_body_force_to_nodes, std::placeholders::_1));
        particles_->iterate_over_particles(std::bind(&mpm::Particle::assign_internal_force_to_nodes, std::placeholders::_1));
        mesh_->iterate_over_nodes_of_p(std::bind(&mpm::Node::solve_acceleration_and_velocity, std::placeholders::_1, dt_));

        particles_->iterate_over_particles(std::bind(&mpm::Particle::compute_strain_rate, std::placeholders::_1));
        particles_->iterate_over_particles(std::bind(&mpm::Particle::compute_centre_strain_rate, std::placeholders::_1));
        particles_->iterate_over_particles(std::bind(&mpm::Particle::compute_BBar_strain_rate, std::placeholders::_1));

        particles_->iterate_over_particles(std::bind(&mpm::Particle::map_volume_to_nodes, std::placeholders::_1));
        particles_->iterate_over_particles(std::bind(&mpm::Particle::map_volumetric_strain_rate_to_nodes, std::placeholders::_1));
        particles_->iterate_over_particles(std::bind(&mpm::Particle::compute_centre_vol_strain_rate_from_nodes, std::placeholders::_1));

        particles_->iterate_over_particles(std::bind(&mpm::Particle::compute_strain, std::placeholders::_1));
        particles_->iterate_over_particles(std::bind(&mpm::Particle::compute_stress, std::placeholders::_1));

        particles_->iterate_over_particles(std::bind(&mpm::Particle::update_velocity, std::placeholders::_1, dt_));
        particles_->iterate_over_particles(std::bind(&mpm::Particle::update_position, std::placeholders::_1, dt_));
        particles_->iterate_over_particles(std::bind(&mpm::Particle::update_density, std::placeholders::_1));
    }
    delete mesh_;
    delete particles_;
    return 0;
}

