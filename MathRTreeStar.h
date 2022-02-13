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

#ifndef MATHRTREESTAR_H
#define MATHRTREESTAR_H

#include <stddef.h>
#include <string>
#include <array>
#include <functional>
#include <algorithm>
#include "MathMBR.h"

//#define	MATH_RTREE_STAR_DEBUG				/* Включить проверки структуры дерева для отладки */
//#define	MATH_RTREE_STAR_USE_REINSERTING		/* Использовать повторную вставку для оптимизации структуры дерева */

/* Класс, реализующий R*-дерево-контейнер */
template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
class MathRTreeStar
{
protected:
	class DataNode;
public:
	class Node;
	typedef std::function<bool(const MathMBR<NumberType, dims>&)>	PredicateType;

	class iterator;
	class const_iterator;
	/* Итератор, для обхода контейнера, как двусвязного списка */
	class list_iterator
	{
	public:
								list_iterator();
								list_iterator(const list_iterator& it) = default;
								list_iterator(DataNode *root);
		DataType&				operator*();
		DataType*				operator->();
		void					operator++();
		void					operator--();
		bool					operator==(const list_iterator& it) const;
		bool					operator!=(const list_iterator& it) const;
		void					operator=(const list_iterator& it);
		void					operator=(const iterator& it);
	private:
		DataNode				*current;
		friend class iterator;
		friend class MathRTreeStar;
	};
	/* const-итератор, для обхода контейнера, как двусвязного списка */
	class const_list_iterator
	{
	public:
								const_list_iterator();
								const_list_iterator(const const_list_iterator& it) = default;
								const_list_iterator(DataNode *root);
		const DataType&			operator*() const;
		const DataType*			operator->() const;
		void					operator++();
		void					operator--();
		bool					operator==(const const_list_iterator& it) const;
		bool					operator!=(const const_list_iterator& it) const;
		void					operator=(const const_list_iterator& it);
		void					operator=(const const_iterator& it);
	private:
		const DataNode			*current;
		friend class const_iterator;
		friend class MathRTreeStar;
	};
	/* Итератор, для оптимизированного обхода контейнера, как R*-дерева */
	class iterator
	{
	public:
								iterator();
								iterator(const iterator& it);
								iterator(Node *root);
								iterator(Node *node, size_t numChild, const PredicateType& objectPredicate, const PredicateType& nodePredicate);
								iterator(Node *node, size_t numChild, PredicateType&& objectPredicate, PredicateType&& nodePredicate);
								iterator(Node *node, size_t numChild);
								iterator(Node *root, const PredicateType& objectPredicate, const PredicateType& nodePredicate);
								iterator(Node *root, PredicateType&& objectPredicate, PredicateType&& nodePredicate);
		DataType&				operator*();
		DataType*				operator->();
		void					operator++();
		bool					operator==(const iterator& it) const;
		bool					operator==(const list_iterator& it) const;
		bool					operator!=(const iterator& it) const;
		bool					operator!=(const list_iterator& it) const;		/* for range-based cycle */
		void					operator=(const iterator& it);
		Node*&					getNode();
		DataNode*				getDataNode() const;
		const size_t&			getNumChild() const;
	private:
		void					start(Node *current);
		Node*					next(Node *current) const;
		Node*					nextLeaf(Node *current) const;
		Node					*current;
		size_t					numChild;
		PredicateType			objectPredicate;
		PredicateType			nodePredicate;
	};
	/* const-итератор, для оптимизированного обхода контейнера, как R*-дерева */
	class const_iterator
	{
	public:
								const_iterator();
								const_iterator(const const_iterator& it);
								const_iterator(const Node *root);
								const_iterator(const Node *node, size_t numChild, const PredicateType& objectPredicate, const PredicateType& nodePredicate);
								const_iterator(const Node *node, size_t numChild, PredicateType&& objectPredicate, PredicateType&& nodePredicate);
								const_iterator(const Node *node, size_t numChild);
								const_iterator(const Node *root, const PredicateType& objectPredicate, const PredicateType& nodePredicate);
								const_iterator(const Node *root, PredicateType&& objectPredicate, PredicateType&& nodePredicate);
		const DataType&			operator*() const;
		const DataType*			operator->() const;
		void					operator++();
		bool					operator==(const const_iterator& it) const;
		bool					operator==(const const_list_iterator& it) const;
		bool					operator!=(const const_iterator& it) const;
		bool					operator!=(const const_list_iterator& it) const;		/* for range-based cycle */
		void					operator=(const const_iterator& it);
		const Node*&			getNode() const;
		const DataNode*			getDataNode() const;
		const size_t&			getNumChild() const;
	private:
		void					start(const Node *current);
		const Node*				next(const Node *current) const;
		const Node*				nextLeaf(const Node *current) const;
		const Node				*current;
		size_t					numChild;
		PredicateType			objectPredicate;
		PredicateType			nodePredicate;
	};
	/* Класс, реализующий не листовые узлы */
	class Node
	{
	public:
																Node();
																Node(const Node& node);
		bool													attach(Node *child);
		bool													attachLight(Node *child);
		bool													attach(DataNode *child);
		bool													attachLight(DataNode *child);
		bool													detach(const size_t& num);
		void													detachAll();
		template <class NodeType>
		Node*													devide(NodeType *newNode);
		bool													isLeaf() const {return _isLeaf;}
		void													updateMBR();
		void													updateUpMBR();
		NumberType												getOverlapIncrease(const size_t numNode, const MathMBR<NumberType, dims>& _mbr) const;
		NumberType												getOverlapIncrease(const size_t numNode) const;
		MathMBR<NumberType, dims>&								getMBR();
		const MathMBR<NumberType, dims>&						getMBR() const;
		const size_t&											getNumChildren() const;
		const size_t&											getMyChildNumber() const;
		Node													*parent;
		std::array<void*,M>										childs;
		Node													*copy;
	private:
		template <class NodeType>
		static size_t											getNumIndex(std::array<NodeType*, M + 1>& array);
		template <class NodeType>
		static size_t											getNumAxis(std::array<NodeType*, M + 1>& array);
		template <class NodeType>
		static NumberType										getTwoGroupsPerimeter(std::array<NodeType*, M + 1>& array, size_t numAxis);
		template <class NodeType>
		static NumberType										getTwoGroupsIntersection(std::array<NodeType*, M + 1>& array, size_t numIndex);
		MathMBR<NumberType, dims>								mbr;
		size_t													myChildNumber;
		size_t													numChildren;
		bool													_isLeaf;
	};
	/* Класс, реализующий исключения */
	class Exception
	{
	public:
								Exception(const std::string& errorStr): errString(errorStr) {}
	private:
		std::string				errString;
	};
								MathRTreeStar();
								MathRTreeStar(MathRTreeStar&& rtree);
								~MathRTreeStar();
	/* Вставить элемент data в дерево */
	void						insert(const DataType& data);
	/* Вставить элемент data в дерево, используется семантика перемещения */
	void						insert(DataType&& data);
	/* Удалить элемент по ссылке data */
	bool						erase(DataType& data);
	/* Повторно вставить элемент по итератору для списка */
	void						reinsert(list_iterator& it);
	/* Повторно вставить элемент по ссылке на элемент */
	bool						reinsert(DataType& data, const MathMBR<NumberType, dims> &mbr);
	/* Удалить элементы с истинным значением предиката. Используется простой обход всех элементов */
	void						remove_if(const std::function<bool(const DataType&)>& predicate);
	/* Поменять местами текущее дерево и rtreestar */
	void						swap(MathRTreeStar& rtreestar);
	/* Слияние текущего дерева и rtreestar в текущее */
	void						splice(MathRTreeStar& rtreestar);
	/* Заменить текущее дерево rtreestar */
	void						operator=(MathRTreeStar&& rtreestar);
	/* Скопировать rtreestar в текущее дерево */
	void						operator=(const MathRTreeStar& rtreestar);
	/* Поиск простым перебором всех элементов */
	list_iterator				find(DataType &data);
	/* Получение итератора на первый элемент для простого перебора всех элементов */
	list_iterator				begin();
	/* Получение итератора на оптимизированный поиск */
	iterator					begin(const PredicateType& objectPredicate, const PredicateType& nodePredicate);
	/* Получение итератора на оптимизированный поиск, для предикатов используется семантика перемещения */
	iterator					begin(PredicateType&& objectPredicate, PredicateType&& nodePredicate);
	/* Получение итератора на оптимизированный поиск по области region */
	iterator					begin(const MathMBR<NumberType, dims>& region);
	/* Получение итератора на последний элемент для простого перебора всех элементов */
	list_iterator				end() const;
	/* Получение const-итератора на первый элемент для простого перебора всех элементов */
	const_list_iterator			cbegin() const;
	/* Получение const-итератора на оптимизированный поиск */
	const_iterator				cbegin(const PredicateType& objectPredicate, const PredicateType& nodePredicate) const;
	/* Получение const-итератора на оптимизированный поиск, для предикатов используется семантика перемещения */
	const_iterator				cbegin(PredicateType&& objectPredicate, PredicateType&& nodePredicate) const;
	/* Получение итератора на оптимизированный поиск по области mbr */
	const_iterator				cbegin(const MathMBR<NumberType, dims>& mbr) const;
	/* Получение const-итератора на последний элемент для простого перебора всех элементов */
	const_list_iterator			cend() const;
	DataType&					last();
	/* Возвращает число уровней дерева */
	size_t						levels() const;
	/* Возвращает число элементов */
	size_t						size() const;
	/* Контейнер пуст? */
	bool						empty() const;
	/* Очистка дерева */
	void						clear();
	/* Обновить MBR всех элементов */
	void						updateMBRs();
	/* Обновить MBR всех элементов и пересобрать дерево */
	void						rebuild();
	/* Возвращает указатель на корень дерева */
	const Node*					getTop() const {return root;}
	/* Возвращает указатель на следующий узел на данном уровне */
	template <class NodeType>
	static NodeType*			nextInThisRow(NodeType* node);
protected:
	/* Класс, реализующий листовые узлы */
	class DataNode
	{
	public:
																DataNode(const DataType& _data);
																DataNode(const DataNode& node);
																DataNode(DataType&& _data);
		MathMBR<NumberType, dims>&								getMBR();
		const MathMBR<NumberType, dims>&						getMBR() const;
		void													updateMBR();
		Node*													parentNode() const;
		size_t													getMyChildNumber() const;
		DataType												data;
		DataNode												*prev;
		DataNode												*next;
	private:
		MathMBR<NumberType, dims>								mbr;
		size_t													myChildNumber;
		Node													*parent;
		friend class Node;
	};
	void						insert(DataNode& data);
	void						erase(DataNode *data);
	Node*						devideAndAttach(Node *startNode, DataNode *child);
	void						reinsertAndAttach(Node *startNode, DataNode *child);
	Node*						selectLeaf(const MathMBR<NumberType, dims>& mbr) const;
	template <class NodeType>
	static NodeType*			firstLeaf(NodeType *startNode);
#ifdef MATH_RTREE_STAR_DEBUG
	void						checkTree() const;
	void						checkMBRs() const;
	void						checkMBRs(const Node& node) const;
	size_t						getCalculatedSize() const;
	void						checkSize() const;
#endif
	DataNode					*firstDataNode;		/* Указатель на первый элемент двусвязного списка */
	Node						*root;				/* Указатель на корень дерева */
	size_t						numElements;		/* Число элементов в дереве */
	size_t						numLevels;			/* Число уровней в дереве */
};

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
MathRTreeStar<DataType, NumberType, dims, m, M>::list_iterator::list_iterator()
{
	this->current = nullptr;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
MathRTreeStar<DataType, NumberType, dims, m, M>::list_iterator::list_iterator(DataNode *node)
{
	this->current = node;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
DataType& MathRTreeStar<DataType, NumberType, dims, m, M>::list_iterator::operator*()
{
	return current->data;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
DataType* MathRTreeStar<DataType, NumberType, dims, m, M>::list_iterator::operator->()
{
	return &current->data;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
void MathRTreeStar<DataType, NumberType, dims, m, M>::list_iterator::operator++()
{
	if(current != nullptr)
		current = current->next;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
void MathRTreeStar<DataType, NumberType, dims, m, M>::list_iterator::operator--()
{
	if(current->prev != nullptr)
		current = current->prev;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
bool MathRTreeStar<DataType, NumberType, dims, m, M>::list_iterator::operator==(const list_iterator& it) const
{
	return current == it.current;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
bool MathRTreeStar<DataType, NumberType, dims, m, M>::list_iterator::operator!=(const list_iterator& it) const
{
	return current != it.current;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
void MathRTreeStar<DataType, NumberType, dims, m, M>::list_iterator::operator=(const list_iterator& it)
{
	current = it.current;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
void MathRTreeStar<DataType, NumberType, dims, m, M>::list_iterator::operator=(const iterator& it)
{
	current = it.getDataNode();
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
MathRTreeStar<DataType, NumberType, dims, m, M>::const_list_iterator::const_list_iterator()
{
	this->current = nullptr;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
MathRTreeStar<DataType, NumberType, dims, m, M>::const_list_iterator::const_list_iterator(DataNode *node)
{
	this->current = node;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
const DataType& MathRTreeStar<DataType, NumberType, dims, m, M>::const_list_iterator::operator*() const
{
	return current->data;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
const DataType* MathRTreeStar<DataType, NumberType, dims, m, M>::const_list_iterator::operator->() const
{
	return &current->data;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
void MathRTreeStar<DataType, NumberType, dims, m, M>::const_list_iterator::operator++()
{
	if(current != nullptr)
		current = current->next;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
void MathRTreeStar<DataType, NumberType, dims, m, M>::const_list_iterator::operator--()
{
	if(current->prev != nullptr)
		current = current->prev;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
bool MathRTreeStar<DataType, NumberType, dims, m, M>::const_list_iterator::operator==(const const_list_iterator& it) const
{
	return current == it.current;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
void MathRTreeStar<DataType, NumberType, dims, m, M>::const_list_iterator::operator=(const const_iterator& it)
{
	current = it.getDataNode();
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
bool MathRTreeStar<DataType, NumberType, dims, m, M>::const_list_iterator::operator!=(const const_list_iterator& it) const
{
	return current != it.current;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
void MathRTreeStar<DataType, NumberType, dims, m, M>::const_list_iterator::operator=(const const_list_iterator& it)
{
	current = it.current;
}


template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
MathRTreeStar<DataType, NumberType, dims, m, M>::iterator::iterator()
{
	this->current = nullptr;
	this->numChild = 0;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
MathRTreeStar<DataType, NumberType, dims, m, M>::iterator::iterator(const iterator& it)
{
	this->current = it.current;
	this->numChild = it.numChild;
	this->nodePredicate = it.nodePredicate;
	this->objectPredicate = it.objectPredicate;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
MathRTreeStar<DataType, NumberType, dims, m, M>::iterator::iterator(Node *root)
{
	this->current = firstLeaf(root);
	this->numChild = 0;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
MathRTreeStar<DataType, NumberType, dims, m, M>::iterator::iterator(Node *node, size_t numChild, const PredicateType& objectPredicate, const PredicateType& nodePredicate)
{
	this->objectPredicate = objectPredicate;
	this->nodePredicate = nodePredicate;
	this->current = node;
	this->numChild = numChild;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
MathRTreeStar<DataType, NumberType, dims, m, M>::iterator::iterator(Node *node, size_t numChild, PredicateType&& objectPredicate, PredicateType&& nodePredicate)
{
	this->objectPredicate = std::move(objectPredicate);
	this->nodePredicate = std::move(nodePredicate);
	this->current = node;
	this->numChild = numChild;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
MathRTreeStar<DataType, NumberType, dims, m, M>::iterator::iterator(Node *node, size_t numChild)
{
	this->current = node;
	this->numChild = numChild;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
MathRTreeStar<DataType, NumberType, dims, m, M>::iterator::iterator(Node *root, const PredicateType& objectPredicate, const PredicateType& nodePredicate)
{
	this->objectPredicate = objectPredicate;
	this->nodePredicate = nodePredicate;

	start(root);
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
MathRTreeStar<DataType, NumberType, dims, m, M>::iterator::iterator(Node *root, PredicateType&& objectPredicate, PredicateType&& nodePredicate)
{
	this->objectPredicate = std::move(objectPredicate);
	this->nodePredicate = std::move(nodePredicate);

	start(root);
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
DataType& MathRTreeStar<DataType, NumberType, dims, m, M>::iterator::operator*()
{
	return static_cast<DataNode*>(current->childs[numChild])->data;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
DataType* MathRTreeStar<DataType, NumberType, dims, m, M>::iterator::operator->()
{
	return &static_cast<DataNode*>(current->childs[numChild])->data;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
void MathRTreeStar<DataType, NumberType, dims, m, M>::iterator::operator++()
{
	Node*			current = this->current;

	/* попытка найти подходящий объект в листе */
	if(objectPredicate)
	{
		for(size_t i = numChild + 1; i < current->getNumChildren(); i++)
			if(objectPredicate(static_cast<DataNode*>(current->childs[i])->getMBR()))
			{
				numChild = i;
				return;
			}
	}
	else if(numChild + 1 < current->getNumChildren())
	{
		numChild++;
		return;
	}
	if(nodePredicate)
	{
		do
		{
			/* попытка не удалсь - движение вверх по дереву */
			current = nextLeaf(current);
			if(current == nullptr) break;
			for(size_t i = 0; i < current->getNumChildren(); i++)
				if(objectPredicate(static_cast<DataNode*>(current->childs[i])->getMBR()))
				{
					this->current = current;
					numChild = i;
					return;
				}
		}
		while(1);
	}
	else
	{
		numChild = 0;
		this->current = nextInThisRow(current);
		return;
	}
	this->current = nullptr;
	numChild = 0;
	return;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
bool MathRTreeStar<DataType, NumberType, dims, m, M>::iterator::operator==(const iterator& it) const
{
	return current == it.current && numChild == it.numChild;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
bool MathRTreeStar<DataType, NumberType, dims, m, M>::iterator::operator==(const list_iterator& it) const
{
	return static_cast<void*>(current) == static_cast<void*>(it.current);
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
bool MathRTreeStar<DataType, NumberType, dims, m, M>::iterator::operator!=(const iterator& it) const
{
	return current != it.current || numChild != it.numChild;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
bool MathRTreeStar<DataType, NumberType, dims, m, M>::iterator::operator!=(const list_iterator& it) const
{
	return static_cast<void*>(current) != static_cast<void*>(it.current);
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
void MathRTreeStar<DataType, NumberType, dims, m, M>::iterator::operator=(const iterator& it)
{
	current = it.current;
	numChild = it.numChild;
	objectPredicate = it.objectPredicate;
	nodePredicate = it.nodePredicate;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
typename MathRTreeStar<DataType, NumberType, dims, m, M>::Node*& MathRTreeStar<DataType, NumberType, dims, m, M>::iterator::getNode()
{
	return current;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
typename MathRTreeStar<DataType, NumberType, dims, m, M>::DataNode* MathRTreeStar<DataType, NumberType, dims, m, M>::iterator::getDataNode() const
{
	return static_cast<DataNode*>(current->childs[numChild]);
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
const size_t& MathRTreeStar<DataType, NumberType, dims, m, M>::iterator::getNumChild() const
{
	return numChild;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
void MathRTreeStar<DataType, NumberType, dims, m, M>::iterator::start(Node* node)
{
	Node*			current = node;
	size_t			i;

	while(!current->isLeaf())
	{
		for(i = 0; i < current->getNumChildren(); i++)
		{
			if(nodePredicate(static_cast<Node*>(current->childs[i])->getMBR()))
				break;
		}
		if(i == current->getNumChildren()) break;
		else current = static_cast<Node*>(current->childs[i]);
	}

	if(!current->isLeaf())
	{
		current = nextLeaf(current);
		if(current == nullptr) goto end;
	}

	do
	{
		for(size_t i = 0; i < current->getNumChildren(); i++)
			if(objectPredicate(static_cast<DataNode*>(current->childs[i])->getMBR()))
			{
				this->current = current;
				numChild = i;
				return;
			}
		current = nextLeaf(current);
		if(current == nullptr) break;
	}
	while(1);
end:
	this->current = nullptr;
	numChild = 0;
	return;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
typename MathRTreeStar<DataType, NumberType, dims, m, M>::Node* MathRTreeStar<DataType, NumberType, dims, m, M>::iterator::next(Node *current) const
{
	bool			changed = false;

	/* движение вверх-вправо по дереву... */
	do
	{
		if(current->parent == nullptr)
		{
			/* больше объектов не найдено - установка конечного итератора */
			return nullptr;
		}
		size_t		currentNumChild = current->getMyChildNumber();
		current = current->parent;
		for(size_t i = currentNumChild + 1; i < current->getNumChildren(); i++)
		{
			if(nodePredicate(static_cast<Node*>(current->childs[i])->getMBR()))
			{
				current = static_cast<Node*>(current->childs[i]);
				changed = true;
				break;
			}
		}
	}
	while(!changed);
	/* ...а затем снова вниз */
	while(!current->isLeaf())
	{
		size_t		i;
		for(i = 0; i < current->getNumChildren(); i++)
		{
			if(nodePredicate(static_cast<Node*>(current->childs[i])->getMBR()))
				break;
		}
		if(i == current->getNumChildren()) break;
		else current = static_cast<Node*>(current->childs[i]);
	}

	return current;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
typename MathRTreeStar<DataType, NumberType, dims, m, M>::Node* MathRTreeStar<DataType, NumberType, dims, m, M>::iterator::nextLeaf(Node *current) const
{
	do
	{
		current = next(current);
		if(current == nullptr) return nullptr;
	}
	while(!current->isLeaf());

	return current;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
MathRTreeStar<DataType, NumberType, dims, m, M>::const_iterator::const_iterator()
{
	this->current = nullptr;
	this->numChild = 0;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
MathRTreeStar<DataType, NumberType, dims, m, M>::const_iterator::const_iterator(const const_iterator& it)
{
	this->current = it.current;
	this->numChild = it.numChild;
	this->nodePredicate = it.nodePredicate;
	this->objectPredicate = it.objectPredicate;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
MathRTreeStar<DataType, NumberType, dims, m, M>::const_iterator::const_iterator(const Node *root)
{
	this->current = firstLeaf(root);
	this->numChild = 0;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
MathRTreeStar<DataType, NumberType, dims, m, M>::const_iterator::const_iterator(const Node *node, size_t numChild, const PredicateType& objectPredicate, const PredicateType& nodePredicate)
{
	this->objectPredicate = objectPredicate;
	this->nodePredicate = nodePredicate;
	this->current = node;
	this->numChild = numChild;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
MathRTreeStar<DataType, NumberType, dims, m, M>::const_iterator::const_iterator(const Node *node, size_t numChild)
{
	this->current = node;
	this->numChild = numChild;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
MathRTreeStar<DataType, NumberType, dims, m, M>::const_iterator::const_iterator(const Node *root, const PredicateType& objectPredicate, const PredicateType& nodePredicate)
{
	this->objectPredicate = objectPredicate;
	this->nodePredicate = nodePredicate;

	start(root);
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
MathRTreeStar<DataType, NumberType, dims, m, M>::const_iterator::const_iterator(const Node *root, PredicateType&& objectPredicate, PredicateType&& nodePredicate)
{
	this->objectPredicate = std::move(objectPredicate);
	this->nodePredicate = std::move(nodePredicate);

	start(root);
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
const DataType& MathRTreeStar<DataType, NumberType, dims, m, M>::const_iterator::operator*() const
{
	return static_cast<const DataNode*>(current->childs[numChild])->data;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
const DataType* MathRTreeStar<DataType, NumberType, dims, m, M>::const_iterator::operator->() const
{
	return &static_cast<const DataNode*>(current->childs[numChild])->data;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
void MathRTreeStar<DataType, NumberType, dims, m, M>::const_iterator::operator++()
{
	const Node*			current = this->current;

	/* попытка найти подходящий объект в листе */
	if(objectPredicate)
	{
		for(size_t i = numChild + 1; i < current->getNumChildren(); i++)
			if(objectPredicate(static_cast<const DataNode*>(current->childs[i])->getMBR()))
			{
				numChild = i;
				return;
			}
	}
	else if(numChild + 1 < current->getNumChildren())
	{
		numChild++;
		return;
	}
	if(nodePredicate)
	{
		do
		{
			/* попытка не удалсь - движение вверх по дереву */
			current = nextLeaf(current);
			if(current == nullptr) break;
			for(size_t i = 0; i < current->getNumChildren(); i++)
				if(objectPredicate(static_cast<const DataNode*>(current->childs[i])->getMBR()))
				{
					this->current = current;
					numChild = i;
					return;
				}
		}
		while(1);
	}
	else
	{
		numChild = 0;
		this->current = nextInThisRow(current);
		return;
	}
	this->current = nullptr;
	numChild = 0;
	return;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
bool MathRTreeStar<DataType, NumberType, dims, m, M>::const_iterator::operator==(const const_iterator& it) const
{
	return current == it.current && numChild == it.numChild;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
bool MathRTreeStar<DataType, NumberType, dims, m, M>::const_iterator::operator==(const const_list_iterator& it) const
{
	return static_cast<const void*>(current) == static_cast<const void*>(it.current);
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
bool MathRTreeStar<DataType, NumberType, dims, m, M>::const_iterator::operator!=(const const_iterator& it) const
{
	return current != it.current || numChild != it.numChild;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
bool MathRTreeStar<DataType, NumberType, dims, m, M>::const_iterator::operator!=(const const_list_iterator& it) const
{
	return static_cast<const void*>(current) != static_cast<const void*>(it.current);
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
void MathRTreeStar<DataType, NumberType, dims, m, M>::const_iterator::operator=(const const_iterator& it)
{
	current = it.current;
	numChild = it.numChild;
	objectPredicate = it.objectPredicate;
	nodePredicate = it.nodePredicate;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
const typename MathRTreeStar<DataType, NumberType, dims, m, M>::Node*& MathRTreeStar<DataType, NumberType, dims, m, M>::const_iterator::getNode() const
{
	return current;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
const size_t& MathRTreeStar<DataType, NumberType, dims, m, M>::const_iterator::getNumChild() const
{
	return numChild;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
void MathRTreeStar<DataType, NumberType, dims, m, M>::const_iterator::start(const Node* node)
{
	const Node*			current = node;
	size_t				i;

	while(!current->isLeaf())
	{
		for(i = 0; i < current->getNumChildren(); i++)
		{
			if(nodePredicate(static_cast<const Node*>(current->childs[i])->getMBR()))
				break;
		}
		if(i == current->getNumChildren()) break;
		else current = static_cast<const Node*>(current->childs[i]);
	}

	if(!current->isLeaf())
	{
		current = nextLeaf(current);
		if(current == nullptr) goto end;
	}

	do
	{
		for(size_t i = 0; i < current->getNumChildren(); i++)
			if(objectPredicate(static_cast<const DataNode*>(current->childs[i])->getMBR()))
			{
				this->current = current;
				numChild = i;
				return;
			}
		current = nextLeaf(current);
		if(current == nullptr) break;
	}
	while(1);
end:
	this->current = nullptr;
	numChild = 0;
	return;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
const typename MathRTreeStar<DataType, NumberType, dims, m, M>::Node* MathRTreeStar<DataType, NumberType, dims, m, M>::const_iterator::next(const Node *current) const
{
	bool			changed = false;

	/* движение вверх-вправо по дереву... */
	do
	{
		if(current->parent == nullptr)
		{
			/* больше объектов не найдено - установка конечного итератора */
			return nullptr;
		}
		size_t		currentNumChild = current->getMyChildNumber();
		current = current->parent;
		for(size_t i = currentNumChild + 1; i < current->getNumChildren(); i++)
		{
			if(nodePredicate(static_cast<const Node*>(current->childs[i])->getMBR()))
			{
				current = static_cast<const Node*>(current->childs[i]);
				changed = true;
				break;
			}
		}
	}
	while(!changed);
	/* ...а затем снова вниз */
	while(!current->isLeaf())
	{
		size_t		i;
		for(i = 0; i < current->getNumChildren(); i++)
		{
			if(nodePredicate(static_cast<const Node*>(current->childs[i])->getMBR()))
				break;
		}
		if(i == current->getNumChildren()) break;
		else current = static_cast<const Node*>(current->childs[i]);
	}

	return current;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
const typename MathRTreeStar<DataType, NumberType, dims, m, M>::Node* MathRTreeStar<DataType, NumberType, dims, m, M>::const_iterator::nextLeaf(const Node *current) const
{
	do
	{
		current = next(current);
		if(current == nullptr) return nullptr;
	}
	while(!current->isLeaf());

	return current;
}

/* Node */
template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
MathRTreeStar<DataType, NumberType, dims, m, M>::Node::Node(): mbr()
{
	parent = nullptr;
	numChildren = 0;
	myChildNumber = 0;
	_isLeaf = false;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
MathRTreeStar<DataType, NumberType, dims, m, M>::Node::Node(const Node& node): mbr(node.mbr), _isLeaf(node._isLeaf)
{
	parent = nullptr;
	numChildren = 0;
	myChildNumber = 0;
	const_cast<Node&>(node).copy = this;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
bool MathRTreeStar<DataType, NumberType, dims, m, M>::Node::attach(Node *child)
{
	if(numChildren == M) return false;
	child->parent = this;
	childs[numChildren] = child;
	child->myChildNumber = numChildren;
	numChildren++;
	_isLeaf = false;
	mbr += child->getMBR();
	return true;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
bool MathRTreeStar<DataType, NumberType, dims, m, M>::Node::attachLight(Node *child)
{
	if(numChildren == M) return false;
	child->parent = this;
	childs[numChildren] = child;
	child->myChildNumber = numChildren;
	numChildren++;
	_isLeaf = false;
	return true;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
bool MathRTreeStar<DataType, NumberType, dims, m, M>::Node::attach(DataNode *child)
{
	if(numChildren == M) return false;
	child->parent = this;
	childs[numChildren] = child;
	child->myChildNumber = numChildren;
	numChildren++;
	_isLeaf = true;
	mbr += child->data.getMBR();
	return true;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
bool MathRTreeStar<DataType, NumberType, dims, m, M>::Node::attachLight(DataNode *child)
{
	if(numChildren == M) return false;
	child->parent = this;
	childs[numChildren] = child;
	child->myChildNumber = numChildren;
	numChildren++;
	_isLeaf = true;
	return true;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
bool MathRTreeStar<DataType, NumberType, dims, m, M>::Node::detach(const size_t& num)
{
	if(num >= numChildren) return false;
	if(!_isLeaf) static_cast<Node*>(childs[num])->parent = nullptr;
	numChildren--;
	if(!_isLeaf)
		for(size_t i = num; i < numChildren; i++)
		{
			childs[i] = childs[i + 1];
			static_cast<Node*>(childs[i])->myChildNumber = i;
		}
	else
		for(size_t i = num; i < numChildren; i++)
		{
			childs[i] = childs[i + 1];
			static_cast<DataNode*>(childs[i])->myChildNumber = i;
		}
	updateMBR();
	return true;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
void MathRTreeStar<DataType, NumberType, dims, m, M>::Node::detachAll()
{
	numChildren = 0;
	_isLeaf = false;
	mbr.clear();
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
template <class NodeType>
typename MathRTreeStar<DataType, NumberType, dims, m, M>::Node* MathRTreeStar<DataType, NumberType, dims, m, M>::Node::devide(NodeType *newNode)
{
	Node								*createdNode;
	std::array<NodeType*, M + 1>		unbalancedChilds;

	for(size_t i = 0; i < M; i++)
		unbalancedChilds[i] = static_cast<NodeType*>(childs[i]);
	unbalancedChilds[M] = newNode;

	detachAll();

	size_t		numAxis = getNumAxis<NodeType>(unbalancedChilds);
	std::sort(unbalancedChilds.begin(), unbalancedChilds.end(), [&numAxis](NodeType *element1, NodeType *element2)->bool
	{
		if(element1->getMBR().minDim(numAxis) != element2->getMBR().minDim(numAxis))
			return element1->getMBR().minDim(numAxis) > element2->getMBR().minDim(numAxis);
		else return element1->getMBR().maxDim(numAxis) > element2->getMBR().maxDim(numAxis);
	});

	size_t		numIndex = getNumIndex<NodeType>(unbalancedChilds);
	createdNode = new Node();

	for(size_t i = 0; i < m + numIndex; i++)
		attach(unbalancedChilds[i]);
	for(size_t i = m + numIndex; i < unbalancedChilds.size(); i++)
		createdNode->attach(unbalancedChilds[i]);

	return createdNode;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
void MathRTreeStar<DataType, NumberType, dims, m, M>::Node::updateMBR()
{
	mbr.clear();
	if(!_isLeaf)
		for(size_t i = 0; i < numChildren; i++)
			mbr += static_cast<Node*>(childs[i])->getMBR();
	else
		for(size_t i = 0; i < numChildren; i++)
			mbr += static_cast<DataNode*>(childs[i])->getMBR();
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
void MathRTreeStar<DataType, NumberType, dims, m, M>::Node::updateUpMBR()
{
	for(auto node = this->parent; node != nullptr; node = node->parent)
		node->updateMBR();
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
NumberType MathRTreeStar<DataType, NumberType, dims, m, M>::Node::getOverlapIncrease(const size_t numNode) const
{
	MathMBR<NumberType, dims>		&mbr = static_cast<Node*>(childs[numNode])->getMBR();
	NumberType								overlap = NumberType(0);

	if(!_isLeaf)
	{
		for(size_t i = 0; i < numChildren; i++)
			if(i != numNode)
				overlap += mbr.overlapVolume(static_cast<Node*>(childs[i])->getMBR());
	}
	else
	{
		for(size_t i = 0; i < numChildren; i++)
			if(i != numNode)
				overlap += mbr.overlapVolume(static_cast<DataNode*>(childs[i])->getMBR());
	}
	return overlap;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
NumberType MathRTreeStar<DataType, NumberType, dims, m, M>::Node::getOverlapIncrease(const size_t numNode, const MathMBR<NumberType, dims>& _mbr) const
{
	MathMBR<NumberType, dims>		overlapMBR = static_cast<Node*>(childs[numNode])->getMBR() + _mbr;
	NumberType								overlap = NumberType(0);

	if(!_isLeaf)
	{
		for(size_t i = 0; i < numChildren; i++)
			if(i != numNode)
				overlap += overlapMBR.overlapVolume(static_cast<Node*>(childs[i])->getMBR());
	}
	else
	{
		for(size_t i = 0; i < numChildren; i++)
			if(i != numNode)
				overlap += overlapMBR.overlapVolume(static_cast<DataNode*>(childs[i])->getMBR());
	}
	return overlap;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
MathMBR<NumberType, dims>& MathRTreeStar<DataType, NumberType, dims, m, M>::Node::getMBR()
{
	return mbr;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
const MathMBR<NumberType, dims>& MathRTreeStar<DataType, NumberType, dims, m, M>::Node::getMBR() const
{
	return mbr;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
const size_t& MathRTreeStar<DataType, NumberType, dims, m, M>::Node::getNumChildren() const
{
	return numChildren;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
const size_t& MathRTreeStar<DataType, NumberType, dims, m, M>::Node::getMyChildNumber() const
{
	return myChildNumber;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
template <class NodeType>
size_t MathRTreeStar<DataType, NumberType, dims, m, M>::Node::getNumIndex(std::array<NodeType*, M + 1>& array)
{
	size_t		ret = 0;
	NumberType	square = getTwoGroupsIntersection<NodeType>(array, ret);

	for(size_t numIndex = 0; numIndex < M - 2*m + 1; numIndex++)
	{
		NumberType	currentSquare = getTwoGroupsIntersection<NodeType>(array, numIndex);

		if(currentSquare < square)
		{
			square = currentSquare;
			ret = numIndex;
		}
	}

	return ret;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
template <class NodeType>
size_t MathRTreeStar<DataType, NumberType, dims, m, M>::Node::getNumAxis(std::array<NodeType*, M + 1>& array)
{
	size_t		ret = 0;
	NumberType	perimeter = getTwoGroupsPerimeter<NodeType>(array, ret);

	for(size_t numAxis = 1; numAxis < dims; numAxis++)
	{
		NumberType	currentPerimeter = getTwoGroupsPerimeter<NodeType>(array, numAxis);

		if(currentPerimeter < perimeter)
		{
			perimeter = currentPerimeter;
			ret = numAxis;
		}
	}

	return ret;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
template <class NodeType>
NumberType MathRTreeStar<DataType, NumberType, dims, m, M>::Node::getTwoGroupsPerimeter(std::array<NodeType *, M + 1> &array, size_t numAxis)
{
	NumberType								minPerimeter = NumberType(0);
	NumberType								perimeter;
	MathMBR<NumberType, dims>		mbr1, mbr2;

	std::sort(array.begin(), array.end(), [&numAxis](NodeType *element1, NodeType *element2)->bool
	{
		if(element1->getMBR().minDim(numAxis) != element2->getMBR().minDim(numAxis))
			return element1->getMBR().minDim(numAxis) > element2->getMBR().minDim(numAxis);
		else return element1->getMBR().maxDim(numAxis) > element2->getMBR().maxDim(numAxis);
	});

	for(size_t j = 0; j < m; j++)
		mbr1 += array[j]->getMBR();
	for(size_t j = m; j < array.size(); j++)
		mbr2 += array[j]->getMBR();
	minPerimeter = mbr1.perimeter() + mbr2.perimeter();

	for(size_t i = m + 1; i < M - m; i++)
	{
		mbr1.clear();
		mbr2.clear();
		for(size_t j = 0; j < i; j++)
			mbr1 += array[j]->getMBR();
		for(size_t j = i; j < array.size(); j++)
			mbr2 += array[j]->getMBR();
		perimeter = mbr1.perimeter() + mbr2.perimeter();
		if(perimeter < minPerimeter) minPerimeter = perimeter;
	}

	return minPerimeter;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
template <class NodeType>
NumberType MathRTreeStar<DataType, NumberType, dims, m, M>::Node::getTwoGroupsIntersection(std::array<NodeType *, M + 1> &array, size_t numIndex)
{
	MathMBR<NumberType, dims>				mbr1, mbr2;

	for (size_t j = 0; j < m + numIndex; j++)
		mbr1 += array[j]->getMBR();
	for (size_t j = m + numIndex; j < array.size(); j++)
		mbr2 += array[j]->getMBR();

	return mbr1.overlapVolume(mbr2);

}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
MathRTreeStar<DataType, NumberType, dims, m, M>::DataNode::DataNode(const DataType& _data): data(_data)
{
	parent = nullptr;
	prev = nullptr;
	next = nullptr;
	myChildNumber = 0;
	mbr = data.getMBR();
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
MathRTreeStar<DataType, NumberType, dims, m, M>::DataNode::DataNode(const DataNode &node): data(node.data), mbr(node.mbr)
{
	parent = nullptr;
	prev = nullptr;
	next = nullptr;
	myChildNumber = 0;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
MathRTreeStar<DataType, NumberType, dims, m, M>::DataNode::DataNode(DataType&& _data): data(std::move(_data))
{
	parent = nullptr;
	prev = nullptr;
	next = nullptr;
	myChildNumber = 0;
	mbr = data.getMBR();
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
MathMBR<NumberType, dims>& MathRTreeStar<DataType, NumberType, dims, m, M>::DataNode::getMBR()
{
	return mbr;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
const MathMBR<NumberType, dims>& MathRTreeStar<DataType, NumberType, dims, m, M>::DataNode::getMBR() const
{
	return mbr;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
void MathRTreeStar<DataType, NumberType, dims, m, M>::DataNode::updateMBR()
{
	mbr = data.getMBR();
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
typename MathRTreeStar<DataType, NumberType, dims, m, M>::Node* MathRTreeStar<DataType, NumberType, dims, m, M>::DataNode::parentNode() const
{
	return parent;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
size_t MathRTreeStar<DataType, NumberType, dims, m, M>::DataNode::getMyChildNumber() const
{
	return myChildNumber;
}

/* class MathRTreeStar */
template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
MathRTreeStar<DataType, NumberType, dims, m, M>::MathRTreeStar()
{
	root = nullptr;
	firstDataNode = nullptr;
	numElements = 0;
	numLevels = 0;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
MathRTreeStar<DataType, NumberType, dims, m, M>::MathRTreeStar(MathRTreeStar &&rtree)
{
	root = rtree.root;
	firstDataNode = rtree.firstDataNode;
	numElements = rtree.numElements;
	numLevels = rtree.numLevels;

	rtree.root = nullptr;
	rtree.firstDataNode = 0;
	rtree.numElements = 0;
	rtree.numLevels = 0;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
MathRTreeStar<DataType, NumberType, dims, m, M>::~MathRTreeStar()
{
	clear();
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
void MathRTreeStar<DataType, NumberType, dims, m, M>::insert(const DataType& newData)
{
	DataNode		*newDataNode = new DataNode(newData);

	newDataNode->prev = nullptr;
	newDataNode->next = firstDataNode;
	if(firstDataNode != nullptr) firstDataNode->prev = newDataNode;
	firstDataNode = newDataNode;
	insert(*newDataNode);
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
void MathRTreeStar<DataType, NumberType, dims, m, M>::insert(DataType&& newData)
{
	DataNode		*newDataNode = new DataNode(std::move(newData));

	newDataNode->prev = nullptr;
	newDataNode->next = firstDataNode;
	if(firstDataNode != nullptr) firstDataNode->prev = newDataNode;
	firstDataNode = newDataNode;
	insert(*newDataNode);
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
bool MathRTreeStar<DataType, NumberType, dims, m, M>::erase(DataType& data)
{
	iterator									deleted;
	const MathMBR<NumberType, dims>	&mbrRegion = data.getMBR();

	for(auto it = begin([&mbrRegion](const MathMBR<NumberType, dims>& mbrObjectNode){return  mbrRegion == mbrObjectNode;},
						[&mbrRegion](const MathMBR<NumberType, dims>& mbrListNode){return mbrListNode.isInside(mbrRegion);}); it != end(); ++it)
	{
		if(&*it == &data)
		{
			deleted = it;
			break;
		}
	}
	if(deleted == end()) return false;

	DataNode		*node = static_cast<DataNode*>(deleted.getNode()->childs[deleted.getNumChild()]);
	erase(node);
	if(node->prev != nullptr) node->prev->next = node->next;
	if(node->next != nullptr) node->next->prev = node->prev;
	if(node == firstDataNode) firstDataNode = node->next;
	delete node;
	return true;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
void MathRTreeStar<DataType, NumberType, dims, m, M>::reinsert(list_iterator& it)
{
	erase(it.current);
	it.current->updateMBR();
	insert(*it.current);
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
bool MathRTreeStar<DataType, NumberType, dims, m, M>::reinsert(DataType& data, const MathMBR<NumberType, dims> &mbr)
{
	iterator									deleted;

	for(auto it = begin([&mbr](const MathMBR<NumberType, dims>& mbrObjectNode){return  mbr == mbrObjectNode;},
						[&mbr](const MathMBR<NumberType, dims>& mbrListNode){return mbrListNode.isInside(mbr);}); it != end(); ++it)
	{
		if(&*it == &data)
		{
			deleted = it;
			break;
		}
	}
	if(deleted == end()) return false;

	DataNode		*node = static_cast<DataNode*>(deleted.getNode()->childs[deleted.getNumChild()]);
	erase(node);
	node->updateMBR();
	insert(*node);
	return true;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
void MathRTreeStar<DataType, NumberType, dims, m, M>::remove_if(const std::function<bool (const DataType &)> &predicate)
{
	for(auto node = firstDataNode; node != nullptr;)
	{
		DataNode		*nextNode = node->next;
		if(predicate(node->data))
		{
			erase(node);
			if(node->prev != nullptr) node->prev->next = node->next;
			if(node->next != nullptr) node->next->prev = node->prev;
			if(node == firstDataNode) firstDataNode = node->next;
			delete node;
		}
		node = nextNode;
	}
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
void MathRTreeStar<DataType, NumberType, dims, m, M>::swap(MathRTreeStar& rtreestar)
{
	std::swap(root, rtreestar.root);
	std::swap(firstDataNode, rtreestar.firstDataNode);
	std::swap(numElements, rtreestar.numElements);
	std::swap(numLevels, rtreestar.numLevels);
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
void MathRTreeStar<DataType, NumberType, dims, m, M>::splice(MathRTreeStar& rtreestar)
{
	Node		*first = firstLeaf(rtreestar.root);
	DataNode	*endDataNode = firstDataNode;

	if(first == nullptr) return;

	for(auto node = rtreestar.firstDataNode; node != nullptr; node = node->next)
		insert(*node);

	if(endDataNode != nullptr)
	{
		while(endDataNode->next != nullptr) endDataNode = endDataNode->next;
		endDataNode->next = rtreestar.firstDataNode;
		rtreestar.firstDataNode->prev = endDataNode;
	}
	else firstDataNode = rtreestar.firstDataNode;

	while(first->parent != nullptr)
	{
		first = first->parent;
		for(auto node = first; node != nullptr; node = nextInThisRow(node))
			for(size_t i = 0; i < node->getNumChildren(); i++)
				delete static_cast<Node*>(node->childs[i]);
	}
	delete first;
	rtreestar.root = nullptr;
	rtreestar.firstDataNode = nullptr;
	rtreestar.numElements = 0;
	rtreestar.numLevels = 0;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
void MathRTreeStar<DataType, NumberType, dims, m, M>::operator=(MathRTreeStar&& rtreestar)
{
	clear();
	root = rtreestar.root;
	firstDataNode = rtreestar.firstDataNode;
	numElements = rtreestar.numElements;
	numLevels = rtreestar.numLevels;
	rtreestar.root = nullptr;
	rtreestar.firstDataNode = nullptr;
	rtreestar.numElements = 0;
	rtreestar.numLevels = 0;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
void MathRTreeStar<DataType, NumberType, dims, m, M>::operator=(const MathRTreeStar& rtreestar)
{
	clear();
	if(rtreestar.root == nullptr) return;
	root = new Node(*rtreestar.root);
	Node		*current = static_cast<Node*>(rtreestar.root);
	do
	{
		current = static_cast<Node*>(current->childs[0]);
		for(auto node = current; node != nullptr; node = rtreestar.nextInThisRow(node))
		{
			Node	*newNode = new Node(*node);

			node->parent->copy->attachLight(newNode);
		}
	}
	while(!current->isLeaf());

	firstDataNode = new DataNode(*rtreestar.firstDataNode);
	rtreestar.firstDataNode->parentNode()->copy->attachLight(firstDataNode);

	DataNode		*dataPrev = firstDataNode;
	for(auto node = rtreestar.firstDataNode->next; node != nullptr; node = node->next)
	{
		DataNode		*dataCurrent = new DataNode(*node);

		node->parentNode()->copy->attachLight(dataCurrent);
		dataPrev->next = dataCurrent;
		dataCurrent->prev = dataPrev;
		dataPrev = dataCurrent;
	}

	numElements = rtreestar.numElements;
	numLevels = rtreestar.numLevels;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
typename MathRTreeStar<DataType, NumberType, dims, m, M>::list_iterator MathRTreeStar<DataType, NumberType, dims, m, M>::find(DataType &data)
{
	list_iterator									found;
	const MathMBR<NumberType, dims>		&mbrRegion = data.getMBR();

	for(auto it = begin([&mbrRegion](const MathMBR<NumberType, dims>& mbrObjectNode){return  mbrRegion == mbrObjectNode;},
						[&mbrRegion](const MathMBR<NumberType, dims>& mbrListNode){return mbrListNode.isInside(mbrRegion);}); it != end(); ++it)
	{
		if(&*it == &data)
		{
			found = it;
			break;
		}
	}
	return found;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
typename MathRTreeStar<DataType, NumberType, dims, m, M>::list_iterator MathRTreeStar<DataType, NumberType, dims, m, M>::begin()
{
	return list_iterator(firstDataNode);
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
typename MathRTreeStar<DataType, NumberType, dims, m, M>::iterator MathRTreeStar<DataType, NumberType, dims, m, M>::begin(const PredicateType& objectPredicate, const PredicateType& nodePredicate)
{
	return iterator(root, objectPredicate, nodePredicate);
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
typename MathRTreeStar<DataType, NumberType, dims, m, M>::iterator MathRTreeStar<DataType, NumberType, dims, m, M>::begin(PredicateType&& objectPredicate, PredicateType&& nodePredicate)
{
	return iterator(root, std::move(objectPredicate), std::move(nodePredicate));
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
typename MathRTreeStar<DataType, NumberType, dims, m, M>::iterator MathRTreeStar<DataType, NumberType, dims, m, M>::begin(const MathMBR<NumberType, dims>& region)
{
	return begin([&region](const MathMBR<double, dims>& mbr) {return region.isIntersect(mbr);}, [&region](const MathMBR<double, dims>& mbr) {return region.isIntersect(mbr);});
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
typename MathRTreeStar<DataType, NumberType, dims, m, M>::list_iterator MathRTreeStar<DataType, NumberType, dims, m, M>::end() const
{
	return list_iterator(nullptr);
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
typename MathRTreeStar<DataType, NumberType, dims, m, M>::const_list_iterator MathRTreeStar<DataType, NumberType, dims, m, M>::cbegin() const
{
	return const_list_iterator(firstDataNode);
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
typename MathRTreeStar<DataType, NumberType, dims, m, M>::const_iterator MathRTreeStar<DataType, NumberType, dims, m, M>::cbegin(const PredicateType& objectPredicate, const PredicateType& nodePredicate) const
{
	return const_iterator(root, objectPredicate, nodePredicate);
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
typename MathRTreeStar<DataType, NumberType, dims, m, M>::const_iterator MathRTreeStar<DataType, NumberType, dims, m, M>::cbegin(PredicateType&& objectPredicate, PredicateType&& nodePredicate) const
{
	return const_iterator(root, std::move(objectPredicate), std::move(nodePredicate));
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
typename MathRTreeStar<DataType, NumberType, dims, m, M>::const_iterator MathRTreeStar<DataType, NumberType, dims, m, M>::cbegin(const MathMBR<NumberType, dims>& region) const
{
	return cbegin([&region](const MathMBR<double, dims>& mbr) {return region.isIntersect(mbr);}, [&region](const MathMBR<double, dims>& mbr) {return region.isIntersect(mbr);});
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
typename MathRTreeStar<DataType, NumberType, dims, m, M>::const_list_iterator MathRTreeStar<DataType, NumberType, dims, m, M>::cend() const
{
	return const_list_iterator(nullptr);
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
DataType& MathRTreeStar<DataType, NumberType, dims, m, M>::last()
{
	return firstDataNode->data;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
size_t MathRTreeStar<DataType, NumberType, dims, m, M>::levels() const
{
	return numLevels;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
size_t MathRTreeStar<DataType, NumberType, dims, m, M>::size() const
{
	return numElements;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
bool MathRTreeStar<DataType, NumberType, dims, m, M>::empty() const
{
	return numElements == 0;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
void MathRTreeStar<DataType, NumberType, dims, m, M>::clear()
{
	Node		*first = firstLeaf(root);

	if(first == nullptr) return;

	for(auto node = firstDataNode; node != nullptr;)
	{
		DataNode		*nextNode = node->next;
		delete node;
		node = nextNode;
	}

	while(first->parent != nullptr)
	{
		first = first->parent;
		for(auto node = first; node != nullptr; node = nextInThisRow(node))
			for(size_t i = 0; i < node->getNumChildren(); i++)
				delete static_cast<Node*>(node->childs[i]);
	}
	delete first;

	root = nullptr;
	firstDataNode = nullptr;
	numElements = 0;
	numLevels = 0;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
void MathRTreeStar<DataType, NumberType, dims, m, M>::updateMBRs()
{
	Node		*first = firstLeaf(root);

	for(auto node = firstDataNode; node != nullptr; node = node->next)
		node->updateMBR();

	if(first == nullptr) return;
	for(;first != nullptr; first = first->parent)
		for(auto node = first; node != nullptr; node = nextInThisRow(node)) node->updateMBR();
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
void MathRTreeStar<DataType, NumberType, dims, m, M>::rebuild()
{
	Node		*first = firstLeaf(root);

	if(first == nullptr) return;

	while(first->parent != nullptr)
	{
		first = first->parent;
		for(auto node = first; node != nullptr; node = nextInThisRow(node))
			for(size_t i = 0; i < node->getNumChildren(); i++)
				delete static_cast<Node*>(node->childs[i]);
	}
	delete first;

	root = nullptr;
	numElements = 0;
	numLevels = 0;
	for(auto node = firstDataNode; node != nullptr; node = node->next)
	{
		node->updateMBR();
		insert(*node);
	}
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
void MathRTreeStar<DataType, NumberType, dims, m, M>::insert(DataNode& newData)
{
	Node		*forInsert = selectLeaf(newData.getMBR());

	if(forInsert == nullptr)
	{
		forInsert = new Node();
		root = forInsert;
		numLevels++;
	}
	numElements++;
	if(forInsert->attach(&newData))
	{
		/* В узле хватило свободного места - вставка нового объекта и пересчет MBR */
		forInsert->updateUpMBR();
		return;
	}
	/* В узле места не хватило - перевставка и/или деление узла */
	reinsertAndAttach(forInsert, &newData);
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
void MathRTreeStar<DataType, NumberType, dims, m, M>::erase(DataNode *data)
{
	Node			*branch = data->parentNode();

	numElements--;
	branch->detach(data->getMyChildNumber());
	if(branch->getNumChildren() >= m)
	{
		branch->updateUpMBR();
		return;
	}
	/* число элементов в узле меньше допустимого - удаление узла и повторная вставка */
	/* поиск удаляемой ветки */
	while(branch->parent != nullptr && branch->parent->getNumChildren() < m + 1)
		branch = branch->parent;
	/* отсоединение удаляемой ветки */
	Node		*first = firstLeaf(branch);
	if(branch->parent == nullptr)
	{
		root = nullptr;
		numLevels = 0;
		numElements = 0;
		/* перенос объектов из удаляемой ветки в дерево */
		for(auto node = first; node != nullptr; node = nextInThisRow(node))
			for(size_t i = 0; i < node->getNumChildren(); i++)
				insert(*static_cast<DataNode*>(node->childs[i]));
	}
	else
	{
		Node	*parent = branch->parent;
		parent->detach(branch->getMyChildNumber());
		parent->updateUpMBR();
		/* перенос объектов из удаляемой ветки в дерево */
		for(auto node = first; node != nullptr; node = nextInThisRow(node))
			for(size_t i = 0; i < node->getNumChildren(); i++)
			{
				insert(*static_cast<DataNode*>(node->childs[i]));
				numElements--;
			}
	}
	/* удаление удаляемой ветки */
	while(first->parent != nullptr)
	{
		first = first->parent;
		if(first->isLeaf()) throw Exception("Wrong rtree");
		for(auto node = first; node != nullptr; node = nextInThisRow(node))
			for(size_t i = 0; i < node->getNumChildren(); i++)
				delete static_cast<Node*>(node->childs[i]);
	}
	delete first;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
typename MathRTreeStar<DataType, NumberType, dims, m, M>::Node* MathRTreeStar<DataType, NumberType, dims, m, M>::devideAndAttach(Node *startNode, DataNode *child)
{
	Node								*newNode = startNode->devide(child);
	Node								*current = startNode;
	bool								done = false;

	while(current->parent != nullptr)
	{
		current = current->parent;
		current->updateMBR();
		if(current->attach(newNode))
		{
			done = true;
			break;
		}
		newNode = current->devide(newNode);
	}
	if(!done)
	{
		root = new Node();
		root->attach(current);
		root->attach(newNode);
		current = root;
		numLevels++;
	}
	return current;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
void MathRTreeStar<DataType, NumberType, dims, m, M>::reinsertAndAttach(Node *startNode, DataNode *child)
{
#ifdef MATH_RTREE_STAR_USE_REINSERTING
	std::array<DataNode*, M + 1>	reinsertedData;
	size_t							p = (M * 33 + 50) / 100;

	for(size_t i = 0; i < M; i++)
		reinsertedData[i] = static_cast<DataNode*>(startNode->childs[i]);
	reinsertedData[M] = child;

	std::sort(reinsertedData.begin(), reinsertedData.end(), [startNode](DataNode* node1, DataNode* node2)->bool
	{
		return startNode->getMBR().distance(node1->getMBR()) > startNode->getMBR().distance(node2->getMBR());
	});

	startNode->detachAll();

	for(size_t i = p; i < M + 1; i++)
		startNode->attach(reinsertedData[i]);
	startNode->updateUpMBR();

	for(size_t i = 0; i < p; i++)
	{
		Node	*forInsert = selectLeaf(reinsertedData[i]->getMBR());

		if(forInsert == nullptr) throw Exception("Can't find leaf on repeated inserting");

		if(!forInsert->attach(reinsertedData[i]))
			forInsert = devideAndAttach(forInsert, reinsertedData[i]);		/* В узле не хватило свободного места - деление узла */
		forInsert->updateUpMBR();
	}
#else
	Node		*forInsert = devideAndAttach(startNode, child);
	forInsert->updateUpMBR();
#endif
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
typename MathRTreeStar<DataType, NumberType, dims, m, M>::Node* MathRTreeStar<DataType, NumberType, dims, m, M>::selectLeaf(const MathMBR<NumberType, dims>& mbr) const
{
	Node			*current = root;

	if(current == nullptr) return nullptr;
	if(current->isLeaf()) return current;

	/* для внутренних узлов используется критерий минимального увеличения площади MBR */
	while(!static_cast<Node*>(current->childs[0])->isLeaf())
	{
		NumberType	minDeltaVolume = mbr.unionVolume(static_cast<Node*>(current->childs[0])->getMBR()) - static_cast<Node*>(current->childs[0])->getMBR().volume();
		size_t		minIndex = 0;

		for(size_t i = 1; i < current->getNumChildren(); i++)
		{
			NumberType	currentDeltaVolume = mbr.unionVolume(static_cast<Node*>(current->childs[i])->getMBR()) - static_cast<Node*>(current->childs[i])->getMBR().volume();

			if(currentDeltaVolume < minDeltaVolume)
			{
				minDeltaVolume = std::move(currentDeltaVolume);
				minIndex = i;
			}
		}
		current = static_cast<Node*>(current->childs[minIndex]);
	}
	/* а для листовых - критерий минимального перекрытия узлов */
	NumberType	minOverlapVolume = current->getOverlapIncrease(0, mbr) - current->getOverlapIncrease(0);
	NumberType	currentOverlapVolume;
	size_t		minIndex = 0;

	for(size_t i = 1; i < current->getNumChildren(); i++)
	{
		currentOverlapVolume = current->getOverlapIncrease(i, mbr) - current->getOverlapIncrease(i);

		if(currentOverlapVolume < minOverlapVolume)
		{
			minOverlapVolume = std::move(currentOverlapVolume);
			minIndex = i;
		}
	}
	current = static_cast<Node*>(current->childs[minIndex]);

	return current;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
template<class NodeType>
NodeType* MathRTreeStar<DataType, NumberType, dims, m, M>::firstLeaf(NodeType *startNode)
{
	NodeType		*first = startNode;

	if(first == nullptr) return nullptr;
	while(!first->isLeaf()) first = static_cast<NodeType*>(first->childs[0]);
	return first;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
template<class NodeType>
NodeType* MathRTreeStar<DataType, NumberType, dims, m, M>::nextInThisRow(NodeType *node)
{
	NodeType		*current = node;
	size_t			upLevels = 0;

	/* движение вверх-вправо по дереву... */
	while(current->parent != nullptr)
	{
		size_t		currentNumChild = current->getMyChildNumber();

		current = current->parent;
		if(currentNumChild + 1 < current->getNumChildren())
		{
			current = static_cast<NodeType*>(current->childs[currentNumChild + 1]);
			break;
		}
		else upLevels++;
	}
	if(current->parent == nullptr) return nullptr;		/* больше объектов не найдено */
	/* ...а затем снова вниз */
	while(upLevels != 0)
	{
#ifdef MATH_RTREE_STAR_DEBUG
		if(current->isLeaf()) throw Exception("RTree is corrupted");
		if(static_cast<Node*>(current->childs[0])->parent != current) throw Exception("RTree is corrupted");
#endif
		current = static_cast<Node*>(current->childs[0]);
		upLevels--;
	}

	return current;
}
#ifdef MATH_RTREE_STAR_DEBUG
template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
void MathRTreeStar<DataType, NumberType, dims, m, M>::checkTree() const
{
	Node		*first = root;
	size_t		num = 1;

	if(first == nullptr) return;

	while(!first->isLeaf())
	{
		first = static_cast<Node*>(first->childs[0]);
		num++;
	}
	if(num != numLevels) throw Exception("RTree is corrupted");
	num = 0;
	for(auto node = first; node != nullptr; node = nextInThisRow(node))
	{
		for(auto nodeUp = node; nodeUp->parent != nullptr; nodeUp = nodeUp->parent)
		{
			if(static_cast<Node*>(nodeUp->parent->childs[nodeUp->getMyChildNumber()]) != nodeUp)
				throw Exception("RTree is corrupted");
		}
		num += node->getNumChildren();
	}
	if(num != numElements) throw Exception("RTree is corrupted");

}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
void MathRTreeStar<DataType, NumberType, dims, m, M>::checkMBRs() const
{
	Node		*first = firstLeaf(root);

	if(first == nullptr) return;

	while(first != nullptr)
	{
		for(auto node = first; node != nullptr; node = nextInThisRow(node))
		{
			MathRTreeStarMBR<NumberType, dims>		mbr;

			if(node->isLeaf())
				for(size_t i = 0; i < node->getNumChildren(); i++)
					mbr += static_cast<DataNode*>(node->childs[i])->getMBR();
			else
				for(size_t i = 0; i < node->getNumChildren(); i++)
					mbr += static_cast<Node*>(node->childs[i])->getMBR();
			if(!(mbr == node->getMBR()))
				throw Exception("RTree is corrupted");
		}
		first = first->parent;
	}
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
void MathRTreeStar<DataType, NumberType, dims, m, M>::checkMBRs(const Node& node) const
{
	MathRTreeStarMBR<NumberType, dims>		mbr;

	if(node.isLeaf())
		for(size_t i = 0; i < node.getNumChildren(); i++)
			mbr += static_cast<DataNode*>(node.childs[i])->getMBR();
	else
		for(size_t i = 0; i < node.getNumChildren(); i++)
			mbr += static_cast<Node*>(node.childs[i])->getMBR();
	if(!(mbr == node.getMBR()))
		throw Exception("RTree is corrupted");
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
size_t MathRTreeStar<DataType, NumberType, dims, m, M>::getCalculatedSize() const
{
	Node		*first = firstLeaf(root);
	size_t		num = 0;

	if(first == nullptr) return 0;
	for(auto node = first; node != nullptr; node = nextInThisRow(node))
		num += node->getNumChildren();
	return num;
}

template<class DataType, class NumberType, size_t dims, size_t m, size_t M>
void MathRTreeStar<DataType, NumberType, dims, m, M>::checkSize() const
{
	if(getCalculatedSize() != numElements) throw Exception("RTree is corrupted");

	size_t		size = 0;
	for(auto element = firstDataNode; element != nullptr; element = element->next)
		size++;
	if(size != numElements) throw Exception("RTree is corrupted");
}
#endif

#endif // MATHRTREESTAR_H
