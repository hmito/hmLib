#ifndef INC_HMLIB_HMRAND_CPP
#define INC_HMLIB_HMRAND_CPP 204
#ifndef INC_HMLIB_HMRAND
#	include "hmLib_v2/hmRand.hpp"
#endif
namespace{
	hmRand Rand;
}
namespace hm{
	int rand(int min,int max){return rand(Rand,min,max);}
	int rand_32(){return rand_32(Rand);}
	uint rand_u32(){return rand_u32(Rand);}
	dlong rand_64(){return rand_64(Rand);}
	udlong rand_u64(){return rand_u64(Rand);}
	double randf_ee(double min,double max){return randf_ee(Rand,min,max);}
	double randf_ue(double min,double max){return randf_ue(Rand,min,max);}
	double randf_eu(double min,double max){return randf_eu(Rand,min,max);}
	double randf_uu(double min,double max){return randf_uu(Rand,min,max);}
	double randNorm(double ave,double sd){return randNorm(Rand,ave,sd);}
	hmRand& getGlobalRand(){return Rand;}
}
#endif
