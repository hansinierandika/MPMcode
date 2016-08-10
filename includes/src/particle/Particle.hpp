/***************************************************************************
                          Material Point Method
                           Shyamini Kularathna
                         University Of Cambridge
File: Particle.hpp
****************************************************************************/
#ifndef MPM_PARTICLE_H
#define MPM_PARTICLE_H

// c++ header files
#include <vector>
#include <fstream>
#include <iostream>

// eigen header files
#include <Eigen/Dense>

// header file wich inclued MPM input parameters
#include "PropertyParse.hpp"

// mpm header files
#include "Constants.hpp"
#include "Node.hpp"
#include "Element.hpp"
#include "MaterialBase.hpp"

namespace mpm {
    class Particle;
}

class mpm::Particle {

protected:

    static const unsigned dim = mpm::constants::DIM;
    static const unsigned numNodes = mpm::constants::NUMNODES;
    static const unsigned dof = 3 * (dim - 1);

    typedef mpm::Element* ElementPtr;
    typedef mpm::Node* NodePtr;
    typedef mpm::material::MaterialBase* MaterialBasePtr;

    typedef Eigen::Matrix<double, 1, dim>        VectorDDIM;
    typedef Eigen::Matrix<double, 1, dof>        VectorDDOF;
    typedef Eigen::Matrix<double, 1, 6>          VectorD1x6;
    typedef Eigen::Matrix<NodePtr,1, numNodes>   VectorNPtrNN;
 
    typedef Eigen::Matrix<double, dim, dim>      MatrixDDIMDIM;
    typedef Eigen::Matrix<double, dof, dim>      MatrixDDOFDIM;
    typedef Eigen::Matrix<double, numNodes, dim> MatrixDNNDIM;

    typedef std::vector<MaterialBasePtr>         VecOfMaterialBasePtr;
    typedef std::vector<MatrixDDOFDIM>           VecOfMatDDOFDIM;

    typedef Eigen::Matrix<double, 1, numNodes>   VectorDNN;
    typedef Eigen::Matrix<double, dim, numNodes> MatrixDDIMNN;

public:

    // constructor
    Particle(const unsigned& id, const unsigned& matID, const VectorDDIM& spacing);

    // initialise particle
    void initialise_particle();

    // set coordinates
    void set_coordinates(const Eigen::Matrix<double, 1, dim> &coord) {
        coord_ = coord;
    }

    // set element and nodes
    void set_element_and_nodes(const ElementPtr& ptrE, const VectorNPtrNN& ptrsN) {
        element_ = ptrE;
        nodes_   = ptrsN;
    }

    // set initial stress
    void set_initial_stress(const VectorD1x6& stress);

    // set material
    void set_material(VecOfMaterialBasePtr materials);

    //set initial velocity
    void set_initial_velocity();

    // map mass to nodes
    void map_mass_to_nodes();

    // map volume to nodes
    void map_volume_to_nodes();

    // map momentum to nodes
    void map_momentum_to_nodes();

    // map momentum to nodes specially begining of time step 
    void map_momentum_to_nodes_at_begin();

    // map momentum to nodes specially end of time step
    void map_momentum_to_nodes_at_end();

    // map pressure to nodes
    void map_pressure_to_nodes();

    // map volumetric strain rate to nodes
    void map_volumetric_strain_rate_to_nodes();

    // assign body force to nodes
    void assign_body_force_to_nodes();

    // assign traction force to nodes
    void assign_traction_force_to_nodes();

    // assign internal force to nodes
    void assign_internal_force_to_nodes();

    // compute strain rate
    void compute_strain_rate();

    // compute strain rate at element center
    void compute_centre_strain_rate();

    // compute strain rate BBar method
    void compute_BBar_strain_rate();

    // compute principle strain
    void compute_principle_strain(const VectorDDOF& strain);

    // compute volumetric strain rate at centre of element mapped from nodes
    void compute_centre_vol_strain_rate_from_nodes();

    // compute strain
    void compute_strain();

    // compute stress
    void compute_stress();

    // compute stress using enhanced strain
    void compute_stress_BBar_method();
    
    // update velocity
    void update_velocity(const double& dt);

    // update position
    void update_position(const double& dt);

    // update density
    void update_density();

    // update pressure
    void update_pressure();

    // give particle id
    unsigned give_id() const {
        return id_;
    }

    // give particle coordinates
    VectorDDIM give_coordinates() const {
        return coord_;
    }

    // give particle spacing
    VectorDDIM give_spacing() const {
        return spacing_;
    }

    // give particle material id
    unsigned give_mat_id() const {
        unsigned mat_id_;
    }

    // give particle velocity
    VectorDDIM give_velocity() const {
        return velocity_;
    }

    // compute local coordinates
    void compute_local_coordinates();

    // compute shape functions
    void compute_shape_functions();

    // compute global derives of shape functions
    void compute_global_derivatives_shape_functions();

    // compute global derivatives of shape function at element centre
    void compute_global_derivatives_shape_functions_at_centre();

    // compute B matrix
    //!           B_ = [B_0, B_1, B_2, B_3]
    //!           where B_i = [B_ix    0      0 ]
    //!                       [ 0     B_iy    0 ]
    //!                       [ 0      0    B_iz]
    //!                        ------------------
    //!                       [B_iy   B_ix    0 ]
    //!                       [ 0     B_iz  B_iy]
    //!                       [B_iz    0    B_ix]
    //!            where B_ij = dN_i(x_p) / dj; j = x,y,z           
    //!
    void compute_B_matrix();

    // compute B matrx at the centre of the element
    void compute_B_matrix_at_centre();

    // compute BBar matrix
    void compute_BBar_matrix(); 



    // WRITE VELOCITY TO FILE
    void write_velocity(std::ostream& oFile);

    // WRITE PRESSURE TO FILE
    void write_pressure(std::ostream& oFile);

    // WRITE STRESS TO FILE
    void write_stress(std::ostream& oFile);

    // WRITE STRAIN TO FILE
    void write_strain(std::ostream& oFile);

private:
    // compute particle mass
    void compute_mass();

    // change the sign of value
    void sign(double& variable, double value);


protected:

    unsigned id_;
    VectorDDIM coord_;
    VectorDDIM spacing_;

    unsigned mat_id_;
    MaterialBasePtr material_;
    VectorDDIM gravity_;

    double mass_;
    double volume_;
    VectorDDIM velocity_;
    double pressure_;
    double density_;

    VectorD1x6 stress_;
    VectorDDOF strain_;

    VectorDDOF strain_rate_;
    VectorDDOF BBar_strain_rate_;
    VectorDDOF centre_strain_rate_;

    VectorDDIM principle_strain_;
    double vol_strain_rate_;
    double centre_vol_strain_rate_p_;
    double centre_vol_strain_rate_n_;

    VectorNPtrNN nodes_;
    ElementPtr element_;

    VectorDDIM xi_;

    VectorDNN shape_fun_;
    VectorDNN shape_fun_centre_;
    MatrixDDIMNN grad_shape_fun_;
    MatrixDDIMNN grad_shape_fun_centre_;

    std::array<Eigen::Matrix<double, dof, dim>, numNodes> B_;
    std::array<Eigen::Matrix<double, dof, dim>, numNodes> BCentre_;
    std::array<Eigen::Matrix<double, dof, dim>, numNodes> BBar_;

};


#include "Particle.ipp"

#endif
