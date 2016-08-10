#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

int main () {

    std::cout << "\n \t \t \t 2-D MESH GENERATOR FOR \n";
    std::cout << "\t \t \t COLLAPSE OF WATER COLUMN \n \n";

    double xmin = 0.0, xmax = 0.0, xspacing = 0.0;
    double ymin = 0.0, ymax = 0.0, yspacing = 0.0;

    std::cout << "Enter x-min, x-max and x-spacing: ";
    std::cin >> xmin >> xmax >> xspacing;
    std::cout << "Enter y-min, y-max and y-spacing: ";
    std::cin >> ymin >> ymax >> yspacing;
    xmax += 0.000000001;
    ymax += 0.000000001;

    int xnumNodes = (xmax - xmin)/xspacing + 1;
    int ynumNodes = (ymax - ymin)/yspacing + 1;
    int xnumElem = (xmax - xmin) / (xspacing);
    int ynumElem = (ymax - ymin) / (yspacing);

    int totalNodes = xnumNodes * ynumNodes;
    int totalElem = xnumElem * ynumElem;
    std::cout << "Total number of Nodes: " << totalNodes << "\n";
    std::cout << "Total number of Elements: " << totalElem << "\n";

    std::ofstream  fnode, fcons, felem, fmesh;
    fnode.open("node.dat", std::ios::app);
    fcons.open("velCon.dat", std::ios::app);
    felem.open("element.dat", std::ios::app);
    fmesh.open("meshData.dat");

    double x, y;
    fnode << totalNodes << "\n";

    for (int j = 0; j < ynumNodes; j++) {
        for (int i = 0; i < xnumNodes; i++) {
            x = xmin + xspacing * i;
            y = ymin + yspacing * j;
            fnode << x << "\t" << y << "\t" << 0. << "\n";
        }
    }


// %%%%%%%%%%%%%%%% BEGIN ELEMENT NODES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    felem << totalElem << "\n";
    for (int n = 0; n < ynumElem; n++) {
            std::vector<int> grid1, grid2;
            for (int p = 0; p < xnumNodes; p++) {
                int G1 = n*xnumNodes + p;
                int G2 = (n+1)*xnumNodes + p;
                grid1.push_back(G1);
                grid2.push_back(G2);
            } 
        for (int m = 0; m < xnumElem; m++) {
            int node1 = grid1[m];
            int node2 = grid1[m+1];
            int node3 = grid2[m+1];
            int node4 = grid2[m];
           felem << node1 << "\t" << node2 << "\t" << node3 << "\t" << node4 << "\n";
        }
    }
// %%%%%%%%%% END ELEMENT NODES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


//%%%%%%%%%%%% BEGIN VELOCITY CONSTRAINT %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    int totalVelCons = (2*xnumNodes) + (2*ynumNodes);

    fcons << totalVelCons << "\t" << 0 << "\n";

// print constrained nodes status and values
    for (unsigned a = 0; a < xnumNodes; a++) {
        if (a == 0 || a == (xnumNodes -1))
            fcons << a << "\t" << 0 << "\t" << 0 << "\n";
        fcons << a << "\t" << 1 << "\t" << 0 << "\n";
    }
    for (unsigned b = 0; b < (ynumNodes-2); b++) {
        unsigned left = xnumNodes * (b + 1);
        unsigned right = left + (xnumNodes - 1);
        fcons << left << "\t" << 0 << "\t" << 0 << "\n";
        // fcons << left << "\t" << 1 << "\t" << 0 << "\n";
        fcons << right << "\t" << 0 << "\t" << 0 << "\n";
        // fcons << right << "\t" << 1 << "\t" << 0 << "\n";
    }
    unsigned top = xnumNodes * (ynumNodes - 1);
    for (unsigned a = 0; a < xnumNodes; a++) {
        if (a == 0 || a == (xnumNodes -1))
            fcons << (top+a) << "\t" << 0 << "\t" << 0 << "\n";
        fcons << (top+a) << "\t" << 1 << "\t" << 0 << "\n";
   }
//%%%%%%%%%%%%%%%%%% END VELOCITY CONSTRAINTS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
// %%%%%%%%%%%%%%%% BEGIN ELEMENT OF ELEMENTS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    for (unsigned i = 0; i < ynumElem; i++) {
        for (unsigned j = 0; j < xnumElem; j++) {
            unsigned elemId = (i*xnumElem) + j;
            if (i == 0) {
                if (j == 0) {
                    unsigned elem_1 = (elemId + 1);
                    unsigned elem_2 = (elemId + xnumElem + 1);
                    unsigned elem_3 = (elemId + xnumElem);
                    felem << 3 << "\t" << elem_1 << "\t" << elem_2 << "\t" << elem_3 << "\n";
                }
                else if (j == (xnumElem -1)) {
                    unsigned elem_1 = (elemId + xnumElem);
                    unsigned elem_2 = (elemId + xnumElem -1);
                    unsigned elem_3 = (elemId -1);
                    felem << 3 << "\t" << elem_1 << "\t" << elem_2 << "\t" << elem_3 << "\n";
                }
                else {
                    unsigned elem_1 = (elemId + 1);
                    unsigned elem_2 = (elemId + xnumElem + 1);
                    unsigned elem_3 = (elemId + xnumElem);
                    unsigned elem_4 = (elemId + xnumElem -1);
                    unsigned elem_5 = (elemId - 1);
                    felem << 5 << "\t" << elem_1 << "\t" << elem_2 << "\t" << elem_3 << "\t" << elem_4 << "\t" << elem_5 << "\n";
                }
            }
            else if ( i == (ynumElem -1)) {
                if (j == 0) {
                    unsigned elem_1 = (elemId - xnumElem);
                    unsigned elem_2 = (elemId - xnumElem + 1);
                    unsigned elem_3 = (elemId + 1);
                    felem << 3 << "\t" << elem_1 << "\t" << elem_2 << "\t" << elem_3 << "\n";
                }
                else if (j == (xnumElem -1)) {
                    unsigned elem_1 = (elemId - xnumElem -1);
                    unsigned elem_2 = (elemId - xnumElem);
                    unsigned elem_3 = (elemId - 1);
                    felem << 3 << "\t" << elem_1 << "\t" << elem_2 << "\t" << elem_3 << "\n";
                }
                else {
                    unsigned elem_1 = (elemId - xnumElem - 1);
                    unsigned elem_2 = (elemId - xnumElem);
                    unsigned elem_3 = (elemId - xnumElem + 1);
                    unsigned elem_4 = (elemId + 1);
                    unsigned elem_5 = (elemId - 1);
                    felem << 5 << "\t" << elem_1 << "\t" << elem_2 << "\t" << elem_3 << "\t" << elem_4 << "\t" << elem_5 << "\n";
                }
            }
            else {
                if (j == 0) {
                    unsigned elem_1 = (elemId - xnumElem);
                    unsigned elem_2 = (elemId - xnumElem + 1);
                    unsigned elem_3 = (elemId + 1);
                    unsigned elem_4 = (elemId + xnumElem +1);
                    unsigned elem_5 = (elemId + xnumElem);
                    felem << 5 << "\t" << elem_1 << "\t" << elem_2 << "\t" << elem_3 << "\t" << elem_4 << "\t" << elem_5 << "\n";
                }
                else if (j == (xnumElem -1)) {
                    unsigned elem_1 = (elemId - xnumElem - 1);
                    unsigned elem_2 = (elemId - xnumElem );
                    unsigned elem_3 = (elemId + xnumElem);
                    unsigned elem_4 = (elemId + xnumElem - 1);
                    unsigned elem_5 = (elemId - 1);
                    felem << 5 << "\t" << elem_1 << "\t" << elem_2 << "\t" << elem_3 << "\t" << elem_4 << "\t" << elem_5 << "\n";
                }
                else {
                    unsigned elem_1 = (elemId - xnumElem - 1);
                    unsigned elem_2 = (elemId - xnumElem);
                    unsigned elem_3 = (elemId - xnumElem + 1);
                    unsigned elem_4 = (elemId + 1);
                    unsigned elem_5 = (elemId + xnumElem + 1);
                    unsigned elem_6 = (elemId + xnumElem);
                    unsigned elem_7 = (elemId + xnumElem - 1);
                    unsigned elem_8 = (elemId - 1);
                    felem << 8 << "\t" << elem_1 << "\t" << elem_2 << "\t" << elem_3 << "\t" << elem_4 << "\t" << elem_5 << "\t" << elem_6 << "\t" << elem_7 << "\t" << elem_8 << "\n";
                }
            }
        }
    }
*/
// %%%%%%%%%%%%%%%% END ELEMENTS OF ELEMENTS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


// %%%%%%%%%%%%%%%% WRITE MESH DATA %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    fmesh << xspacing << "\t" << yspacing << "\n";
    fmesh << xnumElem << "\t" << ynumElem << "\n";
    fmesh << 0 << "\t" << (xnumElem - 1) << "\t" << xnumElem*(ynumElem-1) << "\t" << ((xnumElem*ynumElem)-1)  << "\n";
    fmesh << 0 << "\t" << (xnumNodes - 1) << "\t" << xnumNodes*(ynumNodes-1) << "\t" << (totalNodes -1) << "\n";

    fnode.close();
    fcons.close();
    felem.close();
    fmesh.close();
    return 0;

}
