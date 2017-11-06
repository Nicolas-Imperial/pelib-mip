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


pelib::Algebra
buildAlgebra()
{
pelib::Algebra new_algebra;
std::set<int> set_F;
set_F.insert(1);
set_F.insert(2);
set_F.insert(3);
set_F.insert(4);
set_F.insert(5);
pelib::Set<int> F("F", set_F);
new_algebra.insert(&F);
pelib::Scalar<int> p("p", 4);
new_algebra.insert(&p);
return new_algebra;
}
