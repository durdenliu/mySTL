#pragma once


#define ListNodePos(T) ListNode<T>*

template<typename T>		//列表节点的定义
struct ListNode {
	T data;
	ListNodePos(T) pred;
	ListNodePos(T) succ;
	ListNode() {}
	ListNode(T e, ListNodePos(T) p = NULL, ListNodePos(T) s = NULL) :data(e), pred(p), succ(s) {}

	ListNodePos(T) insertAsPred(T const& e);

	ListNodePos(T) insertAsSucc(T const& e);
};

template <typename T>			//在当前节点的前一个位置插入新节点，并返回新节点位置
ListNodePos(T) ListNode<T>::insertAsPred(T const& e) {
	ListNodePos(T) res = new ListNode<T>(e, pred, this);
	pred->succ = res;
	pred = res;
	return res;
}

template <typename T>			//在当前节点的后一个位置插入新节点，并返回新节点位置
ListNodePos(T) ListNode<T>::insertAsSucc(T const& e) {
	ListNodePos(T) res = new ListNode(e, this, succ);
	succ->pred = res;
	succ = res;
	return res;
}


template<typename T>			//列表的模板类
class List {
private:		//列表的基本成员
	int _size;
	ListNodePos(T) header;
	ListNodePos(T) trailer;
protected:		//列表的内部函数
	void init();	//初始化
	int clear();	//清除列表所有节点
	void copyNodes(ListNodePos(T) p, int n);
	void insertionSort(ListNodePos(T) p, int n);
	void selectionSort(ListNodePos(T) p, int n);
	void mergeSort(ListNodePos(T)& p, int n);
	void merge(ListNodePos(T)& p, int n, List<T>& L, ListNodePos(T) q, int m);
	ListNodePos(T) selectMax(ListNodePos(T) p, int n);

public:
	List() { init(); }		//默认构造函数
	List(ListNodePos(T) p, int n);		//复制列表节点p起的n项
	List(List<T> const& L);		//复制整列表L
	List(List<T> const& L, int r, int n);		//复制列表L的第r项节点后的n项
	ListNodePos(T) first() const { return header->succ; }		//返回列表的首节点

	~List();

	int size() const{
		return _size;
	}

	ListNodePos(T) find(T const& e, ListNodePos(T) p, int n) const;		//在列表内节点p的n个前驱中查找数据等于e的最后节点
	ListNodePos(T) find(T e) const { return find(e, trailer, _size);  }		//在列表内查找最后一个数据等于e的节点
	ListNodePos(T) insertAsFirst(T const& e);		//插入数据等于e的新节点作为首节点
	ListNodePos(T) insertAsLast(T const& e);		//插入数据等于e的新节点作为末节点
	ListNodePos(T) insertAfter(ListNodePos(T) p, T const& e);		//将数据等于e的新节点插入p节点的后继，并返回新节点位置
	ListNodePos(T) insertBefore(ListNodePos(T) p, T const& e);		//将数据等于e的新节点插入p节点的前驱，并返回新节点位置

	T operator[] (int r) const;		//[]运算符重载
	T remove(ListNodePos(T) p);		//删除p节点,并将其值返回
	int deduplicate();		//无序列表的唯一化
	int uniquify();			//有序列表的唯一化

	void traverse(void (*visit) (T&));		//遍历（这里使用函数指针实现，也可用函数对象实现）
	ListNodePos(T) search(T const& e, int n, ListNodePos(T) p) const;		//在有序列表内节点p的n个前驱查找不大于e的最后者

	void sort(ListNodePos(T) p, int n) {
		switch (rand() % 3) {
		case 1:	insertionSort(p, n); break;
		case 2:	selectionSort(p, n); break;
		default: mergeSort(p, n); break;
		}
	}



};

template<typename T>
void List<T>::init() {
	header = new ListNode<T>;
	trailer = new ListNode<T>;
	header->succ = trailer;
	trailer->pred = header;
	_size = 0;
}

template<typename T>
int List<T>::clear() {
	int oldSize = _size;
	while (_size) {
		remove(header->succ);
	}
	return oldSize;
}

template<typename T>
void List<T>::copyNodes(ListNodePos(T) p, int n) {
	init();
	while (0 < n--) {
		insertAsLast(p->data);
		p = p->succ;
	}
}

template<typename T>
List<T>::List(ListNodePos(T) p, int n) {
	copyNodes(p, n);
}

template<typename T>
List<T>::List(List<T> const& L) {
	copyNodes(L.first(), L._size);
}

template<typename T>
List<T>::List(List<T> const& L, int r, int n) {
	copyNodes(L[r], n);
}

template<typename T>
List<T>::~List() {
	clear();
	delete header;
	delete trailer;
}


template<typename T>
ListNodePos(T) List<T>::find(T const& e, ListNodePos(T) p, int n) const {
	while (0 < n--) {
		if ((p = p->pred)->data == e)	return p;
	}
	return NULL;
}

template<typename T>
ListNodePos(T) List<T>::insertAsFirst(T const& e) {
	_size++;
	return header->insertAsSucc(e);
}

template<typename T>
ListNodePos(T) List<T>::insertAsLast(T const& e) {
	_size++;
	return trailer->insertAsPred(e);
}

template<typename T>
ListNodePos(T) List<T>::insertAfter(ListNodePos(T) p, T const& e) {
	_size++;
	return p->insertAsSucc(e);
}

template<typename T>
ListNodePos(T) List<T>::insertBefore(ListNodePos(T) p, T const& e) {
	_size++;
	return p->insertAsPred(e);
}

template<typename T>
T List<T>::operator[](int r) const {

	ListNodePos(T) p = first();
	while (0 < r--) p = p->succ;
	return p->data;
}

template<typename T>
T List<T>::remove(ListNodePos(T) p) {
	p->pred->succ = p->succ;
	p->succ->pred = p->pred;
	T e = p->data;
	delete p;
	_size--;
	return e;
}

template<typename T>
int List<T>::deduplicate() {
	ListNodePos(T) p = header;
	int oldSize = _size;
	int r = 0;
	while (trailer != (p = p->succ)) {
		ListNodePos(T) q = find(p->data, p, r);
		q ? remove(q) : r++;
	}
	return oldSize - _size;
}

template<typename T>		//用函数指针实现遍历
void List<T>::traverse(void (*visit) (T&)) {
	ListNodePos(T) p = header;
	while (trailer != (p = p->succ)) {
		visit(p->data);
	}
}
///用函数对象实现遍历
/*
template<typename T> template<typename VST>
void List<T>::traverse(VST& visit) {
	ListNodePos(T) p = header;
	while (trailer != (p = p->succ)) {
		visit(p->data);
	}
}
*/


template<typename T>
int List<T>::uniquify() {
	int oldSize = _size;
	if (_size < 2)
		return 0;
	ListNodePos(T) p = header->succ;
	while (p->succ != trailer) {
		ListNodePos(T) q = p->succ;
		if (p == q) {
			remove(q);
		}
		else {
			p = p->succ;
		}
	}
	return oldSize - _size;
}

template<typename T>
ListNodePos(T) List<T>::search(T const& e, int n, ListNodePos(T) p) const {
	while (n--) {
		if ((p = p->pred)->data <= e)
			break;
	}
	return p;
}

template<typename T>
void List<T>::insertionSort(ListNodePos(T) p, int n) {
	for (int i = 0; i < n; i++) {
		insertAfter(search(p->data, i, p), p->data);
	}
	p = p->succ;
	remove(p->pred);
}

template<typename T>
void List<T>::selectionSort(ListNodePos(T) p, int n) {
	ListNodePos(T) head = p->pred;
	ListNodePos(T) tail = p;
	for (int i = 0; i < n; i++) {
		tail = tail->succ;
	}
	while (n > 0) {
		ListNodePos(T) max = selectMax(head->succ, n);
		insertBefore(tail, remove(max));
		tail = tail->pred;
		n--;
	}
}

template<typename T>
ListNodePos(T) List<T>::selectMax(ListNodePos(T) p, int n) {
	ListNodePos(T) max = p;
	ListNodePos(T) cur = p;
	for (int i = 1; i < n; i++) {
		if (max->data < cur->succ->data) {
			max = cur;
		}
		cur = cur->succ;
	}
	return max;
}

template<typename T>
void List<T>::merge(ListNodePos(T)& p, int n, List<T>& L, ListNodePos(T) q, int m) {
	ListNodePos(T) pp = p->pred;
	while (0 < m) {
		if ((0 < n) && (p->data <= q->data)) {
			if (q == (p = p->succ))	break;
			n--;
		}
		else {
			insertBefore(p, L.remove((q = q->succ)->pred));
			m--;
		}
	}
	p = pp->succ;
}

template<typename T>
void List<T>::mergeSort(ListNodePos(T)& p, int n) {
	if (n < 2)	return;
	int m = n >> 1;
	ListNodePos(T) q = p;
	for (int i = 0; i < m; i++) {
		q = p->succ;
	}
	mergeSort(p, m);
	mergeSort(q, n - m);
	merge(p, m, *this, q, n - m);
}