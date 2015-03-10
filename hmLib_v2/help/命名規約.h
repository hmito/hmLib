namespace /*===命名規約===*/c1{
	namespace /***** 宣言 定数 列挙型 *****/s1{
		//全大文字、アンダーバー区切り
		#define DEVICE_NUM 29

		//"USE_","VER_","CHG_","INC_"はhmLib予約文字
	}
	namespace /***** 変数 *****/s2{
		//大域変数は大文字スタート、大文字区切り
		int MainStream;
		char GetStr;	

		//局所変数は小文字名詞スタート、大文字区切り
		int i;
		char modeType;

		//const変数は全大文字、アンダーバー区切り
		const double PI=3.1415;
		const int NAME_NUM 300;

		//意味的に大きな区切りがある場合は、アンダーバーで区切る
		//（C++では非推奨、クラス/名前空間推奨）
		const bool MainModeFlag_Jpeg;

		//クラスメンバ関数の引数名は"x_"でスタート
		void setPos(char x_Pos){Pos=x_Pos;}

		//ポインタは必要に応じて'p'、参照は必要に応じて'r'スタート
		char* pBuf;
		string& rName;
	}
	namespace /***** 関数 *****/s3{
		//小文字動詞スタート、大文字区切り
		int getPos();
		char changeModeType();

		//動詞、助動詞が続く場合は、アンダーバーで区切る
		int check_openDoor();
		bool can_getSize();

		//明らかな場合は、名詞の動詞化利用が可能
		string str();
		size_type size();

		//意味的に大きな区切りがある場合は、アンダーバーで区切る
		//（C++では非推奨、クラス/名前空間推奨）
		void setSize_Jpeg();
	
		//型変換関数の場合は"to_","_to_"
		hmr::gps to_gps();
		hmr::double gps_to_f();

		//bool関数には、等価"is_",可能"can_",完了"has_"の使用
		bool can_recv();
		bool is_err();
		bool has_changed();
	}
	namespace /***** 名前空間 *****/s4{
		//数文字程度の小文字
		namespace srt{
		};
	}
	namespace /***** クラス *****/s5{
		//STL型、C++ライク型は、小文字スタート、アンダーバー区切り
		class csv_file;
		class dxstream;

		//通常クラスは大文字スタート、大文字区切り
		class ImGraph;
		class DrawMode;

		//動詞の人称型を使用
		class BmpCreater;
		class GraphViewer;

		//基本クラスの場合は"basic"
		class basicGraph;
		class basic_dxstream;

		//抽象クラスの場合は"itf"
		class itfObject;
		class itf_filetr;

		//例外クラスの場合は"except"
		class except_overrange;
	}
	namespace /***** 単語使用 *****/s6{
		//set:get
			//値の変更、取得
			//保持しているパラメータの変更、設定時
		//put:get
			//値の置換、取得
			//保持しているデータの変更
		//make
			//戻り値として、ある設定のオブジェクトを返す
		//create:release
			//オブジェクト自身の作成、解放
		//read:write
			//データを読み込み、書き出し
		//load:save
			//設定の読み込み、書き出し
		//open:save:close
			//ファイルオブジェクトを開く、保存、閉じる
	}
}
namespace /*===hmLib構成===*/c2{
	namespace /***** ファイル *****/s1{
		//ファイル形式
		//.hpp=標準include用ファイル
		//_vX_XX.hpp=内部用バージョンファイル　単体
		//_vX_XX.h=内部用バージョンファイル　cppとセット
		//_vX_XX.cpp=内部用バージョンファイル　hとセット

		//予約定義語
		//VER_XXXX = 現在使用可能なファイルバージョン
		//USE_XXXX = 使用を宣言するファイルバージョン
		//CHG_XXXX = 使用変更予定のファイルバージョン
		//INC_XXXX = 実際使用されたファイルバージョン
		#define USE_HMLIB 20904		//基本的にv2_09_04を使用
		#define USE_HMBASIC 107		//例外的にhmBasicのみv1_07を使用
		#define CHG_HMLIB 20904		//hmLibを変更予定(この場合は、基本は現状のままv2_09_04使用)
		#define CHG_HMOBJECT 103	//hmObjectについてはv1_03へ変更予定
	}
	//継承ファイルは、より強く制限する型名を引き継ぐ

	//準構成メンバはlib/subフォルダ内におく

	//percial includeが可能となる
}