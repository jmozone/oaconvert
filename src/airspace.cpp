#include <iostream>
#include <string>
#include "airspace.h"
#include "parser.h"
#include "stringutils.h"
using namespace std;

AirSpace::AirSpace()
{}

const string& AirSpace::getCeiling() const
{
  return ceiling;
}

void AirSpace::setCeiling(const string& ceiling)
{
  this->ceiling = ceiling;
}

const string& AirSpace::getFloor() const
{
  return floor;
}

void AirSpace::setFloor(const string& floor)
{
  this->floor = floor;
}

/**
 * Return the Polygon that contains all Coordinates defining this airspace.
 */
const Polygon& AirSpace::getPolygon() const
{
  return polygon;
}

/**
 * Return true if the airspace is defined by a Polygon (some airspaces are
 * only defined by a circle and don't have Polygon points).
 */
bool AirSpace::hasPolygon() const
{
  if (polygon.getNbPoints() > 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

/**
 * Return true if the airspace is defined by a Circle (and airspace can be
 * defined by a circle or a polygon).
 */
bool AirSpace::hasCircle() const
{
  if ( circle.isValid() )
  {
    //cout << "DEBUG: airspace has a circle" << endl;
    return true;
  }
  else
  {
    //cout << "DEBUG: airspace doesn't have a circle" << endl;
    return false;
  }
}

const Circle& AirSpace::getCircle() const
{
  return circle;
}

Circle& AirSpace::getCircle()
{
  return circle;
}

const Arc& AirSpace::getArc() const
{
  return arc;
}

Arc& AirSpace::getArc()
{
  return arc;
}

void AirSpace::setName(const string& mystring)
{
  name = mystring;
}

const string& AirSpace::getName() const
{
  return name;
}

void AirSpace::setClass(const string& clss)
{
  this->clss = clss;
}

const string& AirSpace::getClass() const
{
  return clss;
}

/**
 * Each airspace can have multiple name-labels that each have their location
 * on a map.  With this method, you can add locations for these labels.
 */
void AirSpace::addLabelCoordinate(const Coordinate& c)
{
  labelCoordinates.push_back(c);
}

ostream& operator <<(ostream& outputStream, const AirSpace& s)
{
  outputStream << "Name:    " << s.name << endl;
  outputStream << "Class:   " << s.clss << endl;
  outputStream << "Ceiling: " << s.ceiling << endl;
  outputStream << "Floor:   " << s.floor << endl;
  if (s.labelCoordinates.size() > 0)
  {
    outputStream << "Label coordinates:" << endl;
    for (unsigned int i=0; i<s.labelCoordinates.size(); ++i)
    {
      outputStream << "  " << s.labelCoordinates[i] << endl;
    }
  }
  outputStream << s.polygon << endl;
  outputStream << s.circle << endl;
  //s.circle.toPolish( outputStream );
  return outputStream;
}

void AirSpace::clear()
{
    name = "";
    clss = "";
    ceiling = "";
    floor = "";
    polygon.clear();
    circle.invalidate();
    arc.invalidate();
    labelCoordinates.clear();
}
