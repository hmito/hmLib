#ifndef HMLIB_ENUMERATORS_ENUMERATOR_INC
#define HMLIB_ENUMERATORS_ENUMERATOR_INC 100
#
#include"enumerator_traits.hpp"
#include"enumbase.hpp"
namespace hmLib{
	//ability pack
	namespace enumerators{
		template<typename... abilities>
		struct ability_pack{
		private:
			template<typename enumerator_base, typename... others>
			struct enumeratable_interface_pack{};
			template<typename enumerator_base, typename ability, typename... others>
			struct enumeratable_interface_pack<enumerator_base, ability, others...>
				: public ability::template ability_interface<enumerator_base>
				, public enumeratable_interface_pack<enumerator_base, others...>{
			};

			template<typename enumerator_base, typename enumbase, typename base, typename... others>
			struct enumeratable_pack : public base{};
			template<typename enumerator_base, typename enumbase, typename base, typename ability, typename... others>
			struct enumeratable_pack<enumerator_base, enumbase, base, ability, others...>
				: public enumeratable_pack<enumerator_base, enumbase, typename ability::template ability_enumeratable<enumerator_base, enumbase, base>, others...>{};

			template<typename enumerator_base, typename base, typename... others>
			struct enumerator_pack : public base{};
			template<typename enumerator_base, typename base, typename ability, typename... others>
			struct enumerator_pack<enumerator_base, base, ability, others...>
				: public enumerator_pack<enumerator_base, typename ability::template enumerator_impl<enumerator_base, base>, others...>{};
		public:
			template<typename enumerator_base>
			using enumeratable_interface = enumeratable_interface_pack<enumerator_base, abilities...>;
			template<typename enumerator_base, typename enumbase>
			using enumeratable = enumeratable_pack<enumerator_base, enumbase, enumbase, abilities...>;
			template<typename enumerator_base>
			using enumerator = enumerator_pack<enumerator_base, enumerator_base, abilities...>;
		};

		template<typename enumerator_traits_, typename... abilities>
		struct enumerator_base{
		private:
			using this_type = enumerator_base<enumerator_traits_, abilities...>;
		public:
			using enumerator_traits = enumerator_traits_;
			using enumerator_tag = typename enumerator_traits::enumerator_tag;
			using value_type = typename enumerator_traits::value_type;
			using difference_type = typename enumerator_traits::difference_type;
			using reference = typename enumerator_traits::reference;
			using pointer = typename enumerator_traits::pointer;
		private:
			using this_ability_pack = enumerators::ability_pack<abilities...>;
		public:
			using enumeratable_interface = typename this_ability_pack::template enumeratable_interface<this_type>;
		protected:
			template<typename iterator_>
			using enumbase = typename enumerators::enumbase_from_tag<enumerator_tag, iterator_, enumeratable_interface>::type;
		public:
			template<typename iterator_>
			using enumeratable = typename this_ability_pack::template enumeratable<this_type, enumbase<iterator_> >;
		private:
			enumeratable_interface* Ptr;
		protected:
			void set_enumeratable(enumeratable_interface& Ref){ Ptr = &Ref; }
			enumeratable_interface& ref(){ return *Ptr; }
			const enumeratable_interface& ref()const{ return *Ptr; }
			const enumeratable_interface& cref()const{ return *Ptr; }
		};

		template<typename enumerator_traits_, typename... abilities>
		using enumerator_impl = typename ability_pack<abilities...>::template enumerator< enumerators::enumerator_base<enumerator_traits_, abilities...> >;

		template<typename enumerator_tag_, template<typename iterator_> typename  enumeratable_, template<typename iterator_> typename enumbase_>
		struct make_enumeratable_mixin{
			template<typename iterator_>
			inline static enumeratable_<iterator_> make_enumeratable(iterator_ Beg_, iterator_ End_){
				enumeratable_<iterator_> Enumeratable;
				static_cast<enumbase_<iterator_>&>(Enumeratable).set(Beg_, End_);
				return Enumeratable;
			}
		};
		template<template<typename iterator_> typename  enumeratable_, template<typename iterator_> typename enumbase_>
		struct make_enumeratable_mixin<referable_enumerator_tag, enumeratable_, enumbase_>{
			template<typename iterator_>
			inline static enumeratable_<iterator_> make_enumeratable(iterator_ Cur_){
				enumeratable_<iterator_> Enumeratable;
				static_cast<enumbase_<iterator_>&>(Enumeratable).set(Cur_);
				return Enumeratable;
			}
		};
	}
	template<typename enumerator_traits_, typename... abilities>
	struct enumerator
		: public enumerators::enumerator_impl<enumerator_traits_, abilities...>
		, public enumerators::make_enumeratable_mixin<
		typename enumerator_traits_::enumerator_tag
		, typename enumerators::enumerator_impl<enumerator_traits_, abilities...>::enumeratable
		, typename enumerators::enumerator_impl<enumerator_traits_, abilities...>::enumbase
		>{
	private:
		using base_type = enumerators::enumerator_impl<enumerator_traits_, abilities...>;
		using this_type = enumerator<enumerator_traits_, abilities...>;
	public:
		using enumerator_traits = typename base_type::enumerator_traits;
		using enumerator_tag = typename base_type::enumerator_tag;
		using value_type = typename base_type::value_type;
		using difference_type = typename base_type::difference_type;
		using reference = typename base_type::reference;
		using pointer = typename base_type::pointer;
		using enumeratable_interface = typename base_type::enumeratable_interface;
		template<typename iterator_>
		using enumeratable = typename base_type::template enumeratable<iterator_>;
	public:
		enumerator(const this_type&) = delete;
		this_type& operator=(const this_type&) = delete;
		enumerator(this_type&&) = delete;
		this_type& operator=(this_type&&) = delete;
		enumerator(enumeratable_interface& Enumeratable_){
			base_type::set_enumeratable(Enumeratable_);
		}
	};
}

#
#endif
