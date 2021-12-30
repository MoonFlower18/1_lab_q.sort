#include <iostream>
#include <string>
#include <ctime> 
#include <chrono>

using namespace std;
long long N_op = 0;

struct Node //3
{
    Node(int i) : value(i) { }
    int value; //значение элемента в деке
    Node* prev = nullptr; //указатель на пред. эл.
    Node* next = nullptr; //указатель на след. эл.
};

class Deque
{
public: //4
    Node* head_ = nullptr; //указатель на голову(первый эл.)
    Node* tail_ = nullptr; //указатель на хвост
    int size_; //кол-во эл. в деке

    Deque() : size_(0) {} //конструктор
    ~Deque() { CleanUp(); } //деструктор
    //4+2+2+1+2=11
    void AppendR(int i) //добавление эл. справа
    {
        Node* n = new Node(i); //создание нового элемента, i - значение этого элемента
        if (!head_)
        {
            head_ = n; // голова указывает на первый элемент в очереди
            tail_ = n; // хвост указывает на первый элемент в очереди
            N_op += 3;
        }
        else
        {
            //добавление элемента через хвост в дек
            //tail указывает на первый элемент перед добавляемым
            //в поле next первого элемента записывается адрес нового первого элемента
            tail_->next = n;
            n->prev = tail_;
            tail_ = n; //tail начинает указывать на новый элемент в очереди
            N_op += 5;
        }
        size_ += 1;
        N_op += 6;
    }
    //11
    void AppendL(int i) //добавление эл. слева
    {
        Node* m = new Node(i);
        if (!tail_)
        {
            head_ = m;
            tail_ = m;
            N_op += 3;
        }
        else
        {
            head_->prev = m;
            m->next = head_;
            head_ = m;
            N_op += 5;
        }
        size_ += 1;
        N_op += 6;
    }
    //2+2+2+1+2=9
    void DeleteR() //удаляем правый элемент
    {
        // если дек не пуст
        if (Size() != 0) {
            Node* h = tail_;
            Node* tmp; // создаем указатель на структуру
            tmp = h->prev; //  в указателюь tmp кладем ссылку на prev
            delete h; // удаляем объект по указателю tail
            tail_ = tmp; // tail начинает указывать на предпоcледний элемент справа (последний удален)
            size_ -= 1;
            N_op += 9;
        }
        else { cout << "\nDeque is empty.\n"; }
    }
    //9
    void DeleteL() //удаляем левый элемент
    {
        // если дек не пуст
        if (Size() != 0) {
            Node* n = head_;
            Node* tmp; // создаем указатель на структуру
            tmp = n->next; //  в указателюь tmp кладем ссылку на next предпоследнего элемента   
            delete n; // удаляем объект по указателю head (последний элемент в очереди)
            head_ = tmp; // head начинает указывать на предпоcледний элемент слева (последний удален)
            size_ -= 1;
            N_op += 9;
        }
        else { cout << "\nDeque is empty.\n"; }
    }
    //1
    int Size() //размер дека
    {
        return size_;
        N_op += 1;
    }
    //7
    // Изменение значений 2 узлов
    void Swap(Node* a, Node* b)
    {
        int tmp = a->value;
        a->value = b->value;
        b->value = tmp;
        N_op += 7;
    }
    //7+2*(2+6n+3)=17+12n
    void Swap(int i, int j)
    {
        Node* a = At(i); //2+6n+3
        Node* b = At(j);
        Swap(a, b);
        N_op += 29;
    }
    //3+6n
    //индексация O(n)
    Node* At(int index)
    {
        Node* n = head_;
        int i = 0;
        while (n)
        {
            if (i == index) return n;
            n = n->next;
            ++i;
            N_op += 6;
        }
        N_op += 3;
    }
    //4+(n-1)*4+5=4n+5
    void Show() //вывод всех элементов
    {
        if (!head_) { cout << "\nDeque is empty\n\n"; }
        else
        {
            Node* n = head_;
            Node* h = tail_;
            while (n != h)
            {
                cout << n->value << " ";
                n = n->next;
                N_op += 4;
            }
            cout << h->value << "\n";
            N_op += 5;
        }
    }
    //2+2+1=5
    // получение значения из начала дека
    void ValueR()
    {
        Node* n = head_;
        // если дек не пуст
        if (Size() != 0)
        {
            // через указатель head получить значение value первого элемента 
            cout << " first el.: " << n->value;
            N_op += 5;
        }
        else { cout << "\nDeque is empty.\n" << endl; }
    }
    //5
    // получение значения из конца дека
    void ValueL()
    {
        Node* h = tail_;
        // если дек не пуст
        if (Size() != 0)
        {
            // через указатель tail получить значение value последнего элемента
            cout << " last el.: " << h->value;
            N_op += 5;
        }
        else { cout << "\nDeque is empty.\n" << endl; }
    }

    //2+2+(6+(n-1)*6)+2+1+1=6n+8
    // Удаление всех элементов
    void CleanUp()
    {
        Node* n = head_;
        Node* h = tail_;
        Node* tmp;
        while (n != h)
        {
            tmp = n->next;
            delete n;
            n = tmp;
            size_--;
            N_op += 6;
        }
        delete h;
        size_--;
        head_ = nullptr;
        tail_ = nullptr;
        N_op += 8;
    }
};
//6n+8+4+n*(8n+6+8n+6+2+1+12n+7)=28n*n+28n+12
//разделение элементов
int Partition(Deque* n, int lo, int hi)
{
    //1+1+6n+3+2+1=6n+8
    int pivot = n->At((hi + lo) / 2)->value; //медианный элемент
    int i = lo - 1;
    int j = hi + 1;
    while (i <= j)
    {
        do { ++i; N_op += 6; } while (n->At(i)->value < pivot); //6n+6+2n
        do { --j; N_op += 6; } while (n->At(j)->value > pivot);
        if (i >= j) return j; //2
        n->Swap(i, j);//1+12n+7
        N_op += 5;
    }
    N_op += 11;
}

//O (n ^ 2) время в худшем случае и O (nLogn) в среднем и лучшем случаях. 
//Наихудший случай возникает, когда связанный список уже отсортирован.
void QuickSort(Deque* n, int lo, int hi) //(28n^2+28n+13)*((log(n))^2)
//=56n^2*logn+56n*logn+26logn
{
    if (lo < hi)
    {
        int p = Partition(n, lo, hi); //28n*n+28n+13
        QuickSort(n, lo, p); //log (n)
        QuickSort(n, p + 1, hi); //log (n)
        N_op += 11;
    }
}

int main()
{
    srand(time(NULL));
    Deque* nm = new Deque;
    N_op += 4;
    for (int i = 0; i < 1850; i++) //рандомное заполнение
    {
        nm->AppendR(-40 + rand() % -10);
        nm->AppendL(10 + rand() % 40);
        N_op += 2;
    }
    cout << "\n elements: \n"; //вывод эл.
    nm->Show(); //4n+5
    cout << "\n\n quick sort: \n"; //сортировка
    auto start = chrono::system_clock::now();
    QuickSort(nm, 0, nm->Size() - 1); //56n^2*logn+56n*logn+26logn
    auto end = chrono::system_clock::now();
    nm->Show(); //4n+5
    nm->~Deque(); //1
    delete nm;
    chrono::duration<double> elapsed = end - start;
    cout << "\n\nquick sort time: " << elapsed.count() << " seconds" << endl;
    N_op += 4;
    cout <<"\n   N_op: " << N_op << endl;
    //F(n)=3+n*(9+9+2)+4n+5+56n^2*log n+56n*log n+26*log n+1+4n+5+1+1=56n^2*log n+56n*log n+26*log n+16+28n
    //O(n^2)
    //O(F(n))=n^2*log n
    system("pause");
    return 0;
}
