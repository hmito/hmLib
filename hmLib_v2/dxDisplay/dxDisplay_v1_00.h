#ifndef INC_HMLIB_DXDISPLAY
#define INC_HMLIB_DXDISPLAY 100
#
#ifndef INC_HMLIB_HMPOS
#	include "hmLib_v2/hmPos.hpp"
#endif
#ifndef INC_HMLIB_HMTASK
#	include "hmLib_v2/hmTask.hpp"
#endif
#ifndef INC_HMLIB_DXGRAPH
#	include "hmLib_v2/dxImage.hpp"
#endif
#include "hmLib_v2/DxLib.h"
#define DX_DISPLAY_TASK 0x240
#define DX_DISPLAY_SUBTASK 0x260
namespace hmLib{
	namespace dx{
		class cDisplay{
		private:
			class cTask:public hm::cTask{
				friend class cDisplay;
			private:
				//描画先指定用サブタスク
				class:public hm::cTask{
				private:
					int Handle;
				protected://hm::cTask
					//タスク初期処理
					virtual bool construct(){
						// 描画先画面を裏画面にする
						SetDrawScreen(Handle);
						//終了を要求
						exit();

						return false;
					}
					//タスク通常処理
					virtual bool work(){return false;}
					//タスク終了処理
					virtual bool destruct(){return false;}
				public:
					void setHandle(int Handle_){Handle=Handle_;}
				}SubTask;
			private:
				const cDisplay* Ptr;
			protected://hm::cTask
				//タスク初期処理
				virtual bool construct(){
					//サブタスクに描画先を教える
					SubTask.setHandle(dx::cScreen::DefaultBackScreen.getHandle());
					//描画先指定サブタスク
					hm::cTask::createSister(SubTask,DX_DISPLAY_SUBTASK);
					//ウィンドウアイコン変更
					SetWindowIconID(1) ;

					return false;
				}
				//タスク通常処理
				virtual bool task(){
					// 裏画面の内容を表画面に反映させる
					ScreenFlip();
					//消去先をMainDisplayへ変更
					dx::cScreen::scoped_active TmpScr(dx::cScreen::DefaultBackScreen);
					//画面描画内容消去
					ClearDrawScreen() ;

					return false;
				}
				//タスク終了処理
				virtual bool destruct(){return false;}
			public:
				cTask():hm::cTask(DX_DISPLAY_TASK){return;}
			};
		private:
			Pint Size;
			int ClrBit;
			bool IsWindowMode;
			std::string Name;
			bool CanWindowExtend;
		public:
			cTask Task;
		public:
			cDisplay()
				:Size(Pint(800,600))
				,ClrBit(32)
				,CanWindowExtend(false){
				Name="";
				CanWindowExtend=false;
			}
			cDisplay& operator()(bool WindowMode_=false,const std::string& Name_="",Pint DisplaySize_=Pint(800,600),int DisplayClrBit_=32){
				setSize(DisplaySize_,DisplayClrBit_);
				setWindowMode(WindowMode_);
				setName(Name_);
				return *this;
			}
			bool setSize(Pint Size_,int ClrBit_){
				//描画画面を設定
				SetGraphMode(Size_.x,Size_.y,ClrBit_);
				Size=Size_;
				ClrBit=ClrBit_;
				return false;
			}
			Pint getSize()const{return Size;}
			int getClrBit()const{return ClrBit;}
			bool setName(const std::string& Name_){
				//ウィンドウ名を変更
				SetMainWindowText(Name_.c_str());
				Name=Name_;
				return false;
			}
			std::string getName()const{return Name;}
			bool enableWindowExtend(){
				SetWindowSizeChangeEnableFlag(true);
				CanWindowExtend=true;
				return false;
			}
			bool disableWindowExtend(){
				SetWindowSizeChangeEnableFlag(false);
				CanWindowExtend=false;
			}
			bool canWindowExtend()const{return CanWindowExtend;}
			bool setWindowMode(bool IsWindowMode_){
				//ウィンドウモード
				ChangeWindowMode(static_cast<int>(IsWindowMode));
				IsWindowMode=IsWindowMode_;
				return false;
			}
			bool isWindowMode()const{return IsWindowMode;}
		};
		extern  cDisplay Display;
	}
}
#
#endif
