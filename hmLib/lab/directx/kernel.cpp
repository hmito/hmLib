#ifndef HMLIB_DXLIB_KERNEL_CPP_INC
#define HMLIB_DXLIB_KERNEL_CPP_INC 100
#
#ifndef HMLIB_DXLIB_KERNEL_INC
#	include"kernel.hpp"
#endif
#ifndef HMLIB_DXLIB_EXCEPTIONS_INC
#	include"exceptions.hpp"
#endif
#include<DxLib.h>
namespace hmLib{
	namespace dxlib{
		kernel::kernel(bool CanRunMultiDxLib_, bool IsRunAlways_, bool IsOutputLog_, bool IsCalcLowPrecise_)
			:CanRunMultiDxLib(CanRunMultiDxLib_)
			, IsRunAlways(IsRunAlways_)
			, IsOutputLog(IsOutputLog_)
			, IsCalcLowPrecise(IsCalcLowPrecise_){

			if(exist())hmLib_throw(exceptions::dxlib_exception, "DxLib have already been initialized.");

			//二重起動を容認する
			SetDoubleStartValidFlag(static_cast<int>(CanRunMultiDxLib));

			//Activeでなくても動作する
			SetAlwaysRunFlag(static_cast<int>(IsRunAlways));

			//ログ出力を無効にする
			SetOutApplicationLogValidFlag(static_cast<int>(IsOutputLog));

			//doubleをfloatレベルまで落として演算しない
			SetUseFPUPreserveFlag(static_cast<int>(IsCalcLowPrecise));

			//DxLib起動
			if(DxLib_Init() == -1)hmLib_throw(exceptions::dxlib_exception, "Failed to initialize DxLib.");
		}
		kernel::~kernel(){
			//DxLib終了
			DxLib_End();
		}
		bool kernel::operator()(void){
			// Windows システムからくる情報を処理する
			return (ProcessMessage() != 0);
		}
		bool kernel::exist(){ return DxLib_IsInit(); }
	}
}
#
#endif
