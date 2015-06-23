#ifndef HMLIB_ENUMERATORS_CONTAINERENUMERATOR_INC
#define HMLIB_ENUMERATORS_CONTAINERENUMERATOR_INC 100
#
#include<type_traits>
#include"enumerator_interface.hpp"
namespace hmLib {
	template<typename T, typename iterator>
	class container_enumerator :public enumerator_interface <T> {
	private:
		iterator Begin;
		iterator End;
		iterator Itr;
	public:
		container_enumerator(iterator Begin_, iterator End_)
			: Begin(Begin_)
			, End(End_)
			, Itr(Begin_) {
		}
	public:
		//return true if the object can be accessed to.
		virtual operator bool()const { return Itr!=End; }
		//access to object.
		virtual T& operator*() { return *Itr; }
		//move to next object.
		virtual void operator++() { ++Itr; }
		//restart from the initial object.
		virtual void reset() { Itr=Begin; }
	};
	template<typename container>
	auto make_container_enumerator(container& Container_) -> typename container_enumerator<typename std::remove_reference<decltype(*std::begin(Container_))>::type, decltype(std::begin(Container_))> {
		return container_enumerator<std::remove_reference<decltype(*std::begin(Container_))>::type, decltype(std::begin(Container_))>(std::begin(Container_), std::end(Container_));
	}
	template<typename container>
	auto make_container_const_enumerator(container& Container_) -> typename container_enumerator<const typename std::remove_reference<decltype(*std::begin(Container_))>::type, decltype(std::begin(Container_))> {
		return container_enumerator<const std::remove_reference<decltype(*std::begin(Container_))>::type, decltype(std::begin(Container_))>(std::begin(Container_), std::end(Container_));
	}
}
#
#endif
