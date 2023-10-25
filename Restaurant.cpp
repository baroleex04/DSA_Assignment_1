#include "main.h"
class Restaurant;
class customer;

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
                rear = rear->next;
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

        void remove(string name)
        {
            Queue temp;
            while (!this->isEmpty())
            {
                if (this->front->name == name)
                    this->dequeue();
                else
                {
                    temp.enqueue(this->front->name, this->front->energy);
                    this->dequeue();
                }
            }
            while (!temp.isEmpty())
            {
                this->enqueue(temp.front->name, temp.front->energy);
                temp.dequeue();
            }
        }
    };
    customer *head;
    customer *curr;
    Queue q;
    Queue wait;
    Queue t;
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
        while (!t.isEmpty())
        {
            t.dequeue();
        }
    }

    void RED(string name, int energy)
    {
        if (energy == 0)
            return;
        if (q.size > 0)
        {
            cusNameEnergy *ptr = q.front;
            while (ptr != NULL)
            {
                if (ptr->name == name)
                    return;
                ptr = ptr->next;
            }
        } // check trùng tên
        if (wait.size > 0)
        {
            cusNameEnergy *ptr = wait.front;
            while (ptr != NULL)
            {
                if (ptr->name == name)
                    return;
                ptr = ptr->next;
            }
        }
        if (q.size == MAXSIZE)
        {
            if (wait.size < MAXSIZE) {
                wait.enqueue(name, energy);
                t.enqueue(name, energy);
                return;
            }
            else
                return;
        }
        // check energy = 0
        // bắt đầu thêm người
        if (q.size == 0)
        {
            q.enqueue(name, energy);
            t.enqueue(name, energy);
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
            t.enqueue(name, energy);
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
            t.enqueue(name, energy);
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
            }
            while (!q.isEmpty()) {
				q.dequeue();
			}
			while (!t.isEmpty()) {
				t.dequeue();
			}
        }
        else
        {
            while (num > 0)
            {
                string toDelName = "";
                cusNameEnergy *cn = t.front;
                while (cn != nullptr)
                {
                    bool flag = false;
                    cusNameEnergy *cne = q.front;
                    while (cne != nullptr)
                    {
                        if (cne->name == cn->name)
                        {
                            toDelName = cne->name;
                            flag = true;
                            break;
                        }
                        cne = cne->next;
                    }
                    if (flag)
                        break;
                    cn = cn->next;
                }
                customer *ptr = curr;
                while (ptr->name != toDelName)
                {
                    ptr = ptr->next;
                }
                if (ptr->energy > 0)
                    curr = ptr->next;
                else
                    curr = ptr->prev;
                if (ptr == head)
                    head = head->next;
                customer *tmp = ptr;
                ptr = ptr->next;
                ptr->prev = tmp->prev;
                tmp->prev->next = ptr;
                tmp->prev = nullptr;
                tmp->next = nullptr;
                delete tmp;
                // curr = ptr;
                t.remove(toDelName);
                q.remove(toDelName);
                num--;
            }
        }

        if (wait.size == 0)
            return;
        else
        {
            // while (q.size < MAXSIZE && wait.size > 0)
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
    bool comeEarlier(string n1, string n2)
    {
        cusNameEnergy *ptr = t.front;
        while (ptr->name != n1)
        {
            if (ptr->name == n2)
            {
                return false;
            }
            ptr = ptr->next;
        }
        return true;
    }

    void PURPLE()
    {
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
        ptr = wait.front;
        int size = 1;
        while (ptr != maxEnergyPtr)
        {
            size++;
            ptr = ptr->next;
        }
        int gap = size / 2;
        while (gap > 2)
        {
            cusNameEnergy *current = wait.front;
            cusNameEnergy *gapCurrent = current;
            for (int i = 0; i < gap && gapCurrent != maxEnergyPtr; i++)
            {
                gapCurrent = gapCurrent->next;
            }
            while (current != gapCurrent)
            {
                cusNameEnergy *ptr = current;
                for (int i = 0; i < gap && ptr != maxEnergyPtr; i++)
                {
                    ptr = ptr->next;
                }
                while (ptr != maxEnergyPtr)
                {
                    cusNameEnergy *tmpPtr = ptr;
                    cusNameEnergy *solderedtmpPtr = ptr;
                    for (int i = 0; i < gap && tmpPtr != NULL; i++)
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
                            // solderedtmpPtr = tmpPtr;
                        }
                        else if (solderedtmpPtr->energy == tmpPtr->energy)
                        {
                            if (comeEarlier(solderedtmpPtr->name, tmpPtr->name))
                            {
                                string tmpName = solderedtmpPtr->name;
                                int tmpEnergy = solderedtmpPtr->energy;
                                solderedtmpPtr->energy = tmpPtr->energy;
                                solderedtmpPtr->name = tmpPtr->name;
                                tmpPtr->name = tmpName;
                                tmpPtr->energy = tmpEnergy;
                                count++;
                                // solderedtmpPtr = tmpPtr;
                            }
                        }
                        else
                            break;
                        for (int i = 0; i < gap && tmpPtr != nullptr; i++)
                        {
                            tmpPtr = tmpPtr->prev;
                        }
                    }
                    for (int i = 0; i < gap && ptr != maxEnergyPtr; i++)
                    {
                        ptr = ptr->next;
                    }
                }
                current = current->next;
            }
            gap = gap / 2;
        }
        // insertion sort
        cusNameEnergy *current = wait.front;
        while (current != maxEnergyPtr)
        {
            cusNameEnergy *ptr = current;
            while (ptr != maxEnergyPtr->next)
            {
                cusNameEnergy *tmpPtr = ptr;
                cusNameEnergy *solderedtmpPtr = ptr;
                tmpPtr = tmpPtr->prev;
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
                        // solderedtmpPtr = tmpPtr;
                    }
                    else if (solderedtmpPtr->energy == tmpPtr->energy)
                    {
                        // if (solderedtmpPtr->energy == tmpPtr->energy)
                        // {
                        if (comeEarlier(solderedtmpPtr->name, tmpPtr->name))
                        {
                            string tmpName = solderedtmpPtr->name;
                            int tmpEnergy = solderedtmpPtr->energy;
                            solderedtmpPtr->energy = tmpPtr->energy;
                            solderedtmpPtr->name = tmpPtr->name;
                            tmpPtr->name = tmpName;
                            tmpPtr->energy = tmpEnergy;
                            count++;
                            // solderedtmpPtr = tmpPtr;
                        }
                        // }
                    }
                    else
                        break;
                    tmpPtr = tmpPtr->prev;
                }
                ptr = ptr->next;
            }
            current = current->next;
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
        do
        {
            if (ptr->energy < 0)
            {
                cusNameEnergy newCus(ptr->name, ptr->energy);
                negative.push(newCus);
            }
            else if (ptr->energy > 0)
            {
                cusNameEnergy newCus(ptr->name, ptr->energy);
                positive.push(newCus);
            }
            ptr = ptr->prev;
        } while (ptr != curr);
        customer *tempPtr = nullptr;
        ptr = curr;
        do
        {
            if (ptr->energy > 0)
            {
                cusNameEnergy *temp = positive.top;
                ptr->energy = temp->energy;
                ptr->name = temp->name;
                positive.pop();
                if (positive.isEmpty())
                {
                    if (curr->energy > 0)
                    {
                        tempPtr = ptr;
                    }
                }
            }
            else if (ptr->energy < 0)
            {
                cusNameEnergy *temp = negative.top;
                ptr->energy = temp->energy;
                ptr->name = temp->name;
                negative.pop();
                if (negative.isEmpty())
                {
                    if (curr->energy < 0)
                    {
                        tempPtr = ptr;
                    }
                }
            }
            ptr = ptr->prev;
        } while (ptr != curr);
        curr = tempPtr;
    }

    void UNLIMITED_VOID()
    {
        Queue list;
        if (q.size < 4)
            return;
        customer *smallestStart = nullptr;
        customer *smallestEnd = nullptr;
        int smallestSum = 1e9;
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

                if (sublistSize >= 4 && currentSum <= smallestSum)
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
        else 
            return;
        
        //đã có queue của dãy con và bây giờ sort lại nào
        //tìm con nhỏ nhất
        cusNameEnergy* ptr = list.front;
        cusNameEnergy* smallestptr = list.front;
        int count = 0;
        while (ptr != nullptr) {
            if (ptr->energy < smallestptr->energy) smallestptr = ptr;
            ptr = ptr->next;
        }
        ptr = list.front;
        while (ptr != smallestptr) {
            ptr = ptr->next;
            count++;
        }
        ptr = list.front;
        while (count > 0) {
            cusNameEnergy* temp = list.front;
            list.enqueue(temp->name, temp->energy);
            list.dequeue();
            count--;
        }
        while (!list.isEmpty()) {
            customer temp(list.front->name, list.front->energy, nullptr, nullptr);
            temp.print();
            list.dequeue();
        }
    }
    void createTempPosQueueAndPrintNeg()
    {
        Queue printQueue;
        cusNameEnergy *ptr = t.front;
        for (int i = 0; i < t.size; i++)
        {
            if (ptr->energy < 0)
            {
                printQueue.enqueue(ptr->name, ptr->energy);
                q.remove(ptr->name);
                wait.remove(ptr->name);
            }
            ptr = ptr->next;
        }
        cusNameEnergy *cne = printQueue.rear;
        while (cne != NULL)
        {
            customer tmp(cne->name, cne->energy, nullptr, nullptr);
            tmp.print();
            t.remove(cne->name);
            cne = cne->prev;
        }
        while (!printQueue.isEmpty())
        {
            printQueue.dequeue();
        }
    }
    void createTempNegQueueAndPrintPos()
    {
        Queue printQueue;
        cusNameEnergy *ptr = t.front;
        for (int i = 0; i < t.size; i++)
        {
            if (ptr->energy > 0)
            {
                printQueue.enqueue(ptr->name, ptr->energy);
                wait.remove(ptr->name);
                q.remove(ptr->name);
            }
            ptr = ptr->next;
        }
        cusNameEnergy *cne = printQueue.rear;
        while (cne != NULL)
        {
            customer tmp(cne->name, cne->energy, nullptr, nullptr);
            tmp.print();
            t.remove(cne->name);
            cne = cne->prev;
        }
        while (!printQueue.isEmpty())
        {
            printQueue.dequeue();
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
            Queue temp;
            cusNameEnergy *cne = t.front;
            while (cne != nullptr)
            {
                if (cne->energy < 0)
                {
                    cusNameEnergy *cnn = q.front;
                    while (cnn != NULL)
                    {
                        if (cnn->name == cne->name)
                        {
                            temp.enqueue(cnn->name, cnn->energy);
                            break;
                        }
                        cnn = cnn->next;
                    }
                }
                cne = cne->next;
            }
            while (!temp.isEmpty())
            {
                string toDelete = temp.front->name;
                customer *ptr = curr;
                while (ptr->name != toDelete)
                {
                    ptr = ptr->next;
                }
                // energy âm nên move curr prev
                curr = ptr->prev;
                if (ptr == head)
                    head = head->next;
                customer *tmp = ptr;
                ptr = ptr->next;
                ptr->prev = tmp->prev;
                tmp->prev->next = ptr;
                tmp->prev = nullptr;
                tmp->next = nullptr;
                delete tmp;
                temp.dequeue();
            }
            createTempPosQueueAndPrintNeg();
        }
        else
        {
            Queue temp;
            cusNameEnergy *cne = t.front;
            while (cne != nullptr)
            {
                if (cne->energy > 0)
                {
                    cusNameEnergy *cnn = q.front;
                    while (cnn != NULL)
                    {
                        if (cnn->name == cne->name)
                        {
                            temp.enqueue(cnn->name, cnn->energy);
                            break;
                        }
                        cnn = cnn->next;
                    }
                }
                cne = cne->next;
            }
            while (!temp.isEmpty())
            {
                string toDelete = temp.front->name;
                customer *ptr = curr;
                while (ptr->name != toDelete)
                {
                    ptr = ptr->next;
                }
                // energy dương nên move curr next
                curr = ptr->next;
                if (ptr == head)
                    head = head->next;
                customer *tmp = ptr;
                ptr = ptr->next;
                ptr->prev = tmp->prev;
                tmp->prev->next = ptr;
                tmp->prev = nullptr;
                tmp->next = nullptr;
                delete tmp;
                temp.dequeue();
            }
            createTempNegQueueAndPrintPos();
        }
        // add vô khi có chỗ trống
        while (q.size < MAXSIZE && wait.size > 0)
        {
            cusNameEnergy *newCus = wait.front;
            string newname = newCus->name;
            int newenergy = newCus->energy;
            wait.dequeue();
            RED(newname, newenergy);
        }
    }

    void LIGHT(int num)
    {
        // cout << "LIGHT " << num << endl;
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
