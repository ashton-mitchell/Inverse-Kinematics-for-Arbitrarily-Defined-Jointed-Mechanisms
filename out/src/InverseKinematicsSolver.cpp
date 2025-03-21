// InverseKinematicsSolver.cpp : This file will act as the entry point for the program. 
//                               It will manage all the files related to solving the Inverse Kinematics problem and visualizing it.

#include "../include/InverseKinematicsSolver.h"

Eigen::VectorXd optimizeInitialGuess(MechanismModel *m, Coord2D point)
{
    int numJoints = m->getJoints();
    std::vector<double> linkLengths = m->getLinks();

    Eigen::VectorXd initialGuess(numJoints);
    initialGuess.setZero(); // Default to 0 radians if no better guess found

    double x = point.getX();
    double y = point.getY();

    // Compute base angle based on desired point's quadrant
    double baseAngle = atan2(y, x); // Angle to the desired point

    // Compute total arm length
    double totalArmLength = 0;
    for (double len : linkLengths) totalArmLength += len;

    // Scale factor: how much each joint should bend
    double angleSpread = M_PI / (2.0 * numJoints); // Evenly spread angles

    // Adjust initial guess based on quadrant
    if (x > 0 && y > 0) { // Quadrant I
        initialGuess[0] = baseAngle / 2; // Spread rotation
        for (int i = 1; i < numJoints; i++)
            initialGuess[i] = angleSpread;
    }
    else if (x < 0 && y > 0) { // Quadrant II
        initialGuess[0] = baseAngle / 2;
        for (int i = 1; i < numJoints; i++)
            initialGuess[i] = angleSpread;
    }
    else if (x < 0 && y < 0) { // Quadrant III
        initialGuess[0] = baseAngle / 2;
        for (int i = 1; i < numJoints; i++)
            initialGuess[i] = -angleSpread;
    }
    else if (x > 0 && y < 0) { // Quadrant IV
        initialGuess[0] = baseAngle / 2;
        for (int i = 1; i < numJoints; i++)
            initialGuess[i] = -angleSpread;
    }
    else { // Special cases: (x, y) is exactly on an axis
        if (x == 0) { // Directly above or below origin
            initialGuess[0] = (y > 0) ? M_PI / 2 : -M_PI / 2;
        }
        else if (y == 0) { // Directly left or right
            initialGuess[0] = (x > 0) ? 0 : M_PI;
        }
    }

    return initialGuess;
}

int main() 
{

    // currently the program assumes that the mechanism is defined in a 2d plane and the first joint is located at the origin of this plane
    std::cout << "This program solves for joint angles that solve the inverse kinematics problem for a jointed mechanism.\n" << std::endl;

    // initialize mechanism and solver
    GUI gui = GUI();
    MechanismModel mechanism;
    MechanismModel* m = &mechanism;
    IterativeSolver solver;
    std::vector<Eigen::VectorXd> iterationResults;

    gui.RunGUI();
    GUI* g = &gui;

    // get and initialize mechanism parameters
    mechanism.initializeMechanism(g);
    int joints = m->getJoints();

    // get desired position
    //std::cout << "Now enter the desired position you want the mechanism to reach.\n" << std::endl;

    Coord2D desiredPoint = Coord2D::getValidInput(g);

    // define initial guess for newton method
    Eigen::VectorXd initialGuess(joints);

    initialGuess = optimizeInitialGuess(m, desiredPoint);

    if (mechanism.isOutOfReach(desiredPoint)) // check if the desired points is farther away than the max reach of the mechanism at full extension
    {
        std::cout << "No solution exists because the desired point is out of reach of the mechanism." << std::endl;
        return 0;
    }
    else 
    {
        iterationResults = solver.newtonSolve(m, initialGuess, desiredPoint, 1e-6, 1e-6); // run the solver to find the joint angles needed for the mechanism to reach the desired point
        
        if (iterationResults.size() > 999) return 0;

        gui.VisualizeSolution(iterationResults);
    }

    return 0;
}