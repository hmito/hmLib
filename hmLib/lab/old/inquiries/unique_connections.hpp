#ifndef HMLIB_INQUIRIES_UNIQUECONNECTIONS_INC
#define HMLIB_INQUIRIES_UNIQUECONNECTIONS_INC 100
#
/*===inquiries::unique_connections===
inquiryクラスの接続を管理し、destructorでdiconnectを呼ぶ。

inquiries::unique_connections v1_00/130328 hmIto
	inquiryから分離
*/
#include<vector>
#ifndef HMLIB_EXCEPTION_INC
#	include<hmLib/exception.hpp>
#endif
#ifndef HMLIB_INQUIRIES_INQUIRYABSTRUCT_INC
#	include"inquiry_abstruct.hpp"
#endif
namespace hmLib{
	namespace inquiries{
		//connection管理用クラス
		class unique_connections{
		private:
			typedef unique_connections my_type;
		private:
			std::vector<connection> Connections;
		public:
			unique_connections(){}
			unique_connections(my_type&& My_){Connections.swap(My_.Connections);}
			const my_type& operator=(my_type&& My_){
				hmLib_assert(Connections.size()==0,"Connections already have a connection");
				Connections.swap(My_.Connections);
				return *this;
			}
			~unique_connections(){
				for(auto itr=std::begin(Connections);itr!=std::end(Connections);++itr)itr->disconnect();
			}
		private:
			unique_connections(const my_type&);
			const my_type& operator=(const my_type& My_);
		public:
			void operator()(connection&& Connection_){Connections.push_back(std::move(Connection_));}
			void disconnect(){
				for(auto itr=std::begin(Connections);itr!=std::end(Connections);++itr)itr->disconnect();
				Connections.clear();
			}
		};
	}
}
#
#endif
