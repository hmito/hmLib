#ifndef HMLIB_ENUMERATORS_INC
#define HMLIB_ENUMERATORS_INC 100
#
#include"enumerators/enumerator.hpp"
#include"enumerators/container_enumerator.hpp"
#include<vector>
namespace hmLib {
	namespace enumerators {
		template<typename T>
		struct foward_iterator_concept {
		public:
			struct holder_interface {
			public:
				virtual const T& operator()(void) const=0;
				virtual T& operator()(void)=0;
				virtual operator bool()const=0;
				virtual void operator++()=0;
				virtual void reset()=0;
			};
		public:
			template<typename iterator_>
			struct holder :public  holder_interface {
				iterator_ begin;
				iterator_ end;
				iterator_ itr;
			public:
				holder(iterator_ begin_, iterator_ end_)
					: begub(begin_)
					, end(end_)
					, itr(begin_) {
				}
			public:
				const T& operator()(void) const override{ return *itr; }
				T& operator()(void) override { return *itr; }
				operator bool()const override { return itr!=end; }
				void operator++() override { ++itr; }
				void reset() override { begin=itr; }
			};
		private:
			holder_interface& ref;
		public:
			foward_iterator_concept(holder_interface& ref_) :ref(ref_) {}
		public:
			const T& operator()(void) const { return ref(); }
			T& operator()(void) { return ref(); }
			operator bool()const { return static_cast<bool>(ref()); }
			void operator++() { ++ref; }
			void reset() { ref.reset(); }
		};
		template<typename T>
		struct find_concept {
		public:
			struct mixin_interface {
			public:
				virtual ~mixin_interface() {}
			public:
				virtual void find(T Val_)=0;
				virtual void find_begin(T Val_)=0;
				virtual void find_end(T Val_)=0;
			};
		public:
			template<typename iterator_holder_>
			struct mixin :public mixin_interface {
			private:
				iterator_holder_& ref;
			public:
				mixin(iterator_holder_& ref_) :ref(ref_) {}
			public:
				void find(int Val_) override{
					ref.itr=std::find(ref.begin, ref.end, Val_);
				}
				void find_begin(int Val_) override {
					ref.begin=std::find(ref.begin, ref.end, Val_);
				}
				void find_end(int Val_) override {
					ref.end=std::find(ref.begin, ref.end, Val_);
				}
			};
		private:
			mixin_interface& ref;
		public:
			find_concept(mixin_interface& ref_) :ref(ref_) {}
		public:
			void find(T Val_) { ref.find(Val_); }
			void find_begin(T Val_) { ref.find_begin(Val_); }
			void find_end(T Val_) { ref.find_end(Val_); }
		};
		template<typename T>
		struct findable_forward_enumerator :public foward_iterator_concept<T>, public find_concept<T> {
		public:
			findable_forward_enumerator(foward_iterator_concept<T>::holder_interface& holder_ref_, find_concept<T>::mixin_interface& find_ref_)
				: foward_iterator_concept<T>(holder_ref_)
				, find_concept<T>(find_ref_) {
			}
		};

		void sample1(){
			//コンテナに対して
			std::vector<int> Vector;

			foward_iterator_concept<int>::holder<std::vector<int>::iterator> IteratorConcept(Vector.begin(),Vector.end());
			find_concept<int>::mixin<foward_iterator_concept<int>::holder<std::vector<int>::iterator>> FindConcept(IteratorConcept);
			findable_forward_enumerator<int> Enumerator(IteratorConcept, FindConcept);

			Enumerator.find(40);

			std::cout<<"output values over ten."<<std::endl;
			for(Enumerator.find(10); Enumerator!=false; ++Enumerator) {
				std::cout<<Enumerator()<<std::endl;
			}

			std::cout<<"output values under ten."<<std::endl;
			for(Enumerator.find_end(10); Enumerator!=false; ++Enumerator) {
				std::cout<<Enumerator()<<std::endl;
			}

			std::cout<<"output values under ten."<<std::endl;
			for(Enumerator.find_end(10); Enumerator!=false; ++Enumerator) {
				std::cout<<Enumerator()<<std::endl;
			}
		}

		class cTest {
		public:
			template<typename context_>
			enumerator<int,context_>& range(context_& Context_);
		};

		void sample2() {
			cTest Test;

			find_concept<int> FindConcept;
			auto Enumerator=Test.range(FindConcept);

			
		}
	}
}
#
#endif
