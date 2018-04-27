#include "ActiveList.h"

bool ActiveList::empty() {
	return size == 0 ? true : false;
}
void ActiveList::remove(GraphNode * p) {
	if (empty())
		return;

	current = head;

	//��һ�����ҵ���
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
	//�ж�ActiveListĩβ�Ƿ���p���
	//if (temp->next == p) {
	//	temp->next = nullptr;
	//	size--;
	//}
}

//���б�һֱ�Ǳ��������,����Ч��ΪO(N)
void ActiveList::add(GraphNode *p) {
	if (head == nullptr) {
		head = p;
		size++;
		return;
	}
	current = head;
	while (current->next != nullptr) {
		if (p->global_cost < current->next->global_cost) {//������p����ǰ��
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