#ifndef HMLIB_ALGORITHMTRAITS_INC
#define HMLIB_ALGORITHMTRAITS_INC 100
#
#include<algorithm>
#include<functional>
#include<type_traits>
#include<hmLib/type_traits.hpp>
#include"enumbase.hpp"
namespace hmLib{
	namespace enumerators{
		struct all_of_ability{
			template<typename enumerator_traits, typename enumerator_category = typename near_base_of<typename enumerator_traits::enumerator_tag, sentinel_enumerator_tag, range_enumerator_tag>::type >
			struct ability_interface{
				static_assert(!std::is_void<enumerator_category>::value, "all_of_ability cannot be used in this enumerator_category.");
			};
			template<typename enumerator_traits, typename base, typename enumerator_category = typename near_base_of<typename enumerator_traits::enumerator_tag, sentinel_enumerator_tag, range_enumerator_tag>::type >
			struct ability_impl : public base{};
			template<typename enumerator_traits>
			struct ability_interface<enumerator_traits, sentinel_enumerator_tag>{
			private:
				using value_type = typename enumerator_traits::value_type;
				using pred_type = std::function<bool(const value_type&) >;
			public:
				virtual bool all_of(pred_type Pred) = 0;
				virtual bool any_of(pred_type Pred) = 0;
				virtual bool none_of(pred_type Pred) = 0;
			};
			template<typename enumerator_traits, typename base>
			struct ability_impl<enumerator_traits, base, sentinel_enumerator_tag> : public base{
			private:
				using value_type = typename enumerator_traits::value_type;
				using pred_type = std::function<bool(const value_type&) >;
			public:
				bool all_of(pred_type Pred)override{ return std::all_of(base::Cur, base::End, std::move(Pred)); }
				bool any_of(pred_type Pred)override{ return std::any_of(base::Cur, base::End, std::move(Pred)); }
				bool none_of(pred_type Pred)override{ return std::none_of(base::Cur, base::End, std::move(Pred)); }
			};
			template<typename enumerator_traits>
			struct ability_interface<enumerator_traits, range_enumerator_tag>{
			private:
				using value_type = typename enumerator_traits::value_type;
				using pred_type = std::function<bool(const value_type&) >;
			public:
				virtual bool all_of(pred_type Pred) = 0;
				virtual bool all_sofar_of(pred_type Pred) = 0;
				virtual bool all_range_of(pred_type Pred) = 0;
				virtual bool any_of(pred_type Pred) = 0;
				virtual bool any_sofar_of(pred_type Pred) = 0;
				virtual bool any_range_of(pred_type Pred) = 0;
				virtual bool none_of(pred_type Pred) = 0;
				virtual bool none_sofar_of(pred_type Pred) = 0;
				virtual bool none_range_of(pred_type Pred) = 0;
			};
			template<typename enumerator_traits, typename base>
			struct ability_impl<enumerator_traits, base, range_enumerator_tag> : public base{
			private:
				using value_type = typename enumerator_traits::value_type;
				using pred_type = std::function<bool(const value_type&) >;
			public:
				bool all_of(pred_type Pred)override{ return std::all_of(base::Cur, base::End, std::move(Pred)); }
				bool all_sofar_of(pred_type Pred)override{ return std::all_of(base::Beg, base::Cur, std::move(Pred)); }
				bool all_range_of(pred_type Pred)override{ return std::all_of(base::Beg, base::End, std::move(Pred)); }
				bool any_of(pred_type Pred)override{ return std::any_of(base::Cur, base::End, std::move(Pred)); }
				bool any_sofar_of(pred_type Pred)override{ return std::any_of(base::Beg, base::Cur, std::move(Pred)); }
				bool any_range_of(pred_type Pred)override{ return std::any_of(base::Beg, base::End, std::move(Pred)); }
				bool none_of(pred_type Pred)override{ return std::none_of(base::Cur, base::End, std::move(Pred)); }
				bool none_sofar_of(pred_type Pred)override{ return std::none_of(base::Beg, base::Cur, std::move(Pred)); }
				bool none_range_of(pred_type Pred)override{ return std::none_of(base::Beg, base::End, std::move(Pred)); }
			};
		};
		struct find_ability{
			template<typename enumerator_traits,typename enumerator_category = typename near_base_of<typename enumerator_traits::enumerator_tag, sentinel_enumerator_tag, range_enumerator_tag>::type >
			struct ability_interface{
				static_assert(!std::is_void<enumerator_category>::value, "find_ability cannot be used in this enumerator_category.");
			};
			template<typename enumerator_traits, typename base, typename enumerator_category = typename near_base_of<typename enumerator_traits::enumerator_tag, sentinel_enumerator_tag, range_enumerator_tag>::type >
			struct ability_impl : public base{};
			template<typename enumerator_traits>
			struct ability_interface<enumerator_traits, sentinel_enumerator_tag>{
			private:
				using value_type = typename enumerator_traits::value_type;
				using pred_type = std::function<bool(const value_type&) >;
			public:
				virtual void find(const value_type& Val) = 0;
				virtual void find_if(pred_type Pred) = 0;
			};
			template<typename enumerator_traits, typename base>
			struct ability_impl<enumerator_traits,base, sentinel_enumerator_tag> : public base{
			private:
				using value_type = typename enumerator_traits::value_type;
				using pred_type = std::function<bool(const value_type&) >;
			public:
				void find(const value_type& Val)override{ base::Cur = std::find(base::Cur, base::End, Val); }
				void find_if(pred_type Pred)override{ base::Cur = std::find_if(base::Cur, base::End, std::move(Pred)); }
			};
			template<typename enumerator_traits>
			struct ability_interface<enumerator_traits, range_enumerator_tag>{
			private:
				using value_type = typename enumerator_traits::value_type;
				using pred_type = std::function<bool(const value_type&) >;
			public:
				virtual void find(const value_type& Val) = 0;
				virtual void find_sofar(const value_type& Val) = 0;
				virtual void find_range(const value_type& Val) = 0;
				virtual void find_if(pred_type Pred) = 0;
				virtual void find_sofar_if(pred_type Pred) = 0;
				virtual void find_range_if(pred_type Pred) = 0;
			};
			template<typename enumerator_traits, typename base>
			struct ability_impl<enumerator_traits, base, range_enumerator_tag> : public base{
			private:
				using value_type = typename enumerator_traits::value_type;
				using pred_type = std::function<bool(const value_type&) >;
			public:
				void find(const value_type& Val)override{ base::Cur = std::find(base::Cur, base::End, Val); }
				void find_sofar(const value_type& Val)override{ base::Cur = std::find(base::Beg, base::Cur, Val); }
				void find_range(const value_type& Val)override{ base::Cur = std::find(base::Beg, base::End, Val); }
				void find_if(pred_type Pred)override{ base::Cur = std::find_if(base::Cur, base::End, std::move(Pred)); }
				void find_sofar_if(pred_type Pred)override{ base::Cur = std::find_if(base::Beg, base::Cur, std::move(Pred)); }
				void find_range_if(pred_type Pred)override{ base::Cur = std::find_if(base::Beg, base::End, std::move(Pred)); }
			};
		};
		struct count_ability{
			template<typename enumerator_traits, typename enumerator_category = typename near_base_of<typename enumerator_traits::enumerator_tag, sentinel_enumerator_tag, range_enumerator_tag>::type >
			struct ability_interface{
				static_assert(!std::is_void<enumerator_category>::value, "count_ability cannot be used in this enumerator_category.");
			};
			template<typename enumerator_traits, typename base, typename enumerator_category = typename near_base_of<typename enumerator_traits::enumerator_tag, sentinel_enumerator_tag, range_enumerator_tag>::type >
			struct ability_impl : public base{};
			template<typename enumerator_traits>
			struct ability_interface<enumerator_traits, sentinel_enumerator_tag>{
			private:
				using value_type = typename enumerator_traits::value_type;
				using difference_type = typename enumerator_traits::difference_type;
				using pred_type = std::function<bool(const value_type&) >;
			public:
				virtual difference_type count(const value_type& Val) = 0;
				virtual difference_type count_if(pred_type Pred) = 0;
			};
			template<typename enumerator_traits, typename base>
			struct ability_impl<enumerator_traits, base, sentinel_enumerator_tag> : public base{
			private:
				using value_type = typename enumerator_traits::value_type;
				using difference_type = typename enumerator_traits::difference_type;
				using pred_type = std::function<bool(const value_type&) >;
			public:
				difference_type count(const value_type& Val)override{ return std::count(base::Cur, base::End, Val); }
				difference_type count_if(pred_type Pred)override{ return  std::count_if(base::Cur, base::End, Pred); }
			};
			template<typename enumerator_traits>
			struct ability_interface<enumerator_traits, range_enumerator_tag>{
			private:
				using value_type = typename enumerator_traits::value_type;
				using difference_type = typename enumerator_traits::difference_type;
				using pred_type = std::function<bool(const value_type&) >;
			public:
				virtual difference_type count(const value_type& Val) = 0;
				virtual difference_type count_sofar(const value_type& Val) = 0;
				virtual difference_type count_range(const value_type& Val) = 0;
				virtual difference_type count_if(pred_type Pred) = 0;
				virtual difference_type count_sofar_if(pred_type Pred) = 0;
				virtual difference_type count_range_if(pred_type Pred) = 0;
			};
			template<typename enumerator_traits, typename base>
			struct ability_impl<enumerator_traits, base, range_enumerator_tag> : public base{
			private:
				using value_type = typename enumerator_traits::value_type;
				using difference_type = typename enumerator_traits::difference_type;
				using pred_type = std::function<bool(const value_type&) >;
			public:
				difference_type count(const value_type& Val)override{ return std::count(base::Cur, base::End, Val); }
				difference_type count_sofar(const value_type& Val)override{ return std::count(base::Beg, base::Cur, Val); }
				difference_type count_range(const value_type& Val)override{ return std::count(base::Beg, base::End, Val); }
				difference_type count_if(pred_type Pred)override{ return std::count_if(base::Cur, base::End, Pred); }
				difference_type count_sofar_if(pred_type Pred)override{ return std::count_if(base::Beg, base::Cur, Pred); }
				difference_type count_range_if(pred_type Pred)override{ return  std::count_if(base::Beg, base::End, Pred); }
			};
		};
		struct for_each_ability{
			template<typename enumerator_traits, typename enumerator_category = typename near_base_of<typename enumerator_traits::enumerator_tag, sentinel_enumerator_tag, range_enumerator_tag>::type >
			struct ability_interface{
				static_assert(!std::is_void<enumerator_category>::value, "for_each_ability cannot be used in this enumerator_category.");
			};
			template<typename enumerator_traits, typename base, typename enumerator_category = typename near_base_of<typename enumerator_traits::enumerator_tag, sentinel_enumerator_tag, range_enumerator_tag>::type >
			struct ability_impl : public base{};
			template<typename enumerator_traits>
			struct ability_interface<enumerator_traits, sentinel_enumerator_tag>{
			private:
				using value_type = typename enumerator_traits::value_type;
				using operation_type = std::function<void(const value_type&) >;
			public:
				virtual void for_each(operation_type Operator) = 0;
			};
			template<typename enumerator_traits, typename base>
			struct ability_impl<enumerator_traits, base, sentinel_enumerator_tag> : public base{
			private:
				using value_type = typename enumerator_traits::value_type;
				using operation_type = std::function<void(const value_type&) >;
			public:
				void for_each(operation_type Operator)override{  std::for_each(base::Cur, base::End, std::move(Operator)); }
			};
			template<typename enumerator_traits>
			struct ability_interface<enumerator_traits, range_enumerator_tag>{
			private:
				using value_type = typename enumerator_traits::value_type;
				using operation_type = std::function<void(const value_type&) >;
			public:
				virtual void for_each(operation_type Operator) = 0;
				virtual void for_each_sofar(operation_type Operator) = 0;
				virtual void for_each_range(operation_type Operator) = 0;
			};
			template<typename enumerator_traits, typename base>
			struct ability_impl<enumerator_traits, base, range_enumerator_tag> : public base{
			private:
				using value_type = typename enumerator_traits::value_type;
				using operation_type = std::function<void(const value_type&) >;
			public:
				void for_each(operation_type Operator)override{ std::for_each(base::Cur, base::End, std::move(Operator)); }
				void for_each_sofar(operation_type Operator)override{ std::for_each(base::Beg, base::Cur, std::move(Operator)); }
				void for_each_range(operation_type Operator)override{  std::for_each(base::Beg, base::End, std::move(Operator)); }
			};
		};
		struct sort_ability{
			template<typename enumerator_traits, typename enumerator_category = typename near_base_of<typename enumerator_traits::enumerator_tag, range_enumerator_tag>::type >
			struct ability_interface{
				static_assert(!std::is_void<enumerator_category>::value, "sort ability cannot be used in this enumerator_category.");
			};
			template<typename enumerator_traits, typename base, typename enumerator_category = typename near_base_of<typename enumerator_traits::enumerator_tag, range_enumerator_tag>::type >
			struct ability_impl : public base{};
			template<typename enumerator_traits>
			struct ability_interface<enumerator_traits, sentinel_enumerator_tag>{
			private:
				using value_type = typename enumerator_traits::value_type;
				using compare_type = std::function<bool(const value_type&, const value_type&) >;
			public:
				virtual void sort() = 0;
				virtual void sort(compare_type Compare) = 0;
			};
			template<typename enumerator_traits, typename base>
			struct ability_impl<enumerator_traits, base, sentinel_enumerator_tag> : public base{
			private:
				using value_type = typename enumerator_traits::value_type;
				using compare_type = std::function<bool(const value_type&, const value_type&) >;
			public:
				void sort()override{ std::sort(base::Cur, base::End); }
				void sort(compare_type Compare) override{ std::sort(base::Cur, base::End, std::move(Compare)); }
			};
			template<typename enumerator_traits>
			struct ability_interface<enumerator_traits, range_enumerator_tag>{
			private:
				using value_type = typename enumerator_traits::value_type;
				using compare_type = std::function<bool(const value_type&, const value_type&) >;
			public:
				virtual void sort_sofar() = 0;
				virtual void sort_range() = 0;
				virtual void sort() = 0;
				virtual void sort_sofar(compare_type Compare) = 0;
				virtual void sort_range(compare_type Compare) = 0;
				virtual void sort(compare_type Compare) = 0;
			};
			template<typename enumerator_traits, typename base>
			struct ability_impl<enumerator_traits, base, range_enumerator_tag> : public base{
			private:
				using value_type = typename enumerator_traits::value_type;
				using compare_type = std::function<bool(const value_type&, const value_type&) >;
			public:
				void sort_sofar()override{ std::sort(base::Beg, base::Cur); }
				void sort_range()override{ std::sort(base::Beg, base::End); }
				void sort()override{ std::sort(base::Cur, base::End); }
				void sort_sofar(compare_type Compare)override{ std::sort(base::Beg, base::Cur, std::move(Compare)); }
				void sort_range(compare_type Compare)override{ std::sort(base::Beg, base::End, std::move(Compare)); }
				void sort(compare_type Compare) override{ std::sort(base::Cur, base::End, std::move(Compare)); }
			};
		};
		struct equal_range_ability{
			template<typename enumerator_traits, typename enumerator_category = typename near_base_of<typename enumerator_traits::enumerator_tag, range_enumerator_tag>::type >
			struct ability_interface{
				static_assert(!std::is_void<enumerator_category>::value, "equal_range_ability cannot be used in this enumerator_category.");
			};
			template<typename enumerator_traits, typename base, typename enumerator_category = typename near_base_of<typename enumerator_traits::enumerator_tag, range_enumerator_tag>::type >
			struct ability_impl : public base{};
			template<typename enumerator_traits>
			struct ability_interface<enumerator_traits, sentinel_enumerator_tag>{
			private:
				using value_type = typename enumerator_traits::value_type;
				using compare_type = std::function<bool(const value_type&, const value_type&) >;
			public:
				virtual void lower_bound() = 0;
				virtual void lower_bound(compare_type Compare) = 0;
				virtual void upper_bound() = 0;
				virtual void upper_bound(compare_type Compare) = 0;
			};
			template<typename enumerator_traits, typename base>
			struct ability_impl<enumerator_traits, base, sentinel_enumerator_tag> : public base{
			private:
				using value_type = typename enumerator_traits::value_type;
				using compare_type = std::function<bool(const value_type&, const value_type&) >;
			public:
				void lower_bound()override{ base::Cur = std::lower_bound(base::Cur, base::End); }
				void lower_bound(compare_type Compare)override{ base::Cur = std::lower_bound(base::Cur, base::End, std::move(Compare)); }
				void upper_bound()override{ base::Cur = std::upper_bound(base::Cur, base::End); }
				void upper_bound(compare_type Compare)override{ base::Cur = std::upper_bound(base::Cur, base::End, std::move(Compare)); }
			};
			template<typename enumerator_traits>
			struct ability_interface<enumerator_traits, range_enumerator_tag>{
			private:
				using value_type = typename enumerator_traits::value_type;
				using compare_type = std::function<bool(const value_type&, const value_type&) >;
			public:
				virtual void lower_bound() = 0;
				virtual void lower_bound_sofar() = 0;
				virtual void lower_bound_range() = 0;
				virtual void lower_bound(compare_type Compare) = 0;
				virtual void lower_bound_sofar(compare_type Compare) = 0;
				virtual void lower_bound_range(compare_type Compare) = 0;
				virtual void upper_bound() = 0;
				virtual void upper_bound_sofar() = 0;
				virtual void upper_bound_range() = 0;
				virtual void upper_bound(compare_type Compare) = 0;
				virtual void upper_bound_sofar(compare_type Compare) = 0;
				virtual void upper_bound_range(compare_type Compare) = 0;
				virtual void equal_range() = 0;
				virtual void equal_range(compare_type Compare) = 0;
			};
			template<typename enumerator_traits, typename base>
			struct ability_impl<enumerator_traits, base, range_enumerator_tag> : public base{
			private:
				using value_type = typename enumerator_traits::value_type;
				using compare_type = std::function<bool(const value_type&, const value_type&) >;
			public:
				void lower_bound()override{ base::Cur = std::lower_bound(base::Cur, base::End); }
				void lower_bound_sofar()override{ base::Cur = std::lower_bound(base::Beg, base::Cur); }
				void lower_bound_range()override{ base::Cur = std::lower_bound(base::Beg, base::End); }
				void lower_bound(compare_type Compare)override{ base::Cur = std::lower_bound(base::Cur, base::End, std::move(Compare)); }
				void lower_bound_sofar(compare_type Compare)override{ base::Cur = std::lower_bound(base::Beg, base::Cur, std::move(Compare)); }
				void lower_bound_range(compare_type Compare) override{ base::Cur = std::lower_bound(base::Beg, base::End, std::move(Compare)); }
				void upper_bound()override{ base::Cur = std::upper_bound(base::Cur, base::End); }
				void upper_bound_sofar()override{ base::Cur = std::upper_bound(base::Beg, base::Cur); }
				void upper_bound_range()override{ base::Cur = std::upper_bound(base::Beg, base::End); }
				void upper_bound(compare_type Compare)override{ base::Cur = std::upper_bound(base::Cur, base::End, std::move(Compare)); }
				void upper_bound_sofar(compare_type Compare)override{ base::Cur = std::upper_bound(base::Beg, base::Cur, std::move(Compare));}
				void upper_bound_range(compare_type Compare) override{ base::Cur = std::upper_bound(base::Beg, base::End, std::move(Compare));}
				void equal_range()override{ auto ans = std::equal_range(base::Beg, base::End);  base::Beg = ans.first; base::End = ans.second; base::Cur = base::Beg;}
				void equal_range(compare_type Compare) override{ auto ans = std::equal_range(base::Beg, base::End, std::move(Compare));  base::Beg = ans.first; base::End = ans.second; base::Cur = base::Beg;}
			};
		};
		struct minmax_ability{
			template<typename enumerator_traits, typename enumerator_category = typename near_base_of<typename enumerator_traits::enumerator_tag, sentinel_enumerator_tag, range_enumerator_tag>::type >
			struct ability_interface{
				static_assert(!std::is_void<enumerator_category>::value, "minmax_ability cannot be used in this enumerator_category.");
			};
			template<typename enumerator_traits, typename base, typename enumerator_category = typename near_base_of<typename enumerator_traits::enumerator_tag, sentinel_enumerator_tag, range_enumerator_tag>::type >
			struct ability_impl : public base{};
			template<typename enumerator_traits>
			struct ability_interface<enumerator_traits, sentinel_enumerator_tag>{
			private:
				using value_type = typename enumerator_traits::value_type;
				using reference = typename enumerator_traits::reference;
				using compare_type = std::function<bool(const value_type&, const value_type&) >;
			public:
				virtual reference min() = 0;
				virtual reference min_if(compare_type Compare) = 0;
				virtual reference max() = 0;
				virtual reference max_if(compare_type Compare) = 0;
				virtual void min_element() = 0;
				virtual void min_element_if(compare_type Compare) = 0;
				virtual void max_element() = 0;
				virtual void max_element_if(compare_type Compare) = 0;
			};
			template<typename enumerator_traits, typename base>
			struct ability_impl<enumerator_traits, base, sentinel_enumerator_tag> : public base{
			private:
				using value_type = typename enumerator_traits::value_type;
				using reference = typename enumerator_traits::reference;
				using compare_type = std::function<bool(const value_type&, const value_type&) >;
			public:
				reference min()override{ return *std::min_element(base::Cur, base::End); }
				reference min_if(compare_type Compare)override{ return *std::min_element(base::Cur, base::End, std::move(Compare)); }
				reference max()override{ return *std::max_element(base::Cur, base::End); }
				reference max_if(compare_type Compare)override{ return *std::max_element(base::Cur, base::End, std::move(Compare)); }
				void min_element()override{ base::Cur = std::min_element(base::Cur, base::End); }
				void min_element_if(compare_type Compare)override{ base::Cur = std::min_element(base::Cur, base::End, std::move(Compare)); }
				void max_element()override{ base::Cur = std::max_element(base::Cur, base::End); }
				void max_element_if(compare_type Compare)override{ base::Cur = std::max_element(base::Cur, base::End, std::move(Compare)); }
			};
			template<typename enumerator_traits>
			struct ability_interface<enumerator_traits, range_enumerator_tag>{
			private:
				using value_type = typename enumerator_traits::value_type;
				using reference = typename enumerator_traits::reference;
				using compare_type = std::function<bool(const value_type&, const value_type&) >;
			public:
				virtual reference min_range() = 0;
				virtual reference min_range_if(compare_type Compare) = 0;
				virtual reference max_range() = 0;
				virtual reference max_range_if(compare_type Compare) = 0;
				virtual void min_element_range() = 0;
				virtual void min_element_range_if(compare_type Compare) = 0;
				virtual void max_element_range() = 0;
				virtual void max_element_range_if(compare_type Compare) = 0;
				virtual reference min_sofar() = 0;
				virtual reference min_sofar_if(compare_type Compare) = 0;
				virtual reference max_sofar() = 0;
				virtual reference max_sofar_if(compare_type Compare) = 0;
				virtual void min_element_sofar() = 0;
				virtual void min_element_sofar_if(compare_type Compare) = 0;
				virtual void max_element_sofar() = 0;
				virtual void max_element_sofar_if(compare_type Compare) = 0;
				virtual reference min() = 0;
				virtual reference min_if(compare_type Compare) = 0;
				virtual reference max() = 0;
				virtual reference max_if(compare_type Compare) = 0;
				virtual void min_element() = 0;
				virtual void min_element_if(compare_type Compare) = 0;
				virtual void max_element() = 0;
				virtual void max_element_if(compare_type Compare) = 0;
			};
			template<typename enumerator_traits, typename base>
			struct ability_impl<enumerator_traits, base, range_enumerator_tag> : public base{
			private:
				using value_type = typename enumerator_traits::value_type;
				using reference = typename enumerator_traits::reference;
				using compare_type = std::function<bool(const value_type&, const value_type&) >;
			public:
				reference min_range()override{ return *std::min_element(base::Beg, base::End); }
				reference min_range_if(compare_type Compare)override{ return*std::min_element(base::Beg, base::End, std::move(Compare)); }
				reference max_range()override{ return *std::max_element(base::Beg, base::End); }
				reference max_range_if(compare_type Compare)override{ return *std::max_element(base::Beg, base::End, std::move(Compare)); }
				void min_element_range()override{ base::Cur = std::min_element(base::Beg, base::End); }
				void min_element_range_if(compare_type Compare)override{ base::Cur = std::min_element(base::Beg, base::End, std::move(Compare)); }
				void max_element_range()override{ base::Cur = std::max_element(base::Beg, base::End); }
				void max_element_range_if(compare_type Compare)override{ base::Cur = std::max_element(base::Beg, base::End, std::move(Compare)); }
				reference min_sofar()override{ return *std::min_element(base::Beg, base::Cur); }
				reference min_sofar_if(compare_type Compare)override{ return *std::min_element(base::Beg, base::Cur, std::move(Compare)); }
				reference max_sofar()override{ return *std::max_element(base::Beg, base::Cur); }
				reference max_sofar_if(compare_type Compare)override{ return *std::max_element(base::Beg, base::Cur, std::move(Compare)); }
				void min_element_sofar()override{ base::Cur = std::min_element(base::Beg, base::Cur); }
				void min_element_sofar_if(compare_type Compare)override{ base::Cur = std::min_element(base::Beg, base::Cur, std::move(Compare)); }
				void max_element_sofar()override{ base::Cur = std::max_element(base::Beg, base::Cur); }
				void max_element_sofar_if(compare_type Compare)override{ base::Cur = std::max_element(base::Beg, base::Cur, std::move(Compare)); }
				reference min()override{ return *std::min_element(base::Cur, base::End); }
				reference min_if(compare_type Compare)override{ return *std::min_element(base::Cur, base::End, std::move(Compare)); }
				reference max()override{ return *std::max_element(base::Cur, base::End); }
				reference max_if(compare_type Compare)override{ return *std::max_element(base::Cur, base::End, std::move(Compare)); }
				void min_element()override{ base::Cur = std::min_element(base::Beg, base::End); }
				void min_element_if(compare_type Compare)override{ base::Cur = std::min_element(base::Cur, base::End, std::move(Compare)); }
				void max_element()override{ base::Cur = std::max_element(base::Beg, base::End); }
				void max_element_if(compare_type Compare)override{ base::Cur = std::max_element(base::Cur, base::End, std::move(Compare)); }
			};
		};
		struct remove_ability{
			template<typename enumerator_traits, typename enumerator_category = typename near_base_of<typename enumerator_traits::enumerator_tag, sentinel_enumerator_tag, range_enumerator_tag>::type >
			struct ability_interface{
				static_assert(!std::is_void<enumerator_category>::value, "remove_ability cannot be used in this enumerator_category.");
			};
			template<typename enumerator_traits, typename base, typename enumerator_category = typename near_base_of<typename enumerator_traits::enumerator_tag, sentinel_enumerator_tag, range_enumerator_tag>::type >
			struct ability_impl : public base{};
			template<typename enumerator_traits>
			struct ability_interface<enumerator_traits, sentinel_enumerator_tag>{
			private:
				using value_type = typename enumerator_traits::value_type;
				using pred_type = std::function<bool(const value_type&) >;
			public:
				virtual void remove(const value_type& Val) = 0;
				virtual void remove_if(pred_type Pred) = 0;
			};
			template<typename enumerator_traits, typename base>
			struct ability_impl<enumerator_traits, base, sentinel_enumerator_tag> : public base{
			private:
				using value_type = typename enumerator_traits::value_type;
				using pred_type = std::function<bool(const value_type&) >;
			public:
				void remove(const value_type& Val)override{ base::End = std::remove(base::Cur, base::End, Val); }
				void remove_if(pred_type Pred)override{ base::End = std::remove_if(base::Cur, base::End, std::move(Pred)); }
			};
			template<typename enumerator_traits>
			struct ability_interface<enumerator_traits, range_enumerator_tag>{
			private:
				using value_type = typename enumerator_traits::value_type;
				using pred_type = std::function<bool(const value_type&) >;
			public:
				virtual void remove_sofar(const value_type& Val) = 0;
				virtual void remove_range(const value_type& Val) = 0;
				virtual void remove(const value_type& Val) = 0;
				virtual void remove_sofar_if(pred_type Pred) = 0;
				virtual void remove_range_if(pred_type Pred) = 0;
				virtual void remove_if(pred_type Pred) = 0;
			};
			template<typename enumerator_traits, typename base>
			struct ability_impl<enumerator_traits, base, range_enumerator_tag> : public base{
			private:
				using value_type = typename enumerator_traits::value_type;
				using pred_type = std::function<bool(const value_type&) >;
			public:
				void remove_sofar(const value_type& Val)override{ base::End = std::remove(base::Beg, base::Cur,Val); }
				void remove_range(const value_type& Val)override{ base::End = std::remove(base::Beg, base::End, Val); }
				void remove(const value_type& Val)override{ base::End = std::remove(base::Cur, base::End, Val); }
				void remove_sofar_if(pred_type Pred)override{ base::End = std::remove_if(base::Beg, base::Cur, std::move(Pred)); }
				void remove_range_if(pred_type Pred)override{ base::End = std::remove_if(base::Beg, base::End, std::move(Pred)); }
				void remove_if(pred_type Pred) override{ base::End = std::remove_if(base::Cur, base::End, std::move(Pred)); }
			};
		};
		struct unique_ability{
			template<typename enumerator_traits, typename enumerator_category = typename near_base_of<typename enumerator_traits::enumerator_tag, sentinel_enumerator_tag, range_enumerator_tag>::type >
			struct ability_interface{
				static_assert(!std::is_void<enumerator_category>::value, "unique_ability cannot be used in this enumerator_category.");
			};
			template<typename enumerator_traits, typename base, typename enumerator_category = typename near_base_of<typename enumerator_traits::enumerator_tag, sentinel_enumerator_tag, range_enumerator_tag>::type >
			struct ability_impl : public base{};
			template<typename enumerator_traits>
			struct ability_interface<enumerator_traits, sentinel_enumerator_tag>{
			private:
				using value_type = typename enumerator_traits::value_type;
				using compare_type = std::function<bool(const value_type&, const value_type&) >;
			public:
				virtual void unique() = 0;
				virtual void unique(compare_type Compare) = 0;
			};
			template<typename enumerator_traits, typename base>
			struct ability_impl<enumerator_traits, base, sentinel_enumerator_tag> : public base{
			private:
				using value_type = typename enumerator_traits::value_type;
				using compare_type = std::function<bool(const value_type&, const value_type&) >;
			public:
				void unique()override{ base::End = std::unique(base::Cur, base::End); }
				void unique(compare_type Compare)override{ base::End = std::unique(base::Cur, base::End, std::move(Compare)); }
			};
			template<typename enumerator_traits>
			struct ability_interface<enumerator_traits, range_enumerator_tag>{
			private:
				using value_type = typename enumerator_traits::value_type;
				using compare_type = std::function<bool(const value_type&, const value_type&) >;
			public:
				virtual void unique_sofar() = 0;
				virtual void unique_range() = 0;
				virtual void unique() = 0;
				virtual void unique_sofar(compare_type Compare) = 0;
				virtual void unique_range(compare_type Compare) = 0;
				virtual void unique(compare_type Compare) = 0;
			};
			template<typename enumerator_traits, typename base>
			struct ability_impl<enumerator_traits, base, range_enumerator_tag> : public base{
			private:
				using value_type = typename enumerator_traits::value_type;
				using compare_type = std::function<bool(const value_type&, const value_type&) >;
			public:
				void unique_sofar()override{ base::End = std::unique(base::Beg, base::Cur); }
				void unique_range()override{ base::End = std::unique(base::Beg, base::End); }
				void unique()override{ base::End = std::unique(base::Cur, base::End); }
				void unique_sofar(compare_type Compare)override{ base::End = std::unique(base::Beg, base::Cur, std::move(Compare)); }
				void unique_range(compare_type Compare)override{ base::End = std::unique(base::Beg, base::End, std::move(Compare)); }
				void unique(compare_type Compare) override{ base::End = std::unique(base::Cur, base::End, std::move(Compare)); }
			};
		};
		struct fill_ability{
			template<typename enumerator_traits, typename enumerator_category = typename near_base_of<typename enumerator_traits::enumerator_tag, sentinel_enumerator_tag, range_enumerator_tag>::type >
			struct ability_interface{
				static_assert(!std::is_void<enumerator_category>::value, "fill_ability cannot be used in this enumerator_category.");
			};
			template<typename enumerator_traits, typename base, typename enumerator_category = typename near_base_of<typename enumerator_traits::enumerator_tag, sentinel_enumerator_tag, range_enumerator_tag>::type >
			struct ability_impl : public base{};
			template<typename enumerator_traits>
			struct ability_interface<enumerator_traits, sentinel_enumerator_tag>{
			private:
				using value_type = typename enumerator_traits::value_type;
			public:
				virtual void fill(const value_type& Val) = 0;
			};
			template<typename enumerator_traits, typename base>
			struct ability_impl<enumerator_traits, base, sentinel_enumerator_tag> : public base{
			private:
				using value_type = typename enumerator_traits::value_type;
			public:
				void fill(const value_type& Val)override{ std::fill(base::Cur, base::End, Val); }
			};
			template<typename enumerator_traits>
			struct ability_interface<enumerator_traits, range_enumerator_tag>{
			private:
				using value_type = typename enumerator_traits::value_type;
			public:
				virtual void fill(const value_type& Val) = 0;
				virtual void fill_sofar(const value_type& Val) = 0;
				virtual void fill_range(const value_type& Val) = 0;
			};
			template<typename enumerator_traits, typename base>
			struct ability_impl<enumerator_traits, base, range_enumerator_tag> : public base{
			private:
				using value_type = typename enumerator_traits::value_type;
			public:
				void fill(const value_type& Val)override{ std::fill(base::Cur, base::End, Val); }
				void fill_sofar(const value_type& Val)override{ std::fill(base::Beg, base::Cur, Val); }
				void fill_range(const value_type& Val)override{ std::fill(base::Beg, base::End, Val); }
			};
		};
		struct generate_ability{
			template<typename enumerator_traits, typename enumerator_category = typename near_base_of<typename enumerator_traits::enumerator_tag, sentinel_enumerator_tag, range_enumerator_tag>::type >
			struct ability_interface{
				static_assert(!std::is_void<enumerator_category>::value, "generate_ability cannot be used in this enumerator_category.");
			};
			template<typename enumerator_traits, typename base, typename enumerator_category = typename near_base_of<typename enumerator_traits::enumerator_tag, sentinel_enumerator_tag, range_enumerator_tag>::type >
			struct ability_impl : public base{};
			template<typename enumerator_traits>
			struct ability_interface<enumerator_traits, sentinel_enumerator_tag>{
			private:
				using value_type = typename enumerator_traits::value_type;
				using generate_type = std::function<value_type(void) >;
			public:
				virtual void generate(generate_type Generate) = 0;
			};
			template<typename enumerator_traits, typename base>
			struct ability_impl<enumerator_traits, base, sentinel_enumerator_tag> : public base{
			private:
				using value_type = typename enumerator_traits::value_type;
				using generate_type = std::function<value_type(void) >;
			public:
				void generate(generate_type Generate)override{ std::generate(base::Cur, base::End, std::move(Generate)); }
			};
			template<typename enumerator_traits>
			struct ability_interface<enumerator_traits, range_enumerator_tag>{
			private:
				using value_type = typename enumerator_traits::value_type;
				using generate_type = std::function<value_type(void) >;
			public:
				virtual void generate(generate_type Generate) = 0;
				virtual void generate_sofar(generate_type Generate) = 0;
				virtual void generate_range(generate_type Generate) = 0;
			};
			template<typename enumerator_traits, typename base>
			struct ability_impl<enumerator_traits, base, range_enumerator_tag> : public base{
			private:
				using value_type = typename enumerator_traits::value_type;
				using generate_type = std::function<value_type(void) >;
			public:
				void generate(generate_type Generate)override{ std::generate(base::Cur, base::End, std::move(Generate)); }
				void generate_sofar(generate_type Generate)override{ std::generate(base::Beg, base::Cur, std::move(Generate)); }
				void generate_range(generate_type Generate)override{ std::generate(base::Beg, base::End, std::move(Generate)); }
			};
		};
	}
}
#
#endif
