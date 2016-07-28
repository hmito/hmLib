#ifndef HMLIB_VCOM_VIRTUALCOMABSTRUCT_INC
#define HMLIB_VCOM_VIRTUALCOMABSTRUCT_INC 100
#
/*===virtual_com===
スレッドを使って仮想的にcom通信を再現するクラス群
*/
#include<deque>
#include"virtual_comgate_abstruct.hpp"
namespace hmLib{
	namespace vcom{
		//仮想com
		class virtual_com_abstruct{
		public:
			virtual ~virtual_com_abstruct(){}
		public: //virtual_com_abstruct
			//virtual_comgateを接続する
			virtual void connect(virtual_comgate_abstruct* ptr)=0;
			//virtual_comgateを切断する
			virtual void disconnect(virtual_comgate_abstruct* ptr)=0;
			//virtual_comgateが接続されているかを確認する
			virtual bool is_connect(virtual_comgate_abstruct* ptr=nullptr)=0;
		};
	}
}
#
#endif
