#include<fstream>
#include<iostream>
#include<sstream>
#include<vector>
#include<boost/lexical_cast.hpp>
#include<boost/iterator_adaptors.hpp>
#include<hmLib/csv_iterator.hpp>

using size_type = unsigned int;
using string_type = std::string;
using column_type = std::vector < string_type >;
using data_type = std::pair < string_type, column_type >;
//	std::vector<data_type> Data;

struct element_proxy{
private:
	string_type& Str;
public:
	element_proxy(string_type& Ref) :Str(Ref){}
	operator string_type&(){ return Str; }
	operator const string_type&()const{ return Str; }
	element_proxy& operator=(const string_type& Str_){
		Str = Str_;
		return *this;
	}
	template<typename type>
	void operator>>(type& Val)const{
		Val = boost::lexical_cast<type>(Str);
	}
	template<typename type>
	void operator<<(type Val){
		Str = boost::lexical_cast<string_type>(Val);
	}
};
struct const_element_proxy{
private:
	const string_type& Str;
public:
	const_element_proxy(const string_type& Ref) :Str(Ref){}
	operator const string_type&()const{ return Str; }
	template<typename type>
	void operator>>(type& Val)const{
		Val = boost::lexical_cast<type>(Str);
	}
};
template<typename type>
struct casted_element_proxy :private element_proxy{
public:
	casted_element_proxy(string_type& Ref) :element_proxy(Ref){}
	operator type()const{
		type Val;
		this->operator>>(Val);
		return Val;
	}
	casted_element_proxy<type>& operator=(const type& Val){
		this->operator<<(Val);
		return *this;
	}
};
template<typename type>
struct const_casted_element_proxy :private const_element_proxy{
public:
	const_casted_element_proxy(const string_type& Ref) :const_element_proxy(Ref){}
	operator type()const{
		type Val;
		this->operator>>(Val);
		return Val;
	}
};

struct column_proxy{
private:
	data_type& rData;
	using base_iterator = typename data_type::second_type::iterator;
public:
	struct element_iterator
		: public boost::iterator_adaptor<element_iterator, base_iterator, element_proxy, boost::random_access_traversal_tag, element_proxy>{
		using iterator_adaptor = boost::iterator_adaptor<element_iterator, base_iterator, element_proxy, boost::random_access_traversal_tag, element_proxy>;
	public:
		element_iterator() = default;
		element_iterator(base_iterator bitr) : iterator_adaptor(bitr){}
	private:
		element_proxy dereference()const{
			return element_proxy(*(base_reference()));
		}
	};
	template<typename type>
	struct casted_element_iterator
		: public boost::iterator_adaptor<casted_element_iterator<type>, base_iterator, casted_element_proxy<type>, boost::random_access_traversal_tag, casted_element_proxy<type> >{
		using iterator_adaptor = boost::iterator_adaptor<casted_element_iterator<type>, base_iterator, casted_element_proxy<type>, boost::random_access_traversal_tag, casted_element_proxy<type> >;
	public:
		casted_element_iterator() = default;
		casted_element_iterator(base_iterator) : iterator_adaptor(base_iterator){}
	private:
		casted_element_proxy<type> dereference()const{
			return casted_element_proxy<type>(*(base_reference()));
		}
	};
public:
	column_proxy(data_type& rData_) :rData(rData_){}
	string_type name()const{ return rData.first; }
	element_proxy operator[](size_type Pos){ return element_proxy(rData.second.at(Pos)); }
	const_element_proxy operator[](size_type Pos)const{ return const_element_proxy(rData.second.at(Pos)); }
	element_proxy at(size_type Pos){ return element_proxy(rData.second.at(Pos)); }
	const_element_proxy at(size_type Pos)const{ return const_element_proxy(rData.second.at(Pos)); }
	size_type size()const{ return rData.second.size(); }
	element_iterator begin(){ return element_iterator(rData.second.begin()); }
	element_iterator end(){ return element_iterator(rData.second.end()); }
	template<typename type>
	casted_element_iterator<type> casted_begin(){ return casted_element_iterator<type>(rData.second.begin()); }
	template<typename type>
	casted_element_iterator<type> casted_end(){ return casted_element_iterator<type>(rData.second.end()); }
};


int main(){
	std::ifstream fin("test1.csv");

	hmLib::icsv_iterator itr(fin);
	hmLib::ocsv_iterator oitr(std::cout, '\t');


	hmLib::advance_line(itr);

	while(!itr.eof()){

		std::string Name;
		unsigned int Val;
		unsigned char Sex;
		*itr++ >> Name;
		*itr++ >> Val;
		*itr++ >> Sex;

		*oitr++ << Name;
		*oitr++ << Val;
		*oitr++ << Sex;
		oitr.endl();
	}

	system("pause");



	data_type Data;

	column_proxy cp(Data);

	auto bitr = cp.begin();
	auto eitr = cp.end();


	return 0;
}