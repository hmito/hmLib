#ifndef HMLIB_TASK_INC
#define HMLIB_TASK_INC 100
#
#include <list>
#include <algorithm>
#include<boost/iterator/iterator_facade.hpp>
namespace hmLib{
	class task{
	public:
		task():ParentPtr(0),NextPtr(0),PrevPtr(0),FirstChild(0),LastChild(0),IsRun(false),Priority(0),Status(0){return;}
		explicit task(unsigned int Priority_):ParentPtr(0),NextPtr(0),PrevPtr(0),FirstChild(0),LastChild(0),IsRun(false),Priority(Priority_),Status(0){return;}
		//タスク起動
		bool run(){
			//子タスクなら、要望無視
			if(isChild())return true;

			//すでに実行済みなら、要望無視
			if(is_run())return true;

			//実行処理
			return do_run();
		}
		//タスク実行
		bool work(){
			//子タスクなら、要望無視
			if(isChild())return true;

			//初期化していなければ、要望無視
			if(!is_run())return true;

			//実行処理
			return do_work();
		}
		//タスク終了
		bool exit(){
			//子タスクなら、要望を親タスクに伝える
			if(isChild()){
				Status=-1;
				return false;
			}

			//走っていなければ、要望無視
			if(!is_run())return true;

			//タスクの終了処理
			do_exit();

			return false;
		}
		//優先順位によってソートするためのオペレータ
		bool operator<(const task& rTask_)const{return Priority<rTask_.Priority;}
		//指定回数だけ動作停止
		void sleep(unsigned int Length){if(Status==0)Status=Length;}
		//起動完了済&終了完了前
		bool is_run()const{return IsRun;}
		//子タスクとして従属
		bool isChild()const{return ParentPtr!=0;}
		//自身の子タスクかどうか判定
		bool isMyChild(task& rTask_)const{return rTask_.ParentPtr==this;}
		//子を持っているか判定
		bool hasChild()const{return FirstChild!=0;}
		//優先順位を設定
		bool setPriority(unsigned int Priority_){
			if(is_run())return true;
			Priority=Priority_;
			return false;
		}
		//優先順位を取得
		unsigned int getPriority()const{return Priority;}
	protected://task
		//タスク初期処理
		virtual bool task_construct(){return false;}
		//タスク通常処理
		virtual bool task_work(){return false;}
		//タスク終了処理
		virtual bool task_destruct(){return false;}
	protected:
		bool changePriority(unsigned int Priority_){
			if(isChild()){
				if(Priority > Priority_){
					//より上位へ変更時は-2 優先度上位変更要請
					Status=-2;
				}else if(Priority < Priority_){
					//より下位へ変更時は-3 優先度下位変更要請
					Status=-3;
				}
			}
			Priority=Priority_;
			return false;
		}
		//親タスクに新子タスク（自身の姉妹タスク）を追加
		void createSister(task& rTask_,unsigned int Priority_){
			//子タスクでなければ、要望無視
			if(!isChild())return;
			//追加処理
			ParentPtr->createChild(rTask_,Priority_);
		}
		//子タスクを作成
		bool createChild(task& rTask_){
			//すでに挿入されていれば、要望無視 実行時も要望無視
			if(rTask_.is_run()&&rTask_.isChild())return true;

			//優先度に基づいて、挿入
			insert(rTask_,std::upper_bound(begin(),end(),rTask_));

			//実行処理
			if(is_run())rTask_.do_run();

			return false;
		}
		//子タスクを作成 優先度指定付き
		bool createChild(task& rTask_,unsigned int Priority_){
			//すでに挿入されていれば、要望無視 実行時も要望無視
			if(rTask_.isChild() || rTask_.is_run())return true;

			//初期化
			rTask_.Priority=Priority_;

			//優先度に基づいて、挿入
			insert(rTask_,std::upper_bound(begin(),end(),rTask_));

			//実行処理
			if(is_run())rTask_.do_run();

			return false;
		}
		//子タスクを解放
		bool releaseChild(task& rTask_){
			//自身の子タスクでなければ、終了
			if(!isMyChild(rTask_))return true;

			//タスクの確認
			iterator itr=begin(),enditr=end();

			//タスクを探して削除処理
			for(;itr!=enditr;++itr){
				if(&(*itr)==&rTask_)break;
			}
			erase(itr);

			//終了処理
			if(rTask_.is_run())rTask_.do_exit();

			return false;
		}
		//子タスクを解放 Lower以上のタスク終了
		bool releaseChild(unsigned int LowerPriority){			
			//タスクを探して削除処理
			for(iterator itr=begin();itr!=end();++itr){
				if(itr->Priority <LowerPriority)continue;

				//終了処理
				if(itr->is_run())itr->do_exit();
				erase(itr);
			}

			return false;
		}
		//子タスクを解放 Upper未満、Lower以上のタスク終了
		bool releaseChild(unsigned int LowerPriority,unsigned int UpperPriority){			
			//タスクを探して削除処理
			for(iterator itr=begin();itr!=end();++itr){
				if(itr->Priority < LowerPriority){
					continue;
				}else if(itr->Priority >= UpperPriority){
					break;
				}

				//終了処理
				if(itr->is_run()){
					itr->do_exit();
				}
				erase(itr);
			}
			return false;
		}
	private:
		//run処理が完了済かつexit処理が未完了
		bool IsRun;
		//親タスク
		task* ParentPtr;
		//前のタスク
		task* PrevPtr;
		//次のタスク
		task* NextPtr;
		//次の子タスク
		task* FirstChild;
		//前の子タスク
		task* LastChild;
		//優先度
		unsigned int Priority;
		//-1:終了要求,-2:上位(先)へ優先度変更要請,-3:下位(後)へ優先度変更要請　正なら指定回数実行待機　0なら通常実行
		int Status;
	private://基本子タスク処理関数群
		class iterator;
		iterator begin(){return iterator(FirstChild);}
		iterator end(){return iterator(NULL);}
		//itrの前に子タスクを追加
		iterator insert(task& rTask_,iterator itr_){
			//終端以外に追加
			if(itr_!=end()){
				//先頭以外に追加
				if(itr_->PrevPtr!=0){
					itr_->PrevPtr->NextPtr=&rTask_;
					rTask_.PrevPtr=itr_->PrevPtr;
				}//先頭に追加
				else{
					FirstChild=&rTask_;
					rTask_.PrevPtr=0;
				}

				//次のタスクの処理
				itr_->PrevPtr=&rTask_;
				rTask_.NextPtr=&(*itr_);
			}//終端に追加
			else{
				//先頭以外に追加
				if(LastChild!=0){
					LastChild->NextPtr=&rTask_;
					rTask_.PrevPtr=LastChild;
				}//先頭に追加
				else{
					FirstChild=&rTask_;
					rTask_.PrevPtr=0;
				}

				//次のタスクの処理
				LastChild=&rTask_;
				rTask_.NextPtr=0;
			}

			rTask_.ParentPtr=this;

			return iterator(&rTask_);
		}
		//itrの子タスクを削除
		iterator erase(iterator itr_){
			//終端なら除外
			if(itr_==end())return end();

			//戻り値を確保
			iterator ans(itr_->NextPtr);

			//先頭の除外でないとき
			if(itr_->PrevPtr!=0)itr_->PrevPtr->NextPtr=itr_->NextPtr;
			//先頭の除外の時
			else FirstChild=itr_->NextPtr;

			//終端の除外でないとき
			if(itr_->NextPtr!=0)itr_->NextPtr->PrevPtr=itr_->PrevPtr;
			//終端の除外の時
			else LastChild=itr_->PrevPtr;

			//子タスク指定除外
			itr_->ParentPtr=0;
			itr_->NextPtr=0;
			itr_->PrevPtr=0;

			//戻す
			return ans;
		}
	private://実際処理関数群
		//実際の起動処理
		bool do_run(){
			//起動時の初期化
			Status=0;

			//タスクの起動処理　異常があれば、エラー返答
			if(task_construct())return true;

			//起動処理を無事行ったことを記録
			IsRun=true;

			//終了されていれば、ここで処理を終える
			if(!is_run())return false;

			//子タスクを起動
			for(iterator itr=begin(),enditr=end();itr!=enditr;++itr){
				//起動失敗時
				if(itr->do_run()){
					//現在地点までを終了
					enditr=itr;
					itr=begin();
					for(;itr!=enditr;++itr)itr->do_exit();

					//自身の終了処理
					task_destruct();
					return true;
				}
			}
			return false;
		}
		//実際のタスク処理
		bool do_work(){
			//sleep処理
			if(Status!=0){
				if(Status>0)--Status;
				return false;
			}

			//タスクの起動処理　異常があれば、エラー返答
			if(task_work())return true;

			//終了されていれば、ここで処理を終える
			if(!is_run())return false;

			//子タスクを実行
			iterator itr=begin();
			while(itr!=end()){
				//実行失敗時、エラー返答
				if(itr->do_work())return true;

				//終了処理が実行されていた場合
				if(itr->Status==-1){
					//終了処理
					itr->do_exit();
					//削除処理
					itr=erase(itr);
					continue;
				}//Priority変更要請
				else if(itr->Status<=-2){
					//タスクポインタを取得
					task* ptr=&(*itr);
					//一度削除
					itr=erase(itr);
					//正しいPriority位置へ再度挿入
					insert(*ptr,std::upper_bound(begin(),end(),*ptr));
					//下位への優先度変更
					if(itr->Status==-3){
						//もし次タスクを飛び越えていればスリープ
						if(itr->PrevPtr!=ptr)Status=1;
						//飛び越えていなければ正常へ
						else Status=0;
					}else{
						Status=0;
					}
					continue;
				}
				//次のitrへ
				++itr;
			}

			return false;
		}
		//実際の終了処理
		bool do_exit(){
			//子タスクを終了
			for(iterator itr=begin(),enditr=end();itr!=enditr;++itr){
				itr->do_exit();
			}

			//タスクの終了処理
			task_destruct();
			//終了処理を無事行ったことを記録
			IsRun=false;

			return false;
		}
	};
	class task::iterator:public boost::iterator_facade<iterator,task,boost::bidirectional_traversal_tag>{
	private:
		task* Ptr;
	protected:
		friend class boost::iterator_core_access;
		void increment(){Ptr=Ptr->NextPtr;}
		void decrement(){Ptr=Ptr->PrevPtr;}
		task& dereference()const{return *Ptr;}
		bool equal(const iterator& itr_)const{return Ptr==itr_.Ptr;}
	public:
		iterator():Ptr(0){return;}
		iterator(task* Ptr_):Ptr(Ptr_){return;}
		iterator(const iterator& itr_):Ptr(itr_.Ptr){return;}
		const iterator& operator=(const iterator& itr_){
			if(this!=&itr_)Ptr=itr_.Ptr;
			return *this;
		}
	};
	class public_task:public task{
	private:
		bool CanTouchChild;
	public:
		public_task():task(),CanTouchChild(true){return;}
		explicit public_task(unsigned int Priority_):task(Priority_),CanTouchChild(true){return;}
		//挿入可能か
		bool canTouchChild()const{return CanTouchChild;}
		//子タスクロック
		void enableTouchChild(){CanTouchChild=true;}
		//子タスクアンロック
		void disableTouchChild(){CanTouchChild=false;}
		//子タスクを作成
		bool insertChild(task& rTask_){
			if(!canTouchChild())return true;
			return createChild(rTask_);
		}
		//子タスクを作成 優先度指定付き
		bool insertChild(task& rTask_,unsigned int Priority_){
			if(!canTouchChild())return true;
			return createChild(rTask_,Priority_);
		}
		//子タスクを解放
		bool eraseChild(task& rTask_){
			if(!canTouchChild())return true;
			return releaseChild(rTask_);
		}
		//子タスクを解放 Lower以上のタスク終了
		bool eraseChild(unsigned int LowerPriority){
			if(!canTouchChild())return true;
			return releaseChild(LowerPriority);
		}
		//子タスクを解放 Upper未満、Lower以上のタスク終了
		bool eraseChild(unsigned int LowerPriority,unsigned int UpperPriority){
			if(!canTouchChild())return true;
			return releaseChild(LowerPriority,UpperPriority);
		}
	};
}
#
#endif
