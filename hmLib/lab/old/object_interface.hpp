#ifndef HMLIB_OBJECTINTERFACE_INC
#define HMLIB_OBJECTINTERFACE_INC 1
#
/*===object_interface===
objectが持つ基本的関数群のインターフェースを提供する

object_interface:v0_01/130308 hmIto
	clone,copy,moveを追加
*/
namespace hmLib{
	namespace interfaces{
		//自身の値を相手にcopyするcopy関数
		template<typename my_type>
		struct copy_interface{
		protected:
			virtual void itf_copy(my_type& my_typeo_)const=0;
		public:
			void copy(my_type& my_typeo_)const{itf_copy(my_typeo_);}
		};
		//自身の値を相手にmoveするmove関数
		template<typename my_type>
		struct move_interface{
		protected:
			virtual void itf_move(my_type& my_typeo_)=0;
		public:
			void move(my_type& my_typeo_)const{itf_move(my_typeo_);}
		};
		//動的確保でコピーを作るclone関数 :::allocatorをどうするか問題が残っている
		template<typename my_type>
		struct clone_interface{
		protected:
			virtual my_type* itf_clone(void)const=0;
		public:
			my_type* clone(void)const{return itf_clone();}
		};
		//動的確保でコピーを作るclone関数 :::allocatorをどうするか問題が残っている
		template<typename my_type>
		struct destroy_interface{
		protected:
			virtual void itf_destroy(void)=0;
		public:
			void destroy(void){itf_destroy();}
		};
	}
}
#
#endif
