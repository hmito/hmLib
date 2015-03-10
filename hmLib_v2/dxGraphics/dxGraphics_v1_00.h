#ifndef INC_HMLIB_DXGRAPHICS
#define INC_HMLIB_DXGRAPHICS 100
#
#include <vector>
#ifndef INC_HMLIB_HMCOLOR
#	include "hmLib_v2/hmColor.hpp"
#endif
#ifndef INC_HMLIB_HMRECT
#	include "hmLib_v2/hmRect.hpp"
#endif
#ifndef INC_HMLIB_DXDRAWCTRL
#	include "hmLib_v2/dxDrawCtrl.hpp"
#endif
#ifndef INC_HMLIB_DXIMAGE
#	include "hmLib_v2/dxImage.hpp"
#endif
#include "hmLib_v2/DxLib.h"
namespace hmLib{
	namespace dx{
		class cGraphics;
		class itfDesign{
			friend class cGraphics;
		protected://itfDesign
			virtual int draw(cGraphics& Grp)=0;
		};
		class itfHardDesign:public itfDesign{
		public://itfHardDesign
			virtual Rint area()=0;
		};
		class cGraphics{
		private:
			//描画スクリーン
			const cScreen* pScr;
			//描画素子ベース
			std::vector<itfDesign*> Ptr;
			//Global座標
			std::vector<Pint> Pos;
			//vectorの状態変化をつかさどる変数
			std::vector<int> IsNew;
			//描画エリア Global座標
			std::vector<cArea> Area;
			//明るさ
			std::vector<cBright> Bright;
			//ブレンド
			std::vector<cBlend> Blend;
			//Status
			std::vector<cStatus> Status;
			//Global座標での描画範囲
			std::vector<Rint*> Record;
		private:
			static const int CheckArea;
			static const int CheckBright;
			static const int CheckBlend;
			static const int CheckStatus;
			static const int CheckRecord;
		public://基本関数
			cGraphics(const cScreen& Scr_):
				pScr(&Scr_),
				Ptr(1,0),
				Pos(1,Pint(0,0)),
				IsNew(1,0),
				Area(1,cArea()),
				Bright(1,cBright()),
				Blend(1,cBlend()),
				Status(1,cStatus()),
				Record(1,0){
				return;
			}
			const cScreen& getScreen()const{return *pScr;}
			const Pint& gpos()const{return Pos.back();}
			Pint gtol(const Pint& LPos_){return gpos()+LPos_;}
			Pint ltog(const Pint& GPos_){return GPos_-gpos();}
			bool isInDrawArea(const Rint& DrawArea_)const{return Area.back().cross(DrawArea_+gpos());}
			void record(const Rint& DrawArea_){if(Record.back()!=0)*(Record.back())|=DrawArea_+gpos();}
			void setMode()const{
				cArea::active(Area.back());
				cBright::active(Bright.back());
				cBlend::active(Blend.back());
				cStatus::active(Status.back());
			}
		public://描画関数
			int draw(const Pint& Pos_,itfDesign& Dsg_){
				//ポインタとモード制御を一段階進める
				IsNew.push_back(0);
				Ptr.push_back(&Dsg_);
				Pos.push_back(Pos.back()+Pos_);

				//描画する
				int ans=Dsg_.draw(*this);

				//モードに応じて、追加モード削除
				if(IsNew.back()!=0){
					if(IsNew.back()&CheckArea)Area.pop_back();
					if(IsNew.back()&CheckBright)Bright.pop_back();
					if(IsNew.back()&CheckBlend)Blend.pop_back();
					if(IsNew.back()&CheckStatus)Status.pop_back();
					if(IsNew.back()&CheckRecord)Record.pop_back();
				}

				//ポインタとモード制御を一段階戻す
				IsNew.pop_back();
				Ptr.pop_back();
				Pos.pop_back();

				return ans;
			}
			int draw(const Pint& Pos_,itfHardDesign& HDsg_){
				//ポインタとモード制御を一段階進める
				IsNew.push_back(0);
				Ptr.push_back(&HDsg_);
				Pos.push_back(Pos.back()+Pos_);

				//描画範囲を取得
				Rint DA=HDsg_.area();

				//描画範囲を記録
				record(DA);

				//描画範囲内であった場合のみ、アクティブ化して描画する
				int ans=0;
				if(isInDrawArea(DA)){
					//描画記録アドレスがある場合、描画記録アドレスを上書き
					if(getDrawRecord()!=0)setDrawRecord(0);
					setMode();
					ans=HDsg_.draw(*this);
				}

				//モードに応じて、追加モード削除
				if(IsNew.back()!=0){
					if(IsNew.back()&CheckArea)Area.pop_back();
					if(IsNew.back()&CheckBright)Bright.pop_back();
					if(IsNew.back()&CheckBlend)Blend.pop_back();
					if(IsNew.back()&CheckStatus)Status.pop_back();
					if(IsNew.back()&CheckRecord)Record.pop_back();
				}

				//ポインタとモード制御を一段階戻す
				IsNew.pop_back();
				Ptr.pop_back();
				Pos.pop_back();

				return ans;
			}
			int draw(itfDesign& Dsg_){return draw(Pint(0,0),Dsg_);}
			int draw(itfHardDesign& HDsg_){return draw(Pint(0,0),HDsg_);}
		public://状態制御用クラス群
			bool active(const cArea& Area_){
				if(IsNew.back()&CheckArea){
					Area.back()=Area_+gpos();
				}else{
					Area.push_back(Area_+gpos());
					IsNew.back()^=CheckArea;
				}
				return false;
			}
			const cArea& nowArea()const{return Area.back();}
			bool active(const cBright& Bright_){
				if(IsNew.back()&CheckBright){
					Bright.back()=Bright_;
				}else{
					Bright.push_back(Bright_);
					IsNew.back()^=CheckBright;
				}
				return false;
			}
			const cBright& nowBright()const{return Bright.back();}
			bool active(const cBlend& Blend_){
				if(IsNew.back()&CheckBlend){
					Blend.back()=Blend_;
				}else{
					Blend.push_back(Blend_);
					IsNew.back()^=CheckBlend;
				}
				return false;
			}
			const cBlend& nowBlend()const{return Blend.back();}
			bool active(const cStatus& Status_){
				if(IsNew.back()&CheckStatus){
					Status.back()=Status_;
				}else{
					Status.push_back(Status_);
					IsNew.back()^=CheckStatus;
				}
				return false;
			}
			const cStatus& nowStatus()const{return Status.back();}
			bool setDrawRecord(Rint*const Record_){
				if(IsNew.back()&CheckRecord){
					Record.back()=Record_;
				}else{
					Record.push_back(Record_);
					IsNew.back()^=CheckRecord;
				}
				Record_->set(Pint(INT_MAX,INT_MAX),Pint(INT_MIN,INT_MIN));
				return false;
			}
			Rint*const getDrawRecord()const{return Record.back();}
		public:
			template<class Type>
			class accessor{
			private:
				Type* baseP;
			public:
				accessor<Type>(cGraphics& Grp){
					baseP=dynamic_cast<Type*>(Grp.Ptr.back());
					if(baseP==NULL)throw "cGraphics::accessor::accessor<Type>(cGraphics& Grp) : fail to dynamic_cast<Type>";
				}
				Type& operator*(){return *baseP;}
				Type* operator->(){return baseP;}
			};
		};
	}
}
#
#endif
