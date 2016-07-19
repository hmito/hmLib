#ifndef HMLIB_ODEINT_CONTAINEROBSERVER_INC
#define HMLIB_ODEINT_CONTAINEROBSERVER_INC 100
#
#include<vector>
namespace hmLib{
	namespace odeint{
		template<typename state_, typename time_ = double, typename container_ = std::vector<std::pair<time_, state_>> >
		struct container_observer{
			using state = state_;
			using time = time_;
			using container = container_;
			using pair = std::pair<time, state>;
			using iterator = typename container::iterator;
			using const_iterator = typename container::const_iterator;
			using reverse_iterator = typename container::reverse_iterator;
			using const_reverse_iterator = typename container::const_reverse_iterator;
		private:
			container Log;
		public:
			void operator()(const state& x, time t){Log.emplace_back(t,x);}
		public:
			pair& at(size_t Pos){ return Log.at(Pos); }
			const pair& at(size_t Pos)const{ return Log.at(Pos); }
			pair& operator[](size_t Pos){ return Log[Pos]; }
			const pair& operator[](size_t Pos)const{ return Log[Pos]; }
			bool empty()const{ return Log.empty(); }
			unsigned int size()const{ return Log.size(); }
			void reserve(size_t Size){ Log.reserve(Size); }
		public:
			const container& get()const{ return Log; }
			container&& release(){ return std::move(Log); }
			void swap(container& Container){ std::swap(Container,Log); }
			void clear(){ Log.clear(); }
		public:
			iterator begin(){ return Log.begin(); }
			iterator end(){ return Log.end(); }
			const_iterator begin()const{ return Log.begin(); }
			const_iterator end()const{ return Log.end(); }
			const_iterator cbegin()const{ return Log.begin(); }
			const_iterator cend()const{ return Log.end(); }
			reverse_iterator rbegin(){ return Log.rbegin(); }
			reverse_iterator rend(){ return Log.rend(); }
			const_reverse_iterator rbegin()const{ return Log.rbegin(); }
			const_reverse_iterator rend()const{ return Log.rend(); }
			const_reverse_iterator crbegin()const{ return Log.rbegin(); }
			const_reverse_iterator crend()const{ return Log.rend(); }
		};
	}
}
#
#endif
