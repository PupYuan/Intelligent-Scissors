#pragma once

class GraphNode {
public:
	int x;
	int y;
	int drawed_time;
	double global_cost;
	bool e;
	bool inL;//额外的空间来换取active list更小的操作代价
	GraphNode * back_pointer;//最小代价路径的方向
	GraphNode * next;//用于active list
};