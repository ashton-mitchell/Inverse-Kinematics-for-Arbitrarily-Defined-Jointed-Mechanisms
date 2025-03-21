#include "../include/IterativeSolver.h"

// constructor
IterativeSolver::IterativeSolver() {}

// function that creates the Homogeneous Transformation matrix that represents the forward kinematics of the mechanism


// function that caluclates the error between the desired and actual position of the end-effector in the form of a vector
Eigen::Vector2d IterativeSolver::error(Eigen::Vector2d desiredPosition, Eigen::Vector2d actualPosition)
{	
	return desiredPosition - actualPosition;
}

// function that dynamically calculates the position of the mechanism in the 2d plane based on provided vector of joint angles
Eigen::Vector2d IterativeSolver::endEffectorPosition(MechanismModel *m, Eigen::VectorXd jointAngles)
{
	int joints = m->getJoints();
	std::vector<double> links = m->getLinks(); // get parameters

	Eigen::Vector2d endEffector;

	double x = 0, y = 0, theta = 0;

	for (int i = 0; i < joints; i++) // iteratively construct the position vector for the specifications of the mechanism
	{
		theta += jointAngles[i];
		x += links[i] * std::cos(theta);
		y += links[i] * std::sin(theta);
	}

	endEffector << x, y;

	return endEffector;
}

// function that dynamically calculates the jacobian matrix needed for the iterative step of newton's method
// many assumptions are being made for this function to work
Eigen::MatrixXd IterativeSolver::computeJacobian(MechanismModel *m, Eigen::VectorXd jointAngles)
{
	int joints = m->getJoints();
	std::vector<double> links = m->getLinks(); // get parameters

	Eigen::MatrixXd J(2, joints);
	std::vector<std::pair<double, double>> jacobianColumns(joints, { 0.0, 0.0 });
	double theta = 0, col = 0;

	// for each column in the desired jacobian matrix
	for (std::pair<double, double> &c : jacobianColumns)
	{
		for (int i = 0; i < joints; i++) // for each joint in the mechanism
		{
			theta += jointAngles[i];
			if (i < col) // condition for checking what to add to the elements
			{
				c.first += 0;
				c.second += 0;
			}
			else
			{
				c.first += -1 * links[i] * std::sin(theta); // iteratively construct elements of the Jacobian
				c.second += links[i] * std::cos(theta);
			}
		}
		col++;
	}	

	/*x1 = -1 * links[0] * std::sin(jointAngles[0]) - links[1] * std::sin(jointAngles[0] + jointAngles[1]);
	x2 = -1 * links[1] * std::sin(jointAngles[0] + jointAngles[1]);
	y1 = links[0] * std::cos(jointAngles[0]) + links[1] * std::cos(jointAngles[0] + jointAngles[1]);
	y2 = links[1] * std::cos(jointAngles[0] + jointAngles[1]);*/

	// populate x partial derivatives
	for (int i = 0; i < size(jacobianColumns); i++)
	{
		J(0, i) = jacobianColumns[i].first;
	}

	// populate y partial derivatives
	for (int i = 0; i < size(jacobianColumns); i++)
	{
		J(1, i) = jacobianColumns[i].second;
	}

	return J;
}

// function that performs newton's method on the mechanism to solve for the joint angles necessary to acheive the desired end-effector position
std::vector<Eigen::VectorXd> IterativeSolver::newtonSolve(MechanismModel *m, Eigen::VectorXd initialGuess, Coord2D desiredPosition, double tolerance, double deltaTolerance)
{
	Eigen::Vector2d desired(desiredPosition.getX(), desiredPosition.getY());
	Eigen::Vector2d actual = endEffectorPosition(m, initialGuess); // get desired and actual position vectors

	Eigen::VectorXd iterator = initialGuess; // initialize the iterator vector
	std::vector<Eigen::VectorXd> iterationHistory;
	int iter = 1;

	while (true) // loop until convergence
	{
		Eigen::Vector2d e = error(desired, actual); // calculate error between desired and actual position

		iterationHistory.push_back(iterator); //iterator

		std::cout << e.norm() << "\n";

		if (e.norm() < tolerance) // if convergence
		{
			std::cout << "Converged after " << iter << " iterations (error norm).\n"; // plot error versus iterations
			std::cout << "Desired position was " << desired[0] << ", " << desired[1] << ".\n"; 
			std::cout << "Actual position was " << actual[0] << ", " << actual[1] << ".\n";
			std::cout << "The angles calculated to acheive the desired position are ";
			
			for (int i = 0; i < iterator.size(); i++)
			{
				std::cout << iterator[i] << ", ";
			}
			std::cout << ".\n";
			break;
		}

		Eigen::MatrixXd J = computeJacobian(m, iterator); // calculate the jacobian

		// how much the newton step is incremented by; does not calculate inverse explicityly to avoid O(n^3) time
		Eigen::VectorXd increment = J.colPivHouseholderQr().solve(e); //time versus iterations compared to takng the inverse

		iterator += increment; // iterative newton step
		actual = endEffectorPosition(m, iterator); // update actual position based on new angles
		iter++;

		if (iter > 1000)
		{
			std::cout << "Convergence unstable, aborting...\n";
			break;
		}
	}

	return iterationHistory;
}


/*
Goals to have completed by end of january

make program work for all mechanisms rather than 2-jointed mechanisms *
design GUI
implement GUI with solver

*/