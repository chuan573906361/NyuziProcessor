// 
// Copyright (C) 2011-2014 Jeff Bush
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
// 
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the
// Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
// Boston, MA  02110-1301, USA.
// 

#pragma once

#include "SIMDMath.h"

namespace librender
{

//
// 2D linear interpolator. Given the value of a parameter at 3 points in a 2D space, 
// determine the value at any other arbitrary point.
//
class LinearInterpolator 
{
public:
	void init(float x0, float y0, float c0, float x1, 
		float y1, float c1, float x2, float y2, float c2)
	{
		float a = x1 - x0;
		float b = y1 - y0;
		float c = x2 - x0;
		float d = y2 - y0;
		float e = c1 - c0;
		float f = c2 - c0;

		// Determine partial derivatives using Cramer's rule
		float detA = a * d - b * c;
		fGx = (e * d - b * f) / detA;
		fGy = (a * f - e * c) / detA;
		fC00 = c0 + -x0 * fGx + -y0 * fGy;	// Compute c at 0, 0
	}
	
	// Return values of this parameter at 16 locations given by the vectors
	// x and y.
	inline vecf16_t getValuesAt(vecf16_t x, vecf16_t y) const
	{
		return x * splatf(fGx) + y * splatf(fGy) + splatf(fC00);
	}

private:
	float fGx;	// @C/@X
	float fGy;	// @C/@Y
	float fC00;	// Value of C at 0, 0
};

}
