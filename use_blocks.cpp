#include<iostream>
#include"blocks.hpp"
#include"block_vector.hpp"
int main() {
	hmLib::block_vector<double> BVec(16);
	hmLib::blocks<double,int, hmLib::default_block_hasher<4,int>> Blks;


	
	return 0;
}