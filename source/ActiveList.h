#pragma once
#include "GraphNode.h"
#include <assert.h>

//�������ܼ�����active list
class ActiveList {
public:
	ActiveList() { assert(head == nullptr); size = 0; }
	GraphNode * head;
	GraphNode * current;//�������
	void remove(GraphNode * p);//�ҵ���ɾ��ĳ�����
	void add(GraphNode *p);
	bool empty();
	int size;
};



