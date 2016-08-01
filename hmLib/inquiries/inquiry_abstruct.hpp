#ifndef HMLIB_INQUIRIES_INQUIRYABSTRUCT_INC
#define HMLIB_INQUIRIES_INQUIRYABSTRUCT_INC 104
#
/*
===inquiries::inquiry_abstruct===
inquiry機能の基底クラス
connectionについてもここで管理している
v1_04/140503 hmIto
	inquiry系列をmoveすると、disconnectがconnectionからできなくなる問題を修正
	shared_ptrを使って、flagを管理するように変更
v1_03/130421 hmIto
	connected/unconnected_exception追加
v1_02/130410 hmIto
	ムーブ代入演算子のswap関数の引数をptrではなくnullptrとしていた問題を修正
v1_01/130329 hmIto
	ムーブコンストラクタ/代入演算子周辺の致命的なバグを修正
v1_00/130328 hmIto
	inquiryから分離
*/
#ifndef HMLIB_EXCEPTIONS_INC
#	include"../exceptions.hpp"
#endif
#ifndef HMLIB_MEMORY_CLONECOUNTER_INC
#	include"../lab/memory/clone_counter.hpp"
#endif
namespace hmLib{
	namespace inquiries{
		struct exception_identifier {};
		typedef exceptions::exception_pattern<exception_identifier> inquiry_exception;
		typedef exceptions::io::not_opened<inquiry_exception> unconnected_exception;
		typedef exceptions::io::opened<inquiry_exception> connected_exception;

		//connectionシステムを提供する機能
		class inquiry_abstruct{
			typedef std::shared_ptr<bool> shared_connecting_flag;
		public:
			class connection{
			private:
				shared_connecting_flag TergetFlag;
			private:
				connection(const connection& My_);
				const connection& operator=(const connection& My_);
			public:
				connection() :TergetFlag() {}
				explicit connection(const shared_connecting_flag& ConnectingFlag_) :TergetFlag(ConnectingFlag_) {}
				connection(connection&& My_) :TergetFlag() {
					std::swap(TergetFlag, My_.TergetFlag);
				}
				const connection& operator=(connection&& My_){
					hmLib_assert(static_cast<bool>(TergetFlag), connected_exception, "connection already have a pointer");
					std::swap(TergetFlag, My_.TergetFlag);
					return *this;
				}
				~connection(){}
				bool is_connect()const { return static_cast<bool>(TergetFlag) && *TergetFlag; }
				void disconnect() { if(static_cast<bool>(TergetFlag))*TergetFlag=false; }
				friend class inquiry_abstruct;
			};
		private:
			shared_connecting_flag ConnectFlag;
		protected:
			connection create_connection(void){
				hmLib_assert(!is_connect(),connected_exception,"inquiry_abstruct have already connected");
				//参照してる人がまだいれば、新たに作成
				if(!ConnectFlag || !ConnectFlag.unique())ConnectFlag.reset(new bool(true));
				return connection(ConnectFlag);
			}
		public:
			void disconnect(void){
				hmLib_assert(is_connect(),unconnected_exception,"inquiry_abstruct have no connection");
				*ConnectFlag=false;
			}
			bool is_connect(void)const { 
				if(!ConnectFlag)return false;
				return *ConnectFlag; 
			}
		public:
			inquiry_abstruct() :ConnectFlag() {}
			//コピー禁止
			inquiry_abstruct(const inquiry_abstruct& My_)=delete;
			const inquiry_abstruct& operator=(const inquiry_abstruct& My_)=delete;
			//ムーブ許可
			inquiry_abstruct(inquiry_abstruct&& My_) :ConnectFlag(std::move(My_.ConnectFlag)) {}
			const inquiry_abstruct& operator=(inquiry_abstruct&& My_) {
				hmLib_assert(!is_connect(),connected_exception,"inquiry_abstruct have already connected");
				std::swap(ConnectFlag, My_.ConnectFlag);
				return *this;
			}
			virtual ~inquiry_abstruct(){if(is_connect())disconnect();}
		};
		typedef inquiry_abstruct::connection connection;
	}
}
#
#endif
