#ifndef HMLIB_MEMORY_CLONECOUNTER_INC
#define HMLIB_MEMORY_CLONECOUNTER_INC 100
#
#include<memory>
namespace hmLib {
	namespace {
		class _clone_counter_base {
		private:
			unsigned int CloneCnt;
			unsigned int ObserverCnt;
		public:
			_clone_counter_base() :CloneCnt(1), ObserverCnt(0) {}
			void attachCounter() { ++CloneCnt; }
			void detachCounter() { --CloneCnt; }
			void attachObserver() { ++ObserverCnt; }
			void detachObserver() { --ObserverCnt; }
			operator bool()const { return CloneCnt+ObserverCnt>0; }
			unsigned int counter() { return CloneCnt; }
		};
	}
	class clone_observer;
	class clone_counter {
		friend class clone_observer;
	private:
		_clone_counter_base* p;
	public:
		clone_counter() :p(nullptr) {}
		clone_counter(const clone_counter& Cnt_) :p(nullptr) {
			//相手がnullptrだった場合は、まずカウンタ生成
			if(Cnt_.p==nullptr) {
				const_cast<clone_counter&>(Cnt_).p = new _clone_counter_base();
			}
			//アタッチ
			p=Cnt_.p;
			p->attachCounter();
		}
		const clone_counter& operator=(const clone_counter& Cnt_) {
			//参照先が自分自身でない場合
			if(p != Cnt_.p) {
				//自身の保持カウンタをデタッチ
				if(p!=nullptr) {
					p->detachCounter();
					if(p==false)delete p;
					p=nullptr;
				}
				//相手がnullptrだった場合は、まずカウンタ生成
				if(Cnt_.p==nullptr) {
					const_cast<clone_counter&>(Cnt_).p = new _clone_counter_base();
				}
				//アタッチ
				p=Cnt_.p;
				p->attachCounter();

			}//両カウンタともにnullptrだった場合
			else if(Cnt_.p==nullptr) {
				//まずカウンタ生成
				const_cast<clone_counter&>(Cnt_).p = new _clone_counter_base();
				//アタッチ
				p=Cnt_.p;
				p->attachCounter();
			}

			return *this;
		}
		clone_counter(clone_counter&& Cnt_) :p(nullptr) {
			//数の変更はないので、単にスワップ
			std::swap(p, const_cast<clone_counter&>(Cnt_).p);
		}
		const clone_counter& operator=(clone_counter&& Cnt_) {
			//参照先が自分自身でない場合
			if(p != Cnt_.p) {
				//自身の保持カウンタをデタッチ
				if(p!=nullptr) {
					p->detachCounter();
					if(p==false)delete p;
					p=nullptr;
				}
				//数の変更はないので、単にスワップ
				std::swap(p, const_cast<clone_counter&>(Cnt_).p);
			}

			return *this;
		}
		~clone_counter() {
			//所有カウンタがある場合、デタッチ
			if(p!=nullptr) {
				p->detachCounter();
				if(p==false)delete p;
			}
		}
		unsigned int count() const {
			if(p==nullptr)return 1;
			return p->counter();
		}		
		operator unsigned int()const {
			return count();
		}
		void reset() {
			//自身の保持カウンタをデタッチ
			if(p!=nullptr) {
				p->detachCounter();
				if(p==false)delete p;
				p=nullptr;
			}
		}
		bool operator==(const clone_observer& My2_)const;
		bool operator==(const clone_counter& My2_)const {
			return p!=nullptr && p==My2_.p;
		}
	};
	class clone_observer {
		friend class clone_counter;
	private:
		_clone_counter_base* p;
	public:
		clone_observer(const clone_counter& Cnt_) :p(nullptr) {
			//相手がnullptrだった場合は、まずカウンタ生成
			if(Cnt_.p==nullptr) {
				const_cast<clone_counter&>(Cnt_).p = new _clone_counter_base();
			}
			//アタッチ
			p=Cnt_.p;
			p->attachObserver();
		}
		const clone_observer& operator=(const clone_counter& Cnt_) {
			//参照先が自分自身でない場合
			if(p != Cnt_.p) {
				//自身の保持カウンタをデタッチ
				if(p!=nullptr) {
					p->detachObserver();
					if(p==false)delete p;
					p=nullptr;
				}
				//相手がnullptrだった場合は、まずカウンタ生成
				if(Cnt_.p==nullptr) {
					const_cast<clone_counter&>(Cnt_).p = new _clone_counter_base();
				}
				//アタッチ
				p=Cnt_.p;
				p->attachObserver();

			}//両カウンタともにnullptrだった場合
			else if(Cnt_.p==nullptr) {
				//まずカウンタ生成
				const_cast<clone_counter&>(Cnt_).p = new _clone_counter_base();
				//アタッチ
				p=Cnt_.p;
				p->attachObserver();
			}

			return *this;
		}
		~clone_observer() {
			//所有カウンタがある場合、デタッチ
			if(p!=nullptr) {
				p->detachObserver();
				if(p==false)delete p;
			}
		}
		unsigned int count() const {
			if(p==nullptr)return 0;
			return p->counter();
		}
		operator unsigned int()const {
			return count();
		}
		void reset() {
			//自身の保持カウンタをデタッチ
			if(p!=nullptr) {
				p->detachObserver();
				if(p==false)delete p;
				p=nullptr;
			}
		}
		bool operator==(const clone_counter& My2_)const;
		bool operator==(const clone_observer& My2_) const{
			return p!=nullptr && p==My2_.p;
		}
	};
	inline bool clone_counter::operator==(const clone_observer& My2_) const {
		return p!=nullptr && p==My2_.p;
	}
	inline bool clone_observer::operator==(const clone_counter& My2_) const {
		return p!=nullptr && p==My2_.p;
	}
}
#
#endif
