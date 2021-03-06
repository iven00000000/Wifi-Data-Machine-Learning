#pragma once
#ifndef HASH_H
#define HASH_H

#include <xutility>

namespace std{

template<class MyHash, class TableOrBucketIter, class value_type>
class TableBucketIterator
	: public iterator<random_access_iterator_tag, value_type>
{
public:
	typedef TableBucketIterator<MyHash, TableOrBucketIter, value_type> MyIter;
	typedef TableOrBucketIter ChildIter;

	typedef typename iterator<random_access_iterator_tag, value_type>::difference_type difference_type;
	typedef value_type* pointer;
	typedef value_type& reference;

	TableBucketIterator()
	{
	}

	TableBucketIterator(pointer ptr) : ptr(ptr)
	{
	}

	inline reference operator*() const
	{
		return *ptr;
	}

	inline pointer operator->() const
	{
		return ptr;
	}

	inline ChildIter& operator++()
	{
		++ptr;
		return *(static_cast<ChildIter*>(const_cast<MyIter*>(this)));
	}

	inline ChildIter operator++(int)
	{
		ChildIter tmp = *(static_cast<ChildIter*>(const_cast<MyIter*>(this)));
		++(*this);
		return tmp;
	}

	inline ChildIter& operator--()
	{
		--ptr;
		return *(static_cast<ChildIter*>(const_cast<MyIter*>(this)));
	}

	inline ChildIter operator--(int)
	{
		ChildIter tmp = *(static_cast<ChildIter*>(const_cast<MyIter*>(this)));
		--(*this);
		return tmp;
	}

	inline ChildIter& operator+=(difference_type offset)
	{
		ptr += offset;
		return *(static_cast<ChildIter*>(const_cast<MyIter*>(this)));
	}

	inline ChildIter operator+(difference_type offset)
	{
		ChildIter tmp = *(static_cast<ChildIter*>(const_cast<MyIter*>(this)));
		return tmp += offset;
	}

	friend ChildIter operator+(difference_type offset, ChildIter right);

	inline ChildIter& operator-=(difference_type offset)
	{
		return *(static_cast<ChildIter*>(const_cast<MyIter*>(this))) += -offset;
	}

	inline ChildIter operator-(difference_type offset)
	{
		ChildIter tmp = *(static_cast<ChildIter*>(const_cast<MyIter*>(this)));
		return tmp += -offset;
	}
	
	inline difference_type operator-(ChildIter right)
	{
		return this->ptr - right->ptr;
	}

	inline reference operator[](difference_type offset)
	{
		return *(*(static_cast<ChildIter*>(const_cast<MyIter*>(this))) + offset);
	}

	bool operator==(const ChildIter& right) const
	{
		return this->ptr == right.ptr;
	}

	bool operator!=(const ChildIter& right) const
	{
		return !(*this == right);
	}

	bool operator<(const ChildIter& right) const
	{
		return this->ptr < right.ptr;
	}

	bool operator>(const ChildIter& right) const
	{
		return this->ptr > right.ptr;
	}

	bool operator<=(const MyIter& right) const
	{
		return this->ptr <= right.ptr;
	}

	bool operator>=(const ChildIter& right) const
	{
		return this->ptr >= right.ptr;
	}

	value_type *ptr;

};

template<class MyHash, class TableOrBucketIter, class value_type> inline
TableOrBucketIter operator+(
	typename TableBucketIterator<MyHash, TableOrBucketIter, value_type>::difference_type offset,
	TableOrBucketIter right)
{
	return right + offset;
}

template<class MyHash>
class BucketIterator
	: public TableBucketIterator<MyHash, BucketIterator<MyHash>, typename MyHash::Bucket::value_type>
{
public:
	typedef typename MyHash::Bucket::value_type value_type;

	BucketIterator()
	{
	}

	BucketIterator(pointer ptr) : TableBucketIterator<MyHash, BucketIterator<MyHash>, typename MyHash::Bucket::value_type>(ptr)
	{
	}
};

template<class MyHash>
class TableIterator
	: public TableBucketIterator<MyHash, TableIterator<MyHash>, typename MyHash::Table::value_type>
{
public:
	typedef typename MyHash::Table::value_type value_type;

	TableIterator()
	{
	}

	TableIterator(pointer ptr) : TableBucketIterator<MyHash, TableIterator<MyHash>, typename MyHash::Table::value_type > (ptr)
	{
	}
};

template<class MyHash>
class HashIterator
	: public iterator<bidirectional_iterator_tag,
	typename MyHash::value_type>
{
public:
	typedef HashIterator<MyHash> MyIter;

	typedef typename MyHash::Table Table;
	typedef typename MyHash::Bucket Bucket;
	
	HashIterator()
	{
	}

	HashIterator(typename Table::iterator Titer, typename Bucket::iterator Biter)
	{
		this->Titer = Titer;
		this->Biter = Biter;
	}

	HashIterator(typename Bucket::iterator Biter)
	{
		this->Biter = Biter;
	}

	inline MyIter& operator++()
	{
		++Biter;
		while (Biter == (*Titer).end())
		{
			Biter = (*(++Titer)).begin();
		}
		return(*this);
	}

	inline MyIter operator++(int)
	{
		MyIter tmp = *this;
		++(*this);
		return tmp;
	}

	inline MyIter& operator--()
	{
		while (Biter == (*Titer).begin())
		{
			Biter = (*--Titer).end();
		}
		--Biter;
		return(*this);
	}

	inline MyIter operator--(int)
	{
		MyIter tmp = *this;
		--(*this);
		return tmp;
	}

	inline reference operator*() const
	{
		return *Biter;
	}

	inline pointer operator->() const
	{
		return &(*Biter);
	}

	bool operator==(const MyIter& right) const
	{
		return (this->Titer == right.Titer) && (this->Biter == right.Biter);
	}

	bool operator!=(const MyIter& right) const
	{
		return !(*this == right);
	}

	bool operator<(const MyIter& right) const
	{
		if (this->Titer < right.Titer) return true;
		if (this->Titer == right.Titer) return this->Biter < right.Biter;
		return false;
	}

	bool operator>(const MyIter& right) const
	{
		return !(*this < right || *this == right);
	}

	bool operator<=(const MyIter& right) const
	{
		return !(*this > right);
	}

	bool operator>=(const MyIter& right) const
	{
		return !(*this < right);
	}

	typename Table::iterator Titer;
	typename Bucket::iterator Biter;
};

template<class MyHash, class TableOrBucket, class value_type, class iterator>
class TableBucket
{
public:
	typedef TableBucket<MyHash, TableOrBucket, value_type, iterator> MyTB;

	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef value_type* pointer;
	typedef value_type& reference;

	TableBucket() : myBegin((pointer)NULL)
	{
		myEnd = myBegin;
		myLast = myBegin;
	}

	TableBucket(size_type reserveSize)
	{
		myBegin = new value_type[reserveSize];
		myEnd = myBegin;
		myLast = myBegin + reserveSize;
	}

	reference operator[](size_type n)
	{
		return *(myBegin + n);
	}

	iterator begin()
	{
		return iterator(myBegin);
	}

	iterator end()
	{
		return iterator(myEnd);
	}

	size_type size()
	{
		return myEnd - myBegin;
	}

	bool empty()
	{
		return myBegin == myEnd;
	}

	pointer data()
	{
		return myBegin;
	}

	reference front()
	{
		return *myBegin;
	}

	reference back()
	{
		return *(myEnd - 1);
	}

	size_type capacity() const
	{
		return myLast - myBegin;
	}

	void reserve(size_type size)
	{
		if (myLast - myBegin < size)
		{
			if (myLast - myBegin) delete[] myBegin;
			myBegin = new value_type[size];
			myLast = myBegin + size;
		}
	}



	pointer myBegin, myEnd, myLast;

};

template<class MyHash>
class Bucket :public TableBucket<MyHash, Bucket<MyHash>, typename MyHash::value_type, BucketIterator<MyHash>>
{
public:
	typedef TableBucket<MyHash, Bucket<MyHash>, typename MyHash::value_type, BucketIterator<MyHash>> TB;

	typedef typename MyHash::value_type value_type;
	typedef TB::size_type size_type;
	typedef TB::difference_type difference_type;
	typedef value_type* pointer;
	typedef value_type& reference;

	typedef BucketIterator<MyHash> iterator;

	Bucket() : TB()
	{

	}

	Bucket(size_type reservesize) : TB(reservesize)
	{
	}

	~Bucket()
	{
		if (myLast - myBegin){
			delete[] myBegin;
		} 
	}

	void extend()
	{
		if (myBegin == myLast)
		{
			myBegin = new value_type[1];
			myEnd = myBegin;
			myLast = myBegin + 1;
		}
		else
		{
			value_type *tmp = new value_type[(myLast - myBegin) << 1];
			size_type size = myEnd - myBegin, i = 0;
			myLast = tmp + ((myLast - myBegin) << 1);
			for (; i < size; ++i) tmp[i] = myBegin[i];
			delete[] myBegin;
			myBegin = tmp;
			myEnd = myBegin + size;
		}
	}

	void push_back(const value_type& val)
	{
		if (myEnd == myLast) extend();
		*myEnd++ = val;
	}
};

template<class MyHash>
class Table :public TableBucket<MyHash, Table<MyHash>, typename MyHash::Bucket, TableIterator<MyHash>>
{
public:
	typedef TableBucket<MyHash, Table<MyHash>, typename MyHash::Bucket, TableIterator<MyHash>> TB;

	typedef typename MyHash::Bucket value_type;
	typedef TB::size_type size_type;
	typedef TB::difference_type difference_type;
	typedef value_type* pointer;
	typedef value_type& reference;

	typedef TableIterator<MyHash> iterator;

	Table()
	{
	}

	Table(size_type reservesize) : TB(reservesize)
	{
		myEnd = myLast;
	}

	~Table()
	{
		if (myLast - myBegin) delete[] myBegin;
	}

	void reserve(size_type size)
	{
		TB::reserve(size + 1);
		myEnd = myLast - 1;
	}
};

template<class index_type>
class HashFunction
{
public:
	typedef size_t size_type;

	size_type tableSize;

	virtual size_type operator()(index_type idxItem)
	{
		return 0;
	};

	virtual ~HashFunction(){}
};

class StringSum : public HashFunction<string>
{
public:
	typedef HashFunction::size_type size_type;

	StringSum()
	{
		this->tableSize = 0;
	}
	~StringSum(){}

	StringSum(size_type tableSize)
	{
		this->tableSize = tableSize;
	}
	
	size_type operator()(string str)
	{
		//cout << "tableSize:" << tableSize << endl;
		size_type sum = 0;
		for (int i = 0; i < str.length(); ++i)
		{
			sum += str[i];
		}
		return sum%tableSize;
	}
};

template<class index_type, class content_type>
class Hash
{
public:
	typedef struct {
		index_type idx;
		content_type content;
	} value_type;
	
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef value_type* pointer;
	typedef value_type& reference;

	typedef Table<Hash<index_type, content_type>> Table;
	typedef Bucket<Hash<index_type, content_type>> Bucket;

	typedef HashIterator<Hash<index_type, content_type>> iterator;
	typedef typename Table::iterator titer;
	typedef typename Bucket::iterator biter;

	

	template<class C, C> struct HFChecker; //Hash Function Checker


	Table hashTable;
	size_type tableSize;
	HashFunction<index_type>* hFunc;

	Hash(int tableSize, HashFunction<index_type>* hFunc) : tableSize(tableSize)
	{
		//Check whether the hash function use correct assignment 
		HFChecker<size_type(HashFunction<index_type>::*)(index_type), &HashFunction<index_type>::operator()>;
		this->hFunc = hFunc;
		this->hFunc->tableSize = tableSize;

		hashTable.reserve(tableSize);
		(*hashTable.end()).myEnd = (*hashTable.end()).myBegin + 1;
		(*hashTable.end()).myLast = (*hashTable.end()).myEnd;
	}
	
	~Hash()
	{
		delete hFunc;
	}

	iterator begin()
	{
		iterator tmpIter = iterator(hashTable.begin(), (*hashTable.begin()).begin());
		if(tmpIter.Biter == (*tmpIter.Titer).end()) ++tmpIter;
		if (tmpIter.Titer.ptr > this->end().Titer.ptr) return end();
		return tmpIter;
	}

	iterator end()
	{
		return iterator(hashTable.end(), (*hashTable.end()).begin());
	}

	bool find(index_type idxItem, iterator& Hiter)
	{
		size_type tbIdx = (*hFunc)(idxItem);
		Hiter.Titer.ptr = &hashTable[tbIdx];
		for (Hiter.Biter = hashTable[tbIdx].begin(); Hiter.Biter != hashTable[tbIdx].end(); ++Hiter.Biter)
		{
			if (idxItem == (*Hiter.Biter).idx) return true;
		}
		return false;
	}

	void insert(iterator& iter, index_type idx, content_type content)
	{
		(*iter.Titer).push_back({ idx, content });
		iter.Biter = (*iter.Titer).end() - 1;
	}

	size_type size()
	{
		size_type size = 0;
		for (int i = 0; i < tableSize; ++i) size += hashTable[i].size();
		return size;
	}


private:

};

} //namespace std

#endif //HASH_H
