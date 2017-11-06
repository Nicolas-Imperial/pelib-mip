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


#ifndef PELIB_CONSTRAINT
#define PELIB_CONSTRAINT

/** Yields an error if class B is not derived from class T. See http://www.stroustrup.com/bs_faq2.html#constraints **/
template<class T, class B> struct Derived_from {
	static void constraints(T* p) { B* pb __attribute__((unused)); pb = p;}
	Derived_from() { void(*p)(T*) __attribute__((unused)); p = constraints; }
};

/** Yields an error if and instance of T2 cannot be copied into an instance of T1. See http://www.stroustrup.com/bs_faq2.html#constraints **/
template<class T1, class T2> struct Can_copy {
	static void constraints(T1 a, T2 b) { T2 c = a; b = a; }
	Can_copy() { void(*p)(T1,T2) = constraints; }
};

/** Yields an error if an instance of T1 cannot be compared (equality, difference and inferiority) to an instance of T2 . See http://www.stroustrup.com/bs_faq2.html#constraints **/
template<class T1, class T2 = T1> struct Can_compare {
	static void constraints(T1 a, T2 b) { a==b; a!=b; a<b; }
	Can_compare() { void(*p)(T1,T2) = constraints; }
};

/** Yields an error if instances of classes T1 and T2 cannot be multiplied together or if T3 cannot hold the result of multiplication. See http://www.stroustrup.com/bs_faq2.html#constraints **/
template<class T1, class T2, class T3 = T1> struct Can_multiply {
	static void constraints(T1 a, T2 b, T3 c) { c = a*b; }
	Can_multiply() { void(*p)(T1,T2,T3) = constraints; }
};

#endif
