#ifndef HMLIB_ENUMERATORS_NUMERICABILITY_INC
#define HMLIB_ENUMERATORS_NUMERICABILITY_INC
#
#include<numeric>
#include<functional>
#include<type_traits>
#include<hmLib/type_traits.hpp>
#include"enumbase.hpp"
namespace hmLib{
	namespace enumerators{
		template<typename result_type>
		struct accumulate_ability{
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
				using operation_type = std::function<result_type(result_type, const value_type&) >;
			public:
				virtual result_type accumulate_back(result_type IniVal) = 0;
				virtual result_type accumulate_back(result_type IniVal,operation_type Operation) = 0;
			};
			template<typename enumerator_traits, typename base>
			struct ability_impl<enumerator_traits, base, sentinel_enumerator_tag> : public base{
			private:
				using value_type = typename enumerator_traits::value_type;
				using operation_type = std::function<result_type(result_type, const value_type&) >;
			public:
				result_type accumulate_back(result_type IniVal)override{ return std::accumulate(base::Cur, base::End, IniVal); }
				result_type accumulate_back(result_type IniVal, operation_type Operation)override{ return  std::accumulate(base::Cur, base::End, IniVal, Operation); }
			};
			template<typename enumerator_traits>
			struct ability_interface<enumerator_traits, range_enumerator_tag>{
			private:
				using value_type = typename enumerator_traits::value_type;
				using operation_type = std::function<result_type(result_type, const value_type&) >;
			public:
				virtual result_type accumulate(result_type IniVal) = 0;
				virtual result_type accumulate_front(result_type IniVal) = 0;
				virtual result_type accumulate_back(result_type IniVal) = 0;
				virtual result_type accumulate(result_type IniVal,operation_type Operation) = 0;
				virtual result_type accumulate_front(result_type IniVal, operation_type Operation) = 0;
				virtual result_type accumulate_back(result_type IniVal, operation_type Operation) = 0;
			};
			template<typename enumerator_traits, typename base>
			struct ability_impl<enumerator_traits, base, range_enumerator_tag> : public base{
			private:
				using value_type = typename enumerator_traits::value_type;
				using operation_type = std::function<result_type(result_type, const value_type&) >;
			public:
				result_type accumulate(result_type IniVal)override{ return std::accumulate(base::Beg, base::End, IniVal); }
				result_type accumulate_front(result_type IniVal)override{ return std::accumulate(base::Beg, base::Cur, IniVal); }
				result_type accumulate_back(result_type IniVal)override{ return std::accumulate(base::Cur, base::End, IniVal); }
				result_type accumulate(result_type IniVal, operation_type Operation)override{ return std::accumulate(base::Beg, base::End, IniVal, Operation); }
				result_type accumulate_front(result_type IniVal, operation_type Operation)override{ return std::accumulate(base::Beg, base::Cur, IniVal, Operation); }
				result_type accumulate_back(result_type IniVal, operation_type Operation)override{ return  std::accumulate(base::Cur, base::End, IniVal, Operation); }
			};
		};
	}
}
#
#endif
