/*************************************************************************
                        Material Point Method
                       Author: Shyamini Kularathna
                         University of Cambridge
FILE: FileHandle.hpp
**************************************************************************/
#ifndef MPM_FILEHANDLE_H
#define MPM_FILEHANDLE_H 
                                                                         
// c++ header files
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream> 

// boost header files
#include <boost/filesystem.hpp>

// mpm miscellaneous header files
#include "Verify.hpp"
#include "PropertyParse.hpp"

// mpm main header files
#include "Mesh.hpp"
#include "MpmParticle.hpp"
#include "MaterialBase.hpp"
#include "ReadMaterial.hpp"

namespace mpm {
    class FileHandle;
}

class mpm::FileHandle {

protected:
    typedef boost::filesystem::path path_;

    typedef mpm::Mesh*          MeshPtr;
    typedef mpm::MpmParticle*   ParticleSetPtr;

public:
    // CONSTRUCTOR
    FileHandle(path_& p);

    // READ THE MESH
    MeshPtr read_mesh();

    // READ PARTICLES
    ParticleSetPtr read_particles();

    void WriteData(const unsigned& step, const ParticleSetPtr& particleSet); 

public:
    std::ifstream inputStream;
    std::ifstream nodeStream;
    std::ifstream elementStream;
    std::ifstream meshDataStream;
    std::ifstream particleStream;
    std::ifstream materialStream;
    std::ifstream iStressStream;
    std::ifstream velConstraintStream;
    std::ifstream fricConstraintStream;
    std::ifstream feedParticleStream;

    std::string ResultsDir;
    std::vector<mpm::material::MaterialBase*> materials;
};

#include "FileHandle.ipp"

#endif
