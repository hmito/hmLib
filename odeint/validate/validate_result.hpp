#ifndef HMLIB_ODEINT_VALIDATE_VALIDATERESULT_INC
#define HMLIB_ODEINT_VALIDATE_VALIDATERESULT_INC 100
#
namespace hmLib {
	namespace odeint {
		enum struct validate_result{
			none = 0,
			assigned = 1, 	//assigned but size is same
			resized = 2		//resized
		};
	}
}
#
#endif
