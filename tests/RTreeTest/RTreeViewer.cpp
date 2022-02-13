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

#include "RTreeViewer.h"
#include <QPainter>
#include <QMatrix>
#include <random>


RTreeViewer::RTreeViewer(const std::string &fileName, QWidget *parent)
	: QMainWindow(parent)
{
	std::ifstream	file;
	std::string		string;
	size_t			num = 1;

	file.open(fileName.data());
	if(!file.is_open()) return;
	while(!std::getline(file, string, '\n').eof())
	{
		double					x1, y1, x2, y2, x3, y3;
		std::stringstream		oneTriangle(string);

		oneTriangle >> x1;
		oneTriangle >> y1;
		oneTriangle >> x2;
		oneTriangle >> y2;
		oneTriangle >> x3;
		oneTriangle >> y3;
		string.clear();
		insert(Triangle(MathVector2D<double>(x1, y1), MathVector2D<double>(x2, y2), MathVector2D<double>(x3, y3)));
		std::cout << num << ": " << x1 << " " << y1 << " " << x2 << " " << y2 << " " << x3 << " " << y3 << std::endl;
		num++;
	}

	MBR		mbr;

	mbr.setDim(-3.5, -2.1, 0);
	mbr.setDim(-1, 1, 1);

	std::cout << "Found objects:\n";
	for (auto it = begin([&mbr](const MBR& mbrListNode) {return mbr.isIntersect(mbrListNode);}, [&mbr](const MBR& mbrListNode) {return mbr.isIntersect(mbrListNode);}); it != end(); ++it) {
		std::cout << (*it).point(0) << " " << (*it).point(1) << " " << (*it).point(2) << std::endl;
	}
	std::cout << "Levels in tree:" << levels() << " All elements: " << RTree::size() << std::endl;
	std::cout << std::flush;

	coordLabel = new QLabel("0 : 0");
	coordLabel->setStyleSheet("QLabel {background-color : rgba(255,255,255,100); color : black}");
	statusBar()->addWidget(coordLabel);

	setMouseTracking(true);

	resize(800, 600);
}

RTreeViewer::RTreeViewer(const MathVector2D<double>& minPoint, const MathVector2D<double>& maxPoint, size_t numTriangles, double maxSize, QWidget *parent)
	: QMainWindow(parent)
{
	std::default_random_engine				generator;
	std::uniform_real_distribution<double>	xDistribution(minPoint.getX(), maxPoint.getX() - maxSize);
	std::uniform_real_distribution<double>	yDistribution(minPoint.getY(), maxPoint.getY() - maxSize);
	std::uniform_real_distribution<double>	deltaDistribution(0, maxSize);

	//generator.seed(time(NULL));
	for(size_t i = 0; i < numTriangles; i++)
	{
		double						x = xDistribution(generator);
		double						y = yDistribution(generator);

		insert(Triangle(MathVector2D<double>(x, y),
						MathVector2D<double>(x + deltaDistribution(generator), y + deltaDistribution(generator)),
						MathVector2D<double>(x + deltaDistribution(generator), y + deltaDistribution(generator))));
	}
	MBR		mbr;

	mbr.setDim(-17, -10, 0);
	mbr.setDim(-6.6, -2.5, 1);

	std::cout << "Found objects:\n";
	for (auto it = begin([&mbr](const MBR& mbrListNode) {return mbr.isIntersect(mbrListNode);}, [&mbr](const MBR& mbrListNode) {return mbr.isIntersect(mbrListNode);}); it != end(); ++it) {
		std::cout << (*it).point(0) << " " << (*it).point(1) << " " << (*it).point(2) << std::endl;
	}
	std::cout << "Levels in tree:" << levels() << " All elements: " << RTree::size() << std::endl;
	std::cout << std::flush;

	coordLabel = new QLabel("0 : 0");
	coordLabel->setStyleSheet("QLabel {background-color : rgba(255,255,255,100); color : black}");
	statusBar()->addWidget(coordLabel);

	setMouseTracking(true);

	resize(800, 600);
}

RTreeViewer::~RTreeViewer()
{
}

MathVector2D<double> RTreeViewer::getMin() const
{
	if(root == nullptr) return MathVector2D<double>();
	else return MathVector2D<double>(root->getMBR().minDim(0), root->getMBR().minDim(1));
}

MathVector2D<double> RTreeViewer::getMax() const
{
	if(root == nullptr) return MathVector2D<double>();
	else return MathVector2D<double>(root->getMBR().maxDim(0), root->getMBR().maxDim(1));
}

void RTreeViewer::redraw()
{
	image.fill(Qt::white);
	QPainter				painter(&image);
	min = getMin();
	max = getMax();
	MathVector2D<double>	delta = max - min;
	QMatrix					transform;

	if(delta.getX() == 0.0 || delta.getY() == 0.0) return;

	transform.scale(1.0, -1.0);
	transform.translate(0.0, -height());
	transform.scale(double(width())/delta.getX(), double(height())/delta.getY());
	transform.translate(-min.getX(), -min.getY());
	painter.setMatrix(transform);

	redrawNodes(root, painter);
}

void RTreeViewer::redrawNodes(const Node *node, QPainter& painter)
{
	if(node == nullptr) return;

	std::list<const Node*>			nodes;
	std::list<const Triangle*>		triangles;

	painter.setPen(QPen(QBrush(Qt::black), 0.0));
	painter.setBrush(QBrush(QColor(0, 255, 0, 50)));

	nodes.push_back(node);

	while(nodes.size() != 0)
	{
		const Node		*current = nodes.front();
		nodes.pop_front();

		if(!current->isLeaf())
		{
			for(size_t i = 0; i < current->getNumChildren(); i++)
				nodes.push_back(static_cast<const Node*>(current->childs[i]));
		}
		else {
			for(size_t i = 0; i < current->getNumChildren(); i++)
				triangles.push_back(static_cast<const Triangle*>(current->childs[i]));
		}
		if(current->isLeaf()) painter.setPen(QPen(QBrush(Qt::blue), 0.0));
		painter.drawRect(QRectF(QPointF(current->getMBR().minDim(0), current->getMBR().maxDim(1)), QPointF(current->getMBR().maxDim(0), current->getMBR().minDim(1))));
		if(current->isLeaf()) painter.setPen(QPen(QBrush(Qt::black), 0.0));
	}

	for(auto it = triangles.begin(); it != triangles.end(); ++it)
		painter.drawRect(QRectF(QPointF((*it)->getMBR().minDim(0), (*it)->getMBR().maxDim(1)), QPointF((*it)->getMBR().maxDim(0), (*it)->getMBR().minDim(1))));

	painter.setPen(QPen(QBrush(Qt::red), 0.0));
	painter.setBrush(QBrush(QColor(255, 0, 0, 50)));

	for(auto it = triangles.begin(); it != triangles.end(); ++it)
	{
		QPointF		points[] = {QPointF((*it)->point(0).getX(), (*it)->point(0).getY()), QPointF((*it)->point(1).getX(), (*it)->point(1).getY()), QPointF((*it)->point(2).getX(), (*it)->point(2).getY())};

		painter.drawPolygon(points, 3);

	}
}

void RTreeViewer::paintEvent(QPaintEvent *)
{
	QPainter		painter(this);

	painter.drawImage(0, 0, image);
}

void RTreeViewer::resizeEvent(QResizeEvent *)
{
	image = QImage(width(), height(), QImage::Format_RGB32);
	redraw();
}

void RTreeViewer::mousePressEvent(QMouseEvent *event)
{
	MathVector2D<double>		delta = max - min;
	double						x = double(event->pos().x())/double(width())*delta.getX() + min.getX();
	double						y = double(height() - event->pos().y())/double(height())*delta.getY() + min.getY();

	std::cout << x << " " << y << std::endl;
}

void RTreeViewer::mouseMoveEvent(QMouseEvent *event)
{
	MathVector2D<double>		delta = max - min;
	double						x = double(event->pos().x())/double(width())*delta.getX() + min.getX();
	double						y = double(height() - event->pos().y())/double(height())*delta.getY() + min.getY();

	coordLabel->setText(QString("%1 : %2").arg(x).arg(y));
}
