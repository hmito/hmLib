#ifndef INC_HMLIB_DXINPUT
#define INC_HMLIB_DXINPUT 100
#
#ifndef INC_HMLIB_HMTASK
#	include "hmLib_v2/hmTask.hpp"
#endif
#ifndef INC_HMLIB_HMSTR
#	include "hmLib_v2/hmStr.hpp"
#endif
#ifndef INC_HMLIB_DXFONT
#	include "hmLib_v2/dxFont.hpp"
#endif
#ifndef INC_HMLIB_DXDRAWFUNC
#	include "hmLib_v2/dxDrawFunc.hpp"
#endif
#ifndef INC_HMLIB_DXDRAWCTRL
#	include "hmLib_v2/dxDrawCtrl.hpp"
#endif
#include "hmLib_v2/DxLib.h"
#define DX_INPUT_TASK 0x200
namespace hmLib{
	namespace dx{
		//cInputクラス
		class cInput{
		public:
			//描画標準関数
			class itfInputDrawFn{
			/*	// ＩＭＥ入力文字列の描画に必要な情報の内の文節情報
				typedef struct tagIMEINPUTCLAUSEDATA{
					int						Position ;				// 何バイト目から
					int						Length ;				// 何バイトか
				} IMEINPUTCLAUSEDATA, *LPIMEINPUTCLAUSEDATA ;

				// ＩＭＥ入力文字列の描画に必要な情報
				typedef struct tagIMEINPUTDATA{
					const TCHAR *				InputString ;			// 入力中の文字列
					int							CursorPosition ;		// カーソルの入力文字列中の位置(バイト単位)
					const IMEINPUTCLAUSEDATA *	ClauseData ;			// 文節情報
					int							ClauseNum ;				// 文節情報の数
					int							SelectClause ;			// 選択中の分節( -1 の場合はどの文節にも属していない( 末尾にカーソルがある ) )
					int							CandidateNum ;			// 変換候補の数( 0の場合は変換中ではない )
					const TCHAR **				CandidateList ;			// 変換候補文字列リスト( 例：ｎ番目の候補を描画する場合  DrawString( 0, 0, data.CandidateList[ n ], GetColor(255,255,255) ); )
					int							SelectCandidate ;		// 選択中の変換候補
				} IMEINPUTDATA, *LPIMEINPUTDATA ;*/
			public://basicInputDrawFn
				virtual void operator()(Pint Pos_,IMEINPUTDATA* pIME,const hm::cRGBClr& StrClr_,const dx::cFont& Font_,const hm::cRGBClr& StrEdgeClr_)=0;
				virtual void operator()(Pint Pos_,std::string Str_,const hm::cRGBClr& StrClr_,const dx::cFont& Font_,const hm::cRGBClr& StrEdgeClr_)=0;
				virtual unsigned int getSize(IMEINPUTDATA* pIME,const dx::cFont& Font_){
					if(pIME==0)return 0;
					return dx::getDrawStringWidth(std::string(pIME->InputString),Font_);
				}
				virtual unsigned int getSize(std::string Str_,const dx::cFont& Font_){
					return dx::getDrawStringWidth(Str_,Font_);
				}
			};
		private:
			static int ActiveHandle;
		public:
			static hm::cPublicTask Task;
			static IMEINPUTDATA* getIMEPtr();
		private:
			int Handle;
		public:
			cInput(){Handle=-1;}
			~cInput(){release();}
			void release(){
				if(Handle>0)DeleteKeyInput(Handle);
				Handle=-1;
			}
			void create(int MaxStrLength=256,int CancelValidFlag=0,int SingleCharOnlyFlag=0,int NumCharOnlyFlag=0,std::string IniStr=""){
				inactive();
				release();
				Handle=MakeKeyInput(MaxStrLength,CancelValidFlag,SingleCharOnlyFlag,NumCharOnlyFlag);
				SetKeyInputString(IniStr.c_str(),Handle);
			}
			void setStr(std::string IniStr){SetKeyInputString(IniStr.c_str(),Handle);}
			void active(){
				if(ActiveHandle==Handle){
					ReStartKeyInput(Handle);
				}else{
					SetActiveKeyInput(Handle);
					ActiveHandle=Handle;
				}
			}
			void inactive(){
				if(ActiveHandle==Handle){
					SetActiveKeyInput(-1);
					ActiveHandle=-1;
				}
			}
			int status()const{
				//非入力中:0
				//入力途中:1
				//入力完了:2
				//入力終了:3
				if(ActiveHandle!=Handle)return 0;
				else{
					int ans=CheckKeyInput(Handle);
					if(ans<0)return -1;
					return ans+1;
				}
			}
			int getHandle()const{return Handle;}
			const char* getCStr()const{
				GetKeyInputString(hm::getTmpStrAdress(),Handle);
				return hm::getTmpStrAdress();
			}
			std::string getStr()const{
				GetKeyInputString(hm::getTmpStrAdress(),Handle);
				return std::string(hm::getTmpStrAdress());
			}
			int getNum()const{return GetKeyInputNumber(Handle);}
			double getFloatNum()const{return (double)GetKeyInputNumberToFloat(Handle);}
			int getCursorPos()const{return GetKeyInputCursorPosition(Handle);}
			void setCursorPos(int Pos_)const{SetKeyInputCursorPosition(Pos_,Handle);}
			void getSelectArea(int* pStart_,int* pEnd_)const{
				GetKeyInputSelectArea(pStart_,pEnd_,Handle);
				if(*pStart_<0){
					*pStart_ = getCursorPos();
					*pEnd_=*pStart_;
				}else if(*pStart_>*pEnd_){
					int tmp=*pStart_;
					*pStart_=*pEnd_;
					*pEnd_=tmp;
				}
			}
			void setSelectArea(int Start_,int End_)const{
				if(Start_==End_){
					Start_=-1;
					End_=-1;
				}
				SetKeyInputSelectArea(Start_,End_,Handle);
			}
		};
		//標準InputDrawFnクラス
		class cInputDrawFn:public cInput::itfInputDrawFn{
			class:public hm::cTask{
			public:
				bool ReserveFlag;
				cInputDrawFn* This;
			protected://hmTask
				//タスク初期処理
				virtual bool construct(){return false;}
				//タスク通常処理
				virtual bool task(){
					if(!ReserveFlag){
						exit();
						return false;
					}
					dx::cArea Ar;
					dx::cBlend Bl;
					dx::cBright Br;
					dx::cArea::scoped_active TmpArea(Ar);
					dx::cBlend::scoped_active TmpBlend(Bl);
					dx::cBright::scoped_active TmpBright(Br);
					//IME表示
					if(This->pIME){
						std::string IMEStr=std::string(This->pIME->InputString);
						Pint Pos(0,0);
						//各文節表示
						for(int i=0;i< This->pIME->ClauseNum; ++i){
							std::string Tmp=IMEStr.substr(This->pIME->ClauseData[i].Position,This->pIME->ClauseData[i].Length);
							//選択中の文節
							if(i==This->pIME->SelectClause){
								//変換候補が存在
								if(This->pIME->CandidateNum){
									//表示位置調整単位
									Pint Adj;
									if(dx::cFont::getVertical(*(This->pFont)))Adj.set(dx::cFont::getSize(*(This->pFont)),0);
									else Adj.set(0,dx::cFont::getSize(*(This->pFont)));

									//上下に変換候補表示
									for(int i=0; i < This->pIME->CandidateNum; ++i){
										if( i < This->pIME->SelectCandidate - This->UpNum || i > This->pIME->SelectCandidate + This->DwNum )continue;
										//網掛け文字で表示
 										This->drawStr_withBox(This->Pos+Pos+Adj*(i-This->pIME->SelectCandidate),std::string(This->pIME->CandidateList[i]));
									}
								}//変換候補なし
								else{
									This->drawStr_withBox(This->Pos+Pos,Tmp);
								}
							}//選択されていない文節
							else{
								//線付文字で表示
								This->drawStr_withLine(This->Pos+Pos,Tmp);
							}
							if(This->pFont->getVertical())Pos+=Pint(0,dx::getDrawStringWidth(Tmp,*(This->pFont)));
							else Pos+=Pint(dx::getDrawStringWidth(Tmp,*(This->pFont)),0);
						}
					}

					ReserveFlag=false;

					return false;
				}
				//タスク終了処理
				virtual bool destruct(){return false;}
			}Task;
		protected:
			void drawStr_withLine(Pint Pos_,const std::string& Str_){
				if(dx::cFont::getVertical(*pFont))	dx::drawLine(Pos_+Pint(dx::cFont::getSize(*pFont),dx::cFont::getSize(*pFont)/8),Pos_+Pint(dx::cFont::getSize(*pFont),dx::getDrawStringWidth(Str_,*pFont)-dx::cFont::getSize(*pFont)/8),hm::cRGBClr(255-StrClr.r,255-StrClr.g,255-StrClr.b));
				else							dx::drawLine(Pos_+Pint(dx::cFont::getSize(*pFont)/8,dx::cFont::getSize(*pFont)),Pos_+Pint(dx::getDrawStringWidth(Str_,*pFont)-dx::cFont::getSize(*pFont)/8,dx::cFont::getSize(*pFont)),hm::cRGBClr(255-StrClr.r,255-StrClr.g,255-StrClr.b));
				dx::drawStr(Pos_,Str_,StrClr,*pFont,StrEdgeClr);
			}
			void drawStr_withBox(Pint Pos_,const std::string& Str_){
				if(dx::cFont::getVertical(*pFont))	dx::drawBox(Pos_,Pos_+Pint(dx::cFont::getSize(*pFont),dx::getDrawStringWidth(Str_,*pFont)),hm::cRGBClr(255-StrClr.r,255-StrClr.g,255-StrClr.b));
				else							dx::drawBox(Pos_,Pos_+Pint(dx::getDrawStringWidth(Str_,*pFont),dx::cFont::getSize(*pFont)),hm::cRGBClr(255-StrClr.r,255-StrClr.g,255-StrClr.b));
				dx::drawStr(Pos_,Str_,StrClr,*pFont,StrEdgeClr);
			}
		private:
			int UpNum;
			int DwNum;
		private:
			Pint Pos;
			IMEINPUTDATA* pIME;
			hm::cRGBClr StrClr;
			hm::cRGBClr StrEdgeClr;
			const dx::cFont* pFont;
			std::string Str;
		public://basicInputDrawFn
			virtual void operator()(Pint Pos_,IMEINPUTDATA* pIME_,const hm::cRGBClr& StrClr_,const dx::cFont& Font_,const hm::cRGBClr& StrEdgeClr_){
				Pos=Pos_;
				pIME=pIME_;
				StrClr=StrClr_;
				StrEdgeClr=StrEdgeClr_;
				pFont=&Font_;
				Task.ReserveFlag=true;
				if(!Task.is_run())cInput::Task.insertChild(Task);
			}
			virtual void operator()(Pint Pos_,std::string Str_,const hm::cRGBClr& StrClr_,const dx::cFont& Font_,const hm::cRGBClr& StrEdgeClr_){
				Pos=Pos_;
				pIME=0;
				Str=Str_;
				StrClr=StrClr_;
				StrEdgeClr=StrEdgeClr_;
				pFont=&Font_;
				drawStr_withBox(Pos,Str);
			}
		public:
			cInputDrawFn(int UpNum_,int DwNum_):UpNum(UpNum_),DwNum(DwNum_){Task.This=this;}
		};
		extern cInputDrawFn InputDrawFn;
		inline int drawInpStr(const Pint& Pos,const cInput& Inp,
			const hm::cRGBClr& StrClr,const dx::cFont& Font=dx::cFont::getDefault(false),
			const hm::cRGBClr& StrEdgeClr=hm::cRGBClr(0,0,0),cInputDrawFn& InputDrawFn=dx::InputDrawFn){
			//入力中の文字列
			std::string str=Inp.getStr();

			//入力中でない場合
			if(Inp.status()!=1){
				//普通に文字列描画
				dx::drawStr(Pos,str,StrClr,Font,StrEdgeClr);
				return Inp.status();
			}

			//IME描画に必要なサイズを取得
			IMEINPUTDATA* pIME=cInput::getIMEPtr();

			//変換中のIME文字列が存在しない場合
			if(pIME==0){
				//カーソル位置を取得
				int Cur=Inp.getCursorPos();
				int CurPos;

				//選択範囲を取得
				int Begin,End;
				Inp.getSelectArea(&Begin,&End);
				
				//選択範囲が存在した場合
				if(Begin!=End){
					//選択範囲の描画に必要な範囲を計算
					unsigned int AreaSize=InputDrawFn.getSize(str.substr(Begin,End-Begin),Font);

					//前半の文字列描画に必要な範囲を計算
					unsigned int FirstSize=dx::getDrawStringWidth(str.substr(0,Begin),Font);
					
					//前半の文字列描画
					dx::drawStr(Pos,str.substr(0,Begin),StrClr,Font,StrEdgeClr);

					//後半の文字列描画
					if(dx::cFont::getVertical(Font))	dx::drawStr(Pos+Pint(0,FirstSize+AreaSize),str.substr(End,str.size()-End),StrClr,Font,StrEdgeClr);
					else							dx::drawStr(Pos+Pint(FirstSize+AreaSize,0),str.substr(End,str.size()-End),StrClr,Font,StrEdgeClr);

					//カーソル位置設定
					if(Begin==Cur)CurPos=FirstSize;
					else CurPos=FirstSize+AreaSize;

					//選択範囲描画
					if(dx::cFont::getVertical(Font))	InputDrawFn(Pos+Pint(0,FirstSize),str.substr(Begin,End-Begin),StrClr,Font,StrEdgeClr);
					else							InputDrawFn(Pos+Pint(FirstSize,0),str.substr(Begin,End-Begin),StrClr,Font,StrEdgeClr);
				}//選択範囲が存在しないとき
				else{
					//普通に文字列描画
					dx::drawStr(Pos,str,StrClr,Font,StrEdgeClr);

					//カーソル位置設定
					CurPos=dx::getDrawStringWidth(str.substr(0,Inp.getCursorPos()),Font);
				}

				//カーソル表示
				if(dx::cFont::getVertical(Font))	dx::drawLine(Pos+Pint(-1,CurPos-1),Pos+Pint(dx::cFont::getSize(Font)+1,CurPos-1),StrClr,1);
				else							dx::drawLine(Pos+Pint(CurPos-1,-1),Pos+Pint(CurPos-1,dx::cFont::getSize(Font)+1),StrClr,1);

			}//IMEが起動している場合
			else{
				//選択範囲を取得
				int Begin,End;
				Inp.getSelectArea(&Begin,&End);

				//選択範囲の描画に必要な範囲を計算
				unsigned int AreaSize=InputDrawFn.getSize(pIME,Font);

				//前半の文字列描画に必要な範囲を計算
				unsigned int FirstSize=dx::getDrawStringWidth(str.substr(0,Begin),Font);

				//前半の文字列描画
				dx::drawStr(Pos,str.substr(0,Begin),StrClr,Font,StrEdgeClr);

				//後半の文字列描画
				if(dx::cFont::getVertical(Font))	dx::drawStr(Pos+Pint(0,FirstSize+AreaSize),str.substr(End,str.size()-End),StrClr,Font,StrEdgeClr);
				else							dx::drawStr(Pos+Pint(FirstSize+AreaSize,0),str.substr(End,str.size()-End),StrClr,Font,StrEdgeClr);

				//IME描画
				if(dx::cFont::getVertical(Font))	InputDrawFn(Pos+Pint(0,FirstSize),pIME,StrClr,Font,StrEdgeClr);
				else							InputDrawFn(Pos+Pint(FirstSize,0),pIME,StrClr,Font,StrEdgeClr);
			}

			return Inp.status();
		}
		inline int drawInpStrArea(const Pint& Pos,const Pint& size,const cInput& Inp,align_quad DrawPos=aliq::center,
			const hm::cRGBClr& StrClr=hm::cRGBClr(255,255,255),const dx::cFont& Font=dx::cFont::getDefault(false),
			const hm::cRGBClr& StrEdgeClr=hm::cRGBClr(0,0,0),cInputDrawFn& InputDrawFn=dx::InputDrawFn){
			//描画範囲確定
			Pint strsize;

			//IME描画に必要なサイズを取得
			IMEINPUTDATA* pIME=cInput::getIMEPtr();

			//変換中のIME文字列が存在しない場合
			if(pIME==0){
				//選択範囲を取得
				int Begin,End;
				Inp.getSelectArea(&Begin,&End);
				
				//選択範囲が存在した場合
				if(Begin!=End){
					std::string str=Inp.getStr();

					//Areaの描画範囲＋Areaを除いた描画範囲+
					if(dx::cFont::getVertical(Font)){
						strsize.x=dx::cFont::getSize(Font);
						strsize.y=dx::getDrawStringWidth(str.substr(0,Begin),Font)+InputDrawFn.getSize(str.substr(Begin,End-Begin),Font)+dx::getDrawStringWidth(str.substr(End,str.size()-End),Font);
					}else{
						strsize.x=dx::getDrawStringWidth(str.substr(0,Begin),Font)+InputDrawFn.getSize(str.substr(Begin,End-Begin),Font)+dx::getDrawStringWidth(str.substr(End,str.size()-End),Font);
						strsize.y=dx::cFont::getSize(Font);
					}

				}//選択範囲が存在しないとき
				else{
					//通常の描画範囲
					if(dx::cFont::getVertical(Font)){
						strsize.x=dx::cFont::getSize(Font);
						strsize.y=dx::getDrawStringWidth(Inp.getStr(),Font);
					}else{
						strsize.x=dx::getDrawStringWidth(Inp.getStr(),Font);
						strsize.y=dx::cFont::getSize(Font);
					}
				}
			}//IMEが起動している場合
			else{
				//選択範囲を取得
				int Begin,End;
				Inp.getSelectArea(&Begin,&End);

				//選択範囲が存在した場合
				if(Begin!=End){
					std::string str=Inp.getStr();

					//IMEの描画範囲＋選択されていないStr描画範囲
					if(dx::cFont::getVertical(Font)){
						strsize.x=dx::cFont::getSize(Font);
						strsize.y=dx::getDrawStringWidth(str.substr(0,Begin),Font)+InputDrawFn.getSize(pIME,Font)+dx::getDrawStringWidth(str.substr(End,str.size()-End),Font);
					}else{
						strsize.x=dx::getDrawStringWidth(str.substr(0,Begin),Font)+InputDrawFn.getSize(pIME,Font)+dx::getDrawStringWidth(str.substr(End,str.size()-End),Font);
						strsize.y=dx::cFont::getSize(Font);
					}
				}//選択範囲が存在しないとき
				else{
					//IMEの描画範囲＋Str描画範囲
					if(dx::cFont::getVertical(Font)){
						strsize.x=dx::cFont::getSize(Font);
						strsize.y=dx::getDrawStringWidth(Inp.getStr(),Font)+InputDrawFn.getSize(pIME,Font);
					}else{
						strsize.x=dx::getDrawStringWidth(Inp.getStr(),Font)+InputDrawFn.getSize(pIME,Font);
						strsize.y=dx::cFont::getSize(Font);
					}
				}
			}

			//描画位置調整産出
			Pint adj;
			//x方向調整
			if(aliq::left&DrawPos)adj.x=0;
			else if(aliq::right&DrawPos)adj.x=size.x-strsize.x;
			else adj.x=(size.x-strsize.x)/2;
			//y方向調整
			if(aliq::top&DrawPos)adj.y=0;
			else if(aliq::bottom&DrawPos)adj.y=size.y-strsize.y;
			else adj.y=(size.y-strsize.y)/2;

			//描画
			return drawInpStr(Pos+adj,Inp,StrClr,Font,StrEdgeClr,InputDrawFn);
		}
	}
}
#
#endif
