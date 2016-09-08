#ifndef HMLIB_TRAVERSALTRAITS_INC
#define HMLIB_TRAVERSALTRAITS_INC 100
#
#include<type_traits>
#include<hmLib/type_traits.hpp>
#include"enumbase.hpp"
namespace hmLib{
	namespace enumerators{
		struct forward_traversal_abilities{
			template<typename enumerator_traits, bool is_const_ = std::is_const<typename enumerator_traits::value_type>::value>
			struct ability_interface{
			private:
				using reference = typename enumerator_traits::reference;
				using const_reference = typename std::add_const<reference>::type;
			public:
				virtual reference operator*(void) = 0;
				virtual const_reference operator*(void) const = 0;
				virtual void operator++() = 0;
			public:
				void operator++(int){ operator++(); }
			};
			template<typename enumbase, typename base>
			struct ability_impl : public base{
			private:
				using reference = typename enumerator_traits::reference;
				using const_reference = typename std::add_const<reference>::type;
			public:
				reference operator*(void) override{ return *(enumbase::Cur); }
				const_reference operator*(void) const override{ return *(enumbase::Cur); }
				void operator++() override{ ++(enumbase::Cur); }
			};
		};

	}
}
#
#endif
