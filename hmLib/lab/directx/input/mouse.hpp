#ifndef HMLIB_DIRECTX_MOUSE_INC
#define HMLIB_DIRECTX_MOUSE_INC 100
#
#include<memory>
#include<hmLib/position.hpp>
namespace hmLib {
	namespace directx {
		class mouse_device_interface {
		public:
		};
		class mouse {
		private:
			std::unique_ptr<mouse_device_interface> pMouseDevice;
		public:
			pint cursol();
			bool is_push();
			bool is_click();
			bool is_right_push();
			bool is_right_click(bool push_=false);
			bool is_center_push();
			bool is_center_click(bool push_=false);
		}Mouse;
	}
}
#
#endif
