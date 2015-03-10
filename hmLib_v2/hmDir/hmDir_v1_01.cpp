#ifndef INC_HMLIB_HMDIR_CPP
#define INC_HMLIB_HMDIR_CPP 101
#ifndef INC_HMLIB_HMDIR
#	include "hmLib_v2/hmDir.hpp"
#endif
namespace{
	hmDir getRealCDir(){
		hmDir dir;
		_getcwd(hm::getTmpStrAdress(),hm::getTmpStrAdressSize());
		char* itr=hm::getTmpStrAdress();
		std::string str;
		for(int i=0;i<hm::getTmpStrAdressSize();i++){
			if(*itr==0){
				if(str.size())dir+=str;
				break;
			}else if(*itr=='\\'){
				dir+=str;
				str="";
			}else{
				str.push_back(*itr);
			}
			itr++;
		}
		return dir;
	}
	bool setRealCDir(const hmDir& dir){return _chdir(dir.str().c_str())!=0;}
	hmDir CDir(getRealCDir());
	bool resetRealCDir(){return setRealCDir(CDir);}
}
namespace hm{
	hmDir getCDir(){return CDir;}
	bool setCDir(const hmDir& dir){
		bool ans=setRealCDir(dir);
		if(ans)return true;
		CDir=dir;
		return false;
	}
}
bool hmDir::check()const{
	bool ans=setRealCDir(*this);
	resetRealCDir();
	return ans;
}
bool hmDir::make(const std::string& _name)const{
	setRealCDir(*this);
	bool ans=_mkdir(_name.c_str())!=0;
	resetRealCDir();
	return ans;
}
bool hmDir::del(const std::string& _name)const{
	setRealCDir(*this);
	bool ans=_rmdir(_name.c_str())!=0;
	resetRealCDir();
	return ans;
}
#endif
