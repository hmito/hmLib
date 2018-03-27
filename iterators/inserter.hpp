#ifndef HMLIB_ITERATORS_INSERTER_INC 
#define HMLIB_ITERATORS_INSERTER_INC 100 
# 
#include<iterator> 
#include<utility> 
namespace hmLib {
	template<typename container>
	class back_insert_iterator {
	private:
		using this_type = back_insert_iterator<container>;
	public:
		using iterator_category = std::output_iterator_tag;
		using value_type = typename container::value_type;
		using difference_type = int;
		using pointer = typename container::pointer;
		using reference = typename container::reference;
		using container_type = container;
	private:
		struct proxy {
		public:
			proxy(this_type* This_) :This(This_) {}
			proxy(const proxy&) = default;
			proxy& operator=(const proxy&) = default;
			proxy(proxy&&) = default;
			proxy& operator=(proxy&&) = default;
		public:
			proxy& operator=(const value_type& val) {
				This->insert(val);
				return *this;
			}
			proxy& operator=(value_type&& val) {
				This->insert(std::move(val));
				return *this;
			}
		private:
			this_type* This;
		};
	public:
		back_insert_iterator() :Ptr(nullptr) {}
		explicit back_insert_iterator(container_type& Container): Ptr(&Container) {}
	public:
		proxy operator*() { return proxy(this);}
		this_type& operator++() {
			return (*this);
		}
		this_type operator++(int) {
			return (*this);
		}
	protected:
		void insert(const value_type& value) {Ptr->push_back(value);}
		void insert(value_type&& value) { Ptr->push_back(std::move(value)); }
	private:
		container* Ptr;
	};
	template<typename container>
	back_insert_iterator<container> back_inserter(container& Container) {
		return back_insert_iterator<container>(Container);
	}

	template<typename container>
	class front_insert_iterator {
	private:
		using this_type = front_insert_iterator<container>;
	public:
		using iterator_category = std::output_iterator_tag;
		using value_type = typename container::value_type;
		using difference_type = int;
		using pointer = typename container::pointer;
		using reference = typename container::reference;
		using container_type = container;
	private:
		struct proxy {
		public:
			proxy(this_type* This_) :This(This_) {}
			proxy(const proxy&) = default;
			proxy& operator=(const proxy&) = default;
			proxy(proxy&&) = default;
			proxy& operator=(proxy&&) = default;
		public:
			proxy & operator=(const value_type& val) {
				This->insert(val);
				return *this;
			}
			proxy& operator=(value_type&& val) {
				This->insert(std::move(val));
				return *this;
			}
		private:
			this_type* This;
		};
	public:
		front_insert_iterator() :Ptr(nullptr) {}
		explicit front_insert_iterator(container_type& Container): Ptr(&Container) {}
	public:
		proxy operator*() { return proxy(this); }
		this_type& operator++() {
			return (*this);
		}
		this_type operator++(int) {
			return (*this);
		}
	protected:
		void insert(const value_type& value) { Ptr->push_front(value); }
		void insert(value_type&& value) { Ptr->push_front(std::move(value)); }
	private:
		container* Ptr;
	};
	template<typename container>
	front_insert_iterator<container> front_inserter(container& Container) {
		return front_insert_iterator<container>(Container);
	}

	template<typename container,typename iterator>
	class insert_iterator {
	private:
		using this_type = insert_iterator<container, iterator>;
	public:
		using iterator_category = std::output_iterator_tag;
		using value_type = typename container::value_type;
		using difference_type = int;
		using pointer = typename container::pointer;
		using reference = typename container::reference;
		using container_type = container;
	private:
		struct proxy {
		public:
			proxy(this_type* This_) :This(This_) {}
			proxy(const proxy&) = default;
			proxy& operator=(const proxy&) = default;
			proxy(proxy&&) = default;
			proxy& operator=(proxy&&) = default;
		public:
			proxy& operator=(const value_type& val) {
				This->insert(val);
				return *this;
			}
			proxy& operator=(value_type&& val) {
				This->insert(std::move(val));
				return *this;
			}
		private:
			this_type* This;
		};
	public:
		insert_iterator() :Ptr(nullptr) {}
		explicit insert_iterator(container_type& Container, iterator Iterator) : Ptr(&Container), Itr(Iterator){}
	public:
		proxy operator*() { return proxy(this); }
		this_type& operator++() {
			return (*this);
		}
		this_type operator++(int) {
			return (*this);
		}
	protected:
		void insert(const value_type& value) { Itr = Ptr->insert(Itr, value); }
		void insert(value_type&& value) { Itr = Ptr->insert(Itr, std::move(value)); }
	private:
		container* Ptr;
		iterator Itr;
	};
	template<typename container, typename iterator>
	insert_iterator<container, iterator> inserter(container& Container, iterator Itr) {
		return insert_iterator<container, iterator>(Container, Itr);
	}

}
# 
#endif 
