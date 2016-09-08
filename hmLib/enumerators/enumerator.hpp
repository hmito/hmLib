#ifndef HMLIB_ENUMERATORS_ENUMERATOR_INC
#define HMLIB_ENUMERATORS_ENUMERATOR_INC 100
#
#include"enumerator_traits.hpp"
#include"enumbase.hpp"
namespace hmLib{
	//ability pack
	namespace enumerators{
		template<typename enumerator_traits, typename... others>
		struct enumerator_pack: public enumbase_interface_from_tag<enumerator_traits>::type{};
		template<typename enumerator_traits, typename ability, typename... others>
		struct enumerator_pack<enumerator_traits, ability, others...>
			: public ability::template ability_interface<enumerator_traits>
			, public enumerator_pack<enumerator_traits, others...>{
		};
		template<typename enumbase, typename base, typename... others>
		struct enumerator_impl_pack : public base{};
		template<typename enumbase, typename base, typename ability, typename... others>
		struct enumerator_impl_pack<enumbase, base, ability, others...>
			: public enumerator_impl_pack<enumbase, typename ability::template ability_impl<enumbase, base>, others...>{
		};
	}
	template<typename enumerator_traits, typename enumerator_tag_, typename... abilities>
	struct basic_enumerator :public enumerators::enumerator_pack<enumerator_traits, abilities...>{
	private:
		using this_type = basic_enumerator<enumerator_traits, enumerator_tag_, abilities...>;
	public:
		using enumerator_tag = typename enumerator_traits::enumerator_tag;
		using value_type = typename enumerator_traits::value_type;
		using difference_type = typename enumerator_traits::difference_type;
		using reference = typename enumerator_traits::reference;
		using pointer = typename enumerator_traits::pointer;
	public:
		template<typename iterator_>
		using enumbase = typename enumerators::enumbase_from_tag<enumerator_traits, iterator_, this_type>::type;
	public:
		template<typename iterator_>
		struct impl : public enumerators::enumerator_impl_pack<enumbase<iterator_>, enumbase<iterator_>, abilities...>{};
	public:
		template<typename iterator_>
		static impl<iterator_> make_enumerator(iterator_ Beg_, iterator_ End_){
			impl<iterator_> Impl;
			Impl.initialize(Beg_, End_);
			return Impl;
		}
	};
	template<typename enumerator_traits, typename... abilities>
	struct basic_enumerator<enumerator_traits, referable_enumerator_tag, abilities...> :public enumerators::enumerator_pack<enumerator_traits, abilities...>{
	private:
		using this_type = basic_enumerator<enumerator_traits, referable_enumerator_tag, abilities...>;
	public:
		using enumerator_tag = typename enumerator_traits::enumerator_tag;
		using value_type = typename enumerator_traits::value_type;
		using difference_type = typename enumerator_traits::difference_type;
		using reference = typename enumerator_traits::reference;
		using pointer = typename enumerator_traits::pointer;
	public:
		template<typename iterator_>
		using enumbase = typename enumerators::enumbase_from_tag<enumerator_traits, iterator_, this_type>::type;
	public:
		template<typename iterator_>
		struct impl : public enumerators::enumerator_impl_pack<enumbase<iterator_>, enumbase<iterator_>, abilities...>{};
	public:
		template<typename iterator_>
		static impl<iterator_> make_enumerator(iterator_ Cur_){
			impl<iterator_> Impl;
			Impl.initialize(Cur_);
			return Impl;
		}
	};
	template<typename enumerator_traits, typename... abilities>
	using enumerator = basic_enumerator<enumerator_traits, typename enumerator_traits::enumerator_tag, abilities...>;
}
#
#endif
