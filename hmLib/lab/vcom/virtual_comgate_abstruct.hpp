#ifndef HMLIB_VCOM_VIRTUALCOMGATEABSTRUCT_INC
#define HMLIB_VCOM_VIRTUALCOMGATEABSTRUCT_INC 100
#
/*===virtual_comgate_absturct===
スレッドを使って仮想的にcom通信を再現するクラス群
*/
#include<hmLib/gate.hpp>
namespace hmLib{
	namespace vcom{
		//仮想com用gate
		class virtual_comgate_abstruct:public gate{
		public:
			virtual ~virtual_comgate_abstruct(){}
		public: //virtaul_comgate_abstruct
			//comとして送信するデータを取得する
			virtual size_type com_send(char* ptr_, size_type size_)=0;
			//comとして受信させるデータを与える
			virtual size_type com_recv(const char* ptr_, size_type size_)=0;
		};
	}
}
#
#endif
