#ifndef INC_HMLIB_DXINPUTDEVICE
#define INC_HMLIB_DXINPUTDEVICE 100
#
#ifndef INC_HMLIB_DXKERNEL
#	include "hmLib_v2/dxKernel.hpp"
#endif
#ifndef INC_HMLIB_HMTASK
#	include "hmLib_v2/hmTask.hpp"
#endif
#ifndef INC_HMLIB_HMPOS
#	include "hmLib_v2/hmPos.hpp"
#endif
#include "hmLib_v2/DxLib.h"
#define DX_MOUSE_HMLIB_TASK DX_POST_KERNEL_TASK
#define DX_KEYS_TASK DX_POST_KERNEL_TASK
#define DX_JOYPAD_TASK DX_POST_KERNEL_TASK
namespace hmLib{
	namespace dx{
		class itfInputDevice{
		protected:
			static void changeKeySta(int& _sta,bool _onoff){
				if(_sta>0){
					if(_onoff)_sta=2;
					else _sta=-1;
				}else{
					if(_onoff)_sta=1;
					else _sta=0;
				}
			}
			static void changeTwoKeySta(int& _sta,int _sta1,int _sta2){
				if(_sta1==2 || _sta2==2)_sta=2;	//一方が2
				else if(_sta1*_sta2==-1)_sta=2;		//一方が1,一方が-1
				else if(_sta1==1 || _sta2==1)_sta=1;
				else if(_sta1==-1|| _sta2==-1)_sta=-1;
				else _sta=0;
			}
		};
		class cMouse:public itfInputDevice{
		private:
			Pint Pos;
			Pint PPos;
			Pint LPos;
			Pint CPos;
			Pint RPos;
			int LSta;
			int RSta;
			int CSta;
			int Wheel;
		private:
			class cTask:public hm::cTask{
				friend class cMouse;
			public:
				cTask():hm::cTask(DX_MOUSE_HMLIB_TASK){return;}
			protected://hm::cTask
				//タスク初期処理
				virtual bool construct(){
					Ptr->LSta=0;
					Ptr->RSta=0;
					Ptr->CSta=0;
					Ptr->Wheel=0;
					return false;
				}
				//タスク通常処理
				virtual bool task(){
					changeKeySta(Ptr->LSta,(GetMouseInput() & MOUSE_HMLIB_INPUT_LEFT)!=0);
					changeKeySta(Ptr->RSta,(GetMouseInput() & MOUSE_HMLIB_INPUT_RIGHT)!=0);
					changeKeySta(Ptr->CSta,(GetMouseInput() & MOUSE_HMLIB_INPUT_MIDDLE)!=0);
					Ptr->Wheel=GetMouseWheelRotVol();
					Ptr->PPos=Ptr->Pos;
					GetMousePoint(&(Ptr->Pos.x),&(Ptr->Pos.y));
					if(Ptr->LSta==1)(Ptr->LPos)=(Ptr->Pos);
					if(Ptr->RSta==1)(Ptr->RPos)=(Ptr->Pos);
					if(Ptr->CSta==1)(Ptr->CPos)=(Ptr->Pos);

					return false;
				}
				//タスク終了処理
				virtual bool destruct(){return false;}
			private:
				cMouse* Ptr;
			};
		public:
			cTask Task;
		public:
			cMouse()
				:Pos(0,0),PPos(0,0),LPos(0,0),RPos(0,0),CPos(0,0)
				,LSta(0),RSta(0),CSta(0),Wheel(0){
				Task.Ptr=this;
			}
			int getL(){return LSta;}
			int getR(){return RSta;}
			int getC(){return CSta;}
			int getWheel(){return Wheel;}
			Pint getPos(){return Pos;}
			Pint getLPos(){return LPos;}
			Pint getRPos(){return RPos;}
			Pint getCPos(){return CPos;}
		};
		class cKeyboad:public itfInputDevice{
		private:
			int KeySta[269];//256+Num[10]+Shift,Ctrl,Alt
		public:
			enum KeyName{
				Back=KEY_INPUT_BACK, 	// バックスペースキー
				Tab=KEY_INPUT_TAB, 	// タブキー
				Enter=KEY_INPUT_RETURN, 	// エンターキー
				LShift=KEY_INPUT_LSHIFT, 	// 左シフトキー
				RShift=KEY_INPUT_RSHIFT,	// 右シフトキー
				LCtrl=KEY_INPUT_LCONTROL,	// 左コントロールキー
				RCtrl=KEY_INPUT_RCONTROL, 	// 右コントロールキー
				Esc=KEY_INPUT_ESCAPE, 	// エスケープキー
				Space=KEY_INPUT_SPACE, 	// スペースキー
				PageUp=KEY_INPUT_PGUP,	// ＰａｇｅＵＰキー
				PageDown=KEY_INPUT_PGDN, 	// ＰａｇｅＤｏｗｎキー
				End=KEY_INPUT_END, 	// エンドキー
				Home=KEY_INPUT_HOME, 	// ホームキー
				Left=KEY_INPUT_LEFT, 	// 左キー
				Up=KEY_INPUT_UP, 	// 上キー
				Right=KEY_INPUT_RIGHT, 	// 右キー
				Down=KEY_INPUT_DOWN,	// 下キー
				Insert=KEY_INPUT_INSERT, 	// インサートキー
				Delete=KEY_INPUT_DELETE,	// デリートキー
				Minus=KEY_INPUT_MINUS, 	// －キー
				Yen=KEY_INPUT_YEN, 	// ￥キー
				Hat=KEY_INPUT_PREVTRACK, 	// ＾キー
				Period=KEY_INPUT_PERIOD, 	// ．キー
				Slash=KEY_INPUT_SLASH, 	// ／キー
				LAlt=KEY_INPUT_LALT, 	// 左ＡＬＴキー
				RAlt=KEY_INPUT_RALT, 	// 右ＡＬＴキー
				Scroll=KEY_INPUT_SCROLL, 	// ScrollLockキー
				SemiColon=KEY_INPUT_SEMICOLON, 	// ；キー
				Colon=KEY_INPUT_COLON, 	// ：キー
				LBracket=KEY_INPUT_LBRACKET, 	// ［キー
				RBracket=KEY_INPUT_RBRACKET, 	// ］キー
				At=KEY_INPUT_AT, 	// ＠キー
				BackSlash=KEY_INPUT_BACKSLASH, 	// ＼キー
				Comma=KEY_INPUT_COMMA, 	// ，キー
				CapLock=KEY_INPUT_CAPSLOCK, 	// CaspLockキー
				Pause=KEY_INPUT_PAUSE, 	// PauseBreakキー
				NumPad0=KEY_INPUT_NUMPAD0, 	// テンキー０
				NumPad1=KEY_INPUT_NUMPAD1, 	// テンキー１
				NumPad2=KEY_INPUT_NUMPAD2, 	// テンキー２
				NumPad3=KEY_INPUT_NUMPAD3, 	// テンキー３
				NumPad4=KEY_INPUT_NUMPAD4, 	// テンキー４
				NumPad5=KEY_INPUT_NUMPAD5, 	// テンキー５
				NumPad6=KEY_INPUT_NUMPAD6, 	// テンキー６
				NumPad7=KEY_INPUT_NUMPAD7, 	// テンキー７
				NumPad8=KEY_INPUT_NUMPAD8, 	// テンキー８
				NumPad9=KEY_INPUT_NUMPAD9, 	// テンキー９
				NumPadMulti=KEY_INPUT_MULTIPLY, 	// テンキー＊キー
				NumPadAdd=KEY_INPUT_ADD, 	// テンキー＋キー
				NumPadSub=KEY_INPUT_SUBTRACT, 	// テンキー－キー
				NumPadDot=KEY_INPUT_DECIMAL, 	// テンキー．キー
				NumPadDiv=KEY_INPUT_DIVIDE, 	// テンキー／キー
				NumPadEnter=KEY_INPUT_NUMPADENTER, 	// テンキーのエンターキー
				F1=KEY_INPUT_F1, 	// Ｆ１キー
				F2=KEY_INPUT_F2, 	// Ｆ２キー
				F3=KEY_INPUT_F3, 	// Ｆ３キー
				F4=KEY_INPUT_F4, 	// Ｆ４キー
				F5=KEY_INPUT_F5, 	// Ｆ５キー
				F6=KEY_INPUT_F6, 	// Ｆ６キー
				F7=KEY_INPUT_F7, 	// Ｆ７キー
				F8=KEY_INPUT_F8, 	// Ｆ８キー
				F9=KEY_INPUT_F9, 	// Ｆ９キー
				F10=KEY_INPUT_F10, 	// Ｆ１０キー
				F11=KEY_INPUT_F11, 	// Ｆ１１キー
				F12=KEY_INPUT_F12, 	// Ｆ１２キー
				A=KEY_INPUT_A ,	// Ａキー
				B=KEY_INPUT_B ,	// Ｂキー
				C=KEY_INPUT_C ,	// Ｃキー
				D=KEY_INPUT_D ,	// Ｄキー
				E=KEY_INPUT_E ,	// Ｅキー
				F=KEY_INPUT_F ,	// Ｆキー
				G=KEY_INPUT_G ,	// Ｇキー
				H=KEY_INPUT_H ,	// Ｈキー
				I=KEY_INPUT_I ,	// Ｉキー
				J=KEY_INPUT_J ,	// Ｊキー
				K=KEY_INPUT_K ,	// Ｋキー
				L=KEY_INPUT_L ,	// Ｌキー
				M=KEY_INPUT_M ,	// Ｍキー
				N=KEY_INPUT_N ,	// Ｎキー
				O=KEY_INPUT_O ,	// Ｏキー
				P=KEY_INPUT_P ,	// Ｐキー
				Q=KEY_INPUT_Q ,	// Ｑキー
				R=KEY_INPUT_R ,	// Ｒキー
				S=KEY_INPUT_S ,	// Ｓキー
				T=KEY_INPUT_T ,	// Ｔキー
				U=KEY_INPUT_U ,	// Ｕキー
				V=KEY_INPUT_V ,	// Ｖキー
				W=KEY_INPUT_W ,	// Ｗキー
				X=KEY_INPUT_X ,	// Ｘキー
				Y=KEY_INPUT_Y ,	// Ｙキー
				Z=KEY_INPUT_Z ,	// Ｚキー
				NumKey0=KEY_INPUT_0 ,	// ０キー
				NumKey1=KEY_INPUT_1 ,	// １キー
				NumKey2=KEY_INPUT_2 ,	// ２キー
				NumKey3=KEY_INPUT_3 ,	// ３キー
				NumKey4=KEY_INPUT_4 ,	// ４キー
				NumKey5=KEY_INPUT_5 ,	// ５キー
				NumKey6=KEY_INPUT_6 ,	// ６キー
				NumKey7=KEY_INPUT_7, 	// ７キー
				NumKey8=KEY_INPUT_8, 	// ８キー
				NumKey9=KEY_INPUT_9, 	// ９キー
				Num0=256,
				Num1=257,
				Num2=258,
				Num3=259,
				Num4=260,
				Num5=261,
				Num6=262,
				Num7=263,
				Num8=264,
				Num9=265,
				Shift=266,			//両Shift
				Ctrl=267,			//両CTRL
				Alt=268,			//両ALT
			};

		private:
			class cTask:public hm::cTask{
				friend class cKeyboad;
			public:
				cTask():hm::cTask(DX_MOUSE_HMLIB_TASK){return;}
			protected://hm::cTask
				//タスク初期処理
				virtual bool construct(){
					for(int i=0;i<269;++i)Ptr->KeySta[i]=0;
					return false;
				}
				//タスク通常処理
				virtual bool task(){
					char buf[256];

					GetHitKeyStateAll(buf);

					for(int i=0;i<256;++i)changeKeySta(Ptr->KeySta[i],buf[i]!=0);
					changeTwoKeySta(Ptr->KeySta[Num0],Ptr->KeySta[NumKey0],Ptr->KeySta[NumPad0]);
					changeTwoKeySta(Ptr->KeySta[Num1],Ptr->KeySta[NumKey1],Ptr->KeySta[NumPad1]);
					changeTwoKeySta(Ptr->KeySta[Num2],Ptr->KeySta[NumKey2],Ptr->KeySta[NumPad2]);
					changeTwoKeySta(Ptr->KeySta[Num3],Ptr->KeySta[NumKey3],Ptr->KeySta[NumPad3]);
					changeTwoKeySta(Ptr->KeySta[Num4],Ptr->KeySta[NumKey4],Ptr->KeySta[NumPad4]);
					changeTwoKeySta(Ptr->KeySta[Num5],Ptr->KeySta[NumKey5],Ptr->KeySta[NumPad5]);
					changeTwoKeySta(Ptr->KeySta[Num6],Ptr->KeySta[NumKey6],Ptr->KeySta[NumPad6]);
					changeTwoKeySta(Ptr->KeySta[Num7],Ptr->KeySta[NumKey7],Ptr->KeySta[NumPad7]);
					changeTwoKeySta(Ptr->KeySta[Num8],Ptr->KeySta[NumKey8],Ptr->KeySta[NumPad8]);
					changeTwoKeySta(Ptr->KeySta[Num9],Ptr->KeySta[NumKey9],Ptr->KeySta[NumPad9]);
					changeTwoKeySta(Ptr->KeySta[Shift],Ptr->KeySta[LShift],Ptr->KeySta[RShift]);
					changeTwoKeySta(Ptr->KeySta[Ctrl],Ptr->KeySta[LCtrl],Ptr->KeySta[RCtrl]);
					changeTwoKeySta(Ptr->KeySta[Alt],Ptr->KeySta[LAlt],Ptr->KeySta[RAlt]);

					return false;
				}
				//タスク終了処理
				virtual bool destruct(){return false;}
			private:
				cKeyboad* Ptr;
			};
		public:
			cTask Task;
		public:
			cKeyboad(){Task.Ptr=this;}
			int get(KeyName Name_)const{return KeySta[Name_];}
			int getUp()const{return KeySta[Up];}
			int getDw()const{return KeySta[Down];}
			int getLf()const{return KeySta[Left];}
			int getRi()const{return KeySta[Right];}
		};
		//JoyPad入力
		class cJoypadCtrl:public itfInputDevice{
		private:
			int ButSta[16];
			int PButSta[16];
		public:
			void connect(){ReSetupJoypad();}
			int getNum(){return GetJoypadNum();}
		private:
			class cTask:public hm::cTask{
				friend class cJoypadCtrl;
			protected://hm::cTask
				//タスク初期処理
				virtual bool construct(){
					for(int i=0;i<16;++i){
						Ptr->PButSta[i]=0;
						Ptr->ButSta[i]=0;
					}
					return 0;
					return false;
				}
				//タスク通常処理
				virtual bool task(){
					for(int i=0;i<Ptr->getNum();++i){
						Ptr->PButSta[i]=Ptr->ButSta[i];
						Ptr->ButSta[i]=GetJoypadInputState(i);
					}
					return false;
				}
				//タスク終了処理
				virtual bool destruct(){return false;}
			private:
				cJoypadCtrl* Ptr;
			};
		public:
			cTask Task;
		public:
			class cJoypad{
			private:
				cJoypadCtrl* Ptr;
				int ch;
			public:
				cJoypad(cJoypadCtrl* Ptr_,int ch_):Ptr(Ptr_),ch(ch_){return;}
				~cJoypad(){stopVibration();}
				int get(int _no){
					if((Ptr->ButSta[ch]&(1<<_no))!=0 && (Ptr->PButSta[ch]&(1<<_no))!=0)return 2;
					else if((Ptr->ButSta[ch]&(1<<_no))!=0)return 1;
					else if((Ptr->PButSta[ch]&(1<<_no))!=0)return -1;
					else return 0;
				}
				//右下が+、左上が-
				Pint getLStick(){
					Pint tmp;
					GetJoypadAnalogInput(&(tmp.x),&(tmp.y),ch);
					return tmp;
				}
				//右下が+、左上が-
				Pint getRStick(){
					Pint tmp;
					GetJoypadAnalogInputRight(&(tmp.x),&(tmp.y),ch);
					return tmp;
				}
				void startVibration(int Power,int mSec=0){StartJoypadVibration(ch,Power,mSec);}
				void stopVibration(){StopJoypadVibration(ch);}
			};
		};
		extern cMouse Mouse;
		extern cKeyboad Keyboad;
		extern cJoypadCtrl JoypadCtrl;
	}
}
#
#endif
