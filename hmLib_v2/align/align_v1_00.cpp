#ifndef INC_HMLIB_ALIGN_CPP
#define INC_HMLIB_ALIGN_CPP 100
#ifndef INC_HMLIB_ALIGN
#	include "hmLib_v2/align.hpp"
#endif
namespace hmLib{
	namespace align{
		namespace quad{
			const align_quad center(0),left(1),right(2),top(4),bottom(8),all(left|right|top|bottom);
		}
		namespace octa{
			const align_octa center(0),left(1),right(2),top(4),bottom(8),left_top(16),right_top(32),left_bottom(64),right_bottom(128),all(left|right|top|bottom|left_top|right_top|left_bottom|right_bottom);
		}
	}
}
#endif
