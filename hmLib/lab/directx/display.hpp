#ifndef HMLIB_DXLIB_DISPLAY_INC
#define HMLIB_DXLIB_DISPLAY_INC 100
#
#include<string>
#ifndef HMLIB_POSITION_INC
#	include<hmLib/position.hpp>
#endif
namespace hmLib{
	namespace dxlib{
		class display{
		private:
			class cTask :public hm::cTask{
				friend class cDisplay;
			private:
				//描画先指定用サブタスク
				class :public hm::cTask{
				private:
					int Handle;
				protected://hm::cTask
					//タスク初期処理
					virtual bool construct(){
						// 描画先画面を裏画面にする
						SetDrawScreen(dx::cScreen::DefaultBackScreen.getHandle());

					}
				public:
					void setHandle(int Handle_){ Handle=Handle_; }
				};
			protected://hm::cTask
				//タスク初期処理
				virtual bool construct(){
					//ウィンドウアイコン変更
					SetWindowIconID(1);

					return false;
				}
				//タスク通常処理
				virtual bool task(){
					// 裏画面の内容を表画面に反映させる
					ScreenFlip();
					//消去先をMainDisplayへ変更
					dx::cScreen::scoped_active TmpScr(dx::cScreen::DefaultBackScreen);
					//画面描画内容消去
					ClearDrawScreen();

					return false;
				}
				//タスク終了処理
				virtual bool destruct(){ return false; }
			public:
				cTask() :hm::cTask(DX_DISPLAY_TASK){ return; }
			};
		private:
			pint Size;
			int ClrBit;
			bool IsWindowMode;
			bool IsWindowExtendMode;
			std::string Name;
		public:
			display(bool WindowMode_=false, const std::string& Name_="", pint DisplaySize_=pint(800, 600), int DisplayClrBit_=32)
				:Size(pint(800, 600))
				, ClrBit(32)
				, CanWindowExtend(false){

				dxlib::screen::DefaultBackScreen.getHandle();

				//解像度を変更する
				SetGraphMode(DisplaySize_.x, DisplaySize_.y, DisplayClrBit_);

				//ウィンドウモード
				ChangeWindowMode(IsWindowMode);

				//ウィンドウサイズ変更可能モード
				SetWindowSizeChangeEnableFlag(IsWindowExtendMode);

				//ウィンドウ名を変更
				SetMainWindowText(Name.c_str());

				//ウィンドウアイコン変更
				SetWindowIconID(1);

			}
			void setDisplay(pint Size_, int ClrBit_){
				if(Size==Size_ && ClrBit_==ClrBit)return;
				//描画画面を設定
				SetGraphMode(Size_.x, Size_.y, ClrBit_);
				Size=Size_;
				ClrBit=ClrBit_;
			}
			pint getDisplaySize()const{ return Size; }
			int getDisplayClrBit()const{ return ClrBit; }
			bool setName(const std::string& Name_){
				//ウィンドウ名を変更
				SetMainWindowText(Name_.c_str());
				Name=Name_;
				return false;
			}
			std::string getName()const{ return Name; }
			void setWindowExtendMode(bool IsWindowExtendMode_){
				if(IsWindowExtendMode==IsWindowExtendMode_)return;
				SetWindowSizeChangeEnableFlag(IsWindowExtendMode_);
				IsWindowExtendMode=IsWindowExtendMode_;
			}
			bool getWindowExtendMode()const{ return IsWindowExtendMode; }
			void setWindowMode(bool IsWindowMode_){
				if(IsWindowMode==IsWindowMode_)return;
				ChangeWindowMode(static_cast<int>(IsWindowMode_));
				IsWindowMode=IsWindowMode_;
			}
			bool getWindowMode()const{ return IsWindowMode; }
		};
		extern  cDisplay Display;
	}
}
#
#endif
