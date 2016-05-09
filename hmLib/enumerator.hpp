#ifndef HMLIB_ENUMERATOR_INC
#define HMLIB_ENUMERATOR_INC 100
#
#include<iterator>
#include<memory>
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
		template<typename iterator_>
		class range{
			using this_type = range<iterator_>;
			using iterator = iterator_;
			using value_type = typename iterator::value_type;
			using pointer = typename iterator::pointer;
			using reference = typename iterator::reference;
			using difference_type = typename iterator::difference_type;
		public: // construction, assignment
			range(){}
			range(iterator Begin_, iterator End_) :Begin(Begin_), End(End_){}
			range(const this_type& Other) = default;
			range& operator=(const this_type& Other) = default;
		public:
			iterator begin() const{return Begin;}
			iterator end() const{ return End; }
		public:
			operator bool() const{return Begin != End;}
			void advance_begin(difference_type n){std::advance(Begin, n);}
			void advance_end(difference_type n){ std::advance(End, n); }
			bool empty() const{ return Begin == End; }
		};

		enum class return_target{ begin, end, current };
		template<typename iterator>
		struct iterator_holder{
			using type = decltype(*iterator());
		public:
			iterator begin;
			iterator end;
			iterator current;
			iterator& ref(return_target Target){
				if(Target == return_target::current)return current;
				else if(Target = -return_target::begin)return begin;
				else return end;
			}
		};

		template<typename... concepts>
		struct concept_pack{};
		template<class concept, typename... others>
		struct concept_pack<concept, others...> :public concept, public concept_pack<others...>{
			struct mixin_interface : public concept::mixin_interface, public concept_pack<others...>::mixin_interface{};
			struct mixin : public concept::mixin, public concept_pack<others...>::mixin{};
		};
		template<typename T>
		struct default_concept{};
		template<typename T>
		struct foward_iterator_concept{
			struct concept_interface{
			public:
				virtual const T& operator()(void) const = 0;
				virtual T& operator()(void) = 0;
				virtual operator bool()const = 0;
				virtual void operator++() = 0;
				virtual void reset() = 0;
			};
			template<typename iterator_>
			struct concept :public concept_interface<iterator_>{
				using type = iterator_holder::type;
			public:
				const T& operator()(void) const override{ const auto& ref = static_cast<const iterator_holder&>(*this); return *(ref.itr); }
				T& operator()(void) override{ auto& ref = static_cast<iterator_holder&>(*this); return *(ref.itr); }
				operator bool()const override{ const auto& ref = static_cast<const iterator_holder&>(*this); return ref.itr != ref.end; }
				void operator++() override{ auto& ref = static_cast<iterator_holder&>(*this); ++(ref.itr); }
				void reset() override{ auto& ref = static_cast<iterator_holder&>(*this);  ref.begin = ref.itr; }
			};
			template<typename enumerator>
			struct mixin : public enumerator{
				using type = enumerator::type;
			public:
				const type& operator()(void) const{ const auto& ref = static_cast<const concept_interface<T>&>(static_cast<enumerator*>(this)->Holder); return ref(); }
				type& operator()(void){ auto& ref = static_cast<concept_interface<T>&>(static_cast<enumerator*>(this)->Holder); return ref(); }
				operator bool()const{ const auto& ref = static_cast<const concept_interface<T>&>(static_cast<enumerator*>(this)->Holder); return static_cast<bool>(ref()); }
				void operator++(){ auto& ref = static_cast<concept_interface<T>&>(static_cast<enumerator*>(this)->Holder); ++ref; }
				void reset(){ auto& ref = static_cast<concept_interface<T>&>(static_cast<enumerator*>(this)->Holder); ref.reset(); }
			};
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
	template<typename T, typename concept>
	struct enumerator : public concept::mixin<enumerator<T, concept>>{
		using mixin = concept::mixin;
	private:
		struct basic_enumerator_adaptor{};
		template<typename iterator_>
		struct enumerator_adaptor 
			: public basic_enumerator_adaptor
			, public concept{
			using iterator = iterator_;
		public:
			iterator Begin;
			iterator End;
			iterator Current;
		};
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

	//enumerator
	template<typename T, typename concept = enumerators::default_concept<T> >
	struct enumerator : public concept{
		using this_type = enumerator<T, concept>;
	private:
		struct basic_enumeratable{
		public:
			virtual basic_enumeratable* clone()const = 0;
			virtual void destroy() = 0;
			virtual bool equal(const basic_enumeratable& itr)const = 0;
			virtual void increment() = 0;
			virtual const value_type& dereference()const = 0;
		public:
			template<typename enumerator_>
			static basic_enumeratable* construct(enumerator_ itr);
		};
		template<typename iterator>
		struct enumeratable :public basic_enumeratable{
			typedef enumeratable<iterator> adaptor;
		private:
			iterator Begin;
			iterator End;
			iterator Current;
		public:
			basic_enumeratable* clone()const override{ return adaptor::construct(itr); }
			void destroy() override{ adaptor::destruct(this); }
			bool equal(const basic_enumeratable& itr_)const override{
				const adaptor* adp = static_cast<const adaptor*>(&itr_);
				if(adp == nullptr)return false;
				return itr == adp->itr;
			}
			void increment() override{ ++itr; }
			const value_type& dereference()const override{ return *itr; }
		public:
			enumerator_adaptor(enumerator_ itr_) :itr(itr_){}
		private:
			static boost::fast_pool_allocator<adaptor> allocator;
		public:
			static void destruct(adaptor* Ptr){
				allocator.destroy(Ptr);
				allocator.deallocate(Ptr);
			}
			static basic_enumeratable* construct(enumerator_ titr_){
				adaptor* ans = allocator.allocate();
				allocator.construct(ans, adaptor(titr_));
				return ans;
			}
		};
	private:
		std::unique_ptr<basic_enumeratable, typename basic_enumeratable::deleter> Ptr;
	public:
		//return true if the object can be accessed to.
		virtual operator bool()const{ return (*Ptr; }
		//access to object.
		virtual T& operator*(){ return Ptr->operator*(); }
		//move to next object.
		virtual void operator++(){ return Ptr->operator ++; }
		//restart from the initial object.
		virtual void reset() = 0;
	public://for conctructor
		enumerator() :Ptr(nullptr){}
		enumerator(const my_type& My_) :Ptr(nullptr){ if(My_.Ptr)Ptr.reset(My_.Ptr->clone()); }
		const my_type& operator=(const my_type& My_){
			if(this != &My_)Ptr.reset(My_.Ptr->clone());
			return *this;
		}
		enumerator(my_type&& My_) :Ptr(std::move(My_.Ptr)){}
		const my_type& operator=(my_type&& My_){
			if(this != &My_)Ptr = std::move(My_.Ptr);
			return *this;
		}
		template<typename enumerator_>
		enumerator(enumerator_ itr_) : Ptr(basic_enumerator_adaptor::construct(itr_)){}
		template<typename enumerator_>
		const my_type& operator=(enumerator_ itr_){
			Ptr.reset(basic_enumerator_adaptor::construct(itr));
			return *this;
		}
	};

}
#
#endif
