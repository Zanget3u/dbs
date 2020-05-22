#pragma once

#include <iostream>

template <typename T>
class Liste {
private:
	T *_values;
	int _last, _size;

	bool isFull();
	int find(T val);
	void increase();
	void decrease();

public:
	Liste(int size);
	Liste(const Liste& l);
	~Liste();
	void append(T val);
	T getValueAt(int pos);
	void erase(T val);
	void toScreen();
	unsigned int size();
};
// -------------------------------------------------------------------
template <typename T>
Liste<T>::Liste(int size) {
	_size = size;
	_last = 0;
	_values = new T[size];
}
// -------------------------------------------------------------------
template <typename T>
Liste<T>::Liste(const Liste& l) {
	_size = l._size;
	_last = l._last;
	_values = new T[_size];

	for (int i = 0; i < _last; i++)
		_values[i] = l._values[i];
}
// -------------------------------------------------------------------
template <typename T>
Liste<T>::~Liste() {
	delete[] _values;
}
// -------------------------------------------------------------------
template <typename T>
T Liste<T>::getValueAt(int pos) {
	if (pos < 0 || pos >= _last)
		std::cout << "out of bounds";
	return _values[pos];
}
// -------------------------------------------------------------------
template <typename T>
void Liste<T>::append(T val) {
	if (isFull())
		increase();

	_values[_last] = val;
	_last += 1;
}
// -------------------------------------------------------------------
template <typename T>
bool Liste<T>::isFull() {
	return _last == _size;
}
// -------------------------------------------------------------------
template <typename T>
void Liste<T>::increase() {
	T *tmp = new T[_size * 2];

	for (int i = 0; i < _size; i++)
		tmp[i] = _values[i];

	delete[] _values;
	_values = tmp;
	_size *= 2;
}
// -------------------------------------------------------------------
template <typename T>
void Liste<T>::toScreen() {
	for (int i = 0; i < _last; i++)
		std::cout << i << ": " << _values[i] << std::endl;
}
// -------------------------------------------------------------------
template <typename T>
int Liste<T>::find(T val) {
	for (int pos = 0; pos < _last; pos++)
		if (_values[pos] == val)
			return pos;
	return -1;
}
// -------------------------------------------------------------------
template <typename T>
void Liste<T>::decrease() {
	_size /= 2;
	T *tmp = new T[_size];

	for (int i = 0; i < _size; i++)
		tmp[i] = _values[i];
	delete[] _values;
	_values = tmp;
}
// -------------------------------------------------------------------
template <typename T>
void Liste<T>::erase(T val) {
	int pos = find(val);

	if (pos == -1)
		std::cout << "value not found";

	for (; pos < _last - 1; pos++)
		_values[pos] = _values[pos + 1];
	_last -= 1;

	if (_last < _size / 4)
		decrease();
}
// -------------------------------------------------------------------
template <typename T>
unsigned int Liste<T>::size() {
	return _last;				
}
// -------------------------------------------------------------------