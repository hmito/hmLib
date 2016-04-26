#include<vector>
#include<hmLib/lattice.hpp>

int main(){
	constexpr unsigned int LatSize = 100; 
	std::vector<double> Vec(LatSize*LatSize);
	
	auto Lattice = make_lattice(Vec.begin(),LatSize,LatSize);
	
	Lattice.at(0,2);
	Lattice.size<0>();
	auto dtr=Lattice.point();
	dtr.x++;
	dtr.y--;

	//座標管理クラスなので、itrの指す座標はLattice上でどこかを知ることもできるはず
	Lattice.advance(itr,-2,-2);
	Lattice.next(itr,-2,-2);
	Lattice.index(itr);

	for(auto itr = Lattice.begin(); itr != Lattice.end(); ++itr){
		Lattice.sub_lattice(itr, -2, -2, 5, 5);
	}
	
	return 0;
}
