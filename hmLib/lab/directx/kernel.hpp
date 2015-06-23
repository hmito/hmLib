#ifndef HMLIB_DXLIB_KERNEL_INC
#define HMLIB_DXLIB_KERNEL_INC 100
#
namespace hmLib{
	namespace dxlib{
		class kernel{
		private:
			bool CanRunMultiDxLib;
			bool IsRunAlways;
			bool IsOutputLog;
			bool IsCalcLowPrecise;
		public:
			kernel(bool CanRunMultiDxLib_, bool IsRunAlways_, bool IsOutputLog_, bool IsCalcLowPrecise_);
			~kernel();
			bool operator()(void);
			static bool exist();
		};
	}
}
#
#endif
