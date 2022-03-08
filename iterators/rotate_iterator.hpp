#ifndef HMLIB_ITERATORS_ROTATEITERATOR_INC
#define HMLIB_ITERATORA_ROTATEITERATOR_INC 100
#
#include<memory>
#include<iterator>
namespace hmLib{
	template<typename container>
	struct back_rotate_iterator{
		using container_type = container;
		using value_type = void;
		using pointer = void;
		using reference = void;
		using difference_type = void;
		using iterator_category = std::output_iterator_tag;
	public:
		back_rotate_iterator() = delete;
		back_rotate_iterator(container& x):ptr(std::addressof(x)){}
		back_rotate_iterator& operator*(){return *this;}
		back_rotate_iterator& operator=(const typename container::value_type& value){
			ptr->rotate_back(value);
			return *this;
		}
		back_rotate_iterator& operator=(typename container::value_type&& value){
			ptr->rotate_back(std::move(value));
			return *this;
		}
		insert_iterator& operator++(){return *this;}
		insert_iterator& operator++(int){return *this;}
	private:
		container* ptr;
	};
	template<typename container>
	struct forward_rotate_iterator{
		using container_type = container;
		using value_type = void;
		using pointer = void;
		using reference = void;
		using difference_type = void;
		using iterator_category = std::output_iterator_tag;
	public:
		forward_rotate_iterator() = delete;
		forward_rotate_iterator(container& x):ptr(std::addressof(x)){}
		forward_rotate_iterator& operator*(){return *this;}
		forward_rotate_iterator& operator=(const typename container::value_type& value){
			ptr->rotate_forward(value);
			return *this;
		}
		forward_rotate_iterator& operator=(typename container::value_type&& value){
			ptr->rotate_forward(std::move(value));
			return *this;
		}
		forward_rotate_iterator& operator++(){return *this;}
		forward_rotate_iterator& operator++(int){return *this;}
	private:
		container* ptr;
	};
	template<typename container>
	back_rotate_iterator<container> back_rotator(container& x){return back_rotate_iterator<container>(x);}
	template<typename container>
	forward_rotate_iterator<container> forward_rotator(container& x){return forward_rotate_iterator<container>(x);}
}
#
#endif
