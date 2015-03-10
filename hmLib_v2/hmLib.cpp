#ifdef INC_HMLIB
#	ifdef INC_HMLIB_CSVSTREAM
#		ifndef INC_HMLIB_CSVSTREAM_CPP
#			if INC_HMLIB_CSVSTREAM==100
#				include "hmLib_v2/csvstream/csvstream_v1_00.cpp"
#			endif
#		endif
#	endif
#	ifdef INC_HMLIB_RANDOM
#		ifndef INC_HMLIB_RANDOM_CPP
#			if INC_HMLIB_RANDOM==100
#				include "hmLib_v2/random/random_v1_00.cpp"
#			endif
#		endif
#	endif
#	ifdef INC_HMLIB_ALIGN
#		ifndef INC_HMLIB_ALIGN_CPP
#			if INC_HMLIB_ALIGN==100
#				include "hmLib_v2/align/align_v1_00.cpp"
#			endif
#		endif
#	endif
#	ifdef INC_HMLIB_DEBUGSTREAM
#		ifndef INC_HMLIB_DEBUGSTREAM_CPP
#			if INC_HMLIB_DEBUGSTREAM==100
#				include "hmLib_v2/debugstream/debugstream_v1_00.cpp"
#			endif
#		endif
#	endif
#	ifdef INC_HMLIB_HMBASIC
#		ifndef INC_HMLIB_HMBASIC_CPP
#			if INC_HMLIB_HMBASIC==106
#				include "hmLib_v2/hmBasic/hmBasic_v1_06.cpp"
#			elif INC_HMLIB_HMBASIC==105
#				include "hmLib_v2/hmBasic/hmBasic_v1_05.cpp"
#			elif INC_HMLIB_HMBASIC==104
#				include "hmLib_v2/hmBasic/hmBasic_v1_04.cpp"
#			elif INC_HMLIB_HMBASIC==103
#				include "hmLib_v2/hmBasic/hmBasic_v1_03.cpp"
#			elif INC_HMLIB_HMBASIC==102
#				include "hmLib_v2/hmBasic/hmBasic_v1_02.cpp"
#			elif INC_HMLIB_HMBASIC==100
#				include "hmLib_v2/hmBasic/hmBasic_v1_00.cpp"
#			endif
#		endif
#	endif
#	ifdef INC_HMLIB_DXBASIC
#		ifndef INC_HMLIB_DXBASIC_CPP
#			if INC_HMLIB_DXBASIC==205
#				include "hmLib_v2/dxBasic/dxBasic_v2_05.cpp"
#			elif INC_HMLIB_DXBASIC==204
#				include "hmLib_v2/dxBasic/dxBasic_v2_04.cpp"
#			elif INC_HMLIB_DXBASIC==203
#				include "hmLib_v2/dxBasic/dxBasic_v2_03.cpp"
#			elif INC_HMLIB_DXBASIC==201
#				include "hmLib_v2/dxBasic/dxBasic_v2_01.cpp"
#			elif INC_HMLIB_DXBASIC==200
#				include "hmLib_v2/dxBasic/dxBasic_v2_00.cpp"
#			endif
#		endif
#	endif
#	ifdef INC_HMLIB_DXCOLORSET
#		ifndef INC_HMLIB_DXCOLORSET_CPP
#			if INC_HMLIB_DXCOLORSET==101
#				include "hmLib_v2/dxColorSet/dxColorSet_v1_01.cpp"
#			endif
#		endif
#	endif
#	ifdef INC_HMLIB_HMALIGN
#		ifndef INC_HMLIB_HMALIGN_CPP
#			if INC_HMLIB_HMALIGN==101
#				include "hmLib_v2/hmAlign/hmAlign_v1_01.cpp" 
#			endif
#		endif
#	endif
#	ifdef INC_HMLIB_HMDIR
#		ifndef INC_HMLIB_HMDIR_CPP
#			if INC_HMLIB_HMDIR==102
#				include "hmLib_v2/hmDir/hmDir_v1_02.cpp"
#			elif INC_HMLIB_HMDIR==101
#				include "hmLib_v2/hmDir/hmDir_v1_01.cpp"
#			elif INC_HMLIB_HMDIR==100
#				include "hmLib_v2/hmDir/hmDir_v1_00.cpp"
#			endif
#		endif
#	endif
#	ifdef INC_HMLIB_HMDEBUG
#		ifndef INC_HMLIB_HMDEBUG_CPP
#			if INC_HMLIB_HMDEBUG==101
#				include "hmLib_v2/hmDebug/hmDebug_v1_01.cpp"
#			elif INC_HMLIB_HMDEBUG==100
#				include "hmLib_v2/hmDebug/hmDebug_v1_00.cpp"
#			endif
#		endif
#	endif
#	ifdef INC_HMLIB_HMRAND
#		ifndef INC_HMLIB_HMRAND_CPP
#			if INC_HMLIB_HMRAND==205
#				include "hmLib_v2/hmRand/hmRand_v2_05.cpp"
#			elif INC_HMLIB_HMRAND==204
#				include "hmLib_v2/hmRand/hmRand_v2_04.cpp"
#			elif INC_HMLIB_HMRAND==203
#				include "hmLib_v2/hmRand/hmRand_v2_03.cpp"
#			elif INC_HMLIB_HMRAND==202
#				include "hmLib_v2/hmRand/hmRand_v2_02.cpp"
#			elif INC_HMLIB_HMRAND==201
#				include "hmLib_v2/hmRand/hmRand_v2_01.cpp"
#			elif INC_HMLIB_HMRAND==200
#				include "hmLib_v2/hmRand/hmRand_v2_00.cpp"
#			elif INC_HMLIB_HMRAND==100
#				include "hmLib_v2/hmRand/hmRand_v1_00.cpp"
#			endif
#		endif
#	endif
#	ifdef INC_HMLIB_HMRSGATE
#		ifndef INC_HMLIB_HMRSGATE_CPP
#			if INC_HMLIB_HMRSGATE==102
#				include "hmLib_v2/hmRSGate/hmRSGate_v1_02.cpp"
#			elif INC_HMLIB_HMRSGATE==101
#				include "hmLib_v2/hmRSGate/winRS/winrs.cpp"
#			endif
#		endif
#	endif
#	ifdef INC_HMLIB_COMGATE
#		ifndef INC_HMLIB_COMGATE_CPP
#			if INC_HMLIB_COMGATE==100
#				include "hmLib_v2/comgate/comgate_v1_00.cpp"
#			endif
#		endif
#	endif
#	ifdef INC_HMLIB_HMCOMGATE
#		ifndef INC_HMLIB_HMCOMGATE_CPP
#			if INC_HMLIB_HMCOMGATE==100
#				include "hmLib_v2/hmComGate/hmComGate_v1_00.cpp"
#			endif
#		endif
#	endif
#	ifdef INC_HMLIB_DXKERNEL
#		ifndef INC_HMLIB_DXKERNEL_CPP
#			if INC_HMLIB_DXKERNEL==100
#				include "hmLib_v2/dxKernel/dxKernel_v1_00.cpp"
#			endif
#		endif
#	endif
#	ifdef INC_HMLIB_DXIMAGE
#		ifndef INC_HMLIB_DXIMAGE_CPP
#			if INC_HMLIB_DXIMAGE==100
#				include "hmLib_v2/dxImage/dxImage_v1_00.cpp"
#			endif
#		endif
#	endif
#	ifdef INC_HMLIB_DXDRAWCTRL
#		ifndef INC_HMLIB_DXDRAWCTRL_CPP
#			if INC_HMLIB_DXDRAWCTRL==100
#				include "hmLib_v2/dxDrawCtrl/dxDrawCtrl_v1_00.cpp"
#			endif
#		endif
#	endif
#	ifdef INC_HMLIB_DXDISPLAY
#		ifndef INC_HMLIB_DXDISPLAY_CPP
#			if INC_HMLIB_DXDISPLAY==100
#				include "hmLib_v2/dxDisplay/dxDisplay_v1_00.cpp"
#			endif
#		endif
#	endif
#	ifdef INC_HMLIB_DXFONT
#		ifndef INC_HMLIB_DXFONT_CPP
#			if INC_HMLIB_DXFONT==100
#				include "hmLib_v2/dxFont/dxFont_v1_00.cpp"
#			endif
#		endif
#	endif
#	ifdef INC_HMLIB_DXINPUT
#		ifndef INC_HMLIB_DXINPUT_CPP
#			if INC_HMLIB_DXINPUT==100
#				include "hmLib_v2/dxInput/dxInput_v1_00.cpp"
#			endif
#		endif
#	endif
#	ifdef INC_HMLIB_DXINPUTDEVICE
#		ifndef INC_HMLIB_DXINPUTDEVICE_CPP
#			if INC_HMLIB_DXINPUTDEVICE==100
#				include "hmLib_v2/dxInputDevice/dxInputDevice_v1_00.cpp"
#			endif
#		endif
#	endif
#	ifdef INC_HMLIB_DXGRAPHICS
#		ifndef INC_HMLIB_DXGRAPHICS_CPP
#			if INC_HMLIB_DXGRAPHICS==100
#				include "hmLib_v2/dxInputDevice/dxInputDevice_v1_00.cpp"
#			endif
#		endif
#	endif
#endif
