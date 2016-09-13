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
				void find_back(pred_type Pred)override{ base::Cur = std::find_if(base::Cur, base::End, Pred); }
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
	}
}
#
#endif
