#ifndef HMLIB_SIGNALS_UNIQUECONNECTIONS_INC
#define HMLIB_SIGNALS_UNIQUECONNECTIONS_INC 100
#
/*===signal_unique_connections===
boost::signals2::signalのconnectionを管理し、destructorでdisconnectするクラス

signals::unique_connections v1_00/130328 hmIto
	signals名前空間ファイルに移動
signal_unique_connections v1_00/130328 hmIto
	作成
*/
#include<boost/signals2/connection.hpp>
#ifndef HMLIB_EXCEPTION_INC
#	include<hmLib_v3_04/exception.hpp>
#endif
namespace hmLib{
	namespace signals{
		//connection管理用クラス
		class unique_connections{
		private:
			typedef unique_connections my_type;
			typedef boost::signals2::connection connection;
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
