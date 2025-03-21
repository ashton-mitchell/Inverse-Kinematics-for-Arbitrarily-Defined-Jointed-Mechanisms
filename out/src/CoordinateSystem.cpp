#include "../include/CoordinateSystem.h"

// constructor
Coord2D::Coord2D(double x, double y) : x(x), y(y) {}

// getters
double Coord2D::getX() const { return x; }
double Coord2D::getY() const { return y; }

// setters
void Coord2D::setX(double xVal) { x = xVal; }
void Coord2D::setY(double yVal) { y = yVal; }

// distance between two points
double Coord2D::distance(const Coord2D& other) const 
{
    return std::sqrt((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y));
}

// print coordinates
void Coord2D::print() const 
{
    std::cout << "2D Coordinate: (" << x << ", " << y << ")\n";
}

// static method to get valid input
Coord2D Coord2D::getValidInput(GUI *gui) 
{
    double x, y;
    std::array<float, 2> inputs = gui->getDesiredPosition();
    x = inputs[0];
    y = inputs[1];
    //while (true) 
    //{
    //    std::cout << "Enter desired x and y coordinates separated by a space: ";

    //    // read x and y coords
    //    std::cin >> x >> y;

    //    // check for valid input
    //    if (std::cin.fail()) 
    //    {
    //        std::cin.clear();
    //        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    //        std::cout << "Invalid input. Please enter numeric values.\n";
    //    }
    //    else 
    //    {
    //        // valid input
    //        break;
    //    }
    //}

    return Coord2D(x, y);
}
