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
#include <random>


RTreeViewer::RTreeViewer(const MathVector3D<double>& minPoint, const MathVector3D<double>& maxPoint, size_t numTriangles, double maxSize)
	: MathGrapher()
{
	std::default_random_engine				generator;
	std::uniform_real_distribution<double>	xDistribution(minPoint.getX(), maxPoint.getX() - maxSize);
	std::uniform_real_distribution<double>	yDistribution(minPoint.getY(), maxPoint.getY() - maxSize);
	std::uniform_real_distribution<double>	zDistribution(minPoint.getZ(), maxPoint.getZ() - maxSize);
	std::uniform_real_distribution<double>	deltaDistribution(0, maxSize);

	//generator.seed(time(NULL));
	for(size_t i = 0; i < numTriangles; i++)
	{
		double						x = xDistribution(generator);
		double						y = yDistribution(generator);
		double						z = zDistribution(generator);

		insert(Tetraedr(MathVector3D<double>(x, y, z),
						MathVector3D<double>(x + deltaDistribution(generator), y + deltaDistribution(generator), z + deltaDistribution(generator)),
						MathVector3D<double>(x + deltaDistribution(generator), y + deltaDistribution(generator), z + deltaDistribution(generator)),
						MathVector3D<double>(x + deltaDistribution(generator), y + deltaDistribution(generator), z + deltaDistribution(generator))));
	}
	redrawNodes(root);

	redraw();

	std::cout << "Levels in tree:" << levels() << " All elements: " << RTree::size() << std::endl;
	std::cout << std::flush;
}

void RTreeViewer::redrawNodes(const Node *node)
{
	if(node == nullptr) return;

	std::list<const Node*>			nodes;
	std::list<const Tetraedr*>		tetraedrs;

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
				tetraedrs.push_back(static_cast<const Tetraedr*>(current->childs[i]));
		}
		if(current->isLeaf()) drawMBR(current->getMBR(), Qt::blue);
		else drawMBR(current->getMBR(), Qt::black);
	}

	for(auto &tetraedr: tetraedrs)
		drawMBR(tetraedr->getMBR(), Qt::black);

	for(auto &tetraedr: *this)
        addObject(new MathGrapherObjects::Tetraedr(tetraedr.point(0), tetraedr.point(1), tetraedr.point(2), tetraedr.point(3)), Qt::green);

}

void RTreeViewer::drawMBR(const MBR &mbr, const QColor& color)
{
    addObject(new MathGrapherObjects::Line(MathVector3D<double>(mbr.minDim(0), mbr.minDim(1), mbr.minDim(2)), MathVector3D<double>(mbr.maxDim(0), mbr.minDim(1), mbr.minDim(2))), color);
    addObject(new MathGrapherObjects::Line(MathVector3D<double>(mbr.minDim(0), mbr.maxDim(1), mbr.minDim(2)), MathVector3D<double>(mbr.maxDim(0), mbr.maxDim(1), mbr.minDim(2))), color);
    addObject(new MathGrapherObjects::Line(MathVector3D<double>(mbr.minDim(0), mbr.minDim(1), mbr.maxDim(2)), MathVector3D<double>(mbr.maxDim(0), mbr.minDim(1), mbr.maxDim(2))), color);
    addObject(new MathGrapherObjects::Line(MathVector3D<double>(mbr.minDim(0), mbr.maxDim(1), mbr.maxDim(2)), MathVector3D<double>(mbr.maxDim(0), mbr.maxDim(1), mbr.maxDim(2))), color);

    addObject(new MathGrapherObjects::Line(MathVector3D<double>(mbr.minDim(0), mbr.minDim(1), mbr.minDim(2)), MathVector3D<double>(mbr.minDim(0), mbr.maxDim(1), mbr.minDim(2))), color);
    addObject(new MathGrapherObjects::Line(MathVector3D<double>(mbr.maxDim(0), mbr.minDim(1), mbr.minDim(2)), MathVector3D<double>(mbr.maxDim(0), mbr.maxDim(1), mbr.minDim(2))), color);
    addObject(new MathGrapherObjects::Line(MathVector3D<double>(mbr.minDim(0), mbr.minDim(1), mbr.maxDim(2)), MathVector3D<double>(mbr.minDim(0), mbr.maxDim(1), mbr.maxDim(2))), color);
    addObject(new MathGrapherObjects::Line(MathVector3D<double>(mbr.maxDim(0), mbr.minDim(1), mbr.maxDim(2)), MathVector3D<double>(mbr.maxDim(0), mbr.maxDim(1), mbr.maxDim(2))), color);

    addObject(new MathGrapherObjects::Line(MathVector3D<double>(mbr.minDim(0), mbr.minDim(1), mbr.minDim(2)), MathVector3D<double>(mbr.minDim(0), mbr.minDim(1), mbr.maxDim(2))), color);
    addObject(new MathGrapherObjects::Line(MathVector3D<double>(mbr.maxDim(0), mbr.minDim(1), mbr.minDim(2)), MathVector3D<double>(mbr.maxDim(0), mbr.minDim(1), mbr.maxDim(2))), color);
    addObject(new MathGrapherObjects::Line(MathVector3D<double>(mbr.minDim(0), mbr.maxDim(1), mbr.minDim(2)), MathVector3D<double>(mbr.minDim(0), mbr.maxDim(1), mbr.maxDim(2))), color);
    addObject(new MathGrapherObjects::Line(MathVector3D<double>(mbr.maxDim(0), mbr.maxDim(1), mbr.minDim(2)), MathVector3D<double>(mbr.maxDim(0), mbr.maxDim(1), mbr.maxDim(2))), color);

}

RTreeViewer::~RTreeViewer()
{

}
