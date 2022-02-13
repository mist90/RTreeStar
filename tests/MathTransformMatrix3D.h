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

#ifndef MATHTRANSFORMMATRIX3D_H
#define MATHTRANSFORMMATRIX3D_H
#include <stdint.h>

template<class T>
class MathTransformMatrix3D
{
public:
    class MathTransformMatrix3DExceptionRange
    {
    public:
        MathTransformMatrix3DExceptionRange() {}
    };

    class MathTransformMatrix3DExceptionDivision
    {
    public:
        MathTransformMatrix3DExceptionDivision() {}
    };

    MathTransformMatrix3D()
    {
        for(uint8_t i = 0; i < 4; i++)
            for(uint8_t j = 0; j < 4; j++)
                if(i != j) matrix[i][j] = T(0);
                else matrix[i][j] = T(1);
    }

    MathTransformMatrix3D<T>    operator*(const MathTransformMatrix3D<T>& matrix) const
    {
        MathTransformMatrix3D<T>    ret;

        for(uint8_t i = 0; i < 4; i++)
            for(uint8_t j = 0; j < 4; j++)
            {
                ret.matrix[i][j] = T(0);
                for(uint8_t k = 0; k < 4; k++)
                    ret.matrix[i][j] += this->matrix[i][k]*matrix.matrix[k][j];
            }
        return ret;
    }

    MathTransformMatrix3D<T>    reverse() const
    {
        MathTransformMatrix3D<T>        ret;
        T                               det;

        det = ((this->matrix[0][0]*this->matrix[1][1]-this->matrix[0][1]*this->matrix[1][0])*this->matrix[2][2] +
               (this->matrix[0][2]*this->matrix[1][0]-this->matrix[0][0]*this->matrix[1][2])*this->matrix[2][1] +
               (this->matrix[0][1]*this->matrix[1][2]-this->matrix[0][2]*this->matrix[1][1])*this->matrix[2][0])*this->matrix[3][3] +
              ((this->matrix[0][1]*this->matrix[1][0]-this->matrix[0][0]*this->matrix[1][1])*this->matrix[2][3] +
               (this->matrix[0][0]*this->matrix[1][3]-this->matrix[0][3]*this->matrix[1][0])*this->matrix[2][1] +
               (this->matrix[0][3]*this->matrix[1][1]-this->matrix[0][1]*this->matrix[1][3])*this->matrix[2][0])*this->matrix[3][2] +
              ((this->matrix[0][0]*this->matrix[1][2]-this->matrix[0][2]*this->matrix[1][0])*this->matrix[2][3] +
               (this->matrix[0][3]*this->matrix[1][0]-this->matrix[0][0]*this->matrix[1][3])*this->matrix[2][2] +
               (this->matrix[0][2]*this->matrix[1][3]-this->matrix[0][3]*this->matrix[1][2])*this->matrix[2][0])*this->matrix[3][1] +
              ((this->matrix[0][2]*this->matrix[1][1]-this->matrix[0][1]*this->matrix[1][2])*this->matrix[2][3] +
               (this->matrix[0][1]*this->matrix[1][3]-this->matrix[0][3]*this->matrix[1][1])*this->matrix[2][2] +
               (this->matrix[0][3]*this->matrix[1][2]-this->matrix[0][2]*this->matrix[1][3])*this->matrix[2][1])*this->matrix[3][0];

        if(det == T(0)) throw MathTransformMatrix3DExceptionDivision();

        ret.matrix[0][0] = (this->matrix[1][1]*(this->matrix[2][2]*this->matrix[3][3]-this->matrix[2][3]*this->matrix[3][2])-this->matrix[1][2]*(this->matrix[2][1]*this->matrix[3][3]-this->matrix[2][3]*this->matrix[3][1])+this->matrix[1][3]*(this->matrix[2][1]*this->matrix[3][2]-this->matrix[2][2]*this->matrix[3][1]))/det;
        ret.matrix[0][1] = (-this->matrix[0][1]*(this->matrix[2][2]*this->matrix[3][3]-this->matrix[2][3]*this->matrix[3][2])+this->matrix[0][2]*(this->matrix[2][1]*this->matrix[3][3]-this->matrix[2][3]*this->matrix[3][1])-this->matrix[0][3]*(this->matrix[2][1]*this->matrix[3][2]-this->matrix[2][2]*this->matrix[3][1]))/det;
        ret.matrix[0][2] = (this->matrix[0][1]*(this->matrix[1][2]*this->matrix[3][3]-this->matrix[1][3]*this->matrix[3][2])-this->matrix[0][2]*(this->matrix[1][1]*this->matrix[3][3]-this->matrix[1][3]*this->matrix[3][1])+this->matrix[0][3]*(this->matrix[1][1]*this->matrix[3][2]-this->matrix[1][2]*this->matrix[3][1]))/det;
        ret.matrix[0][3] = (-this->matrix[0][1]*(this->matrix[1][2]*this->matrix[2][3]-this->matrix[1][3]*this->matrix[2][2])+this->matrix[0][2]*(this->matrix[1][1]*this->matrix[2][3]-this->matrix[1][3]*this->matrix[2][1])-this->matrix[0][3]*(this->matrix[1][1]*this->matrix[2][2]-this->matrix[1][2]*this->matrix[2][1]))/det;

        ret.matrix[1][0] = (-this->matrix[1][0]*(this->matrix[2][2]*this->matrix[3][3]-this->matrix[2][3]*this->matrix[3][2])+this->matrix[1][2]*(this->matrix[2][0]*this->matrix[3][3]-this->matrix[2][3]*this->matrix[3][0])-this->matrix[1][3]*(this->matrix[2][0]*this->matrix[3][2]-this->matrix[2][2]*this->matrix[3][0]))/det;
        ret.matrix[1][1] = (this->matrix[0][0]*(this->matrix[2][2]*this->matrix[3][3]-this->matrix[2][3]*this->matrix[3][2])-this->matrix[0][2]*(this->matrix[2][0]*this->matrix[3][3]-this->matrix[2][3]*this->matrix[3][0])+this->matrix[0][3]*(this->matrix[2][0]*this->matrix[3][2]-this->matrix[2][2]*this->matrix[3][0]))/det;
        ret.matrix[1][2] = (-this->matrix[0][0]*(this->matrix[1][2]*this->matrix[3][3]-this->matrix[1][3]*this->matrix[3][2])+this->matrix[0][2]*(this->matrix[1][0]*this->matrix[3][3]-this->matrix[1][3]*this->matrix[3][0])-this->matrix[0][3]*(this->matrix[1][0]*this->matrix[3][2]-this->matrix[1][2]*this->matrix[3][0]))/det;
        ret.matrix[1][3] = (this->matrix[0][0]*(this->matrix[1][2]*this->matrix[2][3]-this->matrix[1][3]*this->matrix[2][2])-this->matrix[0][2]*(this->matrix[1][0]*this->matrix[2][3]-this->matrix[1][3]*this->matrix[2][0])+this->matrix[0][3]*(this->matrix[1][0]*this->matrix[2][2]-this->matrix[1][2]*this->matrix[2][0]))/det;

        ret.matrix[2][0] = (this->matrix[1][0]*(this->matrix[2][1]*this->matrix[3][3]-this->matrix[2][3]*this->matrix[3][1])-this->matrix[1][1]*(this->matrix[2][0]*this->matrix[3][3]-this->matrix[2][3]*this->matrix[3][0])+this->matrix[1][3]*(this->matrix[2][0]*this->matrix[3][1]-this->matrix[2][1]*this->matrix[3][0]))/det;
        ret.matrix[2][1] = (-this->matrix[0][0]*(this->matrix[2][1]*this->matrix[3][3]-this->matrix[2][3]*this->matrix[3][1])+this->matrix[0][1]*(this->matrix[2][0]*this->matrix[3][3]-this->matrix[2][3]*this->matrix[3][0])-this->matrix[0][3]*(this->matrix[2][0]*this->matrix[3][1]-this->matrix[2][1]*this->matrix[3][0]))/det;
        ret.matrix[2][2] = (this->matrix[0][0]*(this->matrix[1][1]*this->matrix[3][3]-this->matrix[1][3]*this->matrix[3][1])-this->matrix[0][1]*(this->matrix[1][0]*this->matrix[3][3]-this->matrix[1][3]*this->matrix[3][0])+this->matrix[0][3]*(this->matrix[1][0]*this->matrix[3][1]-this->matrix[1][1]*this->matrix[3][0]))/det;
        ret.matrix[2][3] = (-this->matrix[0][0]*(this->matrix[1][1]*this->matrix[2][3]-this->matrix[1][3]*this->matrix[2][1])+this->matrix[0][1]*(this->matrix[1][0]*this->matrix[2][3]-this->matrix[1][3]*this->matrix[2][0])-this->matrix[0][3]*(this->matrix[1][0]*this->matrix[2][1]-this->matrix[1][1]*this->matrix[2][0]))/det;

        ret.matrix[3][0] = (-this->matrix[1][0]*(this->matrix[2][1]*this->matrix[3][2]-this->matrix[2][2]*this->matrix[3][1])+this->matrix[1][1]*(this->matrix[2][0]*this->matrix[3][2]-this->matrix[2][2]*this->matrix[3][0])-this->matrix[1][2]*(this->matrix[2][0]*this->matrix[3][1]-this->matrix[2][1]*this->matrix[3][0]))/det;
        ret.matrix[3][1] = (this->matrix[0][0]*(this->matrix[2][1]*this->matrix[3][2]-this->matrix[2][2]*this->matrix[3][1])-this->matrix[0][1]*(this->matrix[2][0]*this->matrix[3][2]-this->matrix[2][2]*this->matrix[3][0])+this->matrix[0][2]*(this->matrix[2][0]*this->matrix[3][1]-this->matrix[2][1]*this->matrix[3][0]))/det;
        ret.matrix[3][2] = (-this->matrix[0][0]*(this->matrix[1][1]*this->matrix[3][2]-this->matrix[1][2]*this->matrix[3][1])+this->matrix[0][1]*(this->matrix[1][0]*this->matrix[3][2]-this->matrix[1][2]*this->matrix[3][0])-this->matrix[0][2]*(this->matrix[1][0]*this->matrix[3][1]-this->matrix[1][1]*this->matrix[3][0]))/det;
        ret.matrix[3][3] = (this->matrix[0][0]*(this->matrix[1][1]*this->matrix[2][2]-this->matrix[1][2]*this->matrix[2][1])-this->matrix[0][1]*(this->matrix[1][0]*this->matrix[2][2]-this->matrix[1][2]*this->matrix[2][0])+this->matrix[0][2]*(this->matrix[1][0]*this->matrix[2][1]-this->matrix[1][1]*this->matrix[2][0]))/det;
        return ret;
    }

    MathTransformMatrix3D<T>    reverse3() const
    {
        MathTransformMatrix3D<T>        ret;
        T                               det;

        det = this->matrix[0][0]*(this->matrix[1][1]*this->matrix[2][2] - this->matrix[1][2]*this->matrix[2][1]) +
              this->matrix[0][1]*(this->matrix[1][2]*this->matrix[2][0] - this->matrix[1][0]*this->matrix[2][2]) +
              this->matrix[0][2]*(this->matrix[1][0]*this->matrix[2][1] - this->matrix[1][1]*this->matrix[2][0]);

        if(det == T(0)) throw MathTransformMatrix3DExceptionDivision();

        ret.matrix[0][0] = (this->matrix[1][1]*this->matrix[2][2] - this->matrix[1][2]*this->matrix[2][1])/det;
        ret.matrix[0][1] = (this->matrix[0][2]*this->matrix[2][1] - this->matrix[0][1]*this->matrix[2][2])/det;
        ret.matrix[0][2] = (this->matrix[0][1]*this->matrix[1][2] - this->matrix[0][2]*this->matrix[1][1])/det;

        ret.matrix[1][0] = (this->matrix[1][2]*this->matrix[2][0] - this->matrix[1][0]*this->matrix[2][2])/det;
        ret.matrix[1][1] = (this->matrix[0][0]*this->matrix[2][2] - this->matrix[0][2]*this->matrix[2][0])/det;
        ret.matrix[1][2] = (this->matrix[0][2]*this->matrix[1][0] - this->matrix[0][0]*this->matrix[1][2])/det;

        ret.matrix[2][0] = (this->matrix[1][0]*this->matrix[2][1] - this->matrix[1][1]*this->matrix[2][0])/det;
        ret.matrix[2][1] = (this->matrix[0][1]*this->matrix[2][0] - this->matrix[0][0]*this->matrix[2][1])/det;
        ret.matrix[2][2] = (this->matrix[0][0]*this->matrix[1][1] - this->matrix[0][1]*this->matrix[1][0])/det;
        return ret;
    }

    T&                          element(uint8_t string, uint8_t column)
    {
        if(string > 3 || column > 3) throw MathTransformMatrix3DExceptionRange();
        return matrix[string][column];
    }

	const T&					element(uint8_t string, uint8_t column) const
    {
        if(string > 3 || column > 3) throw MathTransformMatrix3DExceptionRange();
        return matrix[string][column];
    }

    void                        addMove(const T& dx, const T& dy, const T& dz)
    {
        MathTransformMatrix3D<T>    moveMatrix;

        moveMatrix.matrix[3][0] = dx;
        moveMatrix.matrix[3][1] = dy;
        moveMatrix.matrix[3][2] = dz;

        *this = *this * moveMatrix;
    }

    void                        addRotateParallelAxisX(const T& sinAngle, const T& cosAngle)
    {
        MathTransformMatrix3D<T>    rotateMatrix;

        rotateMatrix.matrix[1][1] = cosAngle;
        rotateMatrix.matrix[1][2] = sinAngle;
        rotateMatrix.matrix[2][1] = -rotateMatrix.matrix[1][2];
        rotateMatrix.matrix[2][2] = rotateMatrix.matrix[1][1];

        *this = *this * rotateMatrix;
    }

    void                        addRotateParallelAxisX(const T& angle)
    {
        addRotateParallelAxisX(sin(angle), cos(angle));
    }

    void                        addRotateParallelAxisY(const T& sinAngle, const T& cosAngle)
    {
        MathTransformMatrix3D<T>    rotateMatrix;

        rotateMatrix.matrix[0][0] = cosAngle;
        rotateMatrix.matrix[0][2] = -sinAngle;
        rotateMatrix.matrix[2][0] = -rotateMatrix.matrix[0][2];
        rotateMatrix.matrix[2][2] = rotateMatrix.matrix[0][0];

        *this = *this * rotateMatrix;
    }

    void                        addRotateParallelAxisY(const T& angle)
    {
        addRotateParallelAxisY(sin(angle), cos(angle));
    }

    void                        addRotateParallelAxisZ(const T& sinAngle, const T& cosAngle)
    {
        MathTransformMatrix3D<T>    rotateMatrix;

        rotateMatrix.matrix[0][0] = cosAngle;
        rotateMatrix.matrix[0][1] = sinAngle;
        rotateMatrix.matrix[1][0] = -rotateMatrix.matrix[0][1];
        rotateMatrix.matrix[1][1] = rotateMatrix.matrix[0][0];

        *this = *this * rotateMatrix;
    }

    void                        addRotateParallelAxisZ(const T& angle)
    {
        addRotateParallelAxisZ(sin(angle), cos(angle));
    }

    void                        addScale(const T& mx, const T& my, const T& mz)
    {
        MathTransformMatrix3D<T>   scaleMatrix;

        scaleMatrix.matrix[0][0] = mx;
        scaleMatrix.matrix[1][1] = my;
        scaleMatrix.matrix[2][2] = mz;

        *this = *this * scaleMatrix;
    }

private:
    T                               matrix[4][4];
};

#endif // MATHTRANSFORMMATRIX3D_H
