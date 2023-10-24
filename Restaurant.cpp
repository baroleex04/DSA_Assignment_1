#include "main.h"
// MAXSIZE có từ file trên
// Cần 1 cái doubly queue
#define INT_MAX 2147483647
extern int MAXSIZE;
class imp_res : public Restaurant
{
public:
	class cusNameEnergy
	{
	public:
		string name;
		int energy;
		cusNameEnergy *next;
		cusNameEnergy *prev;
		cusNameEnergy() : name(""), energy(0), next(nullptr), prev(nullptr) {}
		cusNameEnergy(string cusName, int cusEnergy)
		{
			name = cusName;
			energy = cusEnergy;
			next = nullptr;
			prev = nullptr;
		}
	};
	class Stack
	{
	public:
		cusNameEnergy *top;
		Stack()
		{
			top = NULL;
		}
		~Stack()
		{
			while (top != nullptr)
				pop();
		}
		void push(cusNameEnergy it)
		{
			cusNameEnergy *newNode = new cusNameEnergy(it.name, it.energy);
			if (isEmpty())
			{
				top = newNode;
			}
			else
			{
				newNode->next = top;
				top->prev = newNode;
				top = newNode;
			}
		}
		void pop()
		{
			if (isEmpty())
			{
				return;
			}
			cusNameEnergy *tmp = top;
			top = top->next;
			if (top != nullptr)
				top->prev = nullptr;
			tmp->next = nullptr;
			tmp->prev = nullptr;
			delete tmp;
		}
		cusNameEnergy topValue()
		{
			return *top;
		}
		bool isEmpty()
		{
			return top == nullptr;
		}
	};

	class Queue
	{
	public:
		cusNameEnergy *front;
		cusNameEnergy *rear;
		int size;
		Queue()
		{
			front = rear = nullptr;
			size = 0;
		}

		~Queue()
		{
			while (!isEmpty())
			{
				dequeue();
			}
			size = 0;
		}

		void enqueue(string name, int energy)
		{
			cusNameEnergy *newNode = new cusNameEnergy(name, energy);
			if (isEmpty())
				front = rear = newNode;
			else
			{
				rear->next = newNode;
				newNode->prev = rear;
				rear = newNode;
			}
			size++;
		}

		void dequeue()
		{
			if (!isEmpty())
			{
				cusNameEnergy *temp = front;
				front = front->next;
				if (front != nullptr)
					front->prev = nullptr;
				temp->next = nullptr;
				temp->prev = nullptr;
				delete temp;
				if (front == nullptr)
					rear = nullptr;
				size--;
			}
			else
				return;
		}

		cusNameEnergy *frontValue()
		{
			if (!isEmpty())
				return front;
			return nullptr;
		}

		bool isEmpty()
		{
			return size == 0;
		}
	};
	customer *head;
	customer *curr;
	Queue q;
	Queue wait;
	bool addSuccess = false;
	imp_res()
	{
		curr = head = nullptr;
	};
	~imp_res()
	{
		if (head == nullptr)
		{
			return;
		}
		// customer* tail = head->prev;
		head->prev->next = nullptr;
		head->prev = nullptr;
		curr = head;
		while (head != nullptr)
		{
			customer *ptr = head;
			head = head->next;
			curr = head;
			ptr->next = nullptr;
			ptr->prev = nullptr;
			delete ptr;
		}
		// delete curr;
		// delete head;
		curr = head = nullptr;
		while (!q.isEmpty())
		{
			q.dequeue();
		}
		while (!wait.isEmpty())
		{
			wait.dequeue();
		}
	}

	// void traversal() const
	// {
	// 	customer *t = head;
	// 	do
	// 	{
	// 		t->print();
	// 		t = t->prev;
	// 	} while (t != head);
	// }

	void RED(string name, int energy)
	{
		if (addSuccess)
			addSuccess = false;
		if (q.size > 0) {
			if (q.size == 1) {
				//trùng tên khi nhà hàng có 1 người
				if (head->name == name) return;
			}
			else {//xét trùng tên khi nhà hàng có nhiều người
				cusNameEnergy *ptr = q.front;
				while (ptr != NULL) {
					if (ptr->name == name) return;
					ptr = ptr->next;
				}
				//xét xong người đang ăn thì xét tới hàng đợi
				if (wait.size > 0) {
					ptr = wait.front;
					while (ptr != NULL) {
						if (ptr->name == name) return;
						ptr = ptr->next;
					}
				}
			}
		} // check trùng tên

		if (energy == 0)
			return;
		// check energy = 0
		//bắt đầu thêm người
		if (q.size == 0) {
			q.enqueue(name, energy);
			customer *temp = new customer(name, energy, nullptr, nullptr);
			head = temp;
			head->prev = head;
			head->next = head;
			curr = head;
			addSuccess = true;
		}
		else if (q.size > 0 && q.size < MAXSIZE / 2) {
			if (energy >= curr->energy) {
				customer *cus = new customer(name, energy, nullptr, nullptr);
				cus->next = curr->next;
				curr->next->prev = cus;
				curr->next = cus;
				cus->prev = curr;
				curr = cus;
			}
			else
			{
				customer *cus = new customer(name, energy, nullptr, nullptr);
				cus->prev = curr->prev;
				curr->prev->next = cus;
				curr->prev = cus;
				cus->next = curr;
				curr = cus;
			}
			q.enqueue(name, energy);
			addSuccess = true;
			// Sau khi thêm thì nhập vào hàng
		}
		else if (q.size < MAXSIZE && q.size >= MAXSIZE / 2)
		{
			int res = 0;
			int signedRes = 0;
			customer *ptr = curr;
			customer *cusAdd = nullptr;
			do
			{
				if (abs(ptr->energy - energy) > res)
				{
					cusAdd = ptr;
					res = abs(ptr->energy - energy);
					signedRes = energy - ptr->energy;
				}
				ptr = ptr->next;
			} while (ptr != curr);
			if (signedRes < 0)
			{
				customer *cus = new customer(name, energy, cusAdd->prev, cusAdd);
				cusAdd->prev->next = cus;
				cusAdd->prev = cus;
				curr = cus;
			}
			else
			{
				customer *cus = new customer(name, energy, cusAdd, cusAdd->next);
				cusAdd->next->prev = cus;
				cusAdd->next = cus;
				curr = cus;
			}
			q.enqueue(name, energy);
			addSuccess = true;
		}
		else if (q.size == MAXSIZE)
		{
			if (wait.size == MAXSIZE)
				return;
			else
				wait.enqueue(name, energy);
		}
	}
	void BLUE(int num)
	{
		// traversal();
		if (num >= MAXSIZE || num >= q.size)
		{
			while (head != nullptr)
			{
				customer *temp = head;
				head->prev->next = head->next;
				head->next->prev = head->prev;
				head = head->next;
				curr = head;
				temp->prev = nullptr;
				temp->next = nullptr;
				if (head == temp)
				{
					curr = head = nullptr;
				}
				delete temp;
				q.dequeue();
			}
		}
		else
		{
			while (num > 0)
			{
				string toDelName = q.front->name;
				customer *ptr = curr;
				while (ptr->name != toDelName)
				{
					ptr = ptr->next;
				}
				if (ptr->energy > 0) curr = ptr->next;
				else curr = ptr->prev;
				if (ptr == head) head = head->next;
				customer *tmp = ptr;
				ptr = ptr->next;
				ptr->prev = tmp->prev;
				tmp->prev->next = ptr;
				tmp->prev = nullptr;
				tmp->next = nullptr;
				delete tmp;
				// curr = ptr;
				q.dequeue();
				num--;
			}
		}

		if (wait.size == 0)
			return;
		else
		{
			while (q.size < MAXSIZE && wait.size > 0)
			{
				cusNameEnergy *newCus = wait.front;
				string newname = newCus->name;
				int newenergy = newCus->energy;
				wait.dequeue();
				RED(newname, newenergy);
			}
		}
	}

	void PURPLE()
	{
		// cout << "PURPLEEEEEEEEEEEEEEEEEEEEEEE" << endl;
		if (wait.size == 0)
			return;
		int count = 0;
		int maxEnergy = abs(wait.frontValue()->energy);
		cusNameEnergy *ptr = wait.frontValue();
		cusNameEnergy *maxEnergyPtr = nullptr;
		while (ptr != nullptr)
		{
			if (abs(ptr->energy) >= maxEnergy)
			{
				maxEnergy = abs(ptr->energy);
				maxEnergyPtr = ptr;
			}
			ptr = ptr->next;
		}
		ptr = maxEnergyPtr;
		int size = 0;
		while (ptr != nullptr)
		{
			size++;
			ptr = ptr->next;
		}
		int gap = size / 2;
		while (gap > 0)
		{
			cusNameEnergy *current = maxEnergyPtr;
			cusNameEnergy *gapCurrent = current;
			for (int i = 0; i < gap && gapCurrent != nullptr; i++)
			{
				gapCurrent = current->next;
			}
			while (current != gapCurrent)
			{
				cusNameEnergy *ptr = current;
				for (int i = 0; i < gap && ptr != nullptr; i++)
				{
					ptr = ptr->next;
				}
				while (ptr != nullptr)
				{
					cusNameEnergy *tmpPtr = ptr;
					cusNameEnergy *solderedtmpPtr = ptr;
					for (int i = 0; i < gap && ptr != nullptr; i++)
					{
						tmpPtr = tmpPtr->prev;
					}
					while (tmpPtr != nullptr)
					{
						if (abs(solderedtmpPtr->energy) > abs(tmpPtr->energy))
						{
							string tmpName = solderedtmpPtr->name;
							int tmpEnergy = solderedtmpPtr->energy;
							solderedtmpPtr->energy = tmpPtr->energy;
							solderedtmpPtr->name = tmpPtr->name;
							tmpPtr->name = tmpName;
							tmpPtr->energy = tmpEnergy;
							count++;
							solderedtmpPtr = tmpPtr;
						}
						else
							break;
						for (int i = 0; i < gap && ptr != nullptr; i++)
						{
							tmpPtr = tmpPtr->prev;
						}
					}
					for (int i = 0; i < gap && ptr != nullptr; i++)
					{
						ptr = ptr->next;
					}
				}
				current = current->next;
			}
			gap = gap / 2;
		}
		BLUE(count % MAXSIZE);
	}
	void REVERSAL()
	{
		customer *ptr = curr;
		Stack negative;
		Stack positive;
		if (q.size == 0)
			return;
		do {
			if (ptr->energy < 0) {
				cusNameEnergy newCus(ptr->name, ptr->energy);
				negative.push(newCus);
			}
			else if (ptr->energy > 0) {
				cusNameEnergy newCus(ptr->name, ptr->energy);
				positive.push(newCus);
			}
			ptr = ptr->prev;
		} while (ptr != curr);
		ptr = curr;
		do {
			if (ptr->energy > 0) {
				cusNameEnergy* temp = positive.top;
				ptr->energy = temp->energy;
				ptr->name = temp->name;
				positive.pop();
			}
			else if (ptr->energy < 0) {
				cusNameEnergy* temp = negative.top;
				ptr->energy = temp->energy;
				ptr->name = temp->name;
				negative.pop();
			}
			ptr = ptr->prev;
		} while (ptr != curr);
		// Chơi cái stack âm
		// ptr = curr;
		// if (ptr->energy > 0)
		// {
		// 	ptr->name = positive.topValue().name;
		// 	ptr->energy = positive.topValue().energy;
		// 	positive.pop();
		// }
		// else
		// {
		// 	ptr->name = negative.topValue().name;
		// 	ptr->energy = negative.topValue().energy;
		// 	negative.pop();
		// }
		// ptr = ptr->next;
		// while (ptr != curr)
		// {
		// 	if (ptr->energy > 0)
		// 	{
		// 		ptr->name = positive.topValue().name;
		// 		ptr->energy = positive.topValue().energy;
		// 		positive.pop();
		// 	}
		// 	else
		// 	{
		// 		ptr->name = negative.topValue().name;
		// 		ptr->energy = negative.topValue().energy;
		// 		negative.pop();
		// 	}
		// 	ptr = ptr->next;
		// }
	}

	void UNLIMITED_VOID()
	{
		Queue list;
		if (q.size < 4)
			return;
		customer *smallestStart = nullptr;
		customer *smallestEnd = nullptr;
		int smallestSum = INT_MAX;
		customer *start = curr;
		do
		{
			customer *end = start;
			int currentSum = 0;
			int sublistSize = 0;

			do
			{
				currentSum += end->energy;
				sublistSize++;

				if (sublistSize >= 4 && currentSum < smallestSum)
				{
					smallestSum = currentSum;
					smallestStart = start;
					smallestEnd = end;
				}

				end = end->next;
			} while (end != start);

			start = start->next;
		} while (start != curr);

		if (smallestStart != nullptr)
		{
			customer *current = smallestStart;
			do
			{
				list.enqueue(current->name, current->energy);
				current = current->next;
			} while (current != smallestEnd->next);
		}

		while (!list.isEmpty())
		{
			customer *cus = new customer(list.frontValue()->name, list.frontValue()->energy, nullptr, nullptr);
			list.dequeue();
			cus->print();
			delete cus;
		}
	}

	void createTempPosQueueAndPrintNeg()
	{
		Queue tempQueue; // negative thì in ra, còn postive thì lưu vô queue
		cusNameEnergy *ptr = q.frontValue();
		for (int i = 0; i < q.size; i++)
		{
			if (ptr->energy < 0)
			{
				customer tempCus(ptr->name, ptr->energy, nullptr, nullptr);
				tempCus.print();
			}
			else
			{
				tempQueue.enqueue(ptr->name, ptr->energy);
			}
			ptr = ptr->next;
		}
		while (!q.isEmpty())
		{
			q.dequeue();
		}
		while (!tempQueue.isEmpty())
		{
			q.enqueue(tempQueue.frontValue()->name, tempQueue.frontValue()->energy);
			tempQueue.dequeue();
		}
	}
	void createTempNegQueueAndPrintPos()
	{
		Queue tempQueue; // positive thì in ra, còn negative thì lưu vô queue
		cusNameEnergy *ptr = q.front;
		for (int i = 0; i < q.size; i++)
		{
			if (ptr->energy > 0)
			{
				customer tempCus(ptr->name, ptr->energy, nullptr, nullptr);
				tempCus.print();
			}
			else
			{
				tempQueue.enqueue(ptr->name, ptr->energy);
			}
			ptr = ptr->next;
		}
		while (!q.isEmpty())
		{
			q.dequeue();
		}
		while (!tempQueue.isEmpty())
		{
			q.enqueue(tempQueue.front->name, tempQueue.front->energy);
			tempQueue.dequeue();
		}
	}
	void DOMAIN_EXPANSION()
	{
		// cout << "DOMAIN EXPANSIONNNNNNNNNNNNNNNNNNNNN" << endl;
		int sumPos = 0;
		int sumNeg = 0;
		if (q.size == 0 && wait.size == 0)
			return;
		else
		{
			cusNameEnergy *ptr = q.front;
			while (ptr != nullptr)
			{
				if (ptr->energy > 0)
					sumPos += ptr->energy;
				else
					sumNeg -= ptr->energy;
				ptr = ptr->next;
			}
			cusNameEnergy *ptr1 = wait.front;
			while (ptr1 != nullptr)
			{
				if (ptr1->energy > 0)
					sumPos += ptr1->energy;
				else
					sumNeg -= ptr1->energy;
				ptr1 = ptr1->next;
			}
		}

		if (sumPos >= sumNeg)
		{ 
			customer *ptr = head;
			do
			{
				if (ptr->energy < 0)
				{
					if (q.size == 1)
					{
						head->next = nullptr;
						head->prev = nullptr;
						delete head;
						head = nullptr;
						curr = head;
						break;
					}
					customer *tmp = ptr;
					if (ptr == head)
						head = head->next;
					tmp->prev->next = ptr->next;
					tmp->next->prev = ptr->prev;
					ptr = ptr->next;
					tmp->prev = nullptr;
					tmp->next = nullptr;
					delete tmp;
					curr = ptr;
				}
				else
					ptr = ptr->next;
			} while (ptr != head);
			createTempPosQueueAndPrintNeg();
		}
		else
		{
			customer *ptr = head;
			do
			{
				if (ptr->energy > 0)
				{
					if (q.size == 1)
					{
						head->next = nullptr;
						head->prev = nullptr;
						delete head;
						head = nullptr;
						curr = head;
						break;
					}
					customer *tmp = ptr;
					if (ptr == head)
						head = head->next;
					tmp->prev->next = ptr->next;
					tmp->next->prev = ptr->prev;
					ptr = ptr->next;
					tmp->prev = nullptr;
					tmp->next = nullptr;
					delete tmp;
					curr = ptr;
				}
				else
					ptr = ptr->next;
			} while (ptr != head);
			createTempNegQueueAndPrintPos();
		}
	}

	void LIGHT(int num)
	{
		cout << "LIGHT " << num << endl;
		if (num < 0)
		{
			if (curr == nullptr)
				return;
			else
			{
				if (q.size == 1)
				{
					curr->print();
					return;
				}
				else
				{
					customer *ptr = curr;
					do
					{
						ptr->print();
						ptr = ptr->prev;
					} while (ptr != curr);
				}
			}
		}
		else if (num > 0)
		{
			if (curr == nullptr)
				return;
			else
			{
				if (q.size == 1)
				{
					curr->print();
					return;
				}
				else
				{
					customer *ptr = curr;
					do
					{
						ptr->print();
						ptr = ptr->next;
					} while (ptr != curr);
				}
			}
		}
		else
		{
			cusNameEnergy *ptr = wait.front;
			while (ptr != nullptr)
			{
				customer tmp(ptr->name, ptr->energy, nullptr, nullptr);
				tmp.print();
				ptr = ptr->next;
			}
		}
	}
};
