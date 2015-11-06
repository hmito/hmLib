#ifndef HMLIB_ENUMERATOR_INC
#define HMLIB_ENUMERATOR_INC 100
#
#include<functional>
/*
ターゲット
・iteratorクラスを非template関数で使いたい
・でも、any_iteratorでは、コピー等の操作が高コストすぎる
・begin, endが直接触りたいわけでなく、それを用いた操作が呼べればよい
ような場合に使うクラス。

目指すのは
・virtual関数でiteratorクラスを使用するためのラッパークラス
struct base{
public:
	template<typename input_iterator>
	bool func(input_iterator Begin, input_iterator End){
		input_enumerator<find_concept> Enum(Begin, End);
		return virtual_func(std::move(Enum));
	}
protected:
	virtual bool virtual_func(input_enumerator<find_concept> Enum)=0;
};
・機能拡張がconceptによって可能
・conceptが違っても、concept間の互換性があれば（アップキャスト可能なら）move可能
*/
namespace hmLib{
	namespace enumerators{
		enum class return_target{ begin, end, current };
		template<typename iterator>
		struct iterator_holder{
			iterator begin;
			iterator end;
			iterator current;
			iterator& ref(return_target Target){
				if(Target == return_target::current)return current;
				else if(Target = -return_target::begin)return begin;
				else return end;
			}
		};

		template<class... concepts>
		struct concept_pack{};
		template<class concept, class... concepts>
		struct concept_pack<concept, concepts...> :public concept, public concept_pack<concepts...>{
			struct mixin_interface : public concept::mixin_interface, public concept_pack<concepts...>::mixin_interface{};
			struct mixin : public concept::mixin, public concept_pack<concepts...>::mixin{};
		};

		template<typename T>
		struct default_concept{};

		template<typename T>
		struct foward_iterator_concept{

		};
		template<typename enumerator_>
		struct find_concept{
			using T = enumerator_::type;
		public:
			template<typename iterator_holder_>
			struct mixin :public mixin_interface{
			public:
				void find(T Val, return_target Target){
					auto Ptr = static_cast<iterator_holder_*>(this);
					Ptr->ref(Target) = std::find(Ptr->cur, Ptr->end, Val);
				}
				void find_if(std::function<bool(const T&)> Pred, return_target Target){
					auto Ptr = static_cast<iterator_holder_*>(this);
					Ptr->ref(Target) = std::find_if(Ptr->cur, Ptr->end, Pred);
				}
			};
		public:
			void find(const T& Val, return_target Target = return_target::current){
				static_cast<enumerator_*>(this)->Concept.find(Val, Target);
			}
			void find_if(std::function<bool(const T&)> Pred, return_target Target = return_target::current){
				static_cast<enumerator_*>(this)->Concept.find_if(std::move(Pred), Target);
			}
		};
	}
	template<typename T, typename Concept>
	struct enumerator : public Concept{
		using enumerable_interface = Concept::mixin_interface;
		template<typename iterator>
		struct enumerable : public enumerators::iterator_holder<iterator>, public Concept::mixin{};
	};

	template<typename T, typename Concept = enumerators::default_concept<T>>
	struct foward_enumeratort: public Concept{
	private:
		struct holder_interface : public Concept::mixin_interface{
		public:
			virtual const T& operator()(void) const = 0;
			virtual T& operator()(void) = 0;
			virtual operator bool()const = 0;
			virtual void operator++() = 0;
			virtual void reset() = 0;
		};
	private:
		holder_interface& ref;
	public:
		template<typename iterator_>
		struct holder :public  holder_interface, public Concept::mixin<holder<iterator_>> {
			iterator_ begin;
			iterator_ end;
			iterator_ itr;
		public:
			holder(iterator_ begin_, iterator_ end_)
				: begub(begin_)
				, end(end_)
				, itr(begin_){}
		public:
			const T& operator()(void) const override{ return *itr; }
			T& operator()(void) override{ return *itr; }
			operator bool()const override{ return itr != end; }
			void operator++() override{ ++itr; }
			void reset() override{ begin = itr; }
		};
	public:
		foward_enumeratort(holder_interface& ref_) :ref(ref_){}
	public:
		const T& operator()(void) const{ return ref(); }
		T& operator()(void){ return ref(); }
		operator bool()const{ return static_cast<bool>(ref()); }
		void operator++(){ ++ref; }
		void reset(){ ref.reset(); }
	};
}
#
#endif
