/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
F1. FUNCTION: CONSTRUCTOR
              This function reads material.dat file and creates materials
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
mpm::material::ReadMaterial::ReadMaterial(std::ifstream& matFile) {

    unsigned numMatTypes;
    std::string materialName;
    unsigned numParameters;
    std::string line;

    if (std::getline(matFile, line)) {
        std::istringstream in(line);
        in >> numMatTypes;
    }
    else {
        std::cerr << "ERROR: in reading material.dat file " << "\n";
        abort();
    }

    for (unsigned i = 0; i < numMatTypes; i++) {
        std::getline(matFile, line);
        std::istringstream inpMat(line);
        inpMat >> materialName >> numParameters;
        for (unsigned j = 0; j < numParameters; j++) {
            std::getline(matFile, line);
            mpm::misc::READ_PROPERTIES(line);
        }
        MaterialBasePtr material = this -> registerMaterial(materialName);
        materialPtrs_.push_back(material); 
    }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
F2. FUNCTION: REGISTER MATERIAL
              This function reads material.dat file and creates materials
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
typedef mpm::material::MaterialBase* MaterialBasePtr;
MaterialBasePtr mpm::material::ReadMaterial::registerMaterial(std::string& name) {

    MaterialBasePtr materialPtr = NULL;

    if (name == "Newtonian") { 
        materialPtr = mpm::material::Newtonian::create();
    }
    else if (name == "ModifiedBingham") {
        materialPtr = mpm::material::ModifiedBingham::create();
    }
    else if (name == "MohrCoulomb") {
        materialPtr = mpm::material::MohrCoulomb::create();
    }
    else if (name == "ILE") {
        materialPtr = mpm::material::ILE::create();
    }
    else {
        std::cerr << "ERROR: no material named " << name << "\n";
        abort();
    }

    return materialPtr;
}
