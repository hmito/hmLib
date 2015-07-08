#include<vector>
#include<string>
#include<iostream>
//全体構造を構成するファイル
//	ライン配列の要素がセル配列
//	読み込み高速、ジャギーなcsvにも対応
//	列取得は遅い
class lined_cellset{
private:
	std::vector<std::vector<std::string>> Set;
public:
	lined_cellset(){}
	unsigned int line_num()const{ return Set.size(); }

};

//全体構造を構成するコンテナ
//	列単位で管理されたセルセット
//	列ごとに名前を付けることが可能
//	ある行のデータへのアクセスには、ランダムアクセスが必要
//	読み込みは遅い
class column_cellset;

//位置読み込み
//	位置情報のみを取得する
//	読み込みは一番早い？
//	データを読み込まない
class cellset_map;


//セル単位読み込み
//随時読み込み
//output_iterator
template<typename T,char sep>
class ocell_iterator;
template<typename T>
using ocsv_iterator = ocell_iterator <T, ',' >;
template<typename T>
using otsv_iterator = ocell_iterator <T, '\t' >;


template<typename T, char sep>
class icell_iterator{
private:
	using my_type = icell_iterator < T, sep > ;
private:
	std::istream* pin;
public:
	icell_iterator() :pin(nullptr){}
	icell_iterator(std::istream& in_) :pin(&in_){}
	icell_iterator(const icell_iterator& x) = default;
	T operator*();
	T* operator->();
	my_type& operator++();
	my_type operator++(int){
		icell_iterator ans(*this);
		operator++();
		return ans;
	}
};
template<typename T>
using icsv_iterator = icell_iterator <T, ',' > ;
template<typename T>
using itsv_iterator = icell_iterator <T, '\t' >;
