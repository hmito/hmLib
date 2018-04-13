#ifndef HMLIB_ITERATORS_OVERFLOWITERATOR_INC
#define HMLIB_ITERATORS_OVERFLOWITERATOR_INC 100
#
#include<utility>
#include<iterator>
namespace hmLib {
	template<typename output_iterator>
	struct overflow_iterator {
	private:
		using this_type = overflow_iterator<output_iterator>;
	public:
		using iterator_category = std::output_iterator_tag;
		using value_type = typename std::iterator_traits<output_iterator>::value_type;
		using difference_type = typename std::iterator_traits<output_iterator>::difference_type;
		using pointer = typename std::iterator_traits<output_iterator>::pointer;
		using reference = typename std::iterator_traits<output_iterator>::reference;
		using iterator_type = output_iterator;
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
				if (!This->empty()) *(This->base()) = val;
				return *this;
			}
			proxy& operator=(value_type&& val) {
				if(!This->empty()) *(This->base()) = std::move(val);
				return *this;
			}
		private:
			this_type * This;
		};
	private:
		iterator_type Cur;
		iterator_type End;
		unsigned int Overflow;
	public:
		overflow_iterator(): Cur(), End(), Overflow(0){}
		overflow_iterator(const this_type&) = default;
		this_type& operator=(const this_type&) = default;
		overflow_iterator(iterator_type Cur_, iterator_type End_) : Cur(Cur_), End(End_), Overflow(0){}
	public:
		proxy operator*() { return proxy(this); }
		this_type& operator++() {
			if (Cur != End) {
				++Cur;
			}else {
				++Overflow;
			}
			return *this;
		}
		this_type operator++(int) {
			this_type Prev(Cur, End);
			if (Cur != End) {
				++Cur;
			}else {
				++Overflow;
			}
			return Prev;
		}
		bool overflow()const { return Overflow>0; }
		unsigned int overflow_count()const { return Overflow; }
		bool empty()const { return Cur == End; }
		difference_type remain()const { return std::distance(Cur, End); }
		iterator_type base()const { return Cur; }
	};
	template<typename output_iterator>
	overflow_iterator<output_iterator> make_overflow_iterator(output_iterator Beg, output_iterator End) {
		return overflow_iterator<output_iterator>(Beg, End);
	}
}
#
#endif
