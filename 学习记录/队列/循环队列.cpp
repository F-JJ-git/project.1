#include <iostream>
#include <memory>
#include <stdexcept>
using namespace std;

// 使用 count 区分空队列和满队列，数组中的所有位置都可存放元素。
template<class ElemType>
class CircQueue
{
protected:
    ElemType* elems;
    int front;   // 队头元素的下标。
    int rear;    // 下一次入队的位置。
    int maxSize;
    int count;

public:
    explicit CircQueue(int size);
    CircQueue(const CircQueue<ElemType>& source);
    CircQueue<ElemType>& operator=(const CircQueue<ElemType>& source);
    ~CircQueue();

    bool InQueue(const ElemType& e);
    bool OutQueue(ElemType& e);
    bool GetHead(ElemType& e) const;
    void Traverse(void (*visit)(const ElemType&)) const;
    int Length() const;
    bool Empty() const;
};

template<class ElemType>
CircQueue<ElemType>::CircQueue(int size)
    : elems(nullptr), front(0), rear(0), maxSize(size), count(0)
{
    if (size < 0)
        throw invalid_argument("队列容量不能为负数");

    elems = new ElemType[maxSize];
}

template<class ElemType>
CircQueue<ElemType>::~CircQueue()
{
    delete[] elems;
}

template<class ElemType>
bool CircQueue<ElemType>::InQueue(const ElemType& e)
{
    if (count == maxSize)
        return false;

    elems[rear] = e;
    rear = (rear + 1) % maxSize;
    ++count;
    return true;
}

template<class ElemType>
bool CircQueue<ElemType>::OutQueue(ElemType& e)
{
    if (Empty())
        return false;

    e = elems[front];
    front = (front + 1) % maxSize;
    --count;
    return true;
}

template<class ElemType>
bool CircQueue<ElemType>::GetHead(ElemType& e) const
{
    if (Empty())
        return false;

    e = elems[front];
    return true;
}

template<class ElemType>
void CircQueue<ElemType>::Traverse(void (*visit)(const ElemType&)) const
{
    // 按逻辑顺序从队头访问到队尾，visit 应指向有效的访问函数。
    for (int i = 0; i < count; ++i)
        visit(elems[(front + i) % maxSize]);
}

template<class ElemType>
int CircQueue<ElemType>::Length() const
{
    return count;
}

template<class ElemType>
bool CircQueue<ElemType>::Empty() const
{
    return count == 0;
}

template<class ElemType>
CircQueue<ElemType>::CircQueue(const CircQueue<ElemType>& source)
    : elems(nullptr), front(source.front), rear(source.rear),
      maxSize(source.maxSize), count(source.count)
{
    // 只复制有效元素，同时保留源队列的循环下标。
    unique_ptr<ElemType[]> newElems(new ElemType[maxSize]);
    for (int i = 0; i < count; ++i)
    {
        const int index = (front + i) % maxSize;
        newElems[index] = source.elems[index];
    }
    elems = newElems.release();
}

template<class ElemType>
CircQueue<ElemType>& CircQueue<ElemType>::operator=(const CircQueue<ElemType>& source)
{
    if (this == &source)
        return *this;

    // 先分配并复制，成功后再替换旧数组；异常时临时数组自动释放。
    unique_ptr<ElemType[]> newElems(new ElemType[source.maxSize]);
    for (int i = 0; i < source.count; ++i)
    {
        const int index = (source.front + i) % source.maxSize;
        newElems[index] = source.elems[index];
    }

    delete[] elems;
    elems = newElems.release();
    maxSize = source.maxSize;
    front = source.front;
    rear = source.rear;
    count = source.count;
    return *this;
}

void PrintInt(const int& e)
{
    cout << e << " ";
}

int main()
{
    int e = 0;

    // 创建源队列，并通过出队和再次入队让 rear 回绕到数组开头。
    CircQueue<int> q1(5);
    q1.InQueue(10);
    q1.InQueue(20);
    q1.InQueue(30);
    q1.OutQueue(e);
    q1.InQueue(40);
    q1.InQueue(50);

    cout << "q1原始队列：";
    q1.Traverse(PrintInt);
    cout << endl;

    // 自赋值保持内容不变。
    q1 = q1;
    cout << "自我赋值后q1：";
    q1.Traverse(PrintInt);
    cout << endl;

    // 普通赋值同时复制容量、下标和有效元素。
    CircQueue<int> q2(2);
    q2.InQueue(99);
    q2 = q1;
    cout << "赋值后q2：";
    q2.Traverse(PrintInt);
    cout << endl;

    // 修改副本，验证两个队列拥有相互独立的数组。
    if (q2.OutQueue(e))
        cout << "q2出队元素：" << e << endl;
    q2.InQueue(60);

    cout << "修改后q2：";
    q2.Traverse(PrintInt);
    cout << endl;
    cout << "再次查看q1：";
    q1.Traverse(PrintInt);
    cout << endl;
    return 0;
}
