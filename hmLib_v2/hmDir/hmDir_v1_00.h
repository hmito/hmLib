#ifndef INC_HMLIB_HMDIR
#define INC_HMLIB_HMDIR 100
#ifndef INC_HMLIB_HMBASIC
#	include"hmBasic.hpp"
#endif
#include <direct.h>
#include <string>
#include <vector>
class hmDir;
namespace hm{
	hmDir getCDir();
	bool setCDir(const hmDir& dir);
}
class hmDir{
private:
	std::vector<std::string> pass;
public:
	hmDir(){return;}
	hmDir(const hmDir& _dir){pass=_dir.pass;}
	std::string operator()(const std::string& _name)const{
		std::string ans;
		for(uint i=0;i<pass.size();i++){
			ans+=pass.at(i);
			ans+="\\";
		}
		ans+=_name;
		return ans;
	}
	const hmDir& operator=(const hmDir& _dir){
		if(this!=&_dir)pass=_dir.pass;
		return *this;
	}
	const hmDir& operator+=(const std::string& _name){
		pass.push_back(_name);
		return *this;
	}
	const hmDir& operator-=(uint _num){
		for(uint i=0;i<_num && pass.size()>0;i++)pass.pop_back();
		return *this;
	}
	const hmDir& operator--(){
		if(pass.size())pass.pop_back();
		return *this;
	}
	hmDir operator--(int){
		hmDir ans(*this);
		if(pass.size())pass.pop_back();
		return ans;
	}
	bool up(){
		if(pass.size()){
			pass.pop_back();
			return check();
		}else{
			return true;
		}
	}
	bool down(const std::string& _name){
		pass.push_back(_name);
		if(check()){
			pass.pop_back();
			return true;
		}else{
			return false;
		}
	}
	bool operator==(const hmDir& _dir)const{return pass==_dir.pass;}
	bool operator!=(const hmDir& _dir)const{return pass!=_dir.pass;}
	hmDir operator+(const std::string& _name)const{
		hmDir ans(*this);
		ans+=_name;
		return ans;
	}
	hmDir operator-(uint _num)const{
		hmDir ans(*this);
		ans-=_num;
		return ans;
	}
	bool check()const;
	bool make(const std::string& _name)const;
	bool del(const std::string& _name)const;
	bool make_down(const std::string& _name){
		if(make(_name))return true;
		return down(_name);
	}
	bool del_up(){
		std::string str=pass.back();
		if(up())return true;
		return del(str);
	}
	std::string str()const{
		std::string ans;
		for(uint i=0;i<pass.size();i++){
			if(i!=0)ans+="\\";
			ans+=pass.at(i);
		}
		return ans;
	}
	uint level()const{return pass.size();}
	std::vector<std::string> list()const;
};
#endif
