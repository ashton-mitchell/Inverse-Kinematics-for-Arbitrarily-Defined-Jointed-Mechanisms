// InverseKinematicsSolver.h : Include file for standard system include files,
// or project specific include files.

#include "IterativeSolver.h"
#include "gui.h"
#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif

Eigen::VectorXd optimizeInitialGuess(MechanismModel* m, Coord2D point);