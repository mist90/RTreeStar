/***************************************************************************
 *   MIT License
 * Copyright (c) 2022 Mikhail Tegin
 * michail3110@gmail.com
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.                  *
 ***************************************************************************/

#ifndef MATH_GEOM_OBJECTS
#define MATH_GEOM_OBJECTS
#include <stdint.h>
#include <cmath>
#include "MathVector3D.h"
#include "MathTriangle3D.h"
#include "MathGrapher.h"

namespace MathGrapherObjects {

/* Класс для визуализации точки в пространстве */
class Point: public MathGrapherObject
{
    MathVector3D<double>         _point;
public:
								Point(const MathVector3D<double>& point): _point(point) {}
    virtual size_t              getSurface(std::vector<double>& vertex, std::vector<double>& normals, const double& size) const;
    virtual GLenum              getTypeSurfaces() const
    {
        return GL_QUADS;
    }
    virtual MathGrapherRegion   getRegion() const
    {
		return MathGrapherRegion();
    }
};

/* Класс для визуализации линии в пространстве */
class Line:public MathGrapherObject
{
    MathVector3D<double>         points[2];
public:
								Line(const MathVector3D<double>& point1, const MathVector3D<double>& point2)
    {
        points[0] = point1;
        points[1] = point2;
    }
								Line(const MathFixedVector3D<double>& vector)
    {
        points[0] = vector.getBegin();
        points[1] = vector.getEnd();
    }

    virtual size_t              getSurface(std::vector<double>& vertex, std::vector<double>& normals, const double& size) const;
    virtual GLenum              getTypeSurfaces() const
    {
        return GL_QUADS;
    }
    virtual MathGrapherRegion   getRegion() const;
};

/* Базовый класс для хранения и визуализации множества линий */
class Polyline: public MathGrapherObject
{
	std::vector<Line>			lines;
public:
	virtual size_t              getSurface(std::vector<double>& vertex, std::vector<double>& normals, const double& size) const
	{
		size_t	ret = 0;

		for(auto it = lines.begin(); it != lines.end(); it++)
			ret += it->getSurface(vertex, normals, size);
		return ret;
	}
	virtual GLenum              getTypeSurfaces() const
	{
		return GL_QUADS;
	}
	virtual MathGrapherRegion   getRegion() const
	{
		MathGrapherRegion		ret;

		for(auto it = lines.begin(); it != lines.end(); it++)
			ret += it->getRegion();
		return ret;
	}
protected:
	void						addLine(const MathVector3D<double>& point1, const MathVector3D<double>& point2)
	{
		lines.push_back(Line(point1, point2));
	}
};

class Triangle
{
	const MathTriangle3D<double>	&_triangle;
public:

								Triangle(const MathTriangle3D<double>& triangle): _triangle(triangle) {}
	virtual size_t              getSurface(std::vector<double>& vertex, std::vector<double>& normals, const double& size) const;
	virtual GLenum              getTypeSurfaces() const
	{
		return GL_TRIANGLES;
	}
	virtual MathGrapherRegion   getRegion() const;
};

class Vector:public MathGrapherObject
{
    MathFixedVector3D<double>   _vector;
public:
								Vector(const MathFixedVector3D<double>& vector): _vector(vector) {}
    virtual size_t              getSurface(std::vector<double>& vertex, std::vector<double>& normals, const double& size) const;
    virtual GLenum              getTypeSurfaces() const
    {
        return GL_TRIANGLES;
    }
    virtual MathGrapherRegion   getRegion() const;
};

class Sphere:public MathGrapherObject
{
    MathVector3D<double>         _point;
    double _rad;
public:
								Sphere(const MathVector3D<double>& point, const double& rad): _point(point), _rad(rad) {}
    virtual size_t              getSurface(std::vector<double>& vertex, std::vector<double>& normals, const double& size) const;
    virtual GLenum              getTypeSurfaces() const
    {
        return GL_QUADS;
    }
    virtual MathGrapherRegion   getRegion() const;
};

class Tetraedr:public MathGrapherObject
{
    MathVector3D<double>         points[4];
public:
								Tetraedr(const MathVector3D<double>& point1, const MathVector3D<double>& point2, const MathVector3D<double>& point3, const MathVector3D<double>& point4)
    {
        points[0] = point1;
        points[1] = point2;
        points[2] = point3;
        points[3] = point4;
    }
    virtual size_t              getSurface(std::vector<double>& vertex, std::vector<double>& normals, const double& size) const;
    virtual GLenum              getTypeSurfaces() const
    {
        return GL_TRIANGLES;
    }
    virtual MathGrapherRegion   getRegion() const;
};

};



#endif
