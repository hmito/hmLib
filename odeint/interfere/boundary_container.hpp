#ifndef HMLIB_ODEINT_INTERFERE_BOUNDARYCONTAINER_INC
#define HMLIB_ODEINT_INTERFERE_BOUNDARYCONTAINER_INC 100
#
#include<vector>
namespace hmLib {
	namespace odeint {
		template<typename state_type_, typename boundary_>
		struct boundary_container {
			using boundary_type = boundary_;
			using state_type = state_type_;
			using container = std::vector<boundary_type>;
			using iterator = typename container::iterator;
			using const_iterator = typename container::const_iterator;
		private:
			boundary_type Original;
			container Container;
		public:
			boundary_container(boundary_type Original_):Original(std::move(Original_)){}
		public://for interfere system
			void ready(const state_type& x) { 
				if(Container.size() != x.size()) {
					Container.assign(x.size(), Original);
				}

				auto itr = x.begin();
				
				for(auto bitr = begin(); bitr != end(); ++bitr, ++itr) {
					bitr->ready(*itr);
				}
			}
			bool valid_step(const state_type& x)const {
				auto itr = x.begin();

				for(auto bitr = begin(); bitr != end(); ++bitr, ++itr) {
					if(!bitr->valid_step(*itr))return false;
				}
				return true;
			}
			bool validate(const state_type& x, state_type& vx) const {
				bool IsValidated = false;
				auto itr = x.begin();
				auto vitr = vx.begin();

				for(auto bitr = begin(); bitr != end(); ++bitr, ++itr, ++vitr) {
					IsValidated |= bitr->validate(*itr,*vitr);
				}
				return IsValidated;
			}
		public:
			iterator begin() { return Container.begin(); }
			iterator end() { return Container.end(); }
			const_iterator begin()const { return Container.begin(); }
			const_iterator end()const { return Container.end(); }
			const_iterator cbegin()const { return Container.begin(); }
			const_iterator cend()const { return Container.end(); }
			std::size_t size()const { return Container.size(); }
			bool empty()const { return Container.empty(); }
			boundary_type& at(std::size_t n) { return Container.at(n); }
			const boundary_type& at(std::size_t n)const{ return Container.at(n); }
		};
		template<typename state_type, typename boundary>
		auto make_boundary_container(const state_type& x, boundary b) {
			return boundary_container<typename std::decay<state_type>::type, typename std::decay<boundary>::type>(std::move(b));
		}
	}
}
#
#endif
