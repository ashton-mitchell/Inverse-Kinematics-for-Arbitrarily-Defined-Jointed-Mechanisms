#ifndef GUI_H
#define GUI_H

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <array>
#include <iostream>
#include <cmath>
#include <chrono>
#include <Eigen/Dense>
#include <thread>

class GUI {
	public:
		GUI();
		void RunGUI();
		void VisualizeSolution(const std::vector<Eigen::VectorXd>& solution);
		void CheckOpenGLError(const std::string& label);
		void DrawDottedAxis();
		void DrawMechanism();
		void DrawDesiredPosition();
		int getJoints();
		std::vector<float> getLinkLengths();
		std::array<float, 2> getDesiredPosition();
		int jointCount;
		std::vector<float> linkLengths;
		std::array<float, 2> desiredPosition = { 0 };
};




#endif