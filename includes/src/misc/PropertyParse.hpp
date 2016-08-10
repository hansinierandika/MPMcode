/*************************************************************************
                        Material Point Method
                       Author: Shyamini Kularathna
                         University of Cambridge

NOTE:

FILE: PropertyParse.hpp
**************************************************************************/
#ifndef MPM_MISC_PROPERTYPARSE_H
#define MPM_MISC_PROPERTYPARSE_H
                                                                          
// c++ header files
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream> 
#include <map>

// boost header files
#include <boost/filesystem.hpp>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>

namespace mpm {
    namespace misc {
        void PARSE_PROPERTIES(std::string& iLine);

        bool gravity = 0;
        bool freeSurface = 0.;

        double dt;
        unsigned numOfTotalSteps;
        unsigned numOfSubSteps;
        std::map<std::string, double> propertyList;

        void READ_PROPERTIES(std::string& line);
    }
}

#include "PropertyParse.ipp"

#endif
