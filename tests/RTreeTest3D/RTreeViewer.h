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

#ifndef RTREEVIEWER_H
#define RTREEVIEWER_H

#include <QWidget>
#include <QMainWindow>
#include <QMouseEvent>
#include <QImage>
#include <QStatusBar>
#include <QLabel>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <math.h>
#include <algorithm>
#include "../../MathRTreeStar.h"
#include "../MathGrapher.h"
#include "../MathGrapherObject.h"
#include "../MathVector3D.h"

typedef MathMBR<double, 3>	MBR;

class Tetraedr
{
public:
	Tetraedr(const MathVector3D<double>& point1, const MathVector3D<double>& point2, const MathVector3D<double>& point3, const MathVector3D<double>& point4)
	{
		points[0] = point1;
		points[1] = point2;
		points[2] = point3;
		points[3] = point4;
	}

	const MathVector3D<double>&		point(uint8_t num) const {return points[num % 4];}

	MBR								getMBR() const
	{
		MBR			mbr;

		mbr.setDim(std::min(std::min(std::min(points[0].getX(), points[1].getX()), points[2].getX()), points[3].getX()),
				std::max(std::max(std::max(points[0].getX(), points[1].getX()), points[2].getX()), points[3].getX()), 0);
		mbr.setDim(std::min(std::min(std::min(points[0].getY(), points[1].getY()), points[2].getY()), points[3].getY()),
				std::max(std::max(std::max(points[0].getY(), points[1].getY()), points[2].getY()), points[3].getY()), 1);
		mbr.setDim(std::min(std::min(std::min(points[0].getZ(), points[1].getZ()), points[2].getZ()), points[3].getZ()),
				std::max(std::max(std::max(points[0].getZ(), points[1].getZ()), points[2].getZ()), points[3].getZ()), 2);

		return mbr;
	}
private:
	MathVector3D<double>			points[4];
};


typedef MathRTreeStar<Tetraedr, double, 3, 4, 10>	RTree;

class RTreeViewer : public MathGrapher, private RTree
{
public:
	RTreeViewer(const MathVector3D<double>& minPoint, const MathVector3D<double>& maxPoint, size_t numTriangles, double maxSize);
	~RTreeViewer();
private:
	void							redrawNodes(const Node *node);
	void							drawMBR(const MBR& mbr, const QColor& color);
};

#endif // RTREEVIEWER_H
