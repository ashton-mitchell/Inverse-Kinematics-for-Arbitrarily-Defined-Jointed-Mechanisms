#ifndef COORDINATESYSTEM_H
#define COORDINATESYSTEM_H

#include <cmath>
#include <iostream>
#include <limits>
#include "gui.h"

// the CoordinateSystem class exists to define a base coordinate system that allows for expansion of dimensions
class CoordinateSystem 
{
    public:
        virtual void print() const = 0; // pure virtual method (interface)
        virtual ~CoordinateSystem() = default;
};

// the Coord2D class inherits from CoordinateSystem; defines a 2 dimensional coordinate system
class Coord2D : public CoordinateSystem 
{
    private:
        double x, y;

    public:
        // constructor
        Coord2D(double x = 0.0, double y = 0.0);

        // getters
        double getX() const;
        double getY() const;

        // setters
        void setX(double xVal);
        void setY(double yVal);

        // distance between two points
        double distance(const Coord2D& other) const;

        // print coordinates
        void print() const override;

        // static method to get valid input
        static Coord2D getValidInput(GUI *gui);
};

#endif // COORDINATESYSTEM_H
