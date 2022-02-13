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

#include "MathGrapherObject.h"
#include "GLFunctions.h"
#include <cfloat>

namespace MathGrapherObjects {

size_t Point::getSurface(std::vector<double> &vertex, std::vector<double> &normals, const double& size) const
{
    size_t oldSize = vertex.size()/3;

    GetSphereVertex(size*3.0, _point, vertex, normals);
    return vertex.size()/3 - oldSize;
}

size_t Line::getSurface(std::vector<double> &vertex, std::vector<double> &normals, const double& size) const
{
	MathVector3D<double>        point, normal;
    MathFixedVector3D<double>   vectorBegin, vectorEnd;
    double                      rad = size;

    vectorBegin = MathFixedVector3D<double>(points[0], points[1]);
    vectorEnd = vectorBegin;
    vectorEnd.setBegin(points[1]);

    for(uint8_t i=0; i<10; i++)
    {
		point = GetPointFromCircle(rad, vectorBegin, 36.0*i);
		vertex.push_back(point.getX());
		vertex.push_back(point.getY());
		vertex.push_back(point.getZ());
		normal = (point - vectorBegin.getBegin()).unitVector();
		normals.push_back(normal.getX());
		normals.push_back(normal.getY());
		normals.push_back(normal.getZ());

		point = GetPointFromCircle(rad, vectorBegin, 36.0*(i + 1));
		vertex.push_back(point.getX());
		vertex.push_back(point.getY());
		vertex.push_back(point.getZ());
		normal = (point - vectorBegin.getBegin()).unitVector();
		normals.push_back(normal.getX());
		normals.push_back(normal.getY());
		normals.push_back(normal.getZ());

		point = GetPointFromCircle(rad, vectorEnd, 36.0*(i + 1));
		vertex.push_back(point.getX());
		vertex.push_back(point.getY());
		vertex.push_back(point.getZ());
		normal = (point - vectorEnd.getBegin()).unitVector();
		normals.push_back(normal.getX());
		normals.push_back(normal.getY());
		normals.push_back(normal.getZ());

		point = GetPointFromCircle(rad, vectorEnd, 36.0*i);
		vertex.push_back(point.getX());
		vertex.push_back(point.getY());
		vertex.push_back(point.getZ());
		normal = (point - vectorEnd.getBegin()).unitVector();
		normals.push_back(normal.getX());
		normals.push_back(normal.getY());
		normals.push_back(normal.getZ());
    }
    return 4*10;
}

MathGrapherRegion Line::getRegion() const
{
	MathVector3D<double> begin;
	MathVector3D<double> end;

	begin.setX(points[1].getX() > points[0].getX()? points[0].getX() : points[1].getX());
	begin.setY(points[1].getY() > points[0].getY()? points[0].getY() : points[1].getY());
	begin.setZ(points[1].getZ() > points[0].getZ()? points[0].getZ() : points[1].getZ());

	end.setX(points[1].getX() > points[0].getX()? points[1].getX() : points[0].getX());
	end.setY(points[1].getY() > points[0].getY()? points[1].getY() : points[0].getY());
	end.setZ(points[1].getZ() > points[0].getZ()? points[1].getZ() : points[0].getZ());

	return MathGrapherRegion(begin, end);

}

size_t Triangle::getSurface(std::vector<double> &vertex, std::vector<double> &normals, const double &size) const
{
	(void)size;
	for (uint8_t i = 0; i < 3; i++) {
		vertex.push_back(_triangle.vertex(i).getX());
		vertex.push_back(_triangle.vertex(i).getY());
		vertex.push_back(_triangle.vertex(i).getZ());
	}

	for(uint8_t i = 0; i < 3; i++)
	{
		normals.push_back(_triangle.normal().getX());
		normals.push_back(_triangle.normal().getY());
		normals.push_back(_triangle.normal().getZ());
	}

	return 12;
}

MathGrapherRegion Triangle::getRegion() const
{
	MathVector3D<double> begin = _triangle.vertex(0);
	MathVector3D<double> end = _triangle.vertex(0);

	for (uint8_t i = 1; i < 2; i++) {
		if (_triangle.vertex(i).getX() < begin.getX())
			begin.setX(_triangle.vertex(i).getX());
		if (_triangle.vertex(i).getY() < begin.getY())
			begin.setY(_triangle.vertex(i).getY());
		if (_triangle.vertex(i).getZ() < begin.getZ())
			begin.setZ(_triangle.vertex(i).getZ());

		if (_triangle.vertex(i).getX() > end.getX())
			begin.setX(_triangle.vertex(i).getX());
		if (_triangle.vertex(i).getY() > end.getY())
			begin.setY(_triangle.vertex(i).getY());
		if (_triangle.vertex(i).getZ() > end.getZ())
			begin.setZ(_triangle.vertex(i).getZ());
	}

	return MathGrapherRegion(begin, end);
}

size_t Vector::getSurface(std::vector<double> &vertex, std::vector<double> &normals, const double& size) const
{
    MathVector3D<double>            begin, end;
    MathFixedVector3D<double>       vectorFixLen;

    vectorFixLen = _vector*size*20.0;
    GetConusVertex(size, vectorFixLen.getBegin(), vectorFixLen.getEnd(), vertex, normals, 15);
    end = vectorFixLen.getEnd();
    vectorFixLen = vectorFixLen*(0.6);
    begin = vectorFixLen.getEnd();
    GetConusVertex(size*3.0/20.0, begin, end, vertex, normals, 15);
    return 15*3*2;
}

MathGrapherRegion Vector::getRegion() const
{
    MathVector3D<double> points[2];

    points[0] = _vector.getBegin();
    points[1] = _vector.getEnd();

	MathVector3D<double> begin;
	MathVector3D<double> end;

	begin.setX(points[1].getX() > points[0].getX()? points[0].getX() : points[1].getX());
	begin.setY(points[1].getY() > points[0].getY()? points[0].getY() : points[1].getY());
	begin.setZ(points[1].getZ() > points[0].getZ()? points[0].getZ() : points[1].getZ());

	end.setX(points[1].getX() > points[0].getX()? points[1].getX() : points[0].getX());
	end.setY(points[1].getY() > points[0].getY()? points[1].getY() : points[0].getY());
	end.setZ(points[1].getZ() > points[0].getZ()? points[1].getZ() : points[0].getZ());

	return MathGrapherRegion(begin, end);

}

size_t Sphere::getSurface(std::vector<double> &vertex, std::vector<double> &normals, const double& size) const
{
    size_t oldSize = vertex.size()/3;

    (void)size;
    GetSphereVertex(_rad, _point, vertex, normals);
    return vertex.size()/3 - oldSize;
}

MathGrapherRegion Sphere::getRegion() const
{
	return MathGrapherRegion(_point - MathVector3D<double>(_rad, _rad, _rad), _point + MathVector3D<double>(_rad, _rad, _rad));
}

size_t Tetraedr::getSurface(std::vector<double> &vertex, std::vector<double> &normals, const double& size) const
{
    MathVector3D<double>    vec1, vec2, vecNormal;
    uint8_t i, j;

    (void)size;
    for(i=0; i<4; i++)
    {
        for(j=0; j<3; j++)
        {
            vertex.push_back(points[(i + j)%4].getX());
            vertex.push_back(points[(i + j)%4].getY());
            vertex.push_back(points[(i + j)%4].getZ());
        }
        vec1 = points[(i + 1) % 4] - points[i % 4];
        vec2 = points[(i + 2) % 4] - points[i % 4];
        vecNormal = vec1^vec2;
        vecNormal = vecNormal.unitVector();
        vec1 = points[(i + 3) % 4] - points[i % 4];
        if(vec1*vecNormal > 0.0) vecNormal = MathVector3D<double>(0.0, 0.0, 0.0) - vecNormal;
        for(j=0; j<3; j++)
        {
            normals.push_back(vecNormal.getX());
            normals.push_back(vecNormal.getY());
            normals.push_back(vecNormal.getZ());
        }
    }

    return 12;
}

MathGrapherRegion Tetraedr::getRegion() const
{
    double minX = DBL_MAX, minY = DBL_MAX, minZ = DBL_MAX;
    double maxX = 0.0, maxY = 0.0, maxZ = 0.0;

    for(uint8_t i=0; i<4; i++)
    {
        if(points[i].getX() < minX) minX = points[i].getX();
        if(points[i].getX() > maxX) maxX = points[i].getX();

        if(points[i].getY() < minY) minY = points[i].getY();
        if(points[i].getY() > maxY) maxY = points[i].getY();

        if(points[i].getZ() < minZ) minZ = points[i].getZ();
        if(points[i].getZ() > maxZ) maxZ = points[i].getZ();
    }
	return MathGrapherRegion(MathVector3D<double>(minX, minY, minZ), MathVector3D<double>(maxX, maxY, maxZ));
}
};
