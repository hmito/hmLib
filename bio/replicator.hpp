#ifndef HMLIB_BIO_REPLICATOR_INC
#define HMLIB_BIO_REPLICATOR_INC 100
#
#include <iterator>
#include <ranges>
#include <random>
#include <numeric>
#include "../algorithm/sampling.hpp"
namespace hmLib{ namespace bio {
	template<typename mutator_type>
	struct linew_replicator {
	private:
		mutator_type Mutator;
	public:
		linew_replicator(mutator_type Mutator_) :Mutator(Mutator_) {}
		template<std::ranges::random_access_range player_range,
				 std::ranges::random_access_range fitness_range,
				 std::output_iterator<std::ranges::range_value_t<player_range>> next_players_iterator,
				 std::uniform_random_bit_generator RandEngine>
		void operator()(player_range&& players, fitness_range&& fitness, next_players_iterator next_players_outitr, RandEngine& Engine)const {
			for(auto& fitness_value : fitness){
				if(fitness_value < 0)fitness_value = 0;
			}
			auto Sampler = hmLib::make_roulette_sampler(std::ranges::begin(players), std::ranges::end(players), std::ranges::begin(fitness), std::ranges::end(fitness));

			for(unsigned int i=0; i < std::ranges::size(players); ++i){
				*next_players_outitr = Mutator(*Sampler(Engine), Engine);
				++next_players_outitr;
			}
		}
	};
	template<typename mutator_type>
	struct expw_replicator {
	private:
		double alpha;
		mutator_type Mutator;
	public:
		expw_replicator(double alpha_, mutator_type Mutator_) :alpha(alpha_), Mutator(Mutator_) {}
		template<std::ranges::random_access_range player_range,
				 std::ranges::random_access_range fitness_range,
				 std::output_iterator<std::ranges::range_value_t<player_range>> next_players_iterator,
				 std::uniform_random_bit_generator RandEngine>
		void operator()(player_range players, fitness_range fitness, next_players_iterator next_players_outitr, RandEngine Engine)const {
			double meanv = std::reduce(std::ranges::begin(fitness), std::ranges::end(fitness), 0.0) / std::ranges::size(fitness);
			for(auto& fitness_value : fitness){
				if(fitness_value < 0)fitness_value = 0;
			}
			auto Sampler = hmLib::make_roulette_sampler(std::ranges::begin(players), std::ranges::end(players), std::ranges::begin(fitness), std::ranges::end(fitness));

			for(unsigned int i=0; i < std::ranges::size(players); ++i){
				*next_players_outitr = Mutator(*Sampler(Engine), Engine);
				++next_players_outitr;
			}
		}
	};

} }

#endif