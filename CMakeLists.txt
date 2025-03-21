# CMakeList.txt : CMake project for InverseKinematicsSolver, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("InverseKinematicsSolver")

# set c++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Locate OpenGL
find_package(OpenGL REQUIRED)

# Add GLFW
set(GLFW_DIR ${CMAKE_SOURCE_DIR}/out/external/glfw)
add_subdirectory(${GLFW_DIR} ${CMAKE_BINARY_DIR}/glfw_build)
include_directories(${GLFW_DIR}/include)

# Include Dear ImGui
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/out/external/imgui)
include_directories(${IMGUI_DIR})
include_directories(${IMGUI_DIR}/backends)

set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

# Add source files
file(GLOB_RECURSE SRC_FILES src/*.cpp)
file(GLOB_RECURSE HEADER_FILES include/*.h)

# Add source to this project's executable.
add_executable (InverseKinematicsSolver ${IMGUI_SOURCES} "out/include/gui.h" "out/src/gui.cpp" "out/src/InverseKinematicsSolver.cpp" "out/include/InverseKinematicsSolver.h" "out/include/MechanismModel.h" "out/src/MechanismModel.cpp" "out/src/IterativeSolver.cpp" "out/include/IterativeSolver.h" "out/src/CoordinateSystem.cpp" "out/include/CoordinateSystem.h"  "out/src/IterativeSolver.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET InverseKinematicsSolver PROPERTY CXX_STANDARD 20)
endif()

# Include Eigen
target_include_directories(${PROJECT_NAME} PRIVATE out/external/eigen-3.4.0)

target_link_libraries(InverseKinematicsSolver glfw OpenGL::GL)
