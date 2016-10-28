template<typename T>
struct chain{
public:
	using this_type = chain<T>;
	using size_type = unsigned int;
public:
	struct element{
	private:
		T value;
		element *next;
	public:
		operator bool(){ return next != 0; }
		T& operator*(){ return value; }
		const T& operator*()const{ return value; }
		T* operator->(){ return &value; }
		const T* operator->()const{ return &value; }
	};
	struct iterator{};
	struct const_iterator{};
private:
	element Sentinel;
	size_type Size;
public:
	iterator begin();
	iterator end();
	const_iterator cbegin()const;
	const_iterator cend()const;
	const_iterator begin()const{ return cbegin(); }
	const_iterator end()const{ return cend(); }
public:
	bool empty();
	size_type size();
	T& front();
	T& back();
	const T& front()const;
	const T& back()const;
	void push_back(element& Elem);
	void pop_back();
	void push_front(element& Elem);
	void pop_front();
	iterator insert(const_iterator pos, element& Elem);
	iterator erase(const_iterator pos);
	iterator erase(const_iterator pos, const_iterator last);
	void swap(this_type& other);
	void clear();
	void splice(const_iterator pos, this_type& other);
};

template<typename T, typename Comp = >
struct priority_chain{
public:
	using this_type = chain<T>;
	using size_type = unsigned int;
public:
	struct element{
	private:
		T value;
		element *next;
	public:
		operator bool(){ return next != 0; }
		T& operator*(){ return value; }
		const T& operator*()const{ return value; }
		T* operator->(){ return &value; }
		const T* operator->()const{ return &value; }
	};
	struct iterator{};
	struct const_iterator{};
private:
	element Sentinel;
public:
	iterator begin();
	iterator end();
	const_iterator cbegin()const;
	const_iterator cend()const;
	const_iterator begin()const{ return cbegin(); }
	const_iterator end()const{ return cend(); }
public:
	bool empty();
	T& front();
	const T& front()const;
	void push(element& Elem);
	void pop();
	void swap(this_type& other);
	void clear();
};
