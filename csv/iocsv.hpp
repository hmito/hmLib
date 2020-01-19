#ifndef HMLIB_CSV_IOCSV_INC
#define HMLIB_CSV_IOCSV_INC 100
#
namespace hmLib {
	template<typename csv_traits_>
	class icsv_base {
		bool IsEndOfLine;
	public:
		using string_type = typename csv_traits_::string_type;
	public:
		template<typename output_iterator>
		void read_cell(output_iterator out);
		template<typename output_iterator>
		void read_line(output_iterator out);
	};
	template<typename csv_traits_>
	class ocsv_base {
		bool IsEndOfLine;
		bool DoEndOfLine;
	public:
		using string_type = typename csv_traits_::string_type;
	public:
		string_type write_cell();
		template<typename output_iterator>
		void write_cell(output_iterator out);
		template<typename forward_iterator>
		forward_iterator write_cell(forward_iterator beg, forward_iterator end);
		string_type write_line();
		template<typename output_iterator>
		void write_line(output_iterator out);
		template<typename forward_iterator>
		forward_iterator write_line(forward_iterator beg, forward_iterator end);
	};
	template<typename csv_traits_>
	class icsv :public icsv_base<csv_traits_> {
		istream_type& in;
	};
	template<typename csv_traits_>
	class ocsv :public ocsv_base<csv_traits_> {
		ostream_type& out;
	};
	template<typename csv_traits_>
	class iocsv :public icsv_base<csv_traits_>, public ocsv_base<csv_traits_> {
		iostream_type& io;
	public:
	};
}
#
#endif
