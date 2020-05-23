#pragma once
#define DEFAULT_CAPACITY 4
template<typename T>
class Vector {
protected:
	int _capacity;
	int _size;
	T* _elem;
	void copyFrom(T const* _elem, int left, int right);
	void expand();
	void shrink();
public:
	Vector(int c = DEFAULT_CAPACITY, int s = 0, T v = 0) {	//Ĭ�Ϲ��캯��
		_capacity = c;
		_size = s;
		_elem = new T[_capacity];
		for (int i = 0; i < _size; i++) {
			_elem[i] = v;
		}
	}
	Vector(Vector<T> const& A) {	//���ƹ��캯��
		copyFrom(A._elem, 0, A._size);
	}
	Vector(T* A, int n) {	//���鸳ֵ���캯��
		copyFrom(A, 0, n);
	}
	~Vector() { delete[]_elem; }	//��������

	int size() { return _size; }
	bool empty() { return !_size; }

	T& operator[](int r) const;	//[]������
	Vector<T>& operator=(Vector<T> const& A);	//=������

	int find(T const& e, int left, int right) const;	//�����������������
	int find(T const& e) const {	//����������ȫ�ֲ���
		return find(e, 0, _size);
	}

	int disordered() const;	//�ж��Ƿ��������

	int insert(int pos, T const& e);	//�������ֵ������λ�ã����ز���λ��

	int remove(int left, int right);	//ɾ�����������Ԫ�أ�����ɾ������

	int remove(int r);	//ɾ������λ�õ�Ԫ�أ����ر�ɾ����ֵ

	int deduplicate();	//Ψһ�����������ر�ɾԪ������

	int binSearch(T const& e, int left, int right);	//����������ֲ��ң����ҳɹ�����λ�ã�����ʧ�ܷ���-1

	int uniquify();	//��������Ψһ��,���ر�ɾԪ������

	void unsort(int left, int right);	//��������Ĵ���

	template<typename VST>	//��������
	void traverse(VST& vistit);

	bool bubble(int left, int right);	//�����ҵ�һ������
	void bubbleSort(int left, int right) {	//��������
		while (!bubble(left, right--));
	}
	void merge(int left, int mid, int right);//һ�ι鲢
	void mergeSort(int left, int right);	//�鲢����
};

template<typename T>
void Vector<T>::expand() {
	if (_size < _capacity) return;
	if (_capacity < DEFAULT_CAPACITY) _capacity = DEFAULT_CAPACITY;
	T* oldelem = _elem;
	delete[]_elem;
	_capacity *= 2;
	_elem = new T[_capacity];
	for (int i = 0; i < _size; i++) {
		_elem[i] = oldelem[i];
	}
	delete[]oldelem;
}

template<typename T>
void Vector<T>::shrink() {
	if (_capacity < DEFAULT_CAPACITY * 2) return;
	if (_size * 4 > _capacity) return;
	T* oldelem = _elem;
	_elem = new T[_capacity / 2];
	for (int i = 0; i < _size; i++) {
		_elem[i] = oldelem[i];
	}
	delete[]oldelem;
}

template<typename T>
void Vector<T>::copyFrom(T const* V, int left, int right) {
	_size = right - left;
	_capacity = 2 * _size;
	_elem = new T[_capacity];
	for (int i = 0; i < _size; i++) {
		_elem[i] = V[i];
	}
}

template<typename T>
T& Vector<T>::operator[](int r) const {
	return *(_elem + r);
}

template<typename T>
Vector<T>& Vector<T>::operator=(Vector<T> const& A) {
	if (_elem) delete[]_elem;
	copyFrom(A._elem, 0, A._size);
	return *this;
}

template<typename T>
int Vector<T>::find(T const& e, int left, int right) const {
	int r = right;
	while ((left < r--) && (e != _elem[r]));
	if (r < left)
		return -1;
	else
		return r;
}

template<typename T>
int Vector<T>::disordered() const {
	int n = 0;
	for (int i = 1; i < _size; i++) {
		if (_elem[i - 1] > _elem[i])
			n++;
	}
	return n;
}

template<typename T>
int Vector<T>::insert(int pos, T const& e) {
	expand();
	for (int i = _size; i > pos; i--) {
		_elem[i] = _elem[i - 1];
	}
	_elem[pos] = e;
	_size++;
	return pos;
}

template<typename T>
int Vector<T>::remove(int left, int right) {
	if (left == right)
		return 0;
	while (right < _size) {
		_elem[left] = _elem[right];
		left++;
		right++;
	}
	_size = right;
	shrink();
	return right - left;
}

template<typename T>
int Vector<T>::remove(int r) {
	T e = _elem[r];
	remove(r, r + 1);
	return e;
}

template<typename T>
int Vector<T>::deduplicate() {
	int oldSize = _size;
	int i = 1;
	while (i < (_size - 1)) {
		if (find(_elem[i], 0, i) < 0)
			i++;
		else
			remove(i);
	}
	return oldSize - _size;
}

template<typename T>
int Vector<T>::binSearch(T const& e, int left, int right) {
	while (left < right) {
		int mid = left + (right - left) / 2;
		if (e < _elem[mid])
			right = mid;
		else if (e > _elem[mid])
			left = mid + 1;
		else
			return mid;
	}
	return -1;
}

template<typename T>
int Vector<T>::uniquify() {
	int i = 0;
	int j = 0;
	while (++j < _size) {
		if (_elem[i] != _elem[j]) {
			_elem[++i] = _elem[j];
		}
	}
	_size = ++i;
	shrink();
	return j - i;
}


template<typename T>
void Vector<T>::unsort(int left, int right) {
	T* v = _elem + left;
	for (int i = right - left; i > 0; i--) {
		swap(v[i], v[rand() % i]);
	}
}

template<typename T> template<typename VST>
void Vector<T>::traverse(VST& visit) {
	for (int i = 0; i < _size; i++) {
		visit(_elem[i]);
	}
}

template<typename T>
bool Vector<T>::bubble(int left, int right) {
	bool sorted = true;
	while (++left < right)
		if (_elem[left - 1] > _elem[left]) {
			sorted = false;
			T tmp = _elem[left - 1];
			_elem[left - 1] = _elem[left];
			_elem[left] = tmp;
		}
	return sorted;
}

template<typename T>
void Vector<T>::mergeSort(int left, int right) {
	if (right - left < 2)
		return;
	int mid = (left + right) / 2;
	mergeSort(left, mid);
	mergeSort(mid, right);
	merge(left, mid, right);
}

template<typename T>
void Vector<T>::merge(int left, int mid, int right) {
	T* A = _elem + left;
	int l = mid - left;
	int r = right - mid;
	T* L = new T[l];
	for (int i = 0; i < l; L[i] = A[i++]);
	T* R = _elem + mid;
	for (int i = 0, j = 0, k = 0; (j < l) || (k < r);) {
		if ((j < l) && (!(k < r) || (L[j] <= R[k])))
			A[i++] = L[j++];
		else if ((k < r) && (!(j < l) || (R[k] < L[j])))
			A[i++] = R[k++];
	}
	delete[]L;
}