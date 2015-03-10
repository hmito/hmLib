#ifndef INC_HMLIB_DXINPUT_CPP
#define INC_HMLIB_DXINPUT_CPP 100
#
#ifndef INC_HMLIB_DXINPUT
#	include"dxInput.hpp"
#endif
namespace hmLib{
	namespace dx{
		hm::cPublicTask cInput::Task(DX_INPUT_TASK);
		int cInput::ActiveHandle=0;
		IMEINPUTDATA* cInput::getIMEPtr(){
			return GetIMEInputData();
		}
		cInputDrawFn InputDrawFn(3,3);
	}
}
#
#endif
