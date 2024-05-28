#ifndef HMLIB_RECUR_OBSERVER_ITERATOROBSERVER_INC
#define HMLIB_RECUR_OBSERVER_ITERATOROBSERVER_INC 100
#
namespace hmLib{
	namespace recur{
		template<typename pair_iterator>
		struct pair_iterator_observer{
		private:
			pair_iterator Itr;
		public:
			pair_iterator_observer() = default;
			pair_iterator_observer(pair_iterator Itr_):Itr(Itr_){}
			void reset(pair_iterator Itr_){ Itr = Itr_; }
			pair_iterator get()const{return Itr;}
			template<typename state, typename time>
			void operator()(const state& x, time t){ (*Itr++) = std::pair<time,state>(t,x); }
		};
		template<typename pair_iterator>
		pair_iterator_observer<pair_iterator> make_pair_iterator_observer(pair_iterator Itr) {
			return pair_iterator_observer<pair_iterator>(Itr);
		}

		template<typename state_iterator, typename time_iterator>
		struct iterator_observer{
		private:
			state_iterator StateItr;
			time_iterator TimeItr;
		public:
			iterator_observer() = default;
			iterator_observer(state_iterator StateItr_, time_iterator TimeItr_) :StateItr(StateItr_),TimeItr(TimeItr_){}
			void reset(state_iterator StateItr_, time_iterator TimeItr_){
				StateItr = StateItr_;
				TimeItr = TimeItr_;
			}
			state_iterator get_state_iterator()const{ return StateItr; }
			time_iterator get_time_iterator()const{ return TimeItr; }
			template<typename state, typename time>
			void operator()(const state& x, time t){
				(*StateItr++) = x;
				(*TimeItr++) = t;
			}
		};
		template<typename state_iterator>
		struct iterator_observer<state_iterator, void>{
		private:
			state_iterator Itr;
		public:
			iterator_observer() = default;
			iterator_observer(state_iterator StateItr_) :Itr(StateItr_){}
			void reset(state_iterator StateItr_){ Itr = StateItr_; }
			state_iterator get_state_iterator()const{ return Itr; }
			template<typename state, typename time>
			void operator()(const state& x, time t){
				(*Itr++) = x;
			}
		};

		template<typename state_iterator>
		iterator_observer<state_iterator, void> make_iterator_observer(state_iterator StateItr){
			return iterator_observer<state_iterator, void>(StateItr);
		}
		template<typename state_iterator,typename time_iterator>
		iterator_observer<state_iterator, time_iterator> make_iterator_observer(state_iterator StateItr, time_iterator TimeItr){
			return iterator_observer<state_iterator, time_iterator>(StateItr, TimeItr);
		}
	}
}
#
#endif
