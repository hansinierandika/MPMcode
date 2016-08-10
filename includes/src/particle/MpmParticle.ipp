
mpm::MpmParticle::MpmParticle() {
    particles_.clear();
}

mpm::MpmParticle::~MpmParticle() {
  for (auto pPtr : particles_)
    delete pPtr;
  particles_.clear();
}


void mpm::MpmParticle::read_particles(std::ifstream& pFile, std::ifstream& sFile) {
    std::string line;
    unsigned numOfParticles, matId;
    VectorDDIM spacing, coordinates;
    VectorD6x1 iStress;

    // read particle coordinates
    std::getline(pFile, line);
    std::istringstream in(line);
    in >> numOfParticles >> matId;

    std::getline(pFile, line);
    std::istringstream space(line);
    for (unsigned i = 0; i < dim; i++)
        space >> spacing(i);

    for (unsigned i = 0; i < numOfParticles; i++) {
        ParticlePtr particle = new Particle(i, matId, spacing);
        std::getline(pFile, line);
        std::istringstream coord(line);
        for (unsigned j = 0; j < dim; j++)
            coord >> coordinates(j);
        particle->set_coordinates(coordinates);
        particles_.push_back(particle);
    }

    // read particle initial stress
    unsigned numOfInitStressParticles;
    unsigned particleId;
    std::getline(sFile, line);
    std::istringstream is(line);
    is >> numOfInitStressParticles;
    for (unsigned i = 0; i < numOfInitStressParticles; i++) {
        std::getline(sFile, line);
        std::istringstream stress(line);
        stress >> particleId;
        for (unsigned j = 0; j < 6; j++)
            stress >> iStress(j);
        particles_.at(particleId) -> set_initial_stress(iStress);
    }
}



void mpm::MpmParticle::assign_material_to_particles(VecOfMaterialBasePtr& materialPtrs) {

    for (const auto& particle : particles_)
        particle->set_material(materialPtrs);
}


template<typename FP>
void mpm::MpmParticle::iterate_over_particles(FP function) {
  std::for_each(particles_.begin(), particles_.end(), function);
}





void mpm::MpmParticle::write_particle_velocity_data_to_file(std::ostream& outFile) {
    unsigned numOfParticles = particles_.size();

    outFile << "# vtk DataFile Version 2.0" << "\n";
    outFile << "MPM Particle Velocity Data" << "\n";
    outFile << "ASCII" << "\n" << "DATASET UNSTRUCTURED_GRID" << "\n";
    outFile << "POINTS " << numOfParticles << " float" << "\n";

    VectorDDIM pCord;
    for (auto i : particles_) {
        pCord = i->give_coordinates();
        if (dim == 2)
            outFile << pCord(0) << " " << pCord(1) << " " << "0" << "\n";
        if (dim == 3)
            outFile << pCord(0) << " " << pCord(1) << " " << pCord(2) << "\n";
    }

    outFile << "CELLS " << numOfParticles << " " << 2*numOfParticles << "\n";
    for (unsigned i = 0; i < numOfParticles; i++)
        outFile << "1 " << i << "\n";

    outFile << "CELL_TYPES " << numOfParticles << "\n"; 
    for (unsigned i = 0; i < numOfParticles; i++)
        outFile << "1" << "\n";

    outFile << "POINT_DATA " << numOfParticles << "\n";
    outFile << "VECTORS Velocity float" << "\n";
    for (auto i : particles_)
        i -> write_velocity(outFile);

}




void mpm::MpmParticle::write_particle_pressure_data_to_file(std::ostream& outFile) {
    unsigned numOfParticles = particles_.size();

    outFile << "# vtk DataFile Version 2.0" << "\n";
    outFile << "MPM Particle Pressure Data" << "\n";
    outFile << "ASCII" << "\n" << "DATASET UNSTRUCTURED_GRID" << "\n";
    outFile << "POINTS " << numOfParticles << " float" << "\n";

    VectorDDIM pCord;
    for (auto i : particles_) {
        pCord = i->give_coordinates();
        if (dim == 2)
            outFile << pCord(0) << " " << pCord(1) << " " << "0" << "\n";
        if (dim == 3)
            outFile << pCord(0) << " " << pCord(1) << " " << pCord(2) << "\n";
    }

    outFile << "CELLS " << numOfParticles << " " << 2*numOfParticles << "\n";
    for (unsigned i = 0; i < numOfParticles; i++)
        outFile << "1 " << i << "\n";

    outFile << "CELL_TYPES " << numOfParticles << "\n"; 
    for (unsigned i = 0; i < numOfParticles; i++)
        outFile << "1" << "\n";

    outFile << "POINT_DATA " << numOfParticles << "\n";
    outFile << "SCALARS Pressure float" << "\n";
    outFile << "LOOKUP_TABLE default" << "\n";
    for (auto i : particles_)
        i -> write_pressure(outFile);

}




void mpm::MpmParticle::write_particle_stress_data_to_file(std::ostream& outFile) {
    unsigned numOfParticles = particles_.size();

    outFile << "# vtk DataFile Version 2.0" << "\n";
    outFile << "MPM Particle Stress Data" << "\n";
    outFile << "ASCII" << "\n" << "DATASET UNSTRUCTURED_GRID" << "\n";
    outFile << "POINTS " << numOfParticles << " float" << "\n";

    VectorDDIM pCord;
    for (auto i : particles_) {
        pCord = i->give_coordinates();
        if (dim == 2)
            outFile << pCord(0) << " " << pCord(1) << " " << "0" << "\n";
        if (dim == 3)
            outFile << pCord(0) << " " << pCord(1) << " " << pCord(2) << "\n";
    }

    outFile << "CELLS " << numOfParticles << " " << 2*numOfParticles << "\n";
    for (unsigned i = 0; i < numOfParticles; i++)
        outFile << "1 " << i << "\n";

    outFile << "CELL_TYPES " << numOfParticles << "\n"; 
    for (unsigned i = 0; i < numOfParticles; i++)
        outFile << "1" << "\n";

    outFile << "POINT_DATA " << numOfParticles << "\n";
    outFile << "VECTORS Stress float" << "\n";
    for (auto i : particles_)
        i -> write_stress(outFile);

}



void mpm::MpmParticle::write_particle_strain_data_to_file(std::ostream& outFile) {
    unsigned numOfParticles = particles_.size();

    outFile << "# vtk DataFile Version 2.0" << "\n";
    outFile << "MPM Particle Strai Data" << "\n";
    outFile << "ASCII" << "\n" << "DATASET UNSTRUCTURED_GRID" << "\n";
    outFile << "POINTS " << numOfParticles << " float" << "\n";

    VectorDDIM pCord;
    for (auto i : particles_) {
        pCord = i->give_coordinates();
        if (dim == 2)
            outFile << pCord(0) << " " << pCord(1) << " " << "0" << "\n";
        if (dim == 3)
            outFile << pCord(0) << " " << pCord(1) << " " << pCord(2) << "\n";
    }

    outFile << "CELLS " << numOfParticles << " " << 2*numOfParticles << "\n";
    for (unsigned i = 0; i < numOfParticles; i++)
        outFile << "1 " << i << "\n";

    outFile << "CELL_TYPES " << numOfParticles << "\n"; 
    for (unsigned i = 0; i < numOfParticles; i++)
        outFile << "1" << "\n";

    outFile << "POINT_DATA " << numOfParticles << "\n";
    outFile << "VECTORS Strain float" << "\n";
    for (auto i : particles_)
        i -> write_strain(outFile);

}
