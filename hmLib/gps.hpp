#ifndef HMLIB_GPS_INC
#define HMLIB_GPS_INC 100
#
#include<utility>
#include<cstdlib>
#include<cstdint>
#include<exception>
namespace hmLib {
	//data for gps
	struct gps_data {
		bool IsValid;							//有効なデータならtrue
		std::uint32_t GPSTime_mSec;				//0時00分00秒からの経過時間 mSecが1単位
		std::int32_t Latitude_100uMin;			//緯度 100uMinが1単位	Nが+
		std::int32_t Longitude_100uMin;		//経度 100uMinが1単位	Eが+
		std::uint32_t Altitude_10cm;			//高度 100uMinが1単位
	public:
		gps_data()
			: IsValid(false)
			, GPSTime_mSec(0)
			, Latitude_100uMin(0)
			, Longitude_100uMin(0)
			, Altitude_10cm(0) {}
	public:
		//データ無効化
		void invalid() { IsValid=false; }
		//有効データか？
		bool is_valid()const { return IsValid; }
		//GPS取得　時
		unsigned int getHour()const { return (GPSTime_mSec/1000/60/60)%60; }
		//GPS取得　分
		unsigned int getMin()const { return (GPSTime_mSec/1000/60)%60; }
		//GPS取得　秒
		unsigned int getSec()const { return (GPSTime_mSec/1000)%60; }
		//GPS取得　ミリ秒
		unsigned int getMSec()const { return GPSTime_mSec%1000; }
		//Degree単位の緯度
		double getLatitude()const { return Latitude_100uMin/10000/60; }
		//Degree単位の経度
		double getLongitude()const { return Longitude_100uMin/10000/60; }
		//メートル単位の高度
		double getAltitude()const { return Altitude_10cm/10; }
	public:
		//$GPGGA,...で始まる85byte文字列からdata構築
		template<typename input_iterator>
		static gps_data fromGPGGA(input_iterator begin, input_iterator end) {
			if(end-begin>=85)throw("GPGGA data need 85 bytes including \"$GPGGA,...\"");

			input_iterator buf=begin+7;
			gps_data Data;

			Data.GPSTime_mSec=GPSTime_fromHour((buf[0]-'0')*10+(buf[1]-'0'))	//hour
				+GPSTime_fromMin((buf[2]-'0')*10+(buf[3]-'0'))			//min
				+GPSTime_fromSec((buf[4]-'0')*10+(buf[5]-'0'))			//sec
				+(buf[7]-'0')*100+(buf[8]-'0')*10+(buf[9]-'0');			//msec

			Data.Latitude_100uMin=(((buf[11]-'0')*10+(buf[12]-'0'))*60*10000	//d (緯度：度)
				+((buf[13]-'0')*10+(buf[14]-'0'))*10000						//m (緯度：分)
				+((buf[16]-'0')*10+(buf[17]-'0'))*100						//0.01m	(緯度：0.01分)
				+((buf[18]-'0')*10+(buf[19]-'0'))							//0.0001m (緯度：0.0001分)
				)*(buf[21]=='N'? 1: -1);									//N/S (北緯or南緯)

			Data.Longitude_100uMin=(((buf[23]-'0')*100+(buf[24]-'0')*10+(buf[25]-'0'))*60*10000	//経度　度
				+((buf[26]-'0')*10+(buf[27]-'0'))*10000										//経度 分
				+((buf[29]-'0')*10+(buf[30]-'0'))*100										//経度 0.01分
				+((buf[31]-'0')*10+(buf[32]-'0'))											//経度 0.0001分
				)*(buf[34]=='E'? 1: -1);													//E/W (東経or西経)

			Data.IsValid=((buf[36]-'0')==0);

			Data.Altitude_10cm=(((buf[46]-'0')*10+(buf[47]-'0'))*10000
				+((buf[48]-'0')*10+(buf[49]-'0'))*100
				+((buf[50]-'0')*10+(buf[52]-'0'))
				);
			return Data;
		}
		//$GPGGA,で始まる85byte文字列をdataから復元（一部自動補完）
		template<typename output_iterator>
		static void toGPGGA(gps_data& data_, output_iterator oitr) {
			*oitr++='$';
			*oitr++='G';
			*oitr++='P';
			*oitr++='G';
			*oitr++='G';
			*oitr++='A';
			*oitr++=',';

			*oitr++=(getHour()/10)+'0';
			*oitr++=(getHour()%10)+'0';
			*oitr++=(getMin()/10)+'0';
			*oitr++=(getMin()%10)+'0';
			*oitr++=(getSec()/10)+'0';
			*oitr++=(getSec()%10)+'0';
			*oitr++='.';
			*oitr++=((getMSec()/100)%10)+'0';
			*oitr++=((getMSec()/10)%10)+'0';
			*oitr++=((getMSec())%10)+'0';
			*oitr++=',';

			*oitr++=(std::abs(Latitude_100uMin/60/10000/10)%10)+'0';
			*oitr++=(std::abs(Latitude_100uMin/60/10000)%10)+'0';
			*oitr++=(std::abs(Latitude_100uMin/10000/10)%10)+'0';
			*oitr++=(std::abs(Latitude_100uMin/10000)%10)+'0';
			*oitr++='.';
			*oitr++=(std::abs(Latitude_100uMin/1000)%10)+'0';
			*oitr++=(std::abs(Latitude_100uMin/100)%10)+'0';
			*oitr++=(std::abs(Latitude_100uMin/10)%10)+'0';
			*oitr++=(std::abs(Latitude_100uMin)%10)+'0';
			*oitr++=',';
			*oitr++=(Latitude_100uMin>0? 'N': 'S');
			*oitr++=',';

			*oitr++=(std::abs(Longitude_100uMin/60/10000/100)%10)+'0';
			*oitr++=(std::abs(Longitude_100uMin/60/10000/10)%10)+'0';
			*oitr++=(std::abs(Longitude_100uMin/60/10000)%10)+'0';
			*oitr++=(std::abs(Longitude_100uMin/10000/10)%10)+'0';
			*oitr++=(std::abs(Longitude_100uMin/10000)%10)+'0';
			*oitr++='.';
			*oitr++=(std::abs(Longitude_100uMin/1000)%10)+'0';
			*oitr++=(std::abs(Longitude_100uMin/100)%10)+'0';
			*oitr++=(std::abs(Longitude_100uMin/10)%10)+'0';
			*oitr++=(std::abs(Longitude_100uMin)%10)+'0';
			*oitr++=',';
			*oitr++=(Longitude_100uMin>0? 'E': 'W');
			*oitr++=',';

			*oitr++=(is_valid()? '0': '1');
			*oitr++=',';
			*oitr++='0';
			*oitr++='3';
			*oitr++=',';
			*oitr++='0';
			*oitr++='1';
			*oitr++='.';
			*oitr++='0';
			*oitr++=',';

			*oitr++=((Altitude_10cm/100000)%10)+'0';
			*oitr++=((Altitude_10cm/10000)%10)+'0';
			*oitr++=((Altitude_10cm/1000)%10)+'0';
			*oitr++=((Altitude_10cm/100)%10)+'0';
			*oitr++=((Altitude_10cm/10)%10)+'0';
			*oitr++='.';
			*oitr++=((Altitude_10cm)%10)+'0';
			*oitr++=',';
			*oitr++='M';
			*oitr++=',';

			*oitr++='0';
			*oitr++='0';
			*oitr++='0';
			*oitr++='0';
			*oitr++='.';
			*oitr++='0';
			*oitr++=',';
			*oitr++='M';
			*oitr++=',';
			*oitr++='0';
			*oitr++='0';
			*oitr++='0';
			*oitr++='.';
			*oitr++='0';
			*oitr++=',';
			*oitr++='0';
			*oitr++='0';
			*oitr++='0';
			*oitr++='0';
			*oitr++='*';
			*oitr++='-';	//check sum
			*oitr++=0x0d;
			*oitr++=0x0a;
		}
	public:
		//GPSTimeを時、分、秒から作成
		static std::uint32_t GPSTime_fromHour(unsigned int Hour_) { return Hour_*60*60*1000; }
		static std::uint32_t GPSTime_fromMin(unsigned int Min_) { return Min_*60*1000; }
		static std::uint32_t GPSTime_fromSec(unsigned int Sec_) { return Sec_*1000; }
	};
}
#
#endif
