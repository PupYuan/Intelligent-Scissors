#pragma once
#include "GraphNode.h"
#include <assert.h>

//用于智能剪刀的active list
class ActiveList {
public:
	ActiveList() { assert(head == nullptr); size = 0; }
	GraphNode * head;
	GraphNode * current;//方便插入
	void remove(GraphNode * p);//找到并删除某个结点
	void add(GraphNode *p);
	bool empty();
	int size;
};



