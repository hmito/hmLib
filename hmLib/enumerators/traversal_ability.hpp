#ifndef HMLIB_TRAVERSALTRAITS_INC
#define HMLIB_TRAVERSALTRAITS_INC 100
#
#include<type_traits>
#include<hmLib/type_traits.hpp>
#include"enumbase.hpp"
namespace hmLib{
	namespace enumerators{
		struct forward_traversal_ability{
			template<typename enumerator_traits, bool is_const_ = std::is_const<typename enumerator_traits::value_type>::value>
			struct ability_interface{
			private:
				using reference = typename enumerator_traits::reference;
				using const_reference = const reference;
			public:
				virtual reference operator*(void) = 0;
				virtual const_reference operator*(void) const = 0;
				virtual void operator++() = 0;
			public:
				void operator++(int){ operator++(); }
			};
			template<typename enumerator_traits, typename base, bool is_const_ = std::is_const<typename enumerator_traits::value_type>::value>
			struct ability_impl : public base{
			private:
				using reference = typename enumerator_traits::reference;
				using const_reference = const reference;
			public:
				reference operator*(void) override{ return *(base::Cur); }
				const_reference operator*(void) const override{ return *(base::Cur); }
				void operator++() override{ ++(base::Cur); }
			};
			template<typename enumerator_traits>
			struct ability_interface<enumerator_traits, true>{
			private:
				using const_reference = typename enumerator_traits::reference;
			public:
				virtual const_reference operator*(void) const = 0;
				virtual void operator++() = 0;
			public:
				void operator++(int){ operator++(); }
			};
			template<typename enumerator_traits, typename base>
			struct ability_impl<enumerator_traits, base, true> : public base{
			private:
				using const_reference = typename enumerator_traits::reference;
			public:
				const_reference operator*(void) const override{ return *(base::Cur); }
				void operator++() override{ ++(base::Cur); }
			};
		};
		struct bidirectional_traversal_ability{
			template<typename enumerator_traits, bool is_const_ = std::is_const<typename enumerator_traits::value_type>::value>
			struct ability_interface{
			private:
				using reference = typename enumerator_traits::reference;
				using const_reference = const reference;
			public:
				virtual reference operator*(void) = 0;
				virtual const_reference operator*(void) const = 0;
				virtual void operator++() = 0;
				virtual void operator--() = 0;
			public:
				void operator++(int){ operator++(); }
				void operator--(int){ operator--(); }
			};
			template<typename enumerator_traits, typename base, bool is_const_ = std::is_const<typename enumerator_traits::value_type>::value>
			struct ability_impl : public base{
			private:
				using reference = typename enumerator_traits::reference;
				using const_reference = const reference;
			public:
				reference operator*(void) override{ return *(base::Cur); }
				const_reference operator*(void) const override{ return *(base::Cur); }
				void operator++() override{ ++(base::Cur); }
				void operator--() override{ --(base::Cur); }
			};
			template<typename enumerator_traits>
			struct ability_interface<enumerator_traits, true>{
			private:
				using const_reference = typename enumerator_traits::reference;
			public:
				virtual const_reference operator*(void) const = 0;
				virtual void operator++() = 0;
				virtual void operator--() = 0;
			public:
				void operator++(int){ operator++(); }
				void operator--(int){ operator--(); }
			};
			template<typename enumerator_traits, typename base>
			struct ability_impl<enumerator_traits, base, true> : public base{
			private:
				using const_reference = typename enumerator_traits::reference;
			public:
				const_reference operator*(void) const override{ return *(base::Cur); }
				void operator++() override{ ++(base::Cur); }
				void operator--() override{ --(base::Cur); }
			};
		};
		struct random_access_traversal_ability{
			template<typename enumerator_traits, bool is_const_ = std::is_const<typename enumerator_traits::value_type>::value>
			struct ability_interface{
			private:
				using reference = typename enumerator_traits::reference;
				using const_reference = const reference;
				using difference_type = typename enumerator_traits::difference_type;
			public:
				virtual reference operator*(void) = 0;
				virtual const_reference operator*(void) const = 0;
				virtual reference operator[](difference_type Pos) = 0;
				virtual const_reference operator[](difference_type Pos)const = 0;
				virtual void operator++() = 0;
				virtual void operator--() = 0;
				virtual void operator+=(difference_type Pos) = 0;
			public:
				void operator++(int){ operator++(); }
				void operator--(int){ operator--(); }
				void operator-=(difference_type Pos){ operator+=(-Pos); }
			};
			template<typename enumerator_traits, typename base, bool is_const_ = std::is_const<typename enumerator_traits::value_type>::value>
			struct ability_impl : public base{
			private:
				using reference = typename enumerator_traits::reference;
				using const_reference = const reference;
				using difference_type = typename enumerator_traits::difference_type;
			public:
				reference operator*(void) override{ return *(base::Cur); }
				const_reference operator*(void) const override{ return *(base::Cur); }
				reference operator[](difference_type Pos) override{ return base::Cur[Pos];}
				const_reference operator[](difference_type Pos) const override{ return base::Cur[Pos]; }
				void operator++() override{ ++(base::Cur); }
				void operator--() override{ --(base::Cur); }
				void operator+=(difference_type Pos)override{ base::Cur += Pos; }
			};
			template<typename enumerator_traits>
			struct ability_interface<enumerator_traits, true>{
			private:
				using const_reference = typename enumerator_traits::reference;
				using difference_type = typename enumerator_traits::difference_type;
			public:
				virtual const_reference operator*(void) const = 0;
				virtual const_reference operator[](difference_type Pos)const = 0;
				virtual void operator++() = 0;
				virtual void operator--() = 0;
				virtual void operator+=(difference_type Pos) = 0;
			public:
				void operator++(int){ operator++(); }
				void operator--(int){ operator--(); }
				void operator-=(difference_type Pos){ operator+=(-Pos); }
			};
			template<typename enumerator_traits, typename base>
			struct ability_impl<enumerator_traits, base, true> : public base{
			private:
				using const_reference = typename enumerator_traits::reference;
				using difference_type = typename enumerator_traits::difference_type;
			public:
				const_reference operator*(void) const override{ return *(base::Cur); }
				const_reference operator[](difference_type Pos) const override{ return base::Cur[Pos]; }
				void operator++() override{ ++(base::Cur); }
				void operator--() override{ --(base::Cur); }
				void operator+=(difference_type Pos)override{ base::Cur += Pos; }
			};
		};
	}
}
#
#endif
