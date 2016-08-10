/***************************************************************************
                          Material Point Method
                           Shyamini Kularathna
                         University Of Cambridge
File: Node.hpp
****************************************************************************/
#ifndef MPM_NODE_H
#define MPM_NODE_H

// c++ header files
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

// eigen header files
#include <Eigen/Dense>

// mpm header files which includes input parameters
#include "PropertyParse.hpp"

// mpm header files
#include "Constants.hpp"

namespace mpm {
    class Node;
}

class mpm::Node {

protected:
    static const unsigned dim = mpm::constants::DIM;
    static const unsigned numNodes = mpm::constants::NUMNODES;

    typedef Eigen::Matrix<double, 1, dim>   VectorDDIM;
    typedef Eigen::Matrix<int, 1, dim>      VectorUDIM;


public:

    // constructor
    Node(std::string& iLine, unsigned& id);

    // initialise node
    void initialise_node();

    // set nodal velocity constraint
    void set_velocity_constraints(unsigned& dir, double& value) {
        nVelConState_(dir) = 1;
        nVelConValue_(dir) = value;
    }

    // set nodal pressure constraint
    void set_pressure_constraints(double& value) {
        pressureConstraintStatus_ = 1;
        pressureConstraintValue_ = value;
    }

    // assign mass from particles
    void assign_nodal_mass(const double& mass) {
        nMass_ += mass;
    }

    // assign volume from particles
    void assign_nodal_volume(const double& volume) {
        nVolume_ += volume;
    }

    // assign momentum from paricles
    void assign_nodal_momentum(const VectorDDIM& momentum) {
        nMomentum_ += momentum;
    }

    // assign momentum at the beginig of time step
    void assign_nodal_momentum_at_begin(const VectorDDIM& momentum) {
        nBeginMomentum_ += momentum;
    }

    // assign momentum at the end of time step
    void assign_nodal_momentum_at_end(const VectorDDIM& momentum) {
        nEndMomentum_ += momentum;
    }

    // assign volumetric strain rate from particles
    void assign_nodal_vol_strain_rate(const double& vol_strainrate) {
        nVolStrainRate_ += vol_strainrate;
    }

    // compute nodal velocity from momentum
    void compute_nodal_velocity_from_momentum();

    // compute nodal velocity from moentum at begining of time step
    void compute_nodal_velocity_from_begin_momentum();

    // compute nodal velocity for momentum at end of time step
    void compute_nodal_velocity_from_end_momentum();

    // assign external force
    void assign_external_force(const VectorDDIM& extForce) {
        nExtForce_ += extForce;
    }

    // assign internal force
    void assign_internal_force(const VectorDDIM& intForce) {
        nIntForce_ -= intForce;
    }

    // solve acceleration and velocity
    void solve_acceleration_and_velocity(const double dt);

    // assign nodal pressure
    void assign_nodal_pressure(const double& pressureXmass) {
        nPressure_ += pressureXmass;
    }

    // compute pressure at nodes from mapped values
    void compute_nodal_pressure_from_mapped_pressure();

    // give node id
    unsigned give_id() const {
        return nodeId_;
    }

    // give node coordinates
    VectorDDIM give_node_coordinates() const {
        return nodeCoord_;
    }

    // give velocity at node
    VectorDDIM give_node_velocity() const {
        return nVelocity_;
    }

    // give acceleration at node
    VectorDDIM give_node_acceleration() const {
        return nAcceleration_;
    }

    // give pressure at node 
    double give_node_pressure() const {
        return nPressure_;
    }

    // give volumetric strain rate at node 
    double give_node_vol_strain_rate() const {
        return nVolStrainRate_;
    }

private:

    // CHECK DOUBLE PRECISION OF A GIVEN VALUE
    void check_double_precision(double& value);

    // ASSIGN VELOCITY CONSTRAINT TO THE NODAL VELOCITY
    void apply_velocity_constraint();

    // ASSIGN ACCELERATION CONSTRAINT
    void apply_acceleration_constraint();

    // ASSIGN FRICTION CONSTRAINTS
    void apply_friction_constraint();

    // ASSIGN PRESSURE CONSTRAINT TO THE NODAL PRESSURE
    void apply_pressure_constraint();


protected:
    // ID
    unsigned nodeId_;
    // COORDINATES
    VectorDDIM nodeCoord_;

    // VELOCITY CONSTRAINTS
    VectorUDIM nVelConState_;
    VectorDDIM nVelConValue_;

    // PRESSURE CONSTRAINTS
    bool pressureConstraintStatus_;
    double pressureConstraintValue_;

    // MASS
    double nMass_;
    // MOMENTUM
    VectorDDIM nMomentum_;
    // MOMENTUM AT BEGINING OF TIME STEP
    VectorDDIM nBeginMomentum_;
    // MOMENTUM AT END OF TIME STEP
    VectorDDIM nEndMomentum_;
    // VELOCITY
    VectorDDIM nVelocity_;
    // ACCELERATION
    VectorDDIM nAcceleration_;
    // PRESSURE
    double nPressure_;
    double nVolStrainRate_;
    double nVolume_;

    // EXTERNAL FORCE
    VectorDDIM nExtForce_;
    // INTERNAL FORCE
    VectorDDIM nIntForce_;

};

#include "Node.ipp"

#endif // NODE_MPM_H
