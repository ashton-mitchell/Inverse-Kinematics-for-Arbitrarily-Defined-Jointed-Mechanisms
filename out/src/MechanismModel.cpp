#include "../include/MechanismModel.h"

// constructor
MechanismModel::MechanismModel() : numJoints(0), linkLengths({}) {}

// initialize mechanism
void MechanismModel::initializeMechanism(GUI *gui) 
{
    numJoints = getNumberOfJoints(gui);
    linkLengths = getLinkLengths(gui, numJoints);
}

// returns the number of joints
int MechanismModel::getJoints()
{
    return numJoints;
}

// return a list of link lengths
std::vector<double> MechanismModel::getLinks()
{
    return linkLengths;
}

// get number of joints from the user 
int MechanismModel::getNumberOfJoints(GUI *gui) 
{
    int numJoints = gui->getJoints();

    //while (true) // prompt user continuously until a valid number of joints is provided
    //{
    //    std::cout << "Enter the number of joints in the mechanism:\n";
    //    std::cout << "Number of joints: ";
    //    std::cin >> numJoints;

    //    if (std::cin.fail() || numJoints < 1) 
    //    {
    //        std::cin.clear();
    //        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    //        std::cout << "Invalid input. Please enter a number greater than or equal to 1.\n";
    //    }
    //    else 
    //    {
    //        break;
    //    }
    //}
    return numJoints;
}

// get link lengths from the user
std::vector<double> MechanismModel::getLinkLengths(GUI *gui, int numJoints) 
{
    std::vector<float> extracted = gui->getLinkLengths();
    std::vector<double> lengths(extracted.begin(), extracted.end());

    //double length;

    //std::cout << "Enter the lengths for " << numJoints << " links:\n";
    //for (int i = 0; i < numJoints; i++) // loop for each link in the mechanism defined by the number of joints
    //{
    //    while (true) // promt user continuously until a valid length is provided
    //    {
    //        std::cout << "Length of link " << i + 1 << ": ";
    //        std::cin >> length;

    //        if (std::cin.fail() || length <= 0.0) 
    //        {
    //            std::cin.clear();
    //            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    //            std::cout << "Invalid input. Please enter a positive number.\n";
    //        }
    //        else 
    //        {
    //            lengths.push_back(length);
    //            break;
    //        }
    //    }
    //}
    return lengths;
}

// check if a point is out of reach
bool MechanismModel::isOutOfReach(const Coord2D& point) const 
{
    double maxReach = 0.0;

    for (double length : linkLengths) // calculate max reach of the mechanism at full extension
    {
        maxReach += length;
    }

    Coord2D origin(0.0, 0.0);
    return origin.distance(point) > maxReach;
}

// print mechanism details
void MechanismModel::printMechanismDetails() const 
{
    std::cout << "Mechanism Details:\n";
    std::cout << "Number of Joints: " << numJoints << "\n";
    std::cout << "Link Lengths: ";
    for (double length : linkLengths) 
    {
        std::cout << length << " ";
    }
    std::cout << "\n";
}
