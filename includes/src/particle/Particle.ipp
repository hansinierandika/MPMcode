mpm::Particle::Particle(const unsigned& id, const unsigned& matID, const VectorDDIM& spacing) {
    id_ = id;
    spacing_ = spacing;
    mat_id_ = matID;
    gravity_ = VectorDDIM::Zero();
    if (mpm::misc::gravity)
        gravity_(dim -1) = -9.81;

    mass_ = 0.;
    velocity_ = VectorDDIM::Zero();
    pressure_ = 0.;
    stress_ = VectorD1x6::Zero();
    strain_ = VectorDDOF::Zero();
    principle_strain_ = VectorDDIM::Zero();  
 
    strain_rate_ = VectorDDOF::Zero();
    BBar_strain_rate_ = VectorDDOF::Zero();
    centre_strain_rate_ = VectorDDOF::Zero();

    vol_strain_rate_ = 0.;
    centre_vol_strain_rate_p_ = 0.;
    centre_vol_strain_rate_n_ = 0.;

    nodes_   = VectorNPtrNN::Zero();
    element_ = NULL;
}


void mpm::Particle::initialise_particle() {
    nodes_ = VectorNPtrNN::Zero();
    strain_rate_ = VectorDDOF::Zero();
    BBar_strain_rate_ = VectorDDOF::Zero();
    centre_strain_rate_ = VectorDDOF::Zero();
}



void mpm::Particle::set_initial_stress(const VectorD1x6& stress) {    
    stress_ = stress;
    pressure_ = -(stress(0) + stress(1) + stress(2)) / 3.;   
}


void mpm::Particle::set_material(VecOfMaterialBasePtr materials) {
    material_ = materials.at(mat_id_);
    density_ = material_->giveDensity();
    compute_mass();
}


void mpm::Particle::set_initial_velocity() {
    for (unsigned i = 0; i < numNodes; i++) {
        VectorDDIM nVelocity = nodes_(i)->give_node_velocity();
        velocity_ += (shape_fun_(i) * nVelocity);
    }
}


void mpm::Particle::compute_mass() {
    if (dim == 2)
        mass_ = density_ * spacing_(0)* spacing_(1) * 1.;
    if (dim == 3)
        mass_ = density_ * spacing_(0)* spacing_(1) * spacing_(2);
    volume_ = mass_ / density_;
}


void mpm::Particle::map_mass_to_nodes() {
    double node_mass = 0.;
    for (unsigned i = 0; i < numNodes; i++) {
        node_mass = mass_ * shape_fun_(i);
        nodes_(i)->assign_nodal_mass(node_mass);
    }
}


void mpm::Particle::map_volume_to_nodes() {
    double node_volume = 0.;
    for (unsigned i = 0; i < numNodes; i++) {
        node_volume = volume_ * shape_fun_(i);
        nodes_(i)->assign_nodal_volume(node_volume);
    }
}


void mpm::Particle::map_momentum_to_nodes() {
    VectorDDIM node_momentum = VectorDDIM::Zero();
    for (unsigned i = 0; i < numNodes; i++) {
        node_momentum = mass_ * velocity_ * shape_fun_(i);
        nodes_(i)->assign_nodal_momentum(node_momentum);
    }
}


void mpm::Particle::map_momentum_to_nodes_at_begin() {
    VectorDDIM node_momentum = VectorDDIM::Zero();
    for (unsigned i = 0; i < numNodes; i++) {
        node_momentum = mass_ * velocity_ * shape_fun_(i);
        nodes_(i)->assign_nodal_momentum_at_begin(node_momentum);
    }
}


void mpm::Particle::map_momentum_to_nodes_at_end() {
    VectorDDIM node_momentum = VectorDDIM::Zero();
    for (unsigned i = 0; i < numNodes; i++) {
        node_momentum = mass_ * velocity_ * shape_fun_(i);
        nodes_(i)->assign_nodal_momentum_at_end(node_momentum);
    }
}


void mpm::Particle::map_pressure_to_nodes() {
    double node_pressure = 0.;
    for (unsigned i = 0; i < numNodes; i++) {
        node_pressure = mass_ * pressure_ * shape_fun_(i);
        nodes_(i)->assign_nodal_pressure(node_pressure);
    }
}


void mpm::Particle::map_volumetric_strain_rate_to_nodes() {
    double node_vol_strain_rate = 0.;
    for (unsigned i = 0; i < numNodes; i++) {
        node_vol_strain_rate = volume_ * vol_strain_rate_ * shape_fun_(i);
        nodes_(i)->assign_nodal_vol_strain_rate(node_vol_strain_rate);
    }
}


void mpm::Particle::assign_body_force_to_nodes() {
    for (unsigned i = 0; i < numNodes; i++) {
        VectorDDIM node_body_force = shape_fun_(i) * mass_ * gravity_;
        nodes_(i)->assign_external_force(node_body_force);
    }
}


void mpm::Particle::assign_traction_force_to_nodes() {

}



void mpm::Particle::assign_internal_force_to_nodes() {
    VectorDDIM node_int_force;
    VectorDDOF temp_stress;
    if (dim == 2) {
        temp_stress(0) = stress_(0);
        temp_stress(1) = stress_(1);
        temp_stress(2) = stress_(3);
    }

    else if (dim == 3) {
        temp_stress(0) = stress_(0);
        temp_stress(1) = stress_(1);
        temp_stress(2) = stress_(2);
        temp_stress(3) = stress_(3);
        temp_stress(4) = stress_(4);
        temp_stress(5) = stress_(5);
    }
    Eigen::Matrix<double, dim, 1> int_force;
    for (unsigned i = 0; i < numNodes; i++) {
        MatrixDDOFDIM Bi = B_.at(i);
        int_force = (mass_/density_)*(Bi.transpose() * temp_stress.transpose());
        node_int_force = int_force.transpose();
        nodes_(i)->assign_internal_force(node_int_force);
    }
}




void mpm::Particle::compute_strain_rate() {
    Eigen::Matrix<double, dim, 1> node_velocity;
    strain_rate_ = VectorDDOF::Zero();
    for (unsigned i = 0; i < numNodes; i++) {
        MatrixDDOFDIM Bi = B_.at(i);
        VectorDDIM velocity = nodes_(i)->give_node_velocity();
        node_velocity = velocity.transpose();
        strain_rate_ += (Bi * node_velocity);
    }
    if (dim == 2)
        vol_strain_rate_ = (strain_rate_(0) + strain_rate_(1));
    else if (dim == 3)
        vol_strain_rate_ = (strain_rate_(0) + strain_rate_(1) + strain_rate_(2));  
}

void mpm::Particle::compute_centre_strain_rate() {
    Eigen::Matrix<double, dim, 1> node_Velocity;
    centre_strain_rate_ = VectorDDOF::Zero();
    for (unsigned i = 0; i < numNodes; i++) {
        MatrixDDOFDIM Bi_centre = BCentre_.at(i);
        VectorDDIM velocity = nodes_(i)->give_node_velocity();
        node_Velocity = velocity.transpose();
        centre_strain_rate_ += (Bi_centre * node_Velocity);
    }
    if (dim == 2)
        centre_vol_strain_rate_p_ = (centre_strain_rate_(0) + centre_strain_rate_(1));
    else if (dim == 3)
        centre_vol_strain_rate_p_ = (centre_strain_rate_(0) + centre_strain_rate_(1) + centre_strain_rate_(2));
}
 
void mpm::Particle::compute_BBar_strain_rate() {
    Eigen::Matrix<double, dim, 1> node_Velocity;
    BBar_strain_rate_ = VectorDDOF::Zero();
    for (unsigned i = 0; i < numNodes; i++) {
        MatrixDDOFDIM BBari = BBar_.at(i);
        VectorDDIM velocity = nodes_(i)->give_node_velocity();
        node_Velocity = velocity.transpose();
        BBar_strain_rate_ += (BBari * node_Velocity);
    }
}

void mpm::Particle::compute_centre_vol_strain_rate_from_nodes() {
    double node_centre_vstrainrate = 0.;
    centre_vol_strain_rate_n_ = 0.;
    for (unsigned i = 0; i < numNodes; i++) {
        node_centre_vstrainrate = nodes_(i)->give_node_vol_strain_rate();
        centre_vol_strain_rate_n_ += (shape_fun_centre_(i) * node_centre_vstrainrate);
    }
}

/*
void mpm::Particle::compute_principle_strain(const VectorDDOF& strain) {

    double volStrainOld = 0;
    for (unsigned i = 0; i < dim; i++)
        volStrainOld += particlePrincipleStrain_(i);

    if (dim == 2) {
        particlePrincipleStrain_(0) = ((strain(0) + strain(1))/2) + std::sqrt(((strain(0) - strain(1)) * (strain(0) - strain(1)) / 4) + (strain(2) * strain(2)));
        particlePrincipleStrain_(1) = ((strain(0) + strain(1))/2) - std::sqrt(((strain(0) - strain(1)) * (strain(0) - strain(1)) / 4) + (strain(2) * strain(2)));
    }

    double volStrainNew = 0;
    for (unsigned i = 0; i < dim; i++)
        volStrainNew += particlePrincipleStrain_(i);

    dVolStrain_ = volStrainNew - volStrainOld;

}
*/

void mpm::Particle::compute_strain() {
    double dt = mpm::misc::dt;
    strain_ += (BBar_strain_rate_ * dt);
}


void mpm::Particle::compute_stress() {
    double dt = mpm::misc::dt;
    double dvolume_strain = dt * centre_vol_strain_rate_n_;
    VectorDDOF dstrain = (dt * BBar_strain_rate_);
    material_->computeStress(dstrain, stress_, pressure_, dvolume_strain);
}


void mpm::Particle::update_velocity(const double& dt) {
    VectorDDIM temp_acceleration = VectorDDIM::Zero();
    for (unsigned i = 0; i < numNodes; i++) {
        VectorDDIM node_acceleration = nodes_(i)->give_node_acceleration();
        temp_acceleration += (shape_fun_(i) * node_acceleration);
    }

    for (unsigned i = 0; i < dim; i++) {
        if (std::fabs(temp_acceleration(i)) < 1.0E-16)
            temp_acceleration(i) = 0.;
    }
    velocity_ += (dt * temp_acceleration);
}



void mpm::Particle::update_position(const double& dt) {
    VectorDDIM temp_velocity = VectorDDIM::Zero();
    for (unsigned i = 0; i < numNodes; i++) {
         VectorDDIM node_velocity = nodes_(i)->give_node_velocity();
        temp_velocity += (shape_fun_(i) * node_velocity);
    }
    for (unsigned i = 0; i < dim; i++) {
        if (std::fabs(temp_velocity(i)) < 1.0E-16)
            temp_velocity(i) = 0.;
    }
    coord_ += (dt * temp_velocity);
}



void mpm::Particle::update_density() {
    density_ = density_ / (1 + (mpm::misc::dt * centre_vol_strain_rate_n_));
    volume_ = mass_ / density_;
}


/*
void mpm::Particle::update_pressure() {

    double tempPressure = 0.;
    for (unsigned i = 0; i < numNodes; i++) {
        double nPressure = particleNodes_(i)->giveNodePressure();
        tempPressure += (shapeFun_(i) * nPressure);
    }
    if (std::fabs(tempPressure) < 1.E-15)
        tempPressure = 0.;
    particlePressure_ = tempPressure;
}
*/





void mpm::Particle::write_velocity(std::ostream& oFile) {
    if (dim == 2)
        oFile << velocity_(0) << " " << velocity_(1) << " " << "0" << "\n";
    if (dim == 3)
        oFile << velocity_(0) << " " << velocity_(1) << " " << velocity_(2) << "\n";

}

void mpm::Particle::write_pressure(std::ostream& oFile) {
    oFile << pressure_ << "\n";

}

void mpm::Particle::write_stress(std::ostream& oFile) {
    if (dim == 2)
        oFile << stress_(0) << " " << stress_(1) << " " << stress_(3) << "\n";
    if (dim == 3)
        oFile << stress_(0) << " " << stress_(1) << " " << stress_(2) << " " << stress_(3) << " " << stress_(4) << " " << stress_(5) << "\n";

}

void mpm::Particle::write_strain(std::ostream& oFile) {

    if (dim == 2)
        oFile << strain_(0) << " " << strain_(1) << " " << strain_(2) << "\n";
    if (dim == 3)
        oFile << strain_(0) << " " << strain_(1) << " " << strain_(2) << " " << strain_(3) << " " << strain_(4) << " " << strain_(5) << "\n";

}


void mpm::Particle::compute_local_coordinates() {
    VectorDDIM elem_centre__coord = element_ -> give_element_centre_coord();
    VectorDDIM elem_length        = element_ -> give_element_length();

    for (unsigned i = 0; i < dim; i++) {
        xi_(i) = 2. * (coord_(i) - elem_centre__coord(i)) / elem_length(i);
        if (((std::fabs(xi_(i)) > 0.999999) && (std::fabs(xi_(i)) < 1.)) || (std::fabs(xi_(i)) > 1.))
            sign(xi_(i), 1.);
        else if ((std::fabs(xi_(i)) > 0.) && (std::fabs(xi_(i)) < 0.000001))
            xi_(i) = 0.;
    }
}


void mpm::Particle::compute_shape_functions() {
    if (dim == 2) {
        shape_fun_(0) = 0.25 * fabs((1 - xi_(0)) * (1 - xi_(1)));
        shape_fun_(1) = 0.25 * fabs((1 + xi_(0)) * (1 - xi_(1)));
        shape_fun_(2) = 0.25 * fabs((1 + xi_(0)) * (1 + xi_(1)));
        shape_fun_(3) = 0.25 * fabs((1 - xi_(0)) * (1 + xi_(1)));

        shape_fun_centre_(0) = 0.25;
        shape_fun_centre_(1) = 0.25;
        shape_fun_centre_(2) = 0.25;
        shape_fun_centre_(3) = 0.25;
    } else if (dim == 3) {
        shape_fun_(0) = fabs((1 - xi_(0)) * (1 - xi_(1)) * (1 - xi_(2))) / 8.;
        shape_fun_(1) = fabs((1 + xi_(0)) * (1 - xi_(1)) * (1 - xi_(2))) / 8.;
        shape_fun_(2) = fabs((1 + xi_(0)) * (1 - xi_(1)) * (1 + xi_(2))) / 8.;
        shape_fun_(3) = fabs((1 - xi_(0)) * (1 - xi_(1)) * (1 + xi_(2))) / 8.;
        shape_fun_(4) = fabs((1 - xi_(0)) * (1 + xi_(1)) * (1 - xi_(2))) / 8.;
        shape_fun_(5) = fabs((1 + xi_(0)) * (1 + xi_(1)) * (1 - xi_(2))) / 8.;
        shape_fun_(6) = fabs((1 + xi_(0)) * (1 + xi_(1)) * (1 + xi_(2))) / 8.;
        shape_fun_(7) = fabs((1 - xi_(0)) * (1 + xi_(1)) * (1 + xi_(2))) / 8.;
    }
}


void mpm::Particle::compute_global_derivatives_shape_functions() {
    VectorDDIM L = element_ -> give_element_length();
    if (dim == 2) {
        grad_shape_fun_(0, 0) = -0.5 * (1 - xi_(1)) / L(0);
        grad_shape_fun_(0, 1) =  0.5 * (1 - xi_(1)) / L(0);
        grad_shape_fun_(0, 2) =  0.5 * (1 + xi_(1)) / L(0);
        grad_shape_fun_(0, 3) = -0.5 * (1 + xi_(1)) / L(0);

        grad_shape_fun_(1, 0) = -0.5 * (1 - xi_(0)) / L(1);
        grad_shape_fun_(1, 1) = -0.5 * (1 + xi_(0)) / L(1);
        grad_shape_fun_(1, 2) =  0.5 * (1 + xi_(0)) / L(1);
        grad_shape_fun_(1, 3) =  0.5 * (1 - xi_(0)) / L(1);
    }
    else if (dim == 3) { }
}


void mpm::Particle::compute_global_derivatives_shape_functions_at_centre() {
    VectorDDIM L = element_ -> give_element_length();
    if (dim == 2) {
        grad_shape_fun_centre_(0, 0) = -0.5 / L(0);
        grad_shape_fun_centre_(0, 1) =  0.5 / L(0);
        grad_shape_fun_centre_(0, 2) =  0.5 / L(0);
        grad_shape_fun_centre_(0, 3) = -0.5 / L(0);

        grad_shape_fun_centre_(1, 0) = -0.5 / L(1);
        grad_shape_fun_centre_(1, 1) = -0.5 / L(1);
        grad_shape_fun_centre_(1, 2) =  0.5 / L(1);
        grad_shape_fun_centre_(1, 3) =  0.5 / L(1);
    }
    else if (dim == 3) { }
}



void mpm::Particle::compute_B_matrix() {
    MatrixDDOFDIM Bi_;
    for (unsigned i = 0; i < numNodes; i++) {
        if (dim == 2) {
            Bi_(0,0) = grad_shape_fun_(0,i); 
            Bi_(0,1) = 0.;
            Bi_(1,0) = 0.;              
            Bi_(1,1) = grad_shape_fun_(1,i);
            Bi_(2,0) = grad_shape_fun_(1,i); 
            Bi_(2,1) = grad_shape_fun_(0,i);
        }
        else if (dim == 3) {
            Bi_(0,0) = grad_shape_fun_(0,i); 
            Bi_(0,1) = 0.; 
            Bi_(0,2) = 0.;
            Bi_(1,0) = 0.; 
            Bi_(1,1) = grad_shape_fun_(1,i);
            Bi_(1,2) = 0.;
            Bi_(2,0) = 0.; 
            Bi_(2,1) = 0.;
            Bi_(2,2) = grad_shape_fun_(2,i);
            Bi_(3,0) = grad_shape_fun_(1,i); 
            Bi_(3,1) = grad_shape_fun_(0,i); 
            Bi_(3,2) = 0; 
            Bi_(4,0) = 0; 
            Bi_(4,1) = grad_shape_fun_(2,i); 
            Bi_(4,2) = grad_shape_fun_(1,i);
            Bi_(5,0) = grad_shape_fun_(2,i);
            Bi_(5,1) = 0.;
            Bi_(5,2) = grad_shape_fun_(0,i);
        }
        B_.at(i) = Bi_;
    }
}


void mpm::Particle::compute_B_matrix_at_centre() {
    MatrixDDOFDIM BiCentre_;
    for (unsigned i = 0; i < numNodes; i++) {
        if (dim == 2) {
            BiCentre_(0,0) = grad_shape_fun_centre_(0,i); 
            BiCentre_(0,1) = 0.;
            BiCentre_(1,0) = 0.;              
            BiCentre_(1,1) = grad_shape_fun_centre_(1,i);
            BiCentre_(2,0) = grad_shape_fun_centre_(1,i); 
            BiCentre_(2,1) = grad_shape_fun_centre_(0,i);
        }
        BCentre_.at(i) = BiCentre_;
    }
}



void mpm::Particle::compute_BBar_matrix() {
    MatrixDDOFDIM BBari_;
    for (unsigned i = 0; i < numNodes; i++) {
        if (dim == 2) {
            BBari_(0,0) = (1/3.0)*(grad_shape_fun_centre_(0,i) + (2*grad_shape_fun_(0,i)));
            BBari_(0,1) = (1/3.0)*(grad_shape_fun_centre_(0,i) - grad_shape_fun_(0,i));   
            BBari_(1,1) = (1/3.0)*(grad_shape_fun_centre_(1,i) + (2*grad_shape_fun_(1,i)));
            BBari_(2,0) = grad_shape_fun_(1,i); 
            BBari_(2,1) = grad_shape_fun_(0,i);
        }/*
        else if (dim == 3) {
            BBari_(0,0) = (1/3.0)*(dShapeFunCentre_(0,i) + (2*dShapeFun_(0,i)));
            BBari_(0,1) = (1/3.0)*(dShapeFunCentre_(1,i) - dShapeFun_(1,i)); 
            BBari_(0,2) = (1/3.0)*(dShapeFunCentre_(2,i) - dShapeFun_(2,i));

            BBari_(1,0) = (1/3.0)*(dShapeFunCentre_(0,i) - dShapeFun_(0,i)); 
            BBari_(1,1) = (1/3.0)*(dShapeFunCentre_(1,i) + (2*dShapeFun_(1,i)));
            BBari_(1,2) = (1/3.0)*(dShapeFunCentre_(2,i) - dShapeFun_(2,i));

            BBari_(2,0) = (1/3.0)*(dShapeFunCentre_(0,i) - dShapeFun_(0,i)); 
            BBari_(2,1) = (1/3.0)*(dShapeFunCentre_(1,i) - dShapeFun_(1,i));
            BBari_(2,2) = (1/3.0)*(dShapeFunCentre_(2,i) + (2*dShapeFun_(2,i)));

            BBari_(3,0) = dShapeFun_(1,i); 
            BBari_(3,1) = dShapeFun_(0,i); 
            BBari_(3,2) = 0; 
            BBari_(4,0) = 0; 
            BBari_(4,1) = dShapeFun_(2,i); 
            BBari_(4,2) = dShapeFun_(1,i);
            BBari_(5,0) = dShapeFun_(2,i);
            BBari_(5,1) = 0.;
            BBari_(5,2) = dShapeFun_(0,i);
            }*/
                BBar_.at(i) = BBari_;
    }
}


void mpm::Particle::sign(double& variable, double value) {
    if (variable < 0)
        variable = -value;
    if (variable > 0)
        variable = value;
}
