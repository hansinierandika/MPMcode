/***************************************************************************
                          Material Point Method
                           Shyamini Kularathna
                         University Of Cambridge
File: Element.hpp
****************************************************************************/
#ifndef MPM_ELEMENT_H
#define MPM_ELEMENT_H
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// c++ header files
#include <vector>
#include <fstream>
#include <iostream>

// boost header files
#include <boost/numeric/ublas/vector.hpp>

// eigen header files
#include <Eigen/Dense>

// mpm header files
#include "Constants.hpp"
#include "Node.hpp"


namespace mpm {
    class Element;
}


class mpm::Element {

protected:                    
    static const unsigned dim = mpm::constants::DIM;
    static const unsigned numNodes = mpm::constants::NUMNODES;

    typedef mpm::Node* NodePtr;

    typedef Eigen::Matrix<double, 1, dim>        VectorDDIM;
    typedef Eigen::Matrix<unsigned, 1, numNodes> VectorUNN;
    typedef Eigen::Matrix<NodePtr, 1, numNodes>  VectorNPtrNN;

public:

    // constructor
    Element(const unsigned& id);

    // compute element centre coordinates
    void compute_centre_coordinates();

    // initialise element
    void initialise_element();

    // set element nodes
    void set_element_nodes(const unsigned& i, const unsigned& nId, mpm::Node* nPtr);

    // set elements of element
    void set_element_elements(const unsigned& eId);


    // give element id
    unsigned give_id() const {
        return elemId_;
    }

    // give vector of element node pointers
    VectorNPtrNN give_element_node_ptrs() const {
        return elemNodesPtr_;
    }

    // give pointer of the node of a given index
    NodePtr give_element_node_ptr_at_index(unsigned index) {
        return elemNodesPtr_(index);
    }

    // give coordinates of the centre of the element
    VectorDDIM give_element_centre_coord() const {
        return elemCentreCoord_;
    }

    // give length of the element in each direction
    VectorDDIM give_element_length() const {
        return elemLength_;
    }

    // GIVE IDs OF ELEMENTS OF THE ELEMENTS
    std::vector<unsigned>& giveIDElementsOfElem() {
        return elemElemsId_;
    }

    // GIVE PARTICLE DENSITY IF THE ELEMENT
    unsigned giveParticleDensityOfElem() const {
        return elemParticleDensity_;
    }

    // ASSIGN PARTICLE DENSITY IN THE ELEMENT
    void addToElementParticleDensity() {
        elemParticleDensity_ += 1;
    }

    // SET THE ELEMENT NODES AS FREE
    void setElementNodesPressureConstraints(double& value) {
        for (unsigned i = 0; i < numNodes; i++)
            elemNodesPtr_(i) -> set_pressure_constraints(value);
    }

public:

    // ID
    unsigned elemId_;

    // ELEMENT NODES ID
    Eigen::Matrix<unsigned, 1, numNodes> elemNodesId_;
    // ELEMENT NODES POINTERS
    VectorNPtrNN elemNodesPtr_;
    // ELEMENT ELEMENTS ID
    std::vector<unsigned> elemElemsId_;

    // ELEMENT LENGTH
    VectorDDIM elemLength_;
    // ELEMENT CENTRE COORDINATES
    VectorDDIM elemCentreCoord_;

    // ELEMENT VOLUME
    double elemVolume_;
    // ELEMENT PARTICLE DENSITY
    unsigned elemParticleDensity_;
};

#include "Element.ipp"

#endif


