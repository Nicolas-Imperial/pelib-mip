/*
 Copyright 2015 Nicolas Melot

 This file is part of Pelib.

 Pelib is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Pelib is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Pelib. If not, see <http://www.gnu.org/licenses/>.
*/


#include <pelib/DeadlineCalculator.hpp>
#include <pelib/Algebra.hpp>

#ifndef DEADLINE_CONSTANT
#define DEADLINE_CONSTANT

namespace pelib
{
  /** Holds a contant floating-point value and returns it upon deadline computing request **/
  class DeadlineConstant : public DeadlineCalculator
  {
  public:
    /** Constructor: holds the floating-point value given as parameter to return it upon invocation of calculate() **/
    DeadlineConstant(double val);

    /** Returns the private member value, ignoring instances of Taskgraph and Platform **/
    virtual double calculate(const Taskgraph& tg, const Platform& arch) const;
 
  private:
    /** Holds the value given when the class instance was constructed and returned upon calls to calculate() **/
    const double value;
  };
}



#endif
