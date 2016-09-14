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
			private:
				ability_interface();
			};
			template<typename enumerator_traits, typename base, typename enumerator_category = typename near_base_of<typename enumerator_traits::enumerator_tag, sentinel_enumerator_tag, range_enumerator_tag>::type >
			struct ability_impl : public base{};
			template<typename enumerator_traits>
			struct ability_interface<enumerator_traits, sentinel_enumerator_tag>{
			private:
				using value_type = typename enumerator_traits::value_type;
				using pred_type = std::function<bool(const value_type&) >;
			public:
				virtual bool all_of_back(pred_type Pred) = 0;
				virtual bool any_of_back(pred_type Pred) = 0;
				virtual bool none_of_back(pred_type Pred) = 0;
			};
			template<typename enumerator_traits, typename base>
			struct ability_impl<enumerator_traits, base, sentinel_enumerator_tag> : public base{
			private:
				using value_type = typename enumerator_traits::value_type;
				using pred_type = std::function<bool(const value_type&) >;
			public:
				bool all_of_back(pred_type Pred)override{ return std::all_of(base::Cur, base::End, Pred); }
				bool anu_of_back(pred_type Pred)override{ return std::any_of(base::Cur, base::End, Pred); }
				bool none_of_back(pred_type Pred)override{ return std::none_of(base::Cur, base::End, Pred); }
			};
			template<typename enumerator_traits>
			struct ability_interface<enumerator_traits, range_enumerator_tag>{
			private:
				using value_type = typename enumerator_traits::value_type;
				using pred_type = std::function<bool(const value_type&) >;
			public:
				virtual void all_of(pred_type Pred) = 0;
				virtual void all_of_front(pred_type Pred) = 0;
				virtual void all_of_back(pred_type Pred) = 0;
				virtual void any_of(pred_type Pred) = 0;
				virtual void any_of_front(pred_type Pred) = 0;
				virtual void any_of_back(pred_type Pred) = 0;
				virtual void none_of(pred_type Pred) = 0;
				virtual void none_of_front(pred_type Pred) = 0;
				virtual void none_of_back(pred_type Pred) = 0;
			};
			template<typename enumerator_traits, typename base>
			struct ability_impl<enumerator_traits, base, range_enumerator_tag> : public base{
			private:
				using value_type = typename enumerator_traits::value_type;
				using pred_type = std::function<bool(const value_type&) >;
			public:
				bool all_of(pred_type Pred)override{ return std::all_of(base::Beg, base::End, Pred); }
				bool all_of_front(pred_type Pred)override{ return std::all_of(base::Beg, base::Cur, Pred); }
				bool all_of_back(pred_type Pred)override{ return std::all_of(base::Cur, base::End, Pred); }
				bool any_of(pred_type Pred)override{ return std::any_of(base::Beg, base::End, Pred); }
				bool any_of_front(pred_type Pred)override{ return std::any_of(base::Beg, base::Cur, Pred); }
				bool any_of_back(pred_type Pred)override{ return std::any_of(base::Cur, base::End, Pred); }
				bool none_of(pred_type Pred)override{ return std::none_of(base::Beg, base::End, Pred); }
				bool none_of_front(pred_type Pred)override{ return std::none_of(base::Beg, base::Cur, Pred); }
				bool none_of_back(pred_type Pred)override{ return std::none_of(base::Cur, base::End, Pred); }
			};
		};
		struct find_ability{
			template<typename enumerator_traits,typename enumerator_category = typename near_base_of<typename enumerator_traits::enumerator_tag, sentinel_enumerator_tag, range_enumerator_tag>::type >
			struct ability_interface{
			private:
				ability_interface();
			};
			template<typename enumerator_traits, typename base, typename enumerator_category = typename near_base_of<typename enumerator_traits::enumerator_tag, sentinel_enumerator_tag, range_enumerator_tag>::type >
			struct ability_impl : public base{};
			template<typename enumerator_traits>
			struct ability_interface<enumerator_traits, sentinel_enumerator_tag>{
			private:
				using value_type = typename enumerator_traits::value_type;
				using pred_type = std::function<bool(const value_type&) >;
			public:
				virtual void find_back(const value_type& Val) = 0;
				virtual void find_back_if(pred_type Pred) = 0;
			};
			template<typename enumerator_traits, typename base>
			struct ability_impl<enumerator_traits,base, sentinel_enumerator_tag> : public base{
			private:
				using value_type = typename enumerator_traits::value_type;
				using pred_type = std::function<bool(const value_type&) >;
			public:
				void find_back(const value_type& Val)override{ base::Cur = std::find(base::Cur, base::End, Val); }
				void find_back_if(pred_type Pred)override{ base::Cur = std::find_if(base::Cur, base::End, Pred); }
			};
			template<typename enumerator_traits>
			struct ability_interface<enumerator_traits, range_enumerator_tag>{
			private:
				using value_type = typename enumerator_traits::value_type;
				using pred_type = std::function<bool(const value_type&) >;
			public:
				virtual void find(const value_type& Val) = 0;
				virtual void find_front(const value_type& Val) = 0;
				virtual void find_back(const value_type& Val) = 0;
				virtual void find_if(pred_type Pred) = 0;
				virtual void find_front_if(pred_type Pred) = 0;
				virtual void find_back_if(pred_type Pred) = 0;
			};
			template<typename enumerator_traits, typename base>
			struct ability_impl<enumerator_traits, base, range_enumerator_tag> : public base{
			private:
				using value_type = typename enumerator_traits::value_type;
				using pred_type = std::function<bool(const value_type&) >;
			public:
				void find(const value_type& Val)override{ base::Cur = std::find(base::Beg, base::End, Val); }
				void find_front(const value_type& Val)override{ base::Cur = std::find(base::Beg, base::Cur, Val); }
				void find_back(const value_type& Val)override{ base::Cur = std::find(base::Cur, base::End, Val); }
				void find_if(pred_type Pred)override{ base::Cur = std::find_if(base::Beg, base::End, Pred); }
				void find_front_if(pred_type Pred)override{ base::Cur = std::find_if(base::Beg, base::Cur, Pred); }
				void find_back_if(pred_type Pred)override{ base::Cur = std::find_if(base::Cur, base::End, Pred); }
			};
		};
		struct count_ability{
			template<typename enumerator_traits, typename enumerator_category = typename near_base_of<typename enumerator_traits::enumerator_tag, sentinel_enumerator_tag, range_enumerator_tag>::type >
			struct ability_interface{
			private:
				ability_interface();
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
				virtual difference_type count_back(const value_type& Val) = 0;
				virtual difference_type count_back_if(pred_type Pred) = 0;
			};
			template<typename enumerator_traits, typename base>
			struct ability_impl<enumerator_traits, base, sentinel_enumerator_tag> : public base{
			private:
				using value_type = typename enumerator_traits::value_type;
				using difference_type = typename enumerator_traits::difference_type;
				using pred_type = std::function<bool(const value_type&) >;
			public:
				difference_type count_back(const value_type& Val)override{ return std::count(base::Cur, base::End, Val); }
				difference_type count_back(pred_type Pred)override{ return  std::count_if(base::Cur, base::End, Pred); }
			};
			template<typename enumerator_traits>
			struct ability_interface<enumerator_traits, range_enumerator_tag>{
			private:
				using value_type = typename enumerator_traits::value_type;
				using difference_type = typename enumerator_traits::difference_type;
				using pred_type = std::function<bool(const value_type&) >;
			public:
				virtual difference_type count(const value_type& Val) = 0;
				virtual difference_type count_front(const value_type& Val) = 0;
				virtual difference_type count_back(const value_type& Val) = 0;
				virtual difference_type count_if(pred_type Pred) = 0;
				virtual difference_type count_front_if(pred_type Pred) = 0;
				virtual difference_type count_back_if(pred_type Pred) = 0;
			};
			template<typename enumerator_traits, typename base>
			struct ability_impl<enumerator_traits, base, range_enumerator_tag> : public base{
			private:
				using value_type = typename enumerator_traits::value_type;
				using difference_type = typename enumerator_traits::difference_type;
				using pred_type = std::function<bool(const value_type&) >;
			public:
				difference_type count(const value_type& Val)override{ return std::count(base::Beg, base::End, Val); }
				difference_type count_front(const value_type& Val)override{ return std::count(base::Beg, base::Cur, Val); }
				difference_type count_back(const value_type& Val)override{ return std::count(base::Cur, base::End, Val); }
				difference_type count_if(pred_type Pred)override{ return std::count_if(base::Beg, base::End, Pred); }
				difference_type count_front_if(pred_type Pred)override{ return std::count_if(base::Beg, base::Cur, Pred); }
				difference_type count_back_if(pred_type Pred)override{ return  std::count_if(base::Cur, base::End, Pred); }
			};
		};
		struct sort_ability{
			template<typename enumerator_traits, typename enumerator_category = typename near_base_of<typename enumerator_traits::enumerator_tag, range_enumerator_tag>::type >
			struct ability_interface{
			private:
				ability_interface();
			};
			template<typename enumerator_traits, typename base, typename enumerator_category = typename near_base_of<typename enumerator_traits::enumerator_tag, range_enumerator_tag>::type >
			struct ability_impl : public base{};
			template<typename enumerator_traits>
			struct ability_interface<enumerator_traits, range_enumerator_tag>{
			private:
				using value_type = typename enumerator_traits::value_type;
				using compare_type = std::function<bool(const value_type&, const value_type&) >;
			public:
				virtual void sort_front() = 0;
				virtual void sort_back() = 0;
				virtual void sort() = 0;
				virtual void sort_front(compare_type Comp) = 0;
				virtual void sort_back(compare_type Comp) = 0;
				virtual void sort(compare_type Comp) = 0;
			};
			template<typename enumerator_traits, typename base>
			struct ability_impl<enumerator_traits, base, range_enumerator_tag> : public base{
			private:
				using value_type = typename enumerator_traits::value_type;
				using compare_type = std::function<bool(const value_type&, const value_type&) >;
			public:
				void sort_front()override{ std::sort(base::Beg, base::Cur); }
				void sort_back()override{ std::sort(base::Cur, base::End); }
				void sort()override{ std::sort(base::Beg, base::End); }
				void sort_front(compare_type Comp)override{ std::sort(base::Beg, base::Cur, Comp); }
				void sort_back(compare_type Comp)override{ std::sort(base::Cur, base::End, Comp); }
				void sort(compare_type Comp) override{ std::sort(base::Beg, base::End, Comp); }
			};
		};
		struct equal_range_ability{
			template<typename enumerator_traits, typename enumerator_category = typename near_base_of<typename enumerator_traits::enumerator_tag, range_enumerator_tag>::type >
			struct ability_interface{
			private:
				ability_interface();
			};
			template<typename enumerator_traits, typename base, typename enumerator_category = typename near_base_of<typename enumerator_traits::enumerator_tag, range_enumerator_tag>::type >
			struct ability_impl : public base{};
			template<typename enumerator_traits>
			struct ability_interface<enumerator_traits, range_enumerator_tag>{
			private:
				using value_type = typename enumerator_traits::value_type;
				using compare_type = std::function<bool(const value_type&, const value_type&) >;
			public:
				virtual void lower_bound_front() = 0;
				virtual void lower_bound_back() = 0;
				virtual void lower_bound() = 0;
				virtual void lower_bound_front(compare_type Comp) = 0;
				virtual void lower_bound_back(compare_type Comp) = 0;
				virtual void lower_bound(compare_type Comp) = 0;
				virtual void upper_bound_front() = 0;
				virtual void upper_bound_back() = 0;
				virtual void upper_bound() = 0;
				virtual void upper_bound_front(compare_type Comp) = 0;
				virtual void upper_bound_back(compare_type Comp) = 0;
				virtual void upper_bound(compare_type Comp) = 0;
				virtual void equal_range_front() = 0;
				virtual void equal_range_back() = 0;
				virtual void equal_range() = 0;
				virtual void equal_range_front(compare_type Comp) = 0;
				virtual void equal_range_back(compare_type Comp) = 0;
				virtual void equal_range(compare_type Comp) = 0;
			};
			template<typename enumerator_traits, typename base>
			struct ability_impl<enumerator_traits, base, range_enumerator_tag> : public base{
			private:
				using value_type = typename enumerator_traits::value_type;
				using compare_type = std::function<bool(const value_type&, const value_type&) >;
			public:
				void lower_bound_front()override{ base::Cur = std::lower_bound(base::Beg, base::Cur); }
				void lower_bound_back()override{ base::Cur = std::lower_bound(base::Cur, base::End); }
				void lower_bound()override{ base::Cur = std::lower_bound(base::Beg, base::End); }
				void lower_bound_front(compare_type Comp)override{ base::Cur = std::lower_bound(base::Beg, base::Cur, Comp); }
				void lower_bound_back(compare_type Comp)override{ base::Cur = std::lower_bound(base::Cur, base::End, Comp); }
				void lower_bound(compare_type Comp) override{ base::Cur = std::lower_bound(base::Beg, base::End, Comp); }
				void upper_bound_front()override{ base::Cur = std::upper_bound(base::Beg, base::Cur); }
				void upper_bound_back()override{ base::Cur = std::upper_bound(base::Cur, base::End);  }
				void upper_bound()override{ base::Cur = std::upper_bound(base::Beg, base::End); }
				void upper_bound_front(compare_type Comp)override{ base::Cur = std::upper_bound(base::Beg, base::Cur, Comp);}
				void upper_bound_back(compare_type Comp)override{ base::Cur = std::upper_bound(base::Cur, base::End, Comp);}
				void upper_bound(compare_type Comp) override{ base::Cur = std::upper_bound(base::Beg, base::End, Comp);}
				void equal_range_front()override{ auto ans = std::equal_range(base::Beg, base::Cur); base::Beg = ans.first; base::End = ans.second; base::Cur = base::Beg; }
				void equal_range_back()override{ auto ans = std::equal_range(base::Cur, base::End);  base::Beg = ans.first; base::End = ans.second; base::Cur = base::Beg; }
				void equal_range()override{ auto ans = std::equal_range(base::Beg, base::End);  base::Beg = ans.first; base::End = ans.second; base::Cur = base::Beg;}
				void equal_range_front(compare_type Comp)override{ auto ans = std::equal_range(base::Beg, base::Cur, Comp);  base::Beg = ans.first; base::End = ans.second; base::Cur = base::Beg;}
				void equal_range_back(compare_type Comp)override{ auto ans = std::equal_range(base::Cur, base::End, Comp);  base::Beg = ans.first; base::End = ans.second; base::Cur = base::Beg;}
				void equal_range(compare_type Comp) override{ auto ans = std::equal_range(base::Beg, base::End, Comp);  base::Beg = ans.first; base::End = ans.second; base::Cur = base::Beg;}
			};
		};
		struct minmax_ability{
			template<typename enumerator_traits, typename enumerator_category = typename near_base_of<typename enumerator_traits::enumerator_tag, sentinel_enumerator_tag, range_enumerator_tag>::type >
			struct ability_interface{
			private:
				ability_interface();
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
				virtual reference min_back() = 0;
				virtual reference min_back_if(compare_type Compare) = 0;
				virtual reference max_back() = 0;
				virtual reference max_back_if(compare_type Compare) = 0;
				virtual void min_element_back() = 0;
				virtual void min_element_back_if(compare_type Compare) = 0;
				virtual void max_element_back() = 0;
				virtual void max_element_back_if(compare_type Compare) = 0;
			};
			template<typename enumerator_traits, typename base>
			struct ability_impl<enumerator_traits, base, sentinel_enumerator_tag> : public base{
			private:
				using value_type = typename enumerator_traits::value_type;
				using reference = typename enumerator_traits::reference;
				using compare_type = std::function<bool(const value_type&, const value_type&) >;
			public:
				reference min_back()override{ return std::min(base::Cur, base::End); }
				reference min_back_if(compare_type Compare)override{ return std::min(base::Cur, base::End, Compare); }
				reference max_back()override{ return std::max(base::Cur, base::End); }
				reference max_back_if(compare_type Compare)override{ return std::max(base::Cur, base::End, Compare); }
				void min_element_back()override{ base::Cur = std::min(base::Cur, base::End); }
				void min_element_back_if(compare_type Compare)override{ base::Cur = std::min(base::Cur, base::End, Compare); }
				void max_element_back()override{ base::Cur = std::max(base::Cur, base::End); }
				void max_element_back_if(compare_type Compare)override{ base::Cur = std::max(base::Cur, base::End, Compare); }
			};
			template<typename enumerator_traits>
			struct ability_interface<enumerator_traits, range_enumerator_tag>{
			private:
				using value_type = typename enumerator_traits::value_type;
				using reference = typename enumerator_traits::reference;
				using compare_type = std::function<bool(const value_type&, const value_type&) >;
			public:
				virtual reference min_back() = 0;
				virtual reference min_back_if(compare_type Compare) = 0;
				virtual reference max_back() = 0;
				virtual reference max_back_if(compare_type Compare) = 0;
				virtual void min_element_back() = 0;
				virtual void min_element_back_if(compare_type Compare) = 0;
				virtual void max_element_back() = 0;
				virtual void max_element_back_if(compare_type Compare) = 0;
				virtual reference min_front() = 0;
				virtual reference min_front_if(compare_type Compare) = 0;
				virtual reference max_front() = 0;
				virtual reference max_front_if(compare_type Compare) = 0;
				virtual void min_element_front() = 0;
				virtual void min_element_front_if(compare_type Compare) = 0;
				virtual void max_element_front() = 0;
				virtual void max_element_front_if(compare_type Compare) = 0;
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
				reference min_back()override{ return std::min(base::Cur, base::End); }
				reference min_back_if(compare_type Compare)override{ return std::min(base::Cur, base::End, Compare); }
				reference max_back()override{ return std::max(base::Cur, base::End); }
				reference max_back_if(compare_type Compare)override{ return std::max(base::Cur, base::End, Compare); }
				void min_element_back()override{ base::Cur = std::min(base::Cur, base::End); }
				void min_element_back_if(compare_type Compare)override{ base::Cur = std::min(base::Cur, base::End, Compare); }
				void max_element_back()override{ base::Cur = std::max(base::Cur, base::End); }
				void max_element_back_if(compare_type Compare)override{ base::Cur = std::max(base::Cur, base::End, Compare); }
				reference min_front()override{ return std::min(base::Beg, base::Cur); }
				reference min_front_if(compare_type Compare)override{ return std::min(base::Beg, base::Cur, Compare); }
				reference max_front()override{ return std::max(base::Beg, base::Cur); }
				reference max_front_if(compare_type Compare)override{ return std::max(base::Beg, base::Cur, Compare); }
				void min_element_front()override{ base::Cur = std::min(base::Beg, base::Cur); }
				void min_element_front_if(compare_type Compare)override{ base::Cur = std::min(base::Beg, base::Cur, Compare); }
				void max_element_front()override{ base::Cur = std::max(base::Beg, base::Cur); }
				void max_element_front_if(compare_type Compare)override{ base::Cur = std::max(base::Beg, base::Cur, Compare); }
				reference min()override{ return std::min(base::Beg, base::End); }
				reference min_if(compare_type Compare)override{ return std::min(base::Beg, base::End, Compare); }
				reference max()override{ return std::max(base::Beg, base::End); }
				reference max_if(compare_type Compare)override{ return std::max(base::Beg, base::End, Compare); }
				void min_element()override{ base::Cur = std::min(base::Beg, base::End); }
				void min_element_if(compare_type Compare)override{ base::Cur = std::min(base::Beg, base::End, Compare); }
				void max_element()override{ base::Cur = std::max(base::Beg, base::End); }
				void max_element_if(compare_type Compare)override{ base::Cur = std::max(base::Beg, base::End, Compare); }
			};
		};
		struct remove_ability{
			template<typename enumerator_traits, typename enumerator_category = typename near_base_of<typename enumerator_traits::enumerator_tag, sentinel_enumerator_tag, range_enumerator_tag>::type >
			struct ability_interface{
			private:
				ability_interface();
			};
			template<typename enumerator_traits, typename base, typename enumerator_category = typename near_base_of<typename enumerator_traits::enumerator_tag, range_enumerator_tag>::type >
			struct ability_impl : public base{};
			template<typename enumerator_traits>
			struct ability_interface<enumerator_traits, sentinel_enumerator_tag>{
			private:
				using value_type = typename enumerator_traits::value_type;
				using pred_type = std::function<bool(const value_type&) >;
			public:
				virtual void remove_back(const value_type& Val) = 0;
				virtual void remove_back_if(pred_type Pred) = 0;
			};
			template<typename enumerator_traits, typename base>
			struct ability_impl<enumerator_traits, base, sentinel_enumerator_tag> : public base{
			private:
				using value_type = typename enumerator_traits::value_type;
				using pred_type = std::function<bool(const value_type&) >;
			public:
				void remove_back(const value_type& Val)override{ base::End = std::remove(base::Cur, base::End, Val); }
				void remove_back_if(pred_type Pred)override{ base::End = std::remove_if(base::Cur, base::End, Pred); }
			};
			template<typename enumerator_traits>
			struct ability_interface<enumerator_traits, range_enumerator_tag>{
			private:
				using value_type = typename enumerator_traits::value_type;
				using pred_type = std::function<bool(const value_type&) >;
			public:
				virtual void remove_front() = 0;
				virtual void remove_back() = 0;
				virtual void remove() = 0;
				virtual void remove_front_if(pred_type Pred) = 0;
				virtual void remove_back_if(pred_type Pred) = 0;
				virtual void remove_if(pred_type Pred) = 0;
			};
			template<typename enumerator_traits, typename base>
			struct ability_impl<enumerator_traits, base, range_enumerator_tag> : public base{
			private:
				using value_type = typename enumerator_traits::value_type;
				using pred_type = std::function<bool(const value_type&) >;
			public:
				void remove_front(const value_type& Val)override{ base::End = std::remove(base::Beg, base::Cur,Val); }
				void remove_back(const value_type& Val)override{ base::End = std::remove(base::Cur, base::End, Val); }
				void remove(const value_type& Val)override{ base::End = std::remove(base::Beg, base::End, Val); }
				void remove_front_if(pred_type Pred)override{ base::End = std::remove_if(base::Beg, base::Cur, Pred); }
				void remove_back_if(pred_type Pred)override{ base::End = std::remove_if(base::Cur, base::End, Pred); }
				void remove_if(pred_type Pred) override{ base::End = std::remove_if(base::Beg, base::End, Pred); }
			};
		};
		struct unique_ability{
			template<typename enumerator_traits, typename enumerator_category = typename near_base_of<typename enumerator_traits::enumerator_tag, sentinel_enumerator_tag, range_enumerator_tag>::type >
			struct ability_interface{
			private:
				ability_interface();
			};
			template<typename enumerator_traits, typename base, typename enumerator_category = typename near_base_of<typename enumerator_traits::enumerator_tag, range_enumerator_tag>::type >
			struct ability_impl : public base{};
			template<typename enumerator_traits>
			struct ability_interface<enumerator_traits, sentinel_enumerator_tag>{
			private:
				using value_type = typename enumerator_traits::value_type;
				using compare_type = std::function<bool(const value_type&, const value_type&) >;
			public:
				virtual void unique_back() = 0;
				virtual void unique_back(compare_type Compare) = 0;
			};
			template<typename enumerator_traits, typename base>
			struct ability_impl<enumerator_traits, base, sentinel_enumerator_tag> : public base{
			private:
				using value_type = typename enumerator_traits::value_type;
				using compare_type = std::function<bool(const value_type&, const value_type&) >;
			public:
				void unique_back()override{ base::End = std::unique(base::Cur, base::End); }
				void unique_back(compare_type Compare)override{ base::End = std::unique(base::Cur, base::End, Compare); }
			};
			template<typename enumerator_traits>
			struct ability_interface<enumerator_traits, range_enumerator_tag>{
			private:
				using value_type = typename enumerator_traits::value_type;
				using compare_type = std::function<bool(const value_type&, const value_type&) >;
			public:
				virtual void unique_front() = 0;
				virtual void unique_back() = 0;
				virtual void unique() = 0;
				virtual void unique_front(compare_type Compare) = 0;
				virtual void unique_back(compare_type Compare) = 0;
				virtual void unique(compare_type Compare) = 0;
			};
			template<typename enumerator_traits, typename base>
			struct ability_impl<enumerator_traits, base, range_enumerator_tag> : public base{
			private:
				using value_type = typename enumerator_traits::value_type;
				using compare_type = std::function<bool(const value_type&, const value_type&) >;
			public:
				void unique_front()override{ base::End = std::unique(base::Beg, base::Cur); }
				void unique_back()override{ base::End = std::unique(base::Cur, base::End); }
				void unique()override{ base::End = std::unique(base::Beg, base::End); }
				void unique_front(compare_type Compare)override{ base::End = std::unique(base::Beg, base::Cur, Compare); }
				void unique_back(compare_type Compare)override{ base::End = std::unique(base::Cur, base::End, Compare); }
				void unique(compare_type Compare) override{ base::End = std::unique(base::Beg, base::End, Compare); }
			};
		};
		struct shuffle_ability{
			template<typename enumerator_traits, typename enumerator_category = typename near_base_of<typename enumerator_traits::enumerator_tag, sentinel_enumerator_tag, range_enumerator_tag>::type >
			struct ability_interface{
			private:
				ability_interface();
			};
			template<typename enumerator_traits, typename base, typename enumerator_category = typename near_base_of<typename enumerator_traits::enumerator_tag, range_enumerator_tag>::type >
			struct ability_impl : public base{};
			template<typename enumerator_traits>
			struct ability_interface<enumerator_traits, sentinel_enumerator_tag>{
			private:
				using difference_type = typename enumerator_traits::difference_type;
				using random_engine_type = std::function<difference_type(void) >;
			public:
				virtual void shuffle_back(random_engine_type RandEngine) = 0;
			};
			template<typename enumerator_traits, typename base>
			struct ability_impl<enumerator_traits, base, sentinel_enumerator_tag> : public base{
			private:
				using difference_type = typename enumerator_traits::difference_type;
				using random_engine_type = std::function<difference_type(void) >;
			public:
				void shuffle_back(random_engine_type RandEngine)override{ std::shuffle(base::Cur, base::End, RandEngine); }
			};
			template<typename enumerator_traits>
			struct ability_interface<enumerator_traits, range_enumerator_tag>{
			private:
				using difference_type = typename enumerator_traits::difference_type;
				using random_engine_type = std::function<difference_type(void) >;
			public:
				virtual void shuffle_front(random_engine_type RandEngine) = 0;
				virtual void shuffle_back(random_engine_type RandEngine) = 0;
				virtual void shuffle(random_engine_type RandEngine) = 0;
			};
			template<typename enumerator_traits, typename base>
			struct ability_impl<enumerator_traits, base, range_enumerator_tag> : public base{
			private:
				using value_type = typename enumerator_traits::value_type;
				using pred_type = std::function<bool(const value_type&) >;
			public:
				void shuffle_front(random_engine_type RandEngine)override{ std::shuffle(base::Beg, base::Cur, RandEngine); }
				void shuffle_back(random_engine_type RandEngine)override{ std::shuffle(base::Cur, base::End, RandEngine); }
				void shuffle(random_engine_type RandEngine)override{ std::shuffle(base::Beg, base::End, RandEngine); }
			};
		};
		struct fill_ability{
			template<typename enumerator_traits, typename enumerator_category = typename near_base_of<typename enumerator_traits::enumerator_tag, sentinel_enumerator_tag, range_enumerator_tag>::type >
			struct ability_interface{
			private:
				ability_interface();
			};
			template<typename enumerator_traits, typename base, typename enumerator_category = typename near_base_of<typename enumerator_traits::enumerator_tag, sentinel_enumerator_tag, range_enumerator_tag>::type >
			struct ability_impl : public base{};
			template<typename enumerator_traits>
			struct ability_interface<enumerator_traits, sentinel_enumerator_tag>{
			private:
				using value_type = typename enumerator_traits::value_type;
			public:
				virtual void fill_back(const value_type& Val) = 0;
			};
			template<typename enumerator_traits, typename base>
			struct ability_impl<enumerator_traits, base, sentinel_enumerator_tag> : public base{
			private:
				using value_type = typename enumerator_traits::value_type;
			public:
				void fill_back(const value_type& Val)override{ std::fill(base::Cur, base::End, Val); }
			};
			template<typename enumerator_traits>
			struct ability_interface<enumerator_traits, range_enumerator_tag>{
			private:
				using value_type = typename enumerator_traits::value_type;
			public:
				virtual void fill(const value_type& Val) = 0;
				virtual void fill_front(const value_type& Val) = 0;
				virtual void fill_back(const value_type& Val) = 0;
			};
			template<typename enumerator_traits, typename base>
			struct ability_impl<enumerator_traits, base, range_enumerator_tag> : public base{
			private:
				using value_type = typename enumerator_traits::value_type;
			public:
				void fill(const value_type& Val)override{ std::fill(base::Beg, base::End, Val); }
				void fill_front(const value_type& Val)override{ std::fill(base::Beg, base::Cur, Val); }
				void fill_back(const value_type& Val)override{ std::fill(base::Cur, base::End, Val); }
			};
		};
		struct generate_ability{
			template<typename enumerator_traits, typename enumerator_category = typename near_base_of<typename enumerator_traits::enumerator_tag, sentinel_enumerator_tag, range_enumerator_tag>::type >
			struct ability_interface{
			private:
				ability_interface();
			};
			template<typename enumerator_traits, typename base, typename enumerator_category = typename near_base_of<typename enumerator_traits::enumerator_tag, sentinel_enumerator_tag, range_enumerator_tag>::type >
			struct ability_impl : public base{};
			template<typename enumerator_traits>
			struct ability_interface<enumerator_traits, sentinel_enumerator_tag>{
			private:
				using value_type = typename enumerator_traits::value_type;
				using generate_type = std::function<value_type(void) >;
			public:
				virtual void generate_back(generate_type Generate) = 0;
			};
			template<typename enumerator_traits, typename base>
			struct ability_impl<enumerator_traits, base, sentinel_enumerator_tag> : public base{
			private:
				using value_type = typename enumerator_traits::value_type;
				using generate_type = std::function<value_type(void) >;
			public:
				void generate_back(generate_type Generate)override{ std::generate(base::Cur, base::End, std::move(Generate)); }
			};
			template<typename enumerator_traits>
			struct ability_interface<enumerator_traits, range_enumerator_tag>{
			private:
				using value_type = typename enumerator_traits::value_type;
				using generate_type = std::function<value_type(void) >;
			public:
				virtual void generate(generate_type Generate) = 0;
				virtual void generate_front(generate_type Generate) = 0;
				virtual void generate_back(generate_type Generate) = 0;
			};
			template<typename enumerator_traits, typename base>
			struct ability_impl<enumerator_traits, base, range_enumerator_tag> : public base{
			private:
				using value_type = typename enumerator_traits::value_type;
				using generate_type = std::function<value_type(void) >;
			public:
				void generate(generate_type Generate)override{ std::generate(base::Beg, base::End, std::move(Generate)); }
				void generate_front(generate_type Generate)override{ std::generate(base::Beg, base::Cur, std::move(Generate)); }
				void generate_back(generate_type Generate)override{ std::generate(base::Cur, base::End, std::move(Generate)); }
			};
		};
	}
}
#
#endif
