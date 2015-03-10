#ifndef INC_HMLIB_DXGRAPHICS_CPP
#define INC_HMLIB_DXGRAPHICS_CPP 100
#
#ifndef INC_HMLIB_DXGRAPHICS
#	include	"dxGraphics_v1_00.h"
#endif
namespace hmLib{
	namespace dx{
		const int cGraphics::CheckArea=0x01;
		const int cGraphics::CheckBright=0x02;
		const int cGraphics::CheckBlend=0x04;
		const int cGraphics::CheckStatus=0x08;
		const int cGraphics::CheckRecord=0x10;
	}
}
#
#endif
