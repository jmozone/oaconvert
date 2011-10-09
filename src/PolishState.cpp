/*
  Copyright 2011 Bart Vandewoestyne, Yves Frederix.
  
  This file is part of oaconvert.
  
  oaconvert is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  oaconvert is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with oaconvert.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "PolishState.h"

#include <cassert>

#include "Airspace.h"
#include "Circle.h"
#include "Constants.h"
#include "CurvedPolygon.h"

using namespace std;
using namespace Constants;

PolishState* PolishState::_instance = 0;

PolishState::PolishState()
  {
  }

PolishState* PolishState::getInstance()
  {
  if (_instance == 0)
    {
    _instance = new PolishState;
    }
  return _instance;
  }

void PolishState::writeHeader(std::ostream &out) const
  {
  out << "; Generated by oaconvert (OpenAir to Polish Format converter)\n";

  // Section identifier (mandatory).
  out << "[IMG ID]" << endl;

  // Unique identifier (up to 8 decimal digits) for the map (mandatory).  May
  // be only written in a decimal format.  I have found the following values:
  //   cgpsmapper manual: 11000204
  //   oa2gm:             42200100
  //   gscripts:          42099999
  // Setting this one to my birthday :-)
  out << "ID=19780321" << endl;

  // Map name to be displayed in the GPS receiver's Map Info menu (mandatory).
  out << "Name=My Very Special Map" << endl;

  // Instead of single byte coding, use full-byte (8-bit) character coding
  // with the specified codepage (depending on the GPS firmware) (optional).
  // I have found the following values:
  //   cgpsmapper manual: default 0
  //   oa2gm:             1250 (probably because the author needed it (see for example
  //                            http://en.wikipedia.org/wiki/Windows-1250)
  //   gscripts:          none specified, so default 0
  out << "Codepage=0" << endl;

  // Only index objects if POI index info is explicitly provided.  With
  // 'indexing', we mean 'make searcheable by the Find function in the GPS'.
  // We follow the default value  of 'N' here.
  out << "POIIndex=N" << endl;

  // Maximum allowed region size.  A higher value increases the allowable
  // region size, but may decrease the map performance; a lower value may
  // increase the map size.
  // I have found the following values:
  // cgpsmapper manual:
  //   topo maps: 1000-2000
  //   city (dense streets): 2000-5000
  //   countryside: 6000-10000
  // oa2gm: 1311
  // gscripts: 6000
  out << "TreSize=1311" << endl;

  // Maximal number of elements in one region (mandatory).
  // Can be any value between ~50 and 1024 (values less than 50 don't make
  // sense).  Recent experiments show that this parameter does not impact
  // map performance and can be set to maximum allowed value: 1024.
  // I have found the following values:
  // cgpsmapper suggested value: 1024
  // oa2gm: none specified
  // gscripts: 1024
  out << "RgnLimit=1024" << endl;

  // Kind of pre-processing.  We're setting it to the default here...
  // cgpsmapper default: F
  // oa2gm:              F
  // gscripts:           F
  out << "PreProcess=F" << endl;


  // Level and Zoom level stuff... this is tricky and we need to find good
  // settings here!
  //
  // Some explanatory examples:
  //
  // 'Levels=': Number of map zoom levels (layers) in the map (mandatory, at least 2,
  //            not more than 10, numbered starting at 0).
  // 
  // 'Level0=24': Map zoom level 0 corresponds to hardware zoom level 24 ('Up to 120m')
  //              This means that map objects and coordinates defined as map level 0, will
  //              be used at hardware zoom levels 24 and above.  So they will be visible
  //              if the scale is 120m or more detailed.
  //              The last level is a special one that dictates when our map replaces the
  //              base map.  It means the following: from hardware level
  //              14 ('80km to 120 km') or higher, we will see our map.  At hardware
  //              zoom levels 13 and below, we will see the base map.
  //              We are not allowed to define map objects and coordinates at this level.
  // 'ZoomX=XX': ??
  //
  // Note 1: the last layer must always be empty, e.g. Levels=3
  //         means that only two layers are available for map objects.
  // Note 2: GPS unit map detail must be set to 'Normal'!!!!!
  //         See section 4.4 (on page 40) of cgpsmapper manual (mandatory).
  // Note 3: Hardware Zoom Level 12 represents the most ''zoomed out'' setting for
  //         current Garmin devices, so it makes no sense to go lower.

  // This is what oa2gm and gscripts use, but it didn't work with GPSMapEdit 1.0.70.0.
  //out << "Levels=5\n";
  //out << "Level0=21\n";
  //out << "Level1=19\n";
  //out << "Level2=17\n";
  //out << "Level3=15\n";
  //out << "Level4=14\n";
  //out << "Zoom0=0\n";
  //out << "Zoom1=1\n";
  //out << "Zoom2=2\n";
  //out << "Zoom3=3\n";
  //out << "Zoom4=4\n";

  // This is from http://vjet.f2s.com/gmap/cgpsmapper.html
  // With these, my [POLYLINE]s are visible on my 60CSX if I zoom to 3km or below.
  //out << "Levels=5\n";
  //out << "Level0=24\n";
  //out << "Level1=22\n";
  //out << "Level2=20\n";
  //out << "Level3=19\n";
  //out << "Level4=18\n";
  //out << "Zoom0=0\n";
  //out << "Zoom1=1\n";
  //out << "Zoom2=2\n";
  //out << "Zoom3=3\n";
  //out << "Zoom4=4\n";

  // This is my attempt, trying to get it right for the 60CSX.
  // Order 24/22/20/18/16 -> visible when zoomed in 8 km or finer.
  // Order 24/21/18/15/12 -> visible when zoomed in 50km or finer.
  out << "Levels=5\n";
  out << "Level0=24\n";
  out << "Level1=21\n";
  out << "Level2=18\n";
  out << "Level3=15\n";
  out << "Level4=12\n";
  out << "Zoom0=0\n";
  out << "Zoom1=1\n";
  out << "Zoom2=2\n";
  out << "Zoom3=3\n";
  out << "Zoom4=4\n";

  // Below is what GPSMapEdit 1.0.69.1 outputs, and it seems to work in
  // GPSMapEdit...  Note that this might not work as fluently
  // on actual GPS units like the Garmin 60CSX.
  //out << "Levels=2\n";
  //out << "Level0=24\n";
  //out << "Level1=14\n";
  //out << "Zoom0=0\n";
  //out << "Zoom1=1\n";

  // Section terminator (mandatory)
  out << "[END-IMG ID]\n" << endl;
}


void PolishState::writeFooter(std::ostream &out) const
  {
  // Polish files have no footer.
  }


// void PolishState::write(ostream& out, const Airspace& s) const
// {
//   /* TODO
//   if ( s.hasPolygon() )
//   {
//     //cout << "DEBUG: Airspace has a polygon" << endl;
//     write(out, s.getPolygon(), s.getName());
//   }
//   if ( s.hasCircle() )
//   {
//     //cout << "DEBUG: Airspace has a circle" << endl;
//     write(out, s.getCircle().toPolygon(100), s.getName());;
//   }
//   */
// }

void PolishState::write(std::ostream& stream, const Airspace& airspace) const
  {
  if ( ! airspace.getRegion() )
    {
    return;
    }

  // See section 4.2.4.2 in http://cgpsmapper.com/download/cGPSmapper-UsrMan-v02.1.pdf

  // oa2gm used [RGN40] here, which is a [POLYLINE]... I think one could also
  // use [RGN80] here, which is a [POLYGON].
  stream << "[POLYLINE]" << endl;

  // Type of [POLYGON] element: 'Airport'
  // We should probably change this to something which is more appropriate
  // for each earspace separately.
  // See section 12.3.3 in the cgpsmapper manual.
  // Note that oa2gm used Type=0x0a which is an 'Unpaved Road-thin', but this
  // is for [POLYLINE], not [POLYGON]...
  stream << "Type=" << getType(airspace.getClass()) << endl;

  stream << "Label=" << airspace.getName() << endl;
  stream << "EndLevel=4" << endl; // Number must not be higher than highest X in LevelX in header.
  OutputState::write(stream, airspace.getRegion());
  stream << "[END]\n" << endl;
  }


void PolishState::write(std::ostream& stream, const Circle* circle) const
  {
  std::vector<Coordinate> coords;
  circle->discretize( coords, RESOLUTION );
  write( stream, coords );
//    write(s.getCircle().toPolygon(NBPOINTS), s.getName());;
//  assert( ! "TODO" );
  }


void PolishState::write(std::ostream& stream, const CurvedPolygon* curved_polygon) const
  {
  std::vector<Coordinate> coords;
  curved_polygon->discretize( coords, RESOLUTION );
  write( stream, coords );
//  assert( ! "TODO" );
  }


void PolishState::write(ostream& out, const Coordinate& c) const
{
  out << "(" << c.getLatitude().getAngle() << "," << c.getLongitude().getAngle() << ")";
}


void PolishState::write(std::ostream& out, const std::vector<Coordinate>& coords) const
{
  if (coords.size() > 0)
  {
    out << "Data0=";
    for (size_t i = 0; i < coords.size()-1; ++i)
    {
      write(out, coords[i]);
      out << ",";
    }
    write(out, coords.back());
    out << endl;
  }
}

/*
 * Return the Polish File [POLYGON] type for the given airspace class.
 * For the listing of the different possible types, see cgpsmapper manual,
 * table 9.3.3 page 89.
 *
 * Note: The Python script at
 *
 *   http://www.penguin.cz/~ondrap/paragliding.php
 *
 * sets the polygon type according to the following rules:
 *
 *      if AH - AL < 500 and AL < 2000:
 *          type = '78'
 *      elif AL == 0:
 *          type = '0x08'
 *      elif AL < 500:
 *          type = '0x1e'
 *      elif AL < 1000:
 *          type = '0x3c'
 *      elif AL < 1900:
 *          type = '0x18'
 *      else:
 *          type = '0x53' 
 * 
 */
std::string PolishState::getType(const std::string& airspaceClass) const
{
  if (airspaceClass == "R")
  {
    // Restricted area.
    return string("0x0702");
    // Restriction area/line (invisible)
    //return string("0x0500");
  }
  else if (airspaceClass == "Q")
  {
    // Danger line (invisible).
    return string("0x0409");
  }
  else if (airspaceClass == "P")
  {
    // Prohibited area (invisible).
    return string("0x0503");
  }
  else if (airspaceClass == "CTR")
  {
    // Airport.
    return string("0x07");
  }
  else
  {
    // Defaults used by oa2gm:

    // University.
    return string("0x0a");
    // Golf.
    //return string("0x18");
  }
}
