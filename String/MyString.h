#pragma once
#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <algorithm>

template <typename T = char, typename Allocator = std::allocator<T>> 
class MyString {
private:
	template <typename Alloc>
	struct deleter_allocate {
	private:
		Alloc mAllocator;
		size_t number;
	public:
		deleter_allocate(Alloc& other_allocator, size_t capacity) : mAllocator(other_allocator), number(capacity) {};

		void operator()(T* ptrMemory) {
			std::allocator_traits<Alloc>::deallocate(mAllocator, ptrMemory, number);
		}
	};
	auto allocate_unique(Allocator& Alloc, size_t capacity) {
		T* ptr = nullptr;
		try { 
			ptr = std::allocator_traits<Allocator>::allocate(Alloc, capacity);
			return std::unique_ptr<T[], deleter_allocate<Allocator>>(ptr, deleter_allocate<Allocator>(Alloc, capacity));
		}
		catch (...) {
			std::allocator_traits<Allocator>::deallocate(Alloc, ptr, capacity);
			throw;
		}
	}

	size_t mSize = 0;
	size_t mCapacity = 0;	
	Allocator mAllocator;
	std::unique_ptr<T[], deleter_allocate<Allocator>> mCharArray = allocate_unique(mAllocator, mCapacity);

	

public:
	static const size_t npos = -1;
	// Iterator
	
	class IteratorString: public std::iterator<std::random_access_iterator_tag, T> {
	private:
		T* pString;
	public:
		IteratorString(T* pChar = nullptr) :pString(pChar) {};

		void swap(IteratorString& other);
		T* operator *();
		T* operator ->();
		T* operator [](const size_t index);

		IteratorString& operator =( const IteratorString& other);
		IteratorString& operator +=(const size_t difference);
		IteratorString operator + (const size_t difference);
		IteratorString& operator -=(const size_t difference);
		IteratorString& operator - (const size_t difference);
		size_t operator - (IteratorString& other);
		IteratorString& operator ++();
		IteratorString& operator ++(int);
		IteratorString& operator --();
		IteratorString& operator --(int);

		bool operator ==(IteratorString& other);
		bool operator !=(IteratorString& other);
		bool operator < (IteratorString& other);
		bool operator > (IteratorString& other);
		bool operator <=(IteratorString& other);
		bool operator >=(IteratorString& other);

	};

	using iterator = typename IteratorString;
	using const_iterator = const typename IteratorString;
	using reverse_iterator = typename std::reverse_iterator<iterator>;
	using const_reverse_iterator = typename std::reverse_iterator<const_iterator>;

	// Constructors
	MyString() = default;
	MyString(const size_t size, const char simbol = '\0', const Allocator& alloc = Allocator());
	MyString(const char* pCharArray, const Allocator& alloc = Allocator());
	MyString(const char* pCharArray, const size_t count, const Allocator& alloc = Allocator());
	MyString(const MyString& other, const size_t positions, const Allocator& alloc = Allocator());
	MyString(const MyString& other, const size_t positions, const size_t count, const Allocator& alloc = Allocator());
	MyString(const MyString& other, const Allocator& alloc);
	MyString(const MyString& other);
	MyString(MyString&& str, const Allocator& alloc) noexcept;
	MyString(MyString&& other) noexcept;
	MyString(std::initializer_list<T> ilist, const Allocator& alloc = Allocator());

	// Destructor
	~MyString();

	// Operators
	T& operator[] (const size_t index);
	MyString operator= (const MyString& other);
	MyString& operator= (MyString&& Other) noexcept;
	MyString& operator+= (MyString other);
	//operator addition and bool declared behind the Class


	// Metods get
	T& at(const size_t index);
	size_t capacity() noexcept;
	size_t size() noexcept;

	//Iterators
	iterator begin() noexcept;
	iterator end() noexcept;
	const_iterator cbegin() noexcept;
	const_iterator cend() noexcept;
	reverse_iterator rbegin() noexcept;
	reverse_iterator rend() noexcept;
	const_reverse_iterator crbegin() noexcept;
	const_reverse_iterator crend() noexcept;

	// Metods
	void clear() noexcept;
	void deleter(T* p) {
		std::allocator_traits<Allocator>::deallocate(mAllocator, p, mCapacity);
	}
	bool empty() noexcept;
	void swap(MyString& other) {
		std::swap(mSize, other.mSize);
		std::swap(mCapacity, other.mCapacity);
		std::swap(mAllocator, other.mAllocator);
		std::swap(mCharArray, other.mCharArray);
	}
	void reserve(const size_t num);		// exceptions: bat_alloc, 
	void copy(const MyString& other);	// exceptions: bat_alloc
	void resize(const size_t newSize, T ch = '\0'); // exceptions: bat_alloc
	void shrink_to_fit();			// exceptions: bat_alloc
	void push_back(const T ch);	// exceptions: bat_alloc
	void pop_back() noexcept;
	MyString<T, Allocator>& erace(size_t index = 0, size_t count = npos);		//exceptions: out_of_range
	iterator erace(IteratorString positions) noexcept;
	iterator erace(const_iterator first, const_iterator last) noexcept;
	MyString<T, Allocator> substr(size_t positions, size_t count = npos);		//exceptions: out_of_range

	MyString& insert(size_t index, size_t count, T ch);		// exceptions: bat_alloc
	MyString& insert(size_t index, T* ch);		// exceptions: bat_alloc
	MyString& insert(size_t index, T* ch, size_t count);		// exceptions: bat_alloc
	MyString& insert(size_t index, const MyString& other);		// exceptions: bat_alloc
	MyString& insert(size_t index, const MyString& other, size_t other_index, size_t count);		// exceptions: bat_alloc
	iterator insert(iterator pos, T ch);	// exceptions: bat_alloc
	iterator insert(iterator pos, size_t count, T Ch);		// exceptions: bat_alloc
	template <typename IteratorInput, bool T = !(std::is_base_of<std::input_iterator_tag, IteratorInput>)>	
	iterator insert() = delete;
	template <typename IteratorInput>
	iterator insert(iterator pos, IteratorInput first, IteratorInput last);			// exceptions: bat_alloc

	MyString& append(size_t count, T ch);		// exceptions: bat_alloc
	MyString& append(T* ch);		// exceptions: bat_alloc
	MyString& append(T* ch, size_t count);		// exceptions: bat_alloc
	MyString& append(const MyString& other);		// exceptions: bat_alloc
	MyString& append(const MyString& other, size_t other_index, size_t count);		// exceptions: bat_alloc
	iterator append(T ch, size_t count);		// exceptions: bat_alloc
	template <typename IteratorInput, bool T = !(std::is_base_of<std::input_iterator_tag, IteratorInput>)>		// exceptions: bat_alloc
	iterator append() = delete;
	template <typename IteratorInput>
	iterator append(IteratorInput first, IteratorInput last);		// exceptions: bat_alloc

	MyString& replace(size_t index, size_t count, T ch);		// exceptions: bat_alloc
	MyString& replace(size_t index, T* ch);		// exceptions: bat_alloc
	MyString& replace(size_t index, T* ch, size_t count);		// exceptions: bat_alloc
	MyString& replace(size_t index, const MyString& other);		// exceptions: bat_alloc
	MyString& replace(size_t index,const MyString& other, size_t other_index, size_t count);		// exceptions: bat_alloc
	iterator replace(iterator pos, T ch);		// exceptions: bat_alloc
	iterator replace(iterator pos, size_t count, T ch);		// exceptions: bat_alloc
	template <typename IteratorInput, bool T = !(std::is_base_of<std::input_iterator_tag, IteratorInput>)>
	iterator replace() = delete;
	template <typename IteratorInput>
	iterator replace(iterator pos, IteratorInput first, IteratorInput last);		// exceptions: bat_alloc

};

template <typename T, typename Allocator>
MyString<T, Allocator> operator+ (const MyString<T, Allocator>& strLeft, const MyString<T, Allocator>& strRigth);

template <typename T, typename Allocator>
bool operator== (const MyString<T, Allocator>& strLeft, const MyString<T, Allocator>& strRigth);

template <typename T, typename Allocator>
bool operator!= (const MyString<T, Allocator>& strLeft, const MyString<T, Allocator>& strRigth);

template <typename T, typename Allocator>
bool operator< (const MyString<T, Allocator>& strLeft, const MyString<T, Allocator>& strRigth);

template <typename T, typename Allocator>
bool operator> (const MyString<T, Allocator>& strLeft, const MyString<T, Allocator>& strRigth);

template <typename T, typename Allocator>
bool operator<= (const MyString<T, Allocator>& strLeft, const MyString<T, Allocator>& strRigth);

template <typename T, typename Allocator>
bool operator>= (const MyString<T, Allocator>& strLeft, const MyString<T, Allocator>& strRigth);


//Metods iterator

template<typename T, typename Allocator>
inline void MyString<T, Allocator>::IteratorString::swap(IteratorString& other) {
	std::swap(*pString, *(other.pString));
};

template<typename T, typename Allocator>
T* MyString<T, Allocator>::IteratorString::operator *() {
	return *pString;
};

template<typename T, typename Allocator>
T* MyString<T, Allocator>::IteratorString::operator ->() {
	return *pString;
};

template<typename T, typename Allocator>
T* MyString<T, Allocator>::IteratorString::operator [](const size_t index) {
	return pString[index];
};

template<typename T, typename Allocator>
MyString<T, Allocator>::IteratorString& MyString<T, Allocator>::IteratorString::operator =(const IteratorString& other)
{
	if (pString == other.pString) return *this;
	IteratorString tmp(other);
	return *tmp;
};

template<typename T, typename Allocator>
typename MyString<T, Allocator>::IteratorString& MyString<T, Allocator>::IteratorString::operator +=(const size_t difference)
{
	this->pStirng += difference;
	return *this;
};

template<typename T, typename Allocator>
inline MyString<T, Allocator>::IteratorString MyString<T, Allocator>::IteratorString::operator +(const size_t difference)
{
	IteratorString tmp(*this);
	tmp += difference; 
	return tmp;
};

template<typename T, typename Allocator>
inline MyString<T, Allocator>::IteratorString& MyString<T, Allocator>::IteratorString::operator -=(const size_t difference)
{
	this->pStirng -= difference;
	return *this;
};

template<typename T, typename Allocator>
inline MyString<T, Allocator>::IteratorString& MyString<T, Allocator>::IteratorString::operator -(const size_t difference)
{
	*this -= difference;
	return *this;
};

template<typename T, typename Allocator>
inline size_t MyString<T, Allocator>::IteratorString::operator -(IteratorString& other)
{
	return std::distance(*this, other);
};

template<typename T, typename Allocator>
inline MyString<T, Allocator>::IteratorString& MyString<T, Allocator>::IteratorString::operator ++()
{
	++(this->pString);
	return *this;
};

template<typename T, typename Allocator>
inline MyString<T, Allocator>::IteratorString& MyString<T, Allocator>::IteratorString::operator ++(int)
{
	IteratorString temp = *this;
	++(this->pString);
	return *temp;
};

template<typename T, typename Allocator>
inline MyString<T, Allocator>::IteratorString& MyString<T, Allocator>::IteratorString::operator --()
{
	--(this->pString);
	return *this;
};

template<typename T, typename Allocator>
inline MyString<T, Allocator>::IteratorString& MyString<T, Allocator>::IteratorString::operator --(int)
{
	IteratorString temp = *this;
	--(this->pString);
	return *temp;
};

template<typename T, typename Allocator>
inline bool MyString<T, Allocator>::IteratorString::operator ==(IteratorString& other)
{
	return (this->pString == other.pString);
};

template<typename T, typename Allocator>
inline bool MyString<T, Allocator>::IteratorString::operator !=(IteratorString& other)
{
	return !(*this == other);
};

template<typename T, typename Allocator>
inline bool MyString<T, Allocator>::IteratorString::operator <(IteratorString& other)
{
	return (this->pString < other.pString);
};

template<typename T, typename Allocator>
inline bool MyString<T, Allocator>::IteratorString::operator >(IteratorString& other)
{
	return (this->pString > other.pString);
};

template<typename T, typename Allocator>
inline bool MyString<T, Allocator>::IteratorString::operator <=(IteratorString& other)
{
	return !(*this > other);
};

template<typename T, typename Allocator>
inline bool MyString<T, Allocator>::IteratorString::operator >=(IteratorString& other)
{
	return !(*this < other);
};


// MyString Construstors

template <typename T, typename Allocator>
MyString<T, Allocator>::MyString(const size_t size, const char simbol, const Allocator& alloc) : mSize(size), mCapacity(size), mAllocator(alloc){
	mCharArray = allocate_unique(mAllocator, mCapacity);

	for (size_t i = 0; i < mSize; ++i) {
		std::allocator_traits<Allocator>::construct(mAllocator, mCharArray.get() + i, simbol);
	}
}

template <typename T, typename Allocator>
MyString<T, Allocator>::MyString(const char* pCharArray, const Allocator& alloc) : mSize(0), mCapacity(0), mAllocator(alloc), 
																			mCharArray(allocate_unique(mAllocator, mCapacity)) {
	size_t i = 0;
	while (pCharArray[i] != '\0') {
		push_back(*(pCharArray + i));
		++i;
	}
}

template <typename T, typename Allocator>
MyString<T, Allocator>::MyString(const char* pCharArray, const size_t count, const Allocator& alloc) : mSize(count), mCapacity(count), mAllocator(alloc) {
	mCharArray = allocate_unique(mAllocator, mCapacity);

	for (size_t i = 0; i < mSize; ++i) {
		std::allocator_traits<Allocator>::construct(mAllocator, mCharArray.get() + i, pCharArray[i]);
	}
}

template <typename T, typename Allocator>
MyString<T, Allocator>::MyString(const MyString& other, const size_t positions, const Allocator& alloc) : mAllocator(alloc) {
	mCapacity = mSize = other.mSize - positions;
	mCharArray = allocate_unique(mAllocator, mCapacity);

	for (size_t i = 0; i < mSize; ++i) {
		std::allocator_traits<Allocator>::construct(mAllocator, mCharArray.get() + i, other.mCharArray.get()[positions + i]);
	}
}

template <typename T, typename Allocator>
MyString<T, Allocator>::MyString(const MyString& other, const size_t positions, const size_t count, const Allocator& alloc) : mAllocator(alloc) {
	if (positions + count >= other.mSize)
		mSize = other.mSize - positions;
	else
		mSize = mCapacity = count;
	mCharArray = allocate_unique(mAllocator, mCapacity);

	for (size_t i = 0; i < mSize; ++i) {
		std::allocator_traits<Allocator>::construct(mAllocator, mCharArray.get() + i, other.mCharArray.get()[positions + i]);
	}
}

template <typename T, typename Allocator>
MyString<T, Allocator>::MyString(const MyString& other, const Allocator& alloc) : mSize(other.mSize), mCapacity(other.mCapacity), mAllocator(alloc) {
	mCharArray = allocate_unique(mAllocator, mCapacity);

	for (size_t i = 0; i < mSize; ++i) {
		std::allocator_traits<Allocator>::construct(mAllocator, mCharArray.get() + i, other.mCharArray[i]);
	}
}

template <typename T, typename Allocator>
MyString<T, Allocator>::MyString(const MyString& other) : mSize(other.mSize), mCapacity(other.mCapacity), mAllocator(other.mAllocator) {
	mCharArray = allocate_unique(mAllocator, mCapacity);

	for (size_t i = 0; i < mSize; ++i) {
		std::allocator_traits<Allocator>::construct(mAllocator, mCharArray.get() + i, other.mCharArray[i]);
	}
}

template <typename T, typename Allocator>
MyString<T, Allocator>::MyString(MyString&& other, const Allocator& alloc) noexcept : mSize(other.mSize), mCapacity(other.mCapacity),
mCharArray(other.mCharArray.get()), mAllocator(std::move(alloc)) {
	other.mSize = 0;
	other.mCapacity = 0;
	other.mCharArray = nullptr;
}

template <typename T, typename Allocator>
MyString<T, Allocator>::MyString(MyString&& other) noexcept : mSize(other.mSize), mCapacity(other.mCapacity),
mCharArray(std::move(other.mCharArray)), mAllocator(std::move(other.mAllocator)) {
	other.mSize = 0;
	other.mCapacity = 0;
	other.mCharArray = nullptr;
}

template <typename T, typename Allocator>
MyString<T, Allocator>::MyString(std::initializer_list<T> ilist, const Allocator& alloc) :
	mSize(ilist.size()), mCapacity(ilist.size()), mAllocator(alloc) {
	mCharArray = allocate_unique(mAllocator, mCapacity);
	int i = 0;
	for (auto it = ilist.begin(); it < ilist.end(); ++i, ++it) {
		std::allocator_traits<Allocator>::construct(mAllocator, mCharArray.get() + i, *it);
	}
}

// MyString ~Destructor

template <typename T, typename Allocator>
MyString<T, Allocator>::~MyString() {
	for (int i = 0; i < mSize; ++i) {
		std::allocator_traits<Allocator>::destroy(mAllocator, mCharArray.get() + i);
	}
}

// MyString operators

template <typename T, typename Allocator>
inline T& MyString<T, Allocator>::operator [](const size_t index) {
	return mCharArray[index];
}
template <typename T, typename Allocator>
MyString<T, Allocator> MyString<T, Allocator>::operator =(const MyString& other) {
	MyString temp = other;
	swap(temp);
	return *this;
}

template <typename T, typename Allocator>
MyString<T, Allocator>& MyString<T, Allocator>::operator =(MyString&& other) noexcept {
	MyString<T, Allocator> temp = std::move(other);
	swap(temp);
	return *this;
}

template <typename T, typename Allocator>
MyString<T, Allocator>& MyString<T, Allocator>::operator +=(MyString other) {
	if (mSize + other.mSize > mCapacity) {
		MyString<T, Allocator> temp;
		try {
			temp.reserve(this->mSize + other.mSize);

			for (size_t i = 0; i < mSize; ++i) {
				temp.push_back(this->at(i));
			}

			for (size_t i = 0; i < other.mSize; ++i) {
				temp.push_back(other.at(i)); 
			}

		}
		catch (std::bad_alloc) {
			throw;
		}

		temp.mSize = this->mSize + other.mSize;
		clear();
		return *this = std::move(temp);
	}

	for (auto it_other = other.begin(); it_other < other.end(); ++it_other) {
		push_back(*it_other);
	}

	return *this;
}

template <typename T, typename Allocator>
MyString<T, Allocator> operator+ (const MyString<T, Allocator>& strLeft, const MyString<T, Allocator>& strRigth) {
	MyString<T, Allocator> temp = strLeft;
	temp += strRigth;
	return temp;
}


// bool operators

template <typename T, typename Allocator>
bool operator== (const MyString<T, Allocator>& strLeft, const MyString<T, Allocator>& strRigth) {
	if (strLeft.mSize != strRigth.mSize) return false;

	for (size_t i = 0; i < strLeft.mSize; ++i) {
		if (strLeft.mCharArray.get().get() + i != strRigth.mCharArray.get().get() + i) return true;
	}

	return true;
}

template <typename T, typename Allocator>
bool operator!= (const MyString<T, Allocator>& strLeft, const MyString<T, Allocator>& strRigth) {
	return !(strLeft == strRigth);
}

template <typename T, typename Allocator>
bool operator< (const MyString<T, Allocator>& strLeft, const MyString<T, Allocator>& strRigth) {
	return strLeft.mSize < strRigth.mSize;
}

template <typename T, typename Allocator>
bool operator> (const MyString<T, Allocator>& strLeft, const MyString<T, Allocator>& strRigth) {
	return strLeft.mSize > strRigth.mSize;
}

template <typename T, typename Allocator>
bool operator<= (const MyString<T, Allocator>& strLeft, const MyString<T, Allocator>& strRigth) {
	return !(strLeft > strRigth);
}

template <typename T, typename Allocator>
bool operator>= (const MyString<T, Allocator>& strLeft, const MyString<T, Allocator>& strRigth) {
	return !(strLeft < strRigth);
}

// metods get

template <typename T, typename Allocator>
T& MyString<T, Allocator>::at(const size_t index) {
	if (index >= mSize) throw std::out_of_range("out_of_range");
	return mCharArray[index];
}

template <typename T, typename Allocator>
size_t MyString<T, Allocator>::capacity() noexcept {
	return mCapacity;
}

template <typename T, typename Allocator>
size_t MyString<T, Allocator>::size() noexcept {
	return mSize;
}

// metods iterators

template <typename T, typename Allocator>
typename MyString<T, Allocator>::iterator MyString<T, Allocator>::begin() noexcept {
	iterator it(mCharArray.get());
	return it;
}

template <typename T, typename Allocator>
typename MyString<T, Allocator>::iterator MyString<T, Allocator>::end() noexcept {
	iterator it(mCharArray.get() + mSize);
	return it;
}

template <typename T, typename Allocator>
typename MyString<T, Allocator>::const_iterator MyString<T, Allocator>::cbegin() noexcept {
	const_iterator it(mCharArray.get()); 
	return it;
}

template <typename T, typename Allocator>
typename MyString<T, Allocator>::const_iterator MyString<T, Allocator>::cend() noexcept {
	const_iterator it(mCharArray.get() + mSize); 
	return it;
}

template <typename T, typename Allocator>
typename MyString<T, Allocator>::reverse_iterator MyString<T, Allocator>::rbegin() noexcept {
	reverse_iterator it(mCharArray.get());
	return it;
}

template <typename T, typename Allocator>
typename MyString<T, Allocator>::reverse_iterator MyString<T, Allocator>::rend() noexcept {
	reverse_iterator it(mCharArray.get() + mSize);
	return it;
}
template <typename T, typename Allocator>
typename MyString<T, Allocator>::const_reverse_iterator MyString<T, Allocator>::crbegin() noexcept {
	const_reverse_iterator it(mCharArray.get());
	return it;
}

template <typename T, typename Allocator>
typename MyString<T, Allocator>::const_reverse_iterator MyString<T, Allocator>::crend() noexcept {
	const_reverse_iterator it(mCharArray.get() + mSize);
	return it;
}


// management methods

template <typename T, typename Allocator>
void MyString<T, Allocator>::clear() noexcept {
	if (!empty()) {
		for (size_t i = 0; i < mSize; ++i) {
			std::allocator_traits<Allocator>::destroy(mAllocator, mCharArray.get() + i);
		}

		mSize = 0;
		mCapacity = 0;
		mCharArray = std::move(allocate_unique(mAllocator, 0));
	}
}

template <typename T, typename Allocator>
bool MyString<T, Allocator>::empty() noexcept {
	return begin() == end();
}

template <typename T, typename Allocator>
void MyString<T, Allocator>::reserve(const size_t num) {
	if (num <= mCapacity) return;
	MyString<T, Allocator> temp;
	temp.mCharArray = std::move(allocate_unique(mAllocator, num)); 
	for (size_t i = 0; i < mSize; ++i) {
		std::allocator_traits<Allocator>::construct(mAllocator, temp.mCharArray.get() + i, mCharArray[i]);
	}

	temp.mSize = mSize;
	temp.mCapacity = num;

	clear();
	*this = std::move(temp);
}


template <typename T, typename Allocator>
void MyString<T, Allocator>::copy(const MyString& other) {
	if (!empty()) clear();
	*this += other;
}

template <typename T, typename Allocator>
void MyString<T, Allocator>::resize(const size_t newSize, T ch) {
	if (newSize == mSize) return;	
	if (newSize < mSize) {

		for (size_t i = newSize; i < mSize; ++i) {
			std::allocator_traits<Allocator>::destroy(mAllocator, mCharArray.get() + i);
		}

		mSize = newSize;
		return;
	}
	if (newSize > mCapacity) reserve(newSize);

	for (size_t i = mSize - 1; i < newSize; ++i) {
		std::allocator_traits<Allocator>::construct(mAllocator, mCharArray.get() + i, ch);
	}

	mSize = newSize;
}

template <typename T, typename Allocator>
void MyString<T, Allocator>::shrink_to_fit() {
	MyString<T, Allocator> temp;
	temp.reserve(mSize);

	for (size_t i = 0; i < mSize; ++i) {
		temp.push_back(mCharArray[i]);
	}

	clear();
	*this = std::move(temp);
}

template <typename T, typename Allocator>
void MyString<T, Allocator>::push_back(const T ch) {
	if (empty()) reserve(1);
	if (mSize == mCapacity) reserve(mCapacity * 2);
	std::allocator_traits<Allocator>::construct(mAllocator, mCharArray.get() + mSize, ch);
	++mSize;
}
template <typename T, typename Allocator>
void MyString<T, Allocator>::pop_back() noexcept {
	std::allocator_traits<Allocator>::destroy(mAllocator, mCharArray.get() + (mSize - 1));
	--mSize;
}

template <typename T, typename Allocator>
MyString<T, Allocator>& MyString<T, Allocator>::erace(size_t index, size_t count) {
	size_t index_no_tail_removed = index + count;
	if (index_no_tail_removed >= mSize) throw std::out_of_range("out_of_range");
	if (count == npos) count = mSize - index;

	for (size_t i = index; i < mSize - count; ++i, ++index_no_tail_removed) {
		mCharArray[i] = mCharArray[index_no_tail_removed];
	}

	for (size_t i = mSize - count; i < mSize; ++i) {
		std::allocator_traits<Allocator>::destroy(mAllocator, mCharArray.get() + i);
	}

	mSize -= count;
	return *this;
}

template <typename T, typename Allocator>
MyString<T, Allocator>::iterator MyString<T, Allocator>::erace(iterator positions) noexcept { 
	if (positions == end() - 1) {
		pop_back();
		return positions;
	}

	for (iterator it = positions; it < end() - 1; ++it) {
		it = it + 1;
	}

	if (positions == end() - 1) {
		pop_back();
	}
	mSize -= 1;
	return positions;
}

template <typename T, typename Allocator>
MyString<T, Allocator>::iterator MyString<T, Allocator>::erace(const_iterator first, const_iterator last) noexcept {
	if (last >= end()) {
		for (iterator it = first; it < end(); ++it) {
			std::allocator_traits<Allocator>::destroy(mAllocator, it);
		}
		mSize -= std::distance(first, end());
		return *this;
	}

	for (iterator it = first, it_last = last; it < end() - std::distance(last, first); ++it, ++it_last) {
		it = it_last;
	}

	for (iterator it = end() - std::distance(last, first); it < end(); it) {
		pop_back;
	}
	return last;
}

template <typename T, typename Allocator>
MyString<T, Allocator> MyString<T, Allocator>::substr(size_t positions, size_t count) {
	if (count > mSize) throw std::out_of_range;
	if (count == npos) count = mSize;
	MyString<T, Allocator> temp(mCharArray.get() + positions, count);
	return temp;
}


template <typename T, typename Allocator>
MyString<T, Allocator>& MyString<T, Allocator>::insert(size_t index, size_t count, T ch) {
	if (mSize + count > mCapacity) {
		MyString<T, Allocator> temp;
		try {
			temp.reserve(mSize + count);

			for (size_t i = 0; i < index; ++i) {
				temp.push_back(mCharArray.get()[i]);
			}

			for (size_t i = 0; i < count; ++i) {
				temp.push_back(ch);
			}

			for (size_t i = index; i < mSize; ++i) {
				temp.push_back(mCharArray.get()[i]);
			}
		}
		catch (std::bad_alloc) {
			throw;
		}

		clear();
		return *this = std::move(temp);
	}

	for (size_t i = mSize - 1; i > index; --i) {
		if (i > mSize - count - 1) {
			std::allocator_traits<Allocator>::construct(mAllocator, mCharArray.get() + i + count, mCharArray.get()[i]);
			continue;
		}
		mCharArray.get()[i + count] = mCharArray.get()[i];
	}

	for (size_t i = index; i < index + count; ++i) {
		mCharArray.get()[i] = ch;
	}

	mSize += count;
	return *this;
}

template <typename T, typename Allocator>
MyString<T, Allocator>& MyString<T, Allocator>::insert(size_t index, T* ch) {
	MyString temp(*this, 0, index);
	iterator it = temp.begin() + index;
	for (size_t i = 0; *(ch + i) != '\0'; ++it, ++i) {
		if (it == temp.end()) {
			temp.push_back(*(ch + i));
			continue;
		}
		*it = *(ch + i);
	}

	for (size_t i = index; i < mSize; ++i, ++it) {
		if (it == temp.end()) {
			temp.push_back(*(mCharArray.get() + i));
			continue;
		}
		*it = *(mCharArray.get() + i);
	}

	clear();
	*this = std::move(temp);
	return *this;
}

template <typename T, typename Allocator>
MyString<T, Allocator>& MyString<T, Allocator>::insert(size_t index, T* ch, size_t count) {
	if (mSize + count > mCapacity) {
		try {
			MyString temp(*this, 0, index);
			temp.reserve(mSize + count);
			iterator it = temp.begin() + index;

			for (size_t i = 0; i < count; ++i, ++it) {
				push_back(*(ch + i));
			}

			for (size_t i = index; i < mSize; +i, ++it) {
				push_back(*(mCharArray.get() + i));
			}

			clear();
			*this = std::move(temp);
			return *this;
		}
		catch (std::bad_alloc) {
			throw;
		}
	}

	for (iterator it = end() - 1; it >= begin() + index; --it) {
		*it = *(it + count);
	}

	iterator it = begin() + index;
	for (size_t i = 0; i < count; ++it, ++i, ++it) {
		*it = *(ch + i);
	}

	mSize += count;
	return *this;
}

template <typename T, typename Allocator>
MyString<T, Allocator>& MyString<T, Allocator>::insert(size_t index, const MyString& other) {
	if (mSize + other.mSize > mCapacity) {
		MyString temp;
		temp.reserve(mSize + other.size() * 2);

		for (size_t i = 0; i < index; ++i) {
			push_back(*this->at(i));
		}

		temp += other;

		for (size_t i = index; i < mSize; ++i) {
			push_back(*this->at(i));
		}

		clear();
		*this = std::move(temp);
		return *this;
	}

	for (iterator it = end() - 1; it >= begin() + index; --it) {
		if (it >= end()) {
			std::allocator_traits<Allocator>::construct(mAllocator, it + other.size(), *it);
			continue;
		}
		*it = *(it + other.size());
	}

	for (iterator it = begin() + index, it_other = other.begin(); it_other < other.end(); ++it, ++it_other) {
		*it = *it_other;
	}

	mSize += other.size();
	return *this;

}

template <typename T, typename Allocator>
MyString<T, Allocator>& MyString<T, Allocator>::insert(size_t index, const MyString& other, size_t other_index, size_t count) {
	if (mSize + count > mCapacity) {
		try {
			MyString temp;
			temp.reserve(mSize + count * 2);

			for (size_t i = 0; i < index; ++i) {
				temp.push_back(*this->at(i));
			}

			for (size_t i = other_index; i < count + other_index; ++i) {
				temp.push_back(i);
			}

			for (size_t i = index; i < mSize; ++i) {
				temp.push_back(*this->at(i));
			}

			clear();
			*this = std::move(temp);
			return *this;
		}
		catch (std::bad_alloc) {
			throw;
		}
	}

	for (iterator it = *this.end() - 1; it >= begin() + index; --it) {
		if (it + count >= end()) {
			std::allocator_traits<Allocator>::construct(mAllocator, it + count, *it);
			continue;
		}
		*(it + count) = *it;
	}


	for (iterator it = begin() + index, it_other = other.begin() + other_index; it_other < (other.begin() + other_index + count); ++it, ++it_other) {
		*it = *it_other;
	}

	mSize += count;
	return *this;
}

template <typename T, typename Allocator>
typename MyString<T, Allocator>::iterator MyString<T, Allocator>::insert(iterator pos, T ch) {
	try {
		push_back(end() - 1);

		for (iterator it = *this.end() - 3; it > pos; --it) {
			*(it + 1) = *it;
		}

		*pos = ch;
		return pos + 1;
	}
	catch (std::bad_alloc) {
		throw;
	}
};

template <typename T, typename Allocator>
typename MyString<T, Allocator>::iterator MyString<T, Allocator>::insert(iterator pos, size_t count, T ch) {
	if (mSize + count >= mCapacity) {
		try {
			MyString temp;
			temp.reserve(mSize + count * 2);
			iterator it_temp = temp.begin();
			iterator it_this = begin();

			for (; it_temp <= pos; ++it_temp, ++it_this) {
				*it_temp = *it_this;
			}

			for (; it_temp <= pos + count; ++it_temp) {
				*it_temp = ch;
			}

			for (; it_this < end(); ++it_temp, ++it_this) {
				*it_temp = *it_this;
			}

			temp.mSize = size() + count;
			clear();
			return *this = std::move(temp);
		}
		catch (std::bad_alloc) {
			throw;
		}
	}
	for (iterator it = *this.end() - 1; it > pos; --it) {
		if (it + count >= end()) {
			std::allocator_traits<Allocator>::construct(mAllocator, it + count, *it);
			continue;
		}
		*(it + count) = *it;
	}

	for (iterator it = pos + 1; it <= pos + count; ++it) {
		*it = ch;
	}

	mSize += count;
	return pos + count;
};

template <typename T, typename Allocator>
template <typename IteratorInput>
typename MyString<T, Allocator>::iterator MyString<T, Allocator>::insert(iterator pos, IteratorInput first, IteratorInput last) {
	if (mSize + std::distance<T>(first, last) > mCapacity) {
		try {
			MyString temp;
			temp.reserve(mCapacity + std::distance<T>(first, last) * 2);
			iterator it_temp = temp.begin();

			for (iterator it_this = begin(); it_this < pos; ++it_temp, ++it_this) {
				*it_temp = *it_this;
			}

			std::copy(first, last, it_temp);

			for (iterator it_this = pos + 1; it_this < end(); ++it_this, ++it_temp) {
				*it_temp = *it_this;
			}

			temp.mSize = mSize + std::distance<T>(first, last);
			clear();
			return *this = std::move(temp);
		}
		catch (std::bad_alloc) {
			throw;
		}
	}

	for (iterator it = end() - 1; it > pos; --it) {
		*(it + std::distance(first, last)) = *it;
	}

	std::copy(first, last, pos);

	mSize += std::distance(first, last);
	return pos + std::distance(first, last);
};

// append

template <typename T, typename Allocator>
MyString<T, Allocator>& MyString<T, Allocator>::append(size_t count, T ch) {
	if (mSize + count > mCapacity) {
		try {
			MyString temp;
			temp.reserve(mSize + count * 2);
			temp.copy(*this);
			clear();
			*this = std::move(temp);
		}
		catch (std::bad_alloc) {
			throw;
		}

	}

	for (size_t i = 0; i < count; ++i) {
		push_back(*ch);
	}

	return *this;
};

template <typename T, typename Allocator>
MyString<T, Allocator>& MyString<T, Allocator>::append(T* ch) {
	MyString temp(*this);

	for (; *ch != '\0'; ++ch) {
		temp.push_back(*ch);
	}

	clear();
	return *this = std::move(temp);
};

template <typename T, typename Allocator>
MyString<T, Allocator>& MyString<T, Allocator>::append(T* ch, size_t count) {
	if (mSize + count > mCapacity) {
		try {
			MyString temp;
			reserve(mSize + count * 2);
			temp.copy(*this);
			clear();
			*this = std::move(temp);
		}
		catch (std::bad_alloc) {
			throw;
		}
	}

	for (size_t i = 0; i < count; ++i, ++ch) {
		push_back(*ch);
	}

	return *this;
};

template <typename T, typename Allocator>
MyString<T, Allocator>& MyString<T, Allocator>::append(const MyString& other) {
	return *this += other;
}

template <typename T, typename Allocator>
MyString<T, Allocator>& MyString<T, Allocator>::append(const MyString& other, size_t other_index, size_t count) {
	if (mSize + count > mCapacity) {
		MyString temp;
		reserve(mSize + count * 2);
		temp.copy(*this);
		clear();
		*this = std::move(temp);
	}

	for (size_t i = 0; i < count; ++i, ++other_index) {
		push_back(other.at(other_index));
	}

	return *this;
}

template <typename T, typename Allocator>
typename MyString<T, Allocator>::iterator MyString<T, Allocator>::append(T ch, size_t count) {
	if (mSize + count > mCapacity) {
		MyString temp;
		reserve(mSize + count * 2);
		temp.copy(*this);
		clear();
		*this = std::move(temp);
	}

	for (size_t i = 0; i < count; ++i) {
		push_back(ch);
	}

	return end() - 1;
}

template <typename T, typename Allocator>
template <typename InputIterator>
typename MyString<T, Allocator>::iterator MyString<T, Allocator>::append(InputIterator first, InputIterator last) {
	if (mSize + std::distance(first, last) > mCapacity) {
		MyString temp;
		reserve(mSize + std::distance(first, last) * 2);
		temp.copy(*this);
		clear();
		*this = std::move(temp);
	}

	for (size_t i = 0; first + i != last; ++i) {
		push_back(*(first + i));
	}

	return end() - 1;
}


//replace

template <typename T, typename Allocator>
MyString<T, Allocator>& MyString<T, Allocator>::replace(size_t index, size_t count, T ch) {
	if (index >= mSize && index + count >= mSize) throw std::out_of_range;

	for (size_t i = index; i < count + index; ++i) {
		this->at(i) = ch;
	}

	return *this;
}

template <typename T, typename Allocator>
MyString<T, Allocator>& MyString<T, Allocator>::replace(size_t index, T* ch) {
	if (index >= mSize) throw std::out_of_range;
	MyString temp = *this;

	for (size_t i = 0; i != '\0'; ++i, ++index) {
		if (index >= temp.size()) {
			temp.clear();
			throw std::out_of_range;
		}
		temp[index] = ch[i];
	}

	clear();
	return *this = std::move(temp);
}

template <typename T, typename Allocator>
MyString<T, Allocator>& MyString<T, Allocator>::replace(size_t index, T* ch, size_t count) {
	if (index >= mSize && index + count >= mSize) throw std::out_of_range;

	for (size_t i = 0; i < count; ++i, ++index) {
		this->at(index) = ch[i];
	}

	return *this;
}

template <typename T, typename Allocator>
MyString<T, Allocator>& MyString<T, Allocator>::replace(size_t index, const MyString& other) {
	if (index >= mSize && index + other.mSize() >= mSize) throw std::out_of_range;

	for (size_t i = 0; i < other.size(); ++i, ++index) {
		this->at(index) = other[0];
	}

	return *this;
}

template <typename T, typename Allocator>
MyString<T, Allocator>& MyString<T, Allocator>::replace(size_t index, const MyString& other, size_t other_index, size_t count) {
	if (index >= mSize && index + count >= mSize) throw std::out_of_range;

	for (; other_index < count; ++index, ++other_index) {
		this->at(index) = other[other_index];
	}

	return *this;
}

template <typename T, typename Allocator>
typename MyString<T, Allocator>::iterator MyString<T, Allocator>::replace(iterator pos, T ch) {
	if (pos >= end()) throw std::out_of_range;
	*pos = ch;
	return *pos;
}

template <typename T, typename Allocator>
typename MyString<T, Allocator>::iterator MyString<T, Allocator>::replace(iterator pos, size_t count, T ch) {
	if (pos >= end() && pos + count >= end()) throw std::out_of_range;

	for (; pos < pos + count; ++pos) {
		*pos = ch;
	}

	return *pos;
}

template <typename T, typename Allocator>
template <typename InputIterator>
typename MyString<T, Allocator>::iterator MyString<T, Allocator>::replace(iterator pos, InputIterator first, InputIterator last) {
	if (pos >= end() && pos + std::distance(first, last) >= end()) throw std::out_of_range;

	for (; first < last; ++pos, ++first) {
		*pos = *first;
	}

	return *pos;
}