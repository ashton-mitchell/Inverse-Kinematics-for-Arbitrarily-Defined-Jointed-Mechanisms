#ifndef ITERATIVESOLVER_H
#define ITERATIVESOLVER_H

#include <Eigen/Dense>
#include "MechanismModel.h"

// this class defines the functions and parameters needed to implement newton's method
class IterativeSolver
{
	private:
		int id;
	public:
		IterativeSolver();
		Eigen::Matrix4d constructForwardMatrix(MechanismModel* m);
		Eigen::Vector2d error(Eigen::Vector2d desiredPosition, Eigen::Vector2d actualPosition);
		Eigen::Vector2d endEffectorPosition(MechanismModel* m, Eigen::VectorXd jointAngles);
		Eigen::MatrixXd computeJacobian(MechanismModel *m, Eigen::VectorXd jointAngles);
		std::vector<Eigen::VectorXd> newtonSolve(MechanismModel *m, Eigen::VectorXd initialGuess, Coord2D desiredPosition, double tolerance, double deltaTolerance);
};

#endif // ITERATIVESOLVER_H