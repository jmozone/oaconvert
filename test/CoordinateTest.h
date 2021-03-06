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

#ifndef COORDINATETEST_H
#define COORDINATETEST_H

#include <cppunit/extensions/HelperMacros.h>

class CoordinateTest : public CppUnit::TestFixture
{

  CPPUNIT_TEST_SUITE( CoordinateTest );
  CPPUNIT_TEST( testGetDistance );
  CPPUNIT_TEST( testCoutOperator );
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();
  void testGetDistance();
  void testCoutOperator();

};

#endif // COORDINATETEST_H
