#pragma once

class GraphNode {
public:
	int x;
	int y;
	int drawed_time;
	double global_cost;
	bool e;
	bool inL;//����Ŀռ�����ȡactive list��С�Ĳ�������
	GraphNode * back_pointer;//��С����·���ķ���
	GraphNode * next;//����active list
};