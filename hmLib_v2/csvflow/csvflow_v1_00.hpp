#ifndef INC_HMLIB_CSVFLOW
#define INC_HMLIB_CSVFLOW 100
#
#ifndef INC_HMLIB_HMPOS
#	include "hmLib_v2/hmPos.hpp"
#endif
#include <sstream>
namespace hmstd{
	class csvflowType{
	protected:
		char sepC;
		char strC;
		char endC;
		Pint pos;
	public:
		csvflowType(char _sepC=',',char _strC='"',char _endC='\n'){
			setC(_sepC,_strC,_endC);
		}
		void setC(const csvflowType& _bcsvf){
			sepC=_bcsvf.sepC;
			strC=_bcsvf.strC;
			endC=_bcsvf.endC;
			pos=_bcsvf.pos;
		}
		void setC(char _sepC=',',char _strC='"',char _endC='\n'){
			sepC=_sepC;
			strC=_strC;
			endC=_endC;
			pos=Pint(0,0);
		}
	};
	template<class IStream>
	class icsvflow_:virtual public csvflowType{
	protected:
		IStream* isp;
		uint lastlng;
	public:
		icsvflow_<IStream>(char _sepC=',',char _strC='"',char _endC='\n'):basic_csvflow(_sepC,_strC,_endC){
			isp=0;
			lastlng=0;
		}
		icsvflow_<IStream>(IStream& _is,char _sepC=',',char _strC='"',char _endC='\n'){open(_is,_sepC,_strC,_endC);}
		icsvflow_<IStream>(const icsvflow_<IStream>& _icsvf){copy(_icsvf);}
		virtual ~icsvflow_<IStream>(){close();}
		void open(IStream& _is,char _sepC=',',char _strC='"',char _endC='\n'){
			isp=&_is;
			setC(_sepC,_strC,_endC);
			lastlng=0;
		}
		void copy(const icsvflow_<IStream>& _icsvf){
			isp=_icsvf.isp;
			setC(_icsvf);
		}
		icsvflow_<IStream>& operator=(const icsvflow_<IStream>& _icsvf){
			if(this==&_icsvf)return *this;
			copy(_icsvf);
			return *this;
		}
		void close(){isp=0;}
		std::string get(){
			std::string buf;
			char tmp;
			char strFlag=2;	//0:strC–³Œø 1:strC“à•” 2:strCŠO•”
			if(strC<0)strFlag=0;
			lastlng=0;
			tmp=(char)isp->get();
			while(isp->eof()==0){
				lastlng++;
				if(strFlag==1){
					if(tmp==strC)strFlag=2;
					else buf+=tmp;
				}else{
					if(tmp==strC&&strFlag==2){
						strFlag=1;
						continue;
					}
					if(tmp==sepC){
						pos.x+=1;
						return buf;
					}
					if(tmp==endC){
						pos.y+=1;
						pos.x=0;
						return buf;
					}
					buf+=tmp;
				}
				tmp=(char)isp->get();
			}
			return buf;
		}
		template<class T>
		icsvflow_<IStream>& get(T& buf){
			std::stringstream strs(get());
			strs>>buf;
			return *this;
		}	
		icsvflow_<IStream>& ignore(int num=1){
			char tmp;
			char strFlag;//0:strC–³Œø 1:strC“à•” 2:strCŠO•”
			
			for(int i=0;i<num;i++){
				strFlag=2;
				if(strC<0)strFlag=0;
				lastlng=0;
				while(isp->eof()==0){
					tmp=(char)isp->get();
					lastlng++;
					if(strFlag==1){
						if(tmp==strC)strFlag=2;
					}else{
						if(tmp==strC&&strFlag==2){
							strFlag=1;
							continue;
						}
						if(tmp==sepC){
							pos.x+=1;
							break;
						}
						if(tmp==endC){
							pos.y+=1;
							pos.x=0;
							break;
						}
					}
				}
			}
			return *this;
		}
		icsvflow_<IStream>& ignore(int num,const std::string& term){
			for(int i=0;i<num;i++){
				if(term==get())return *this;
			}
			return *this;
		}
		icsvflow_<IStream>& ignoreline(int num=1){
			for(int i=0;i<num;i++){
				while(isp->eof()==0){
					ignore();
					if(pos.x==0)break;
				}
			}
			return *this;
		}
		icsvflow_<IStream>& unget(){
			isp->seekg(-1*(int)lastlng,std::ios_base::cur);
		}
		std::string peek(){
			std::string tmp=get();
			unget();
			return tmp;
		}
		Pint tellg(){return pos;}
		bool tellendl(){return pos.x==0;}
		template<class T>
		icsvflow_<IStream>& operator>>(T& buf){
			get(buf);
			return *this;
		}
		bool eof(){return isp->eof();}
		bool eol(){return pos.x==0;}
	};
	template<class OStream>
	class ocsvflow_:virtual public csvflowType{
		friend ocsvflow_<OStream>& endl(ocsvflow_<OStream>& ocsvf){
			ocsvf.putendl();
			return ocsvf;
		}
	protected:
		OStream* osp;
	public:
		ocsvflow_<OStream>(char _sepC=',',char _strC='"',char _endC='\n'):basic_csvflow(_sepC,_strC,_endC){osp=0;}
		ocsvflow_<OStream>(OStream& _os,char _sepC=',',char _strC='"',char _endC='\n'){open(_os,_sepC,_strC,_endC);}
		ocsvflow_<OStream>(const ocsvflow_<OStream>& _ocsvf){copy(_ocsvf);}
		virtual ~ocsvflow_<OStream>(){close();}
		void open(OStream& _os,char _sepC=',',char _strC='"',char _endC='\n'){
			osp=&_os;
			setC(_sepC,_strC,_endC);
		}
		void copy(const ocsvflow_<OStream>& _ocsvf){
			osp=_ocsvf.osp;
			setC(_ocsvf);
		}
		ocsvflow_<OStream>& operator=(const ocsvflow_<OStream>& _ocsvf){
			if(this==&_ocsvf)return *this;
			copy(_ocsvf);
			return *this;
		}
		void close(){osp=0;}
		ocsvflow_<OStream>& putstr(std::string str){
			if(pos.x!=0)*osp<<sepC;
			*osp<<str;
			pos.x++;
			return *this;
		}
		template<class T>
		ocsvflow_<OStream>& put(const T& data){
			std::stringstream strs;
			char flag=0;
			if(strC>=0&&
				typeid(T)!=typeid(int)&&
				typeid(T)!=typeid(uint)&&
				typeid(T)!=typeid(double)&&
				typeid(T)!=typeid(float))
				flag=1;
			if(flag)strs<<strC;
			strs<<data;
			if(flag)strs<<strC;
			putstr(strs.str());
			return *this;
		}
		ocsvflow_<OStream>& putendl(){
			*osp<<endC;
			pos.x=0;
			pos.y+=1;
			return *this;
		}
		template<class T>
		ocsvflow_<OStream>& operator<<(const T& data){
			put(data);
			return *this;
		}
		ocsvflow_<OStream>& operator<<(ocsvflow_<OStream>& (*p_ocsvflow_MF)(ocsvflow_<OStream>&)){return (*p_ocsvflow_MF)(*this);}
	};
	template<class IOStream>
	class csvflow_:public icsvflow_<IOStream>,public ocsvflow_<IOStream>{
	public:
		csvflow_<IOStream>(char _sepC=',',char _strC='"',char _endC='\n')
			:icsvflow_<IOStream>(_sepC,_strC,_endC),ocsvflow_<IOStream>(_sepC,_strC,_endC){return;}
		csvflow_<IOStream>(IOStream& _ios,char _sepC=',',char _strC='"',char _endC='\n')
			:icsvflow_<IOStream>(_ios,_sepC,_strC,_endC),ocsvflow_<IOStream>(_ios,_sepC,_strC,_endC){return;}
		csvflow_<IOStream>(const csvflow_<IOStream>& _csvf)
			:icsvflow_<IOStream>(_csvf),ocsvflow_<IOStream>(_csvf){return;}
		const csvflow_<IOStream>& operator=(const csvflow_<IOStream>& _csvios){
			if(this!=&_csvios)copy(_csvios);
			return *this;
		}
		void open(IOStream& _ios,char _sepC=',',char _strC='"',char _endC='\n'){
			icsvflow_<IOStream>::open(_ios,_sepC,_strC,_endC);
			ocsvflow_<IOStream>::open(_ios,_sepC,_strC,_endC);
		}
		void copy(const csvflow_<IOStream>& _csvf){
			icsvflow_<IOStream>::copy(_csvf);
			ocsvflow_<IOStream>::copy(_csvf);
		}
		void close(){
			icsvflow_<IOStream>::close();
			ocsvflow_<IOStream>::close();
		}
	};
	typedef icsvflow_<std::istream> icsvflow;
	typedef ocsvflow_<std::ostream> ocsvflow;
	typedef csvflow_<std::iostream> csvflow;
}
#
#endif
