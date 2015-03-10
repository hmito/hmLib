#ifndef HMLIB_INQUIRIES_INQUIRYABSTRUCT_INC
#define HMLIB_INQUIRIES_INQUIRYABSTRUCT_INC 103
#
/*===inquiries::inquiry_abstruct===
inquiry機能の基底クラス
connectionについてもここで管理している

inquiries::inquiry_abstruct v1_03/130421 hmIto
	connected/unconnected_exception追加
inquiries::inquiry_abstruct v1_02/130410 hmIto
	ムーブ代入演算子のswap関数の引数をptrではなくnullptrとしていた問題を修正
inquiries::inquiry_abstruct v1_01/130329 hmIto
	ムーブコンストラクタ/代入演算子周辺の致命的なバグを修正
inquiries::inquiry_abstruct v1_00/130328 hmIto
	inquiryから分離
*/
#ifndef HMLIB_EXCEPTIONS_INC
#	include<hmLib_v3_05/exceptions.hpp>
#endif
namespace hmLib{
	namespace inquiries{
		class connected_exception:public exceptions::io_opened{
			typedef exception excp_type;
			typedef exceptions::io_opened base_type;
		public:
			explicit connected_exception(const std::string& Message_):excp_type(Message_.c_str()),base_type(Message_.c_str()){}
			explicit connected_exception(const char* Message_):excp_type(Message_),base_type(Message_){}
		};
		class unconnected_exception:public exceptions::io_not_opened{
			typedef exception excp_type;
			typedef exceptions::io_not_opened base_type;
		public:
			explicit unconnected_exception(const std::string& Message_):excp_type(Message_.c_str()),base_type(Message_.c_str()){}
			explicit unconnected_exception(const char* Message_):excp_type(Message_),base_type(Message_){}
		};
		//connectionシステムを提供する機能
		class inquiry_abstruct{
		public:
			class connection{
			private:
				inquiry_abstruct* ptr;
			private:
				connection(const connection& My_);
				const connection& operator=(const connection& My_);
			public:
				connection():ptr(nullptr){}
				connection(inquiry_abstruct* ptr_):ptr(ptr_){}
				connection(connection&& My_):ptr(nullptr){
					std::swap(ptr,My_.ptr);
					ptr->ptr=this;
				}
				const connection& operator=(connection&& My_){
					hmLib_assert(ptr==nullptr,connected_exception,"connection already have a pointer");
					std::swap(ptr,My_.ptr);
					ptr->ptr=this;
					return *this;
				}
				~connection(){if(ptr)ptr->ptr=nullptr;}
				bool is_connect()const{return ptr!=nullptr;}
				void disconnect(){if(ptr)ptr->disconnect();}
				friend class inquiry_abstruct;
			};
		private:
			bool IsConnect;
			connection* ptr;
		protected:
			connection create_connection(void){
				hmLib_assert(!is_connect(),connected_exception,"inquiry_abstruct have already connected");
				IsConnect=true;
				return connection(this);
			}
		public:
			virtual void disconnect(void){
				hmLib_assert(is_connect(),unconnected_exception,"inquiry_abstruct have no connection");
				if(ptr)ptr->ptr=nullptr;
				ptr=nullptr;
				IsConnect=false;
			}
			bool is_connect(void)const{return IsConnect;}
		public:
			inquiry_abstruct():ptr(nullptr),IsConnect(false){}
			inquiry_abstruct(inquiry_abstruct&& My_):ptr(My_.ptr),IsConnect(My_.IsConnect){
				My_.ptr=nullptr;
				My_.IsConnect=false;
			}
			const inquiry_abstruct& operator=(inquiry_abstruct&& My_){
				hmLib_assert(!is_connect(),connected_exception,"inquiry_abstruct have already connected");
				std::swap(IsConnect,My_.IsConnect);
				std::swap(ptr,My_.ptr);
				if(ptr)ptr->ptr=this;
				return *this;
			}
			virtual ~inquiry_abstruct(){if(is_connect())disconnect();}
		private:
			inquiry_abstruct(const inquiry_abstruct& My_);
			const inquiry_abstruct& operator=(const inquiry_abstruct& My_);
		};
		typedef inquiry_abstruct::connection connection;
	}
}
#
#endif
