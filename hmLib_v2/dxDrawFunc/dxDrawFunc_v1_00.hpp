#ifndef INC_HMLIB_DXDRAWFUNC
#define INC_HMLIB_DXDRAWFUNC 100
#
#ifndef INC_HMLIB_DXGRAPH
#	include "hmLib_v2/dxImage.hpp"
#endif
#ifndef INC_HMLIB_DXFONT
#	include "hmLib_v2/dxFont.hpp"
#endif
#ifndef INC_HMLIB_HMALIGN
#	include "hmLib_v2/hmAlign.hpp"
#endif
#include "hmLib_v2/DxLib.h"
namespace hmLib{
	namespace dx{
		typedef hm::cRGBClr clr_type;
		inline int getClr(const clr_type& clr){return GetColor(clr.r,clr.g,clr.b);}
		inline void fill(const Pint& pos,const clr_type& clr){Paint(pos.x,pos.y,getClr(clr),-1);}
		inline void fill(const Pint& pos,const clr_type& clr,clr_type& bound_clr){Paint(pos.x,pos.y,getClr(clr),getClr(bound_clr));}
		inline void drawBox(const Pint& pos1,const Pint& pos2,const clr_type& clr,bool IsFill=true){
			DrawBox(pos1.x,pos1.y,pos2.x,pos2.y,getClr(clr),static_cast<int>(IsFill));
		}
		inline void drawTriangle(const Pint& pos1,const Pint& pos2,const Pint& pos3,const clr_type& clr,bool IsFill=true){
			DrawTriangle(pos1.x,pos1.y,pos2.x,pos2.y,pos3.x,pos3.y,getClr(clr),static_cast<int>(IsFill));
		}
		inline void drawQuadrangle(const Pint& pos1,const Pint& pos2,const Pint& pos3,const Pint& pos4,const clr_type& clr,bool IsFill=true){
			DrawQuadrangle(pos1.x,pos1.y,pos2.x,pos2.y,pos3.x,pos3.y,pos4.x,pos4.y,getClr(clr),static_cast<int>(IsFill));
		}
		inline void drawCircle(const Pint& pos,const int& r,const clr_type& clr,bool IsFill=true){
			DrawCircle(pos.x,pos.y,r,getClr(clr),static_cast<int>(IsFill));
		}
		inline void drawOval(const Pint& pos,const Pint& r,const clr_type& clr,bool IsFill=true){
			DrawOval(pos.x,pos.y,r.x,r.y,getClr(clr),static_cast<int>(IsFill));
		}
		inline void drawPixel(const Pint& pos,const clr_type& clr){
			DrawPixel(pos.x,pos.y,getClr(clr));
		}
		inline void drawLine(const Pint& pos1,const Pint& pos2,const clr_type& clr,int Thick=1){
			DrawLine(pos1.x,pos1.y,pos2.x,pos2.y,getClr(clr),Thick);
		}
		inline void drawStr(const Pint& pos,const std::string Str,const clr_type& StrClr=clr_type(255,255,255),const dx::cFont& Font=dx::cFont::getDefault(false),const clr_type& StrEdgeClr=clr_type(0,0,0)){
			if(Font.getHandle()==-1){
				if(Font.getVertical())DrawVString(pos.x,pos.y,Str.c_str(),getClr(StrClr),getClr(StrEdgeClr));
				else DrawString(pos.x,pos.y,Str.c_str(),getClr(StrClr),getClr(StrEdgeClr));
			}else{
				if(Font.getVertical())DrawVStringToHandle(pos.x,pos.y,Str.c_str(),getClr(StrClr),Font.getHandle(),getClr(StrEdgeClr));
				else DrawStringToHandle(pos.x,pos.y,Str.c_str(),getClr(StrClr),Font.getHandle(),getClr(StrEdgeClr));
			}
		}
		inline void drawStrArea(const Pint& pos,const Pint& size,const std::string Str,align_quad DrawPos=aliq::center,const clr_type& StrClr=clr_type(255,255,255),const dx::cFont& Font=dx::cFont::getDefault(false),const clr_type& StrEdgeClr=clr_type(0,0,0)){
			//ï`âÊîÕàÕämíË
			Pint strsize=Font.getDrawStrSize(Str);

			//ï`âÊà íuí≤êÆéYèo
			Pint adj;
			//xï˚å¸í≤êÆ
			if(aliq::left&DrawPos)adj.x=0;
			else if(aliq::right&DrawPos)adj.x=size.x-strsize.x;
			else adj.x=(size.x-strsize.x)/2;
			//yï˚å¸í≤êÆ
			if(aliq::top&DrawPos)adj.y=0;
			else if(aliq::bottom&DrawPos)adj.y=size.y-strsize.y;
			else adj.y=(size.y-strsize.y)/2;

			//ï`âÊ
			drawStr(pos+adj,Str,StrClr,Font,StrEdgeClr);
		}
		inline void drawImage(const Pint& pos,const cImage& grp,bool IsTrans=true,bool IsTurn=false){
			if(IsTurn)DrawTurnGraph(pos.x,pos.y,grp.getHandle(),static_cast<int>(IsTrans));
			else DrawGraph(pos.x,pos.y,grp.getHandle(),static_cast<int>(IsTrans));
		}
		inline void drawImage(const Pint& pos,const cImage& grp,const Pint& grppos,const Pint& grpsize,bool IsTrans=true,bool IsTurn=false){
			DrawRectGraph(pos.x,pos.y,grppos.x,grppos.y,grpsize.x,grpsize.y,grp.getHandle(),static_cast<int>(IsTrans),static_cast<int>(IsTurn));
		}
		inline void drawImage(const Pint& pos,const cImage& grp,const Pint& size,bool IsTrans=false){
			DrawExtendGraph(pos.x,pos.y,pos.x+size.x,pos.y+size.y,grp.getHandle(),static_cast<int>(IsTrans));
		}
		inline void drawImage(const Pint& pos,const cImage& grp,const double& rate,const double& angle,bool IsTrans=true,bool IsTurn=false){
			DrawRotaGraph(pos.x,pos.y,rate,angle,grp.getHandle(),static_cast<int>(IsTrans),static_cast<int>(IsTurn));
		}
		inline void drawImage(const Pint& pos,const cImage& grp,const double& rate,const double& angle,const Pint& cpos,bool IsTrans=true,bool IsTurn=false){
			DrawRotaGraph2(pos.x,pos.y,cpos.x,cpos.y,rate,angle,grp.getHandle(),static_cast<int>(IsTrans),static_cast<int>(IsTurn));
		}
		inline void drawSoftImage(const Pint& pos,const cSoftImage& imgrp){
			DrawSoftImage(pos.x,pos.y,imgrp.getHandle());
		}
	}
}
#
#endif
