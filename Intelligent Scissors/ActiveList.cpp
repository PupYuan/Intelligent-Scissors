#include "ActiveList.h"

bool ActiveList::empty() {
	return size == 0 ? true : false;
}
void ActiveList::remove(GraphNode * p) {
	if (empty())
		return;

	current = head;

	//第一个就找到了
	if (head == p) {
		current = current->next;
		head = current;
		size--;
		return;
	}
	while (current->next != nullptr) {
		if (current->next == p) {
			current->next = current->next->next;
			size--;
			return;
		}
		current = current->next;
	}
	//判断ActiveList末尾是否与p相等
	//if (temp->next == p) {
	//	temp->next = nullptr;
	//	size--;
	//}
}

//该列表一直是保持有序的,插入效率为O(N)
void ActiveList::add(GraphNode *p) {
	if (head == nullptr) {
		head = p;
		size++;
		return;
	}
	current = head;
	while (current->next != nullptr) {
		if (p->global_cost < current->next->global_cost) {//新来的p插在前面
			p->next = current->next;
			current->next = p;
			size++;
			return;
		}
		current = current->next;
	}
	current->next = p;
	size++;
}