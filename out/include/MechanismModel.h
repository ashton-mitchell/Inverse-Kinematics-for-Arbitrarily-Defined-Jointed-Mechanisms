#ifndef MECHANISMMODEL_H
#define MECHANISMMODEL_H

#include <iostream>
#include <limits>
#include <vector>
#include "CoordinateSystem.h"

// this class defines the parameters for a mechanism
class MechanismModel 
{
    private:
	    int numJoints;
	    std::vector<double> linkLengths; // each length corresponds to link index + 1
    public:
	    // constructor
        MechanismModel();

        // getters
        int getJoints();
        std::vector<double> getLinks();

        void initializeMechanism(GUI *gui);             // gets the number of joints and link lengths from the user
        bool isOutOfReach(const Coord2D& point) const; // checks if the desired point is out of reach
        void printMechanismDetails() const;     // prints the mechanism details

        // helper functions
        static int getNumberOfJoints(GUI *gui);              // static function to get the number of joints
        static std::vector<double> getLinkLengths(GUI *gui, int numJoints); // static function to get link lengths
};

#endif