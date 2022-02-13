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

#ifndef GLFUNCTIONS_H
#define GLFUNCTIONS_H
#include <GL/gl.h>
#include <vector>
#include <cmath>
#include "MathVector3D.h"

/* получение точки на окружности */
template <class T>
MathVector3D<T> GetPointFromCircle(T rad, const MathFixedVector3D<T> &normal, T angleXY)
{
    MathVector3D<T>              ret(rad, 0, 0);
    MathVector3D<T>             vector = normal.freeVector();
    MathVector3D<T>             unitNormal = vector.unitVector();
    T                           scalarXY;

    scalarXY = vector.getX()*vector.getX() + vector.getY()*vector.getY();
    ret = ret.rotateParallelAxisZ(MathVector3D<T>(), angleXY*M_PI/T(180));
    ret = ret.rotateParallelAxisY(MathVector3D<T>(), sqrt(1 - unitNormal.getZ()*unitNormal.getZ()), unitNormal.getZ());
    if(scalarXY != 0)
    {
        T len = normal.lenght();
        T divXY = sqrt(scalarXY/len/len);
        ret = ret.rotateParallelAxisZ(MathVector3D<T>(), unitNormal.getY()/divXY, unitNormal.getX()/divXY);
    }
    ret = ret + normal.getBegin();
    return ret;
}


void GetSphereVertex(GLdouble r, MathVector3D<double> point, std::vector<double>& points, std::vector<double> &normals, unsigned int numStepVer=10, unsigned int numStepRad=10);
void DrawSphereTexture(GLdouble r, MathVector3D<double> point, GLuint numTextures, GLdouble xozAngle=0, unsigned int numStepVer=30,unsigned int numStepRad=30);

void GetConusVertex(GLdouble r, MathVector3D<double> begin, MathVector3D<double> end, std::vector<double>& points, std::vector<double> &normals, unsigned int numSurfaces=15);

#endif // GLFUNCTIONS_H
