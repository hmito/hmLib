#ifndef HMLIB_DXLIB_DRIBER_INC
#define HMLIB_DXLIB_DRIBER_INC 100
#
#include<memory>
namespace hmLib{
	namespace dxlib{
		class driver_engine{

		};
		class driver{
		public:
			virtual std::unique_ptr<driver_engine> create_engine()=0;
		};
	}
}
#
#endif
