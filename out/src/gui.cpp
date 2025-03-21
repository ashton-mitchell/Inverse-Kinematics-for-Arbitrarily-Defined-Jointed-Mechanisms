#include "../include/gui.h"


// OpenGL version
#define GLSL_VERSION "#version 130"

// GUI State Variables
bool showStartFrame = true;
//int jointCount = 1;
//std::vector<float> linkLengths;
//std::array<float, 2> desiredPosition = { 0.0f, 0.0f };
std::vector<float> jointAngles; // Initial angles for each joint

GUI::GUI() {
    this->jointCount = 1; // Default number of joints
    this->linkLengths = { 1.0f }; // At least one default link
    this->desiredPosition = { 0.0f, 0.0f }; // Default desired position
}

int GUI::getJoints() {
    return this->jointCount;
}

std::vector<float> GUI::getLinkLengths() {
    return this->linkLengths;
}

std::array<float, 2> GUI::getDesiredPosition() {
    return this->desiredPosition;
}

// Function to check OpenGL errors
void GUI::CheckOpenGLError(const std::string& label) {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL Error (" << label << "): " << err << std::endl;
    }
}

// Function to draw the X and Y axis
void GUI::DrawDottedAxis() {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0xF0F0); // More spaced dots

    glLineWidth(5.0f); // Thicker axis lines

    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 0.0f); // Black color for axes

    // X-Axis
    glVertex2f(-1.0f, 0.0f);
    glVertex2f(1.0f, 0.0f);

    // Y-Axis
    glVertex2f(0.0f, -1.0f);
    glVertex2f(0.0f, 1.0f);

    glEnd();

    glLineWidth(1.0f); // Reset to default
    glDisable(GL_LINE_STIPPLE);
    glFlush();
}

// Scale factor: Reduce size by a factor of 7
#define SCALE_FACTOR (1.0f / 7.0f)

// Function to draw the mechanism (now scaled correctly)
void GUI::DrawMechanism() {
    if (linkLengths.empty()) return;

    glColor3f(0.0f, 0.0f, 1.0f); // Blue for links
    glLineWidth(5.0f); // Thicker links

    float x = 0.0f, y = 0.0f;
    float angleSum = 0.0f;

    glBegin(GL_LINES); // Start drawing all links at once
    for (int i = 0; i < jointCount; i++) {
        angleSum += jointAngles[i]; // Accumulate joint angles

        // Apply the scaling factor to link lengths
        float newX = x + (linkLengths[i] * SCALE_FACTOR) * cos(angleSum);
        float newY = y + (linkLengths[i] * SCALE_FACTOR) * sin(angleSum);

        glVertex2f(x, y); // Start of the link
        glVertex2f(newX, newY); // End of the link

        x = newX;
        y = newY;
    }
    glEnd(); // Only end once after all links are drawn

    // Draw joints
    glColor3f(0.0f, 0.0f, 0.0f); // Black for joints
    glPointSize(15.0f); // Bigger joint dots

    glBegin(GL_POINTS);
    x = 0.0f;
    y = 0.0f;
    angleSum = 0.0f;

    glVertex2f(x, y); // Draw the base joint at origin

    for (int i = 0; i < jointCount; i++) {
        angleSum += jointAngles[i];
        x += (linkLengths[i] * SCALE_FACTOR) * cos(angleSum);
        y += (linkLengths[i] * SCALE_FACTOR) * sin(angleSum);

        glVertex2f(x, y); // Draw joint at the end of each link
    }
    glEnd(); // End joint drawing

    glFlush(); // Ensure rendering is pushed to GPU
}

// Function to draw the desired position (as a red dot)
void GUI::DrawDesiredPosition() {
    glColor3f(1.0f, 0.0f, 0.0f); // Red for the desired position
    glPointSize(15.0f); // Larger for visibility

    glBegin(GL_POINTS);
    glVertex2f(desiredPosition[0] * SCALE_FACTOR, desiredPosition[1] * SCALE_FACTOR);
    glEnd();
    glFlush();
}


void GUI::RunGUI() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return;
    }

    // Get primary monitor resolution
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    int screenWidth = mode->width;
    int screenHeight = mode->height;

    // Create a full-screen OpenGL window
    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "IK Solver GUI", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable VSync

    CheckOpenGLError("After OpenGL Initialization");

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsLight();
    io.FontGlobalScale = 2.0f;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(GLSL_VERSION);

    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    bool countdownStarted = false; // Track whether countdown has started

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // If the initial mechanism is shown, start the countdown
        if (!showStartFrame && !countdownStarted) {
            startTime = std::chrono::high_resolution_clock::now(); // Start countdown
            countdownStarted = true;
        }

        // If countdown started, check elapsed time
        if (countdownStarted) {
            auto currentTime = std::chrono::high_resolution_clock::now();
            double elapsedSeconds = std::chrono::duration<double>(currentTime - startTime).count();
            if (elapsedSeconds >= 5.0) { // Exit after 5 seconds
                break;
            }
        }

        // Update the screen size dynamically
        glfwGetWindowSize(window, &screenWidth, &screenHeight);

        // Clear OpenGL screen
        glViewport(0, 0, screenWidth, screenHeight);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (!showStartFrame) {
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            DrawDottedAxis();
            DrawMechanism();
            DrawDesiredPosition();
            CheckOpenGLError("After Drawing Mechanism");
        }

        // Start ImGui Frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (showStartFrame) {
            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImVec2((float)screenWidth, (float)screenHeight));

            ImGui::Begin("Setup Mechanism", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

            // Update class member instead of a local variable
            ImGui::Text("Enter Number of Joints:");
            ImGui::InputInt("##num_joints", &this->jointCount);
            if (this->jointCount < 1) this->jointCount = 1;

            // Ensure linkLengths and jointAngles match joint count
            if (this->linkLengths.size() != static_cast<size_t>(this->jointCount)) {
                this->linkLengths.resize(this->jointCount, 1.0f);
            }
            if (jointAngles.size() != static_cast<size_t>(this->jointCount)) {
                jointAngles.resize(this->jointCount, 0.5f);
            }

            // Input: Link Lengths
            for (int i = 0; i < this->jointCount; i++) {
                std::string label = "Link " + std::to_string(i + 1) + " Length";
                ImGui::InputFloat(label.c_str(), &this->linkLengths[i]); // Update class variable
            }

            // Desired Position Input (Update class member)
            ImGui::Text("Desired Position (X, Y):");
            ImGui::InputFloat2("##desired_position", this->desiredPosition.data());


            // Submit button
            if (ImGui::Button("Submit", ImVec2(200, 50))) {
                showStartFrame = false;
            }

            ImGui::End();
        }

        // Render ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap buffers (force screen update)
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}

void GUI::VisualizeSolution(const std::vector<Eigen::VectorXd>& solution)
{
    if (solution.empty()) {
        std::cerr << "Solver returned an empty solution. Cannot visualize." << std::endl;
        return;
    }

    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return;
    }

    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    int screenWidth = mode->width;
    int screenHeight = mode->height;

    // Open a GLFW window for visualization
    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "IK Solver Visualization", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window for solution visualization\n";
        return;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable VSync

    // Keep the same projection from initial visualization
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0); // Ensures same scaling
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Loop through each iteration's joint angles and visualize
    for (const auto& jointState : solution) {
        // Update joint angles with current iteration values
        for (size_t i = 0; i < jointAngles.size(); i++) {
            jointAngles[i] = jointState[i]; // Assign angles from solution
        }

        // Clear OpenGL screen
        glViewport(0, 0, screenWidth, screenHeight);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Preserve initial visualization scale & position
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // Draw updated visualization
        DrawDottedAxis();
        DrawMechanism();
        DrawDesiredPosition();

        // Swap buffers to display updated frame
        glfwSwapBuffers(window);
        glfwPollEvents();

        // Delay for 1 second between iterations
        std::this_thread::sleep_for(std::chrono::seconds(2));

        // If window is closed, stop visualization early
        if (glfwWindowShouldClose(window)) break;
    }

    // Cleanup
    glfwDestroyWindow(window);
}
