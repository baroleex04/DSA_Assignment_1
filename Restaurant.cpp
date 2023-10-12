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
		if (q.size == 1)
		{
			customer *tmp = head;
			tmp->next = nullptr;
			tmp->prev = nullptr;
			delete tmp;
			head = curr = nullptr;
			while (!q.isEmpty())
			{
				q.dequeue();
			}
			while (!wait.isEmpty())
			{
				wait.dequeue();
			}
		}
		else
		{
			while (!q.isEmpty())
			{
				customer *ptr = head;
				head = head->next;
				head->prev = ptr->prev;
				if (ptr->prev != nullptr)
					ptr->prev->next = head;
				ptr->prev = nullptr;
				ptr->next = nullptr;
				delete ptr;
				q.dequeue();
			}
			while (!wait.isEmpty())
			{
				wait.dequeue();
			}
		}
	}

	void traversal() const
	{
		customer *t = head;
		do
		{
			t->print();
			t = t->prev;
		} while (t != head);
	}

	void RED(string name, int energy)
	{
		if (q.size > 0)
		{
			if (q.size == 1)
			{
				if (head->name == name)
					return;
			}
			else
			{
				cusNameEnergy *ptr = q.front;
				while (ptr != NULL)
				{
					if (ptr->name == name)
						return;
					ptr = ptr->next;
				}
				if (wait.size > 0)
				{
					ptr = wait.front;
					while (ptr != NULL)
					{
						if (ptr->name == name)
							return;
						ptr = ptr->next;
					}
				}
			}
		} // check trùng tên
		if (energy == 0)
			return;
		// check energy = 0
		if (q.size == 0)
		{
			q.enqueue(name, energy);
			customer *temp = new customer(name, energy, nullptr, nullptr);
			head = temp;
			head->prev = head;
			head->next = head;
			curr = head;
		}
		else if (q.size > 0 && q.size < MAXSIZE / 2)
		{
			if (energy >= curr->energy)
			{
				customer *cus = new customer(name, energy, nullptr, nullptr);
				cus->next = curr->next;
				cus->prev = curr;
				curr->next->prev = cus;
				curr->next = cus;
				curr = cus;
			}
			else
			{
				customer *cus = new customer(name, energy, nullptr, nullptr);
				cus->next = curr;
				cus->prev = curr->prev;
				curr->prev->next = cus;
				curr->prev = cus;
				curr = cus;
			}
			q.enqueue(name, energy);
			// Sau khi thêm thì nhập vào hàng
		}
		else if (q.size < MAXSIZE && q.size >= MAXSIZE / 2)
		{
			int res = 0;
			int signedRes = res;
			customer *ptr = head;
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
			} while (ptr != head);
			if (signedRes < 0)
			{
				customer *tmp = cusAdd->prev;
				customer *cus = new customer(name, energy, tmp, cusAdd);
				cusAdd->prev = cus;
				tmp->next = cus;
				curr = cus;
			}
			else
			{
				customer *tmp = cusAdd->next;
				customer *cus = new customer(name, energy, cusAdd, tmp);
				cusAdd->next = cus;
				tmp->prev = cus;
				curr = cus;
			}
			q.enqueue(name, energy);
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
		traversal();
		if (num >= MAXSIZE || num >= q.size)
		{
			if (head == nullptr)
				return;
			if (q.size == 1)
			{
				customer *tmp = head;
				tmp->next = nullptr;
				tmp->prev = nullptr;
				delete tmp;
				head = nullptr;
				curr = head;
				q.dequeue();
			}
			else
			{
				while (head != nullptr)
				{
					if (q.size == 1)
					{
						customer *tmp = head;
						head->next = nullptr;
						head->prev = nullptr;
						delete tmp;
						head = nullptr;
						curr = head;
						q.dequeue();
					}
					else
					{
						customer *temp = head;
						head->prev->next = head->next;
						head->next->prev = head->prev;
						head = head->next;
						temp->prev = nullptr;
						temp->next = nullptr;
						delete temp;
						q.dequeue();
					}
				}
				while (!q.isEmpty())
				{
					q.dequeue();
				}
			}
		}
		else
		{
			if (head == nullptr)
			{
				return;
			}
			if (q.size == 1)
			{
				customer *tmp = head;
				head->prev = nullptr;
				head->next = nullptr;
				delete tmp;
				head = nullptr;
				curr = head;
				q.dequeue();
				return;
			}
			while (num > 0)
			{
				string nameToRemove = q.frontValue()->name;
				customer *ptr = head;
				do
				{
					if (q.size == 1)
					{
						customer *tmp = head;
						head->next = nullptr;
						head->prev = nullptr;
						tmp->next = nullptr;
						tmp->prev = nullptr;
						delete tmp;
						head = nullptr;
						curr = head;
						break;
					}
					if (ptr->name == nameToRemove)
					{
						customer *toDelete = ptr;
						if (curr == ptr)
						{
							if (curr->energy > 0)
								curr = ptr->next;
							else
								curr = ptr->prev;
						}
						ptr->prev->next = ptr->next;
						ptr->next->prev = ptr->prev;
						if (ptr == head)
							head = head->next;
						ptr = ptr->next;
						toDelete->next = nullptr;
						toDelete->prev = nullptr;
						delete toDelete;
						break;
					}
					else
						ptr = ptr->next;
				} while (ptr != head);
				num--;
				q.dequeue();
			}
		}
		// chọn chỗ cho khách
		if (wait.size == 0)
			return;
		else
		{
			while (wait.size > 0)
			{
				cusNameEnergy *newCus = wait.frontValue();
				RED(newCus->name, newCus->energy);
				wait.dequeue();
			}
		}
	}
	void PURPLE()
	{
		int count = 0;
		if (wait.size == 0)
			return;
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
		{ // bắt đầu shellsort
			size++;
			ptr = ptr->next;
		}
		for (int gap = size / 2; gap > 0; gap /= 2)
		{
			for (int i = gap; i < size; i++)
			{
				cusNameEnergy *temp = maxEnergyPtr;
				for (int j = 0; j < i - gap; j++)
				{
					temp = temp->next;
				}
				cusNameEnergy *curr = temp->next;
				while (temp != nullptr && curr != nullptr && abs(temp->energy) < abs(curr->energy))
				{
					swap(temp->energy, curr->energy);
					swap(temp->name, curr->name);
					count++;
					temp = temp->next;
					curr = curr->next;
				}
			}
		}
		BLUE(count % MAXSIZE);
	}
	void REVERSAL()
	{
		customer *ptr = curr;
		Stack negative;
		Stack positive;
		if (curr == nullptr)
			return;
		if (ptr->energy < 0)
		{
			cusNameEnergy newCus(ptr->name, ptr->energy);
			negative.push(newCus);
		}
		else
		{
			cusNameEnergy newCus(ptr->name, ptr->energy);
			positive.push(newCus);
		}
		ptr = ptr->next;
		// Chia 2 stack dương âm
		while (ptr != curr)
		{
			if (ptr->energy < 0)
			{
				cusNameEnergy newCus(ptr->name, ptr->energy);
				negative.push(newCus);
			}
			else
			{
				cusNameEnergy newCus(ptr->name, ptr->energy);
				positive.push(newCus);
			}
			ptr = ptr->next;
		}
		// Chơi cái stack âm
		ptr = curr;
		if (ptr->energy > 0)
		{
			ptr->name = positive.topValue().name;
			ptr->energy = positive.topValue().energy;
			positive.pop();
		}
		else
		{
			ptr->name = negative.topValue().name;
			ptr->energy = negative.topValue().energy;
			negative.pop();
		}
		ptr = ptr->next;
		while (ptr != curr)
		{
			if (ptr->energy > 0)
			{
				ptr->name = positive.topValue().name;
				ptr->energy = positive.topValue().energy;
				positive.pop();
			}
			else
			{
				ptr->name = negative.topValue().name;
				ptr->energy = negative.topValue().energy;
				negative.pop();
			}
			ptr = ptr->next;
		}
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
	void DOMAIN_EXPANSION()
	{
		int sumPos = 0;
		int sumNeg = 0;
		if (q.size == 0 && wait.size == 0)
			return;
		else
		{
			cusNameEnergy *ptr = q.front;
			while (ptr != nullptr) {
				if (ptr->energy > 0) sumPos += ptr->energy;
				else sumNeg -= ptr->energy;
				ptr = ptr->next;
			}
			cusNameEnergy *ptr1 = wait.front;
			while (ptr1 != nullptr) {
				if (ptr->energy > 0) sumPos += ptr->energy;
				else sumNeg -= ptr->energy;
				ptr1 = ptr1->next;
			}
		}
		// Calculate sumPos and sumNeg while iterating through the list
		
		if (sumPos >= sumNeg)
		{ // đuổi energy < 0
			// customer* ptr = head;
			// for (int i = 0; i < q.size; i++)
			// {
			// 	if (ptr->energy < 0)
			// 	{
			// 		customer *tmp = ptr;
			// 		if (ptr == head)
			// 			head = head->next;
			// 		tmp->prev->next = ptr->next;
			// 		tmp->next->prev = ptr->prev;
			// 		ptr = ptr->next;
			// 		tmp->prev = nullptr;
			// 		tmp->next = nullptr;
			// 		delete tmp;
			// 	}
			// 	else
			// 		ptr = ptr->next;
			// }
			// cusNameEnergy *negPtr = q.front;
			// while (negPtr != nullptr)
			// {
			// 	if (negPtr->energy < 0) {
			// 		cusNameEnergy* tmp = negPtr;
			// 		negPtr = negPtr->next;
			// 		if (tmp->prev != nullptr) {
			// 			tmp->prev->next = tmp->next;
			// 		}
			// 		else q.front = tmp->next;
			// 		if (tmp->next != nullptr){
			// 			tmp->next->prev = tmp->prev;
			// 		}
			// 		else q.rear = tmp->prev;
			// 		tmp->next = nullptr;
			// 		tmp->prev = nullptr;
			// 		delete tmp;
			// 	}	
			// 	else negPtr = negPtr->next;
			// }
		}
		else
		{
			customer* ptr = head;
			for (int i = 0; i < q.size; i++)
			{
				if (ptr->energy > 0)
				{
					if (q.size == 1) {
						customer* tmp = head;
						tmp->next = nullptr;
						tmp->prev = nullptr;
						delete tmp;
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
			}
			cusNameEnergy *negPtr = q.front;
			while (negPtr != nullptr)
			{
				if (negPtr->energy > 0) {
					cusNameEnergy* tmp = negPtr;
					negPtr = negPtr->next;
					if (tmp->prev != nullptr) {
						tmp->prev->next = negPtr;
					}
					else q.front = negPtr;
					if (tmp->next != nullptr){
						tmp->next->prev = tmp->prev;
					}
					else q.rear = tmp->prev;
					tmp->next = nullptr;
					tmp->prev = nullptr;
					delete tmp;
					q.size = q.size - 1;
					//in ra thông tin của chú thuật sư hoặc oán linh
					negPtr = negPtr->next;
				}	
				else negPtr = negPtr->next;
			}
		}
	}

	void LIGHT(int num)
	{
		if (num < 0)
		{
			curr->print();
			customer *ptr = curr->next;
			while (ptr != curr)
			{
				ptr->print();
				ptr = ptr->next;
			}
		}
		else if (num > 0)
		{
			curr->print();
			customer *ptr = curr->prev;
			while (ptr != curr)
			{
				ptr->print();
				ptr = ptr->prev;
			}
		}
		else {
			cusNameEnergy* ptr = wait.front;
			while (ptr != nullptr)
			{
				if (ptr != nullptr) {
					customer* tmp = new customer(ptr->name, ptr->energy, nullptr, nullptr);
					tmp->print();
					delete tmp;
				}	
			} 
		}
	}
};
