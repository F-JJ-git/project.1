#include <iostream>
#include <memory>
#include <stdexcept>
using namespace std;

// 顺序栈：有效元素位于 [0, count)，栈顶下标为 count - 1。
template<class ElemType>
class SqStack
{
private:
    ElemType* elems;
    int count;
    int maxSize;

public:
    explicit SqStack(int size);
    SqStack(const SqStack<ElemType>& source);
    SqStack<ElemType>& operator=(const SqStack<ElemType>& source);
    ~SqStack();

    bool Push(const ElemType& e);
    bool Pop(ElemType& e);
    bool Empty() const;
    bool Top(ElemType& e) const;
    int Length() const;
    void Clear();
    void Traverse(void (*visit)(const ElemType&)) const;
};

template<class ElemType>
SqStack<ElemType>::SqStack(int size)
    : elems(nullptr), count(0), maxSize(size)
{
    if (size < 0)
        throw invalid_argument("栈容量不能为负数");

    elems = new ElemType[maxSize];
}

// 深拷贝：副本拥有自己的数组，修改副本不会影响原栈。
template<class ElemType>
SqStack<ElemType>::SqStack(const SqStack<ElemType>& source)
    : elems(nullptr), count(source.count), maxSize(source.maxSize)
{
    // 复制中途发生异常时，临时数组会自动释放。
    unique_ptr<ElemType[]> newElems(new ElemType[maxSize]);
    for (int i = 0; i < count; ++i)
        newElems[i] = source.elems[i];

    elems = newElems.release();
}

template<class ElemType>
SqStack<ElemType>& SqStack<ElemType>::operator=(const SqStack<ElemType>& source)
{
    if (this != &source)
    {
        // 先完成分配和复制，再替换旧数组；失败时原栈保持不变。
        unique_ptr<ElemType[]> newElems(new ElemType[source.maxSize]);
        for (int i = 0; i < source.count; ++i)
            newElems[i] = source.elems[i];

        delete[] elems;
        elems = newElems.release();
        maxSize = source.maxSize;
        count = source.count;
    }
    return *this;
}

template<class ElemType>
SqStack<ElemType>::~SqStack()
{
    delete[] elems;
}

template<class ElemType>
bool SqStack<ElemType>::Push(const ElemType& e)
{
    if (count == maxSize)
        return false;

    elems[count] = e;
    ++count;
    return true;
}

template<class ElemType>
bool SqStack<ElemType>::Pop(ElemType& e)
{
    if (Empty())
        return false;

    e = elems[count - 1];
    --count;
    return true;
}

template<class ElemType>
bool SqStack<ElemType>::Empty() const
{
    return count == 0;
}

template<class ElemType>
bool SqStack<ElemType>::Top(ElemType& e) const
{
    if (Empty())
        return false;

    e = elems[count - 1];
    return true;
}

template<class ElemType>
int SqStack<ElemType>::Length() const
{
    return count;
}

template<class ElemType>
void SqStack<ElemType>::Clear()
{
    count = 0; // 逻辑清空，保留已分配的数组和容量。
}

template<class ElemType>
void SqStack<ElemType>::Traverse(void (*visit)(const ElemType&)) const
{
    // 按栈底到栈顶的顺序访问，visit 应指向有效的访问函数。
    for (int i = 0; i < count; ++i)
        visit(elems[i]);
}

void PrintInt(const int& e)
{
    cout << e << " ";
}

int main()
{
    // 基本操作。
    SqStack<int> s1(5);
    s1.Push(10);
    s1.Push(20);
    s1.Push(30);

    cout << "s1遍历：";
    s1.Traverse(PrintInt);
    cout << endl;
    cout << "s1长度：" << s1.Length() << endl;

    int e = 0;
    if (s1.Top(e))
        cout << "栈顶元素：" << e << endl;
    if (s1.Pop(e))
        cout << "出栈元素：" << e << endl;

    // 复制构造：s2 的后续修改不影响 s1。
    SqStack<int> s2 = s1;
    s2.Push(40);

    cout << "复制后s1：";
    s1.Traverse(PrintInt);
    cout << endl;
    cout << "复制后s2：";
    s2.Traverse(PrintInt);
    cout << endl;

    // 赋值运算符：替换已有栈的内容和容量。
    SqStack<int> s3(2);
    s3.Push(99);
    s3 = s1;

    cout << "赋值后s3：";
    s3.Traverse(PrintInt);
    cout << endl;

    s3.Clear();
    cout << "清空后s3长度：" << s3.Length() << endl;
    return 0;
}
