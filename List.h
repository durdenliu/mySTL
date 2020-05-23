#pragma once


#define ListNodePos(T) ListNode<T>*

template<typename T>		//�б�ڵ�Ķ���
struct ListNode {
	T data;
	ListNodePos(T) pred;
	ListNodePos(T) succ;
	ListNode() {}
	ListNode(T e, ListNodePos(T) p = NULL, ListNodePos(T) s = NULL) :data(e), pred(p), succ(s) {}

	ListNodePos(T) insertAsPred(T const& e);

	ListNodePos(T) insertAsSucc(T const& e);
};

template <typename T>			//�ڵ�ǰ�ڵ��ǰһ��λ�ò����½ڵ㣬�������½ڵ�λ��
ListNodePos(T) ListNode<T>::insertAsPred(T const& e) {
	ListNodePos(T) res = new ListNode<T>(e, pred, this);
	pred->succ = res;
	pred = res;
	return res;
}

template <typename T>			//�ڵ�ǰ�ڵ�ĺ�һ��λ�ò����½ڵ㣬�������½ڵ�λ��
ListNodePos(T) ListNode<T>::insertAsSucc(T const& e) {
	ListNodePos(T) res = new ListNode(e, this, succ);
	succ->pred = res;
	succ = res;
	return res;
}


template<typename T>			//�б��ģ����
class List {
private:		//�б�Ļ�����Ա
	int _size;
	ListNodePos(T) header;
	ListNodePos(T) trailer;
protected:		//�б���ڲ�����
	void init();	//��ʼ��
	int clear();	//����б����нڵ�
	void copyNodes(ListNodePos(T) p, int n);
	void insertionSort(ListNodePos(T) p, int n);
	void selectionSort(ListNodePos(T) p, int n);
	void mergeSort(ListNodePos(T)& p, int n);
	void merge(ListNodePos(T)& p, int n, List<T>& L, ListNodePos(T) q, int m);
	ListNodePos(T) selectMax(ListNodePos(T) p, int n);

public:
	List() { init(); }		//Ĭ�Ϲ��캯��
	List(ListNodePos(T) p, int n);		//�����б�ڵ�p���n��
	List(List<T> const& L);		//�������б�L
	List(List<T> const& L, int r, int n);		//�����б�L�ĵ�r��ڵ���n��
	ListNodePos(T) first() const { return header->succ; }		//�����б���׽ڵ�

	~List();

	int size() const{
		return _size;
	}

	ListNodePos(T) find(T const& e, ListNodePos(T) p, int n) const;		//���б��ڽڵ�p��n��ǰ���в������ݵ���e�����ڵ�
	ListNodePos(T) find(T e) const { return find(e, trailer, _size);  }		//���б��ڲ������һ�����ݵ���e�Ľڵ�
	ListNodePos(T) insertAsFirst(T const& e);		//�������ݵ���e���½ڵ���Ϊ�׽ڵ�
	ListNodePos(T) insertAsLast(T const& e);		//�������ݵ���e���½ڵ���Ϊĩ�ڵ�
	ListNodePos(T) insertAfter(ListNodePos(T) p, T const& e);		//�����ݵ���e���½ڵ����p�ڵ�ĺ�̣��������½ڵ�λ��
	ListNodePos(T) insertBefore(ListNodePos(T) p, T const& e);		//�����ݵ���e���½ڵ����p�ڵ��ǰ�����������½ڵ�λ��

	T operator[] (int r) const;		//[]���������
	T remove(ListNodePos(T) p);		//ɾ��p�ڵ�,������ֵ����
	int deduplicate();		//�����б��Ψһ��
	int uniquify();			//�����б��Ψһ��

	void traverse(void (*visit) (T&));		//����������ʹ�ú���ָ��ʵ�֣�Ҳ���ú�������ʵ�֣�
	ListNodePos(T) search(T const& e, int n, ListNodePos(T) p) const;		//�������б��ڽڵ�p��n��ǰ�����Ҳ�����e�������

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

template<typename T>		//�ú���ָ��ʵ�ֱ���
void List<T>::traverse(void (*visit) (T&)) {
	ListNodePos(T) p = header;
	while (trailer != (p = p->succ)) {
		visit(p->data);
	}
}
///�ú�������ʵ�ֱ���
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