#ifndef INC_HMLIB_DXFONT_CPP
#define INC_HMLIB_DXFONT_CPP 100
#
#ifndef INC_HMLIB_DXFONT
#	include"dxFont.hpp"
#endif
namespace hmLib{
	namespace dx{
		cFont cFont::Default(16,6,"ÇlÇr ÉSÉVÉbÉN",false,Normal);
		cFont cFont::VDefault(16,6,"ÇlÇr ÉSÉVÉbÉN",true,Normal);
	}
}
#
#endif
