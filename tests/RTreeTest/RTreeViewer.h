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
#include "../MathVector2D.h"

class Triangle;

typedef MathMBR<double, 2>	MBR;

class Triangle
{
public:
	Triangle(const MathVector2D<double>& point1, const MathVector2D<double>& point2, const MathVector2D<double>& point3)
	{
		points[0] = point1;
		points[1] = point2;
		points[2] = point3;
	}

	const MathVector2D<double>&		point(uint8_t num) const {return points[num % 3];}

	MBR								getMBR() const
	{
		MBR			mbr;

		mbr.setDim(std::min(std::min(points[0].getX(), points[1].getX()), points[2].getX()), std::max(std::max(points[0].getX(), points[1].getX()), points[2].getX()), 0);
		mbr.setDim(std::min(std::min(points[0].getY(), points[1].getY()), points[2].getY()), std::max(std::max(points[0].getY(), points[1].getY()), points[2].getY()), 1);

		return mbr;
	}
private:
	MathVector2D<double>			points[3];
};

typedef MathRTreeStar<Triangle, double, 2, 4, 10>	RTree;

class RTreeViewer : public QMainWindow, private RTree
{
	Q_OBJECT

public:
	RTreeViewer(const std::string& fileName, QWidget *parent = nullptr);
	RTreeViewer(const MathVector2D<double>& minPoint, const MathVector2D<double>& maxPoint, size_t numTriangles, double maxSize, QWidget *parent = nullptr);
	~RTreeViewer();
private:
	MathVector2D<double>			getMin() const;
	MathVector2D<double>			getMax() const;
	void							redraw();
	void							redrawNodes(const Node *node, QPainter &painter);
	void							paintEvent(QPaintEvent *);
	void							resizeEvent(QResizeEvent *);
	void							mousePressEvent(QMouseEvent *event);
	void							mouseMoveEvent(QMouseEvent *event);

	QImage							image;
	MathVector2D<double>			min, max;
	QLabel							*coordLabel;
};

#endif // RTREEVIEWER_H
