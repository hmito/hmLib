#ifndef HMLIB_TABLE_INC
#define HMLIB_TABLE_INC
#
#include<hmLib/position.hpp>
namespace hmLib{
	template<typename T>
	class table{
	public:
		using size_type = unsigned int;
		using column_type = std::vector < T > ;
	private:
	public:
		T& at(size_type pos_row, size_type pos_col);
		const T& at(size_type pos_row, size_type pos_col)const;
		T& operator()(size_type pos_row, size_type pos_col);
		const T& operator()(size_type pos_row, size_type pos_col)const;
		size_type column_size(size_type column_pos_)const;
	};

	template<typename T>
	class named_table;

}
#
#endif
