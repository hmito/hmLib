#ifndef INC_HMLIB_HMBYTE
#define INC_HMLIB_HMBYTE 101
#
#ifndef INC_HMLIB_HMBASIC
#	include "hmLib_v2/hmBasic.hpp"
#endif
#include <deque>
//manipulater関連
namespace hmstd{
	class bytetype{
		uint byte;
	public:
		bytetype(uint _byte=1){byte=_byte;}
	private:
		std::ostream& operator()(std::ostream& ros) {
			return ros<<std::hex<<std::setw(2*byte)<<std::right<<std::setfill('0');
		}
		friend std::ostream& operator <<(std::ostream& ros,bytetype _bytet){
			return _bytet(ros);
		}
	};
}
class Byte{
private:
	uchar c;
public:
	Byte(){c=0;}
	Byte(char _c){c=static_cast<uchar>(_c);}
	Byte(uchar _c){c=_c;}
	Byte(int _c){c=static_cast<uchar>(_c);}
	Byte(uint _c){c=static_cast<uchar>(_c);}
	const Byte& operator=(const Byte& _b){
		if(&_b!=this)c=_b.c;
		return *this;
	}
	const Byte& operator+=(const Byte& _b){
		c+=_b.c;
		return *this;
	}
	const Byte& operator-=(const Byte& _b){
		c-=_b.c;
		return *this;
	}
	const Byte& operator*=(const Byte& _b){
		c*=_b.c;
		return *this;
	}
	const Byte& operator/=(const Byte& _b){
		c/=_b.c;
		return *this;
	}
	const Byte& operator%=(const Byte& _b){
		c%=_b.c;
		return *this;
	}
	const Byte& operator&=(const Byte& _b){
		c&=_b.c;
		return *this;
	}
	const Byte& operator|=(const Byte& _b){
		c|=_b.c;
		return *this;
	}
	const Byte& operator^=(const Byte& _b){
		c^=_b.c;
		return *this;
	}
	const Byte& operator<<=(const Byte& _b){
		c<<=_b.c;
		return *this;
	}
	const Byte& operator>>=(const Byte& _b){
		c>>=_b.c;
		return *this;
	}
	Byte& operator++(){
		++c;
		return *this;
	}
	Byte& operator--(){
		--c;
		return *this;
	}
	Byte operator~()const{
		return Byte(~c);
	}
	Byte operator+(const Byte& _b)const{
		return Byte(c+_b.c);
	}
	Byte operator-(const Byte& _b)const{
		return Byte(c-_b.c);
	}
	Byte operator*(const Byte& _b)const{
		return Byte(c*_b.c);
	}
	Byte operator/(const Byte& _b)const{
		return Byte(c/_b.c);
	}
	Byte operator%(const Byte& _b)const{
		return Byte(c%_b.c);
	}
	Byte operator&(const Byte& _b)const{
		return Byte(c&_b.c);
	}
	Byte operator|(const Byte& _b)const{
		return Byte(c|_b.c);
	}
	Byte operator^(const Byte& _b)const{
		return Byte(c^_b.c);
	}
	Byte operator<<(const Byte& _b)const{
		return Byte(c<<_b.c);
	}
	Byte operator>>(const Byte& _b)const{
		return Byte(c>>_b.c);
	}
	Byte operator++(int){
		Byte tmp(*this);
		++c;
		return tmp;
	}
	Byte operator--(int){
		Byte tmp(*this);
		--c;
		return tmp;
	}
	bool operator==(const Byte& _b)const{
		return c==_b.c;
	}
	bool operator!=(const Byte& _b)const{
		return c!=_b.c;
	}
	bool operator<(const Byte& _b)const{
		return c<_b.c;
	}
	bool operator>(const Byte& _b)const{
		return c>_b.c;
	}
	bool operator<=(const Byte& _b)const{
		return c<=_b.c;
	}
	bool operator>=(const Byte& _b)const{
		return c>=_b.c;
	}
	bool bittest(char num){
		return hmstd::bittest(c,num);
	}
	void bitput(char num,bool value){
		hmstd::bitput(c,num,value);
	}
	void biton(char num){
		hmstd::biton(c,num);
	}
	void bitoff(char num){
		hmstd::bitoff(c,num);
	}
	friend std::ostream& operator<<(std::ostream& ost,Byte& b){
		return ost<<b.str();
	}
	std::string str(){
		return hmstd::fStr("%02x",c);
	}
	std::string bin(){
		std::string Ans;
		Ans.push_back(c);
		return Ans;
	}
	void set(uchar val){c=val;}
	uchar getuc(){return c;}
	char getc(){return static_cast<char>(c);}
};
class Bytes{
private:
	std::deque<Byte> buf;
public:
	Bytes(){return;}
	Bytes(const Bytes& _bs){buf=_bs.buf;}
	template<class T>
	Bytes(T _t){set(_t);}
	Byte& operator[](uint i){
		if(i>=buf.size())throw "Bytes::operator[] out of range";
		else return buf[i];
	}
	const Bytes& operator=(const Bytes& _bs){
		if(this!=&_bs)buf=_bs.buf;
		return *this;
	}
	const Bytes& operator&=(const Bytes& _bs){
		*this=*this&_bs;
		return *this;
	}
	const Bytes& operator|=(const Bytes& _bs){
		*this=*this|_bs;
		return *this;
	}
	const Bytes& operator^=(const Bytes& _bs){
		*this=*this^_bs;
		return *this;
	}
	const Bytes& operator<<=(uint num){
		*this=*this<<num;
		return *this;
	}
	const Bytes& operator>>=(uint num){
		*this=*this>>num;
		return *this;
	}
	bool operator==(const Bytes& _bs)const{
		if(this->size()!=_bs.size())return false;
		for(uint i=0;i<this->size();i++){
			if(buf[i]!=_bs.buf[i])return false;
		}
		return true;
	}
	bool operator!=(const Bytes& _bs)const{
		if(this->size()!=_bs.size())return true;
		for(uint i=0;i<this->size();i++){
			if(buf[i]!=_bs.buf[i])return true;
		}
		return false;
	}
	Bytes operator~()const{
		Bytes tmp;
		tmp.resize(buf.size());
		for(uint i=0;i<buf.size();i++)tmp[i]=~buf[i];
		return tmp;
	}
	Bytes operator&(const Bytes& _bs)const{
		if(size()!=_bs.size())return *this;
		Bytes tmp(*this);
		for(uint i=0;i<buf.size();i++)tmp.buf[i]=this->buf[i]&_bs.buf[i];
		return tmp;
	}
	Bytes operator|(const Bytes& _bs)const{
		if(size()!=_bs.size())return *this;
		Bytes tmp;
		for(uint i=0;i<buf.size();i++)tmp.buf[i]=this->buf[i]|_bs.buf[i];
		return tmp;
	}
	Bytes operator^(const Bytes& _bs)const{
		if(size()!=_bs.size())return *this;
		Bytes tmp(*this);
		for(uint i=0;i<buf.size();i++)tmp.buf[i]=this->buf[i]^_bs.buf[i];
		return tmp;
	}
	Bytes operator<<(uint num)const{
		if(buf.size()<=0)return *this;
		Bytes tmp(*this);
		uchar val;
		while(num>0){
			Byte tmpb1(0),tmpb2(0);
			if(num>=8)val=8;
			else val=num;
			num-=val;
			for(uint i=0;i<tmp.size();i++){
				tmpb2=tmp[i]>>(8-val);
				tmp[i]<<=val;
				tmp[i]+=tmpb1;
				tmpb1=tmpb2;
			}
		}
		return tmp;
	}
	Bytes operator>>(uint num)const{
		if(buf.size()<=0)return *this;
		Bytes tmp(*this);
		Byte tmpb1(0),tmpb2(0);
		uchar val;
		while(num>0){
			if(num>=8)val=8;
			else val=num;
			num-=val;
			for(uint i=0;i<tmp.size();i++){
				tmpb2=tmp[tmp.size()-1-i]<<(8-val);
				tmp[tmp.size()-1-i]>>=val;
				tmp[tmp.size()-1-i]+=tmpb1;
				tmpb1=tmpb2;
			}
		}
		return tmp;
	}
	void resize(uint i,Byte b=Byte()){buf.resize(i,b);}
	uint size()const{return buf.size();}
	template<class T>
	void set(T& _t){
		resize(sizeof(T));
		const uchar* itr=reinterpret_cast<const uchar*>(&_t);
		for(uint i=0;i<buf.size();i++){
			buf[i]=Byte(*itr);
			itr++;
		}
	}
	template<class T>
	bool get(T& _t){
		if(sizeof(T)!=this->size())return true;
		uchar* itr=reinterpret_cast<uchar*>(&_t);
		for(uint i=0;i<buf.size();i++){
			(*itr)=buf[i].getuc();
			++itr;
		}
		return false;
	}
	bool bittest(uint num){
		if(num/8>buf.size())return false;
		return buf[num/8].bittest(num%8);
	}
	void bitput(uint num,bool value){
		if(num/8>buf.size())return;
		return buf[num/8].bitput(num%8,value);
	}
	void biton(uint num){
		if(num/8>buf.size())return;
		return buf[num/8].biton(num%8);
	}
	void bitoff(uint num){
		if(num/8>buf.size())return;
		return buf[num/8].bitoff(num%8);
	}
	//降順str出力 表示用に空白が表示される。
	friend std::ostream& operator<<(std::ostream& ost,Bytes& bs){
		for(uint i=0;i<bs.size();i++){
			ost<<bs[bs.size()-1-i]<<" ";
		}
		return ost;
	}
	//昇順str出力
	std::string str(bool DownF=false){
		std::string Str;
		for(uint i=0;i<buf.size();i++){
			if(DownF)Str+=buf[buf.size()-1-i].str();
			else Str+=buf[i].str();
		}
		return Str;
	}
	//昇順bin出力
	std::string bin(bool DownF=false){
		std::string Str;
		for(uint i=0;i<buf.size();i++){
			if(DownF)Str+=buf[buf.size()-1-i].bin();
			else Str+=buf[i].bin();
		}
		return Str;
	}	
};
#
#endif
