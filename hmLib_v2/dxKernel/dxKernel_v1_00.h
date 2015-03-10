#ifndef INC_HMLIB_DXKERNEL
#define INC_HMLIB_DXKERNEL 100
#
#ifndef INC_HMLIB_HMTASK
#	include "hmLib_v2/hmTask.hpp"
#endif
#include "hmLib_v2/DxLib.h"
#include <list>
#define DX_KERNEL_TASK 0x250
#define DX_PRE_KERNEL_TASK 0x210
#define DX_POST_KERNEL_TASK 0x290
namespace hmLib{
	namespace dx{
		class cKernel{
		private:
			unsigned int Cnt;
			bool IsOutputLog;
			bool CanRunMultiDxLib;
			bool IsRunAlways;
			bool IsCalcLowPrecise;
		private:
			class cTask:public hm::cTask{
				friend cKernel;
			protected://hmTask
				//タスク初期処理
				virtual bool construct(){
					//ログ出力を無効にする
					SetOutApplicationLogValidFlag(static_cast<int>(Ptr->IsOutputLog));

					//二重起動を容認する
					SetDoubleStartValidFlag(static_cast<int>(Ptr->CanRunMultiDxLib));

					//Activeでなくても動作する
					SetAlwaysRunFlag(static_cast<int>(Ptr->IsRunAlways));

					//doubleをfloatレベルまで落として演算しない
					SetUseFPUPreserveFlag(static_cast<int>(Ptr->IsCalcLowPrecise));

					//DxLib起動
					if( DxLib_Init() == -1 )return true;

					//カウンタ初期化
					Ptr->Cnt=0;
					
					return false;
				}
				//タスク通常処理
				virtual bool task(){
					// Windows システムからくる情報を処理する
					if( ProcessMessage() != 0 )return true;

					//カウンタを進める
					++(Ptr->Cnt);

					return false;
				}
				//タスク終了処理
				virtual bool destruct(){
					//DxLib終了
					DxLib_End();

					return false;
				}
			private:
				cKernel* Ptr;
			public:
				cTask():hm::cTask(DX_KERNEL_TASK){return;}
			};
		public:
			cTask Task;
		public:
			cKernel()
				:IsRunAlways(true)
				,IsCalcLowPrecise(true)
				,CanRunMultiDxLib(true)
				,IsOutputLog(true){
				Task.Ptr=this;
				return;
			}
			unsigned int getCount()const{return Cnt;}
			cKernel& operator()(bool IsRunAlways_=true,bool IsCalcLowPrecise_=true,bool CanRunMultiDxLib_=true,bool IsOutputLog_=false){
				IsOutputLog=IsOutputLog_;
				CanRunMultiDxLib=CanRunMultiDxLib_;
				IsCalcLowPrecise=IsCalcLowPrecise_;
				IsOutputLog=IsOutputLog_;
				return *this;
			}
		};
		extern cKernel Kernel;
	}
}
#
#endif
