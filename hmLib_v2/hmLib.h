/*
////////////////////////
        hmLib        
////////////////////////
v3_00_00
	ライブラリ規約変更
		percial_include機能追加
			hmLibバージョンが異なるファイルを共存インクルード可能に
			#define USE_XXX VerNo と記述することで、XXXファイルのバージョンを指定可能
			プログラムが古いhmLibに依存しているが、主要部に影響のない追加機能/バグ修正を適用したい場合に使用
		出力表示変更
			hmLibからのメッセージ出力のグレードを変更可能
			#include OUT_USE_HMLIB Lv あるいは OUT_CHG_HMLIB Lv と記述することで、出力設定
			Lv	0:何も出力しない(fatal errorは強制的に表示)
				1:errorレベルまで表示　主たる動作が予期せぬ動作を見せる、深刻な修正が存在する場合。
				2:warningレベルまで表示　主たる動作に影響はないが状況に応じて問題となる、通常の修正が存在する場合。
				3:infoレベルまで表示　動作に問題はないが、修正が存在する場合。
		upgrade_check機能の変更
			ファイルのバージョンアップの際の表示を、新たに統一
			#define CHG_HMLIB VerNo と記述することで、hmLib全体のバージョン変更
			#define CHG_XXXXX VerNo と記述することで、XXXXXファイルのみのバージョン変更を指定可能 同時に複数記載可能/CHG_HMLIBとの併用も可能
	hmBasic.hpp廃止
		基本となるファイルを統合してしまうと、一部の変更がライブラリ全体に及ぼす影響が大きいため
		提供されていた大部分の機能は、複数の後継ファイルへ継承される
			hmStr 文字列関連
			hmAlgorithm 数値処理関連
			hmType 型関連
			hmPrcTime 時間関連
			hmBit bit処理関連
	hmStreamBuf_v1_02.hpp 
		hmLib_v3に対応
			namespace hmLibに対応
		class名を変更
			basic_buf_streambuf : バッファ付ストリームバッファ基底クラス
			basic_nonbuf_streambuf : バッファ無ストリームバッファ基底クラス
		string系関数をcharに変更
	hmType_v1_00.hpp
		hmBasic後継ファイルの一つ
		環境非依存のタイプが使用可能
			プレフィックス
				s signed
				u unsigned
				f float
			typename
				byte 8bit
				short 16bit
				int 32bit
				long 64bit
				llong 128bit
				float 32bit
				double 64bit
				ldouble 128bit
		PIの宣言
		使用には、環境の型サイズを記録したhmTypeSize.hppが必要
			make_hmTypeSize.c(exeファイル有)の実行で自動生成可能
	hmRand_v2_05.h/cpp
		includeファイルをv3_00_00へ適合
		クラス名をhm::Randに変更
		標準Randインスタンス、GlbRandを追加
			getGlbRandは廃止
	hmStr_v1_00.h/cpp
		hmBasic後継ファイルの一つ
		文字列変換関数群
			書式文字列を文字列に変換するtoStr,toCStr
			オブジェクトを文字列に変換するcast_str,cast_cstr
	hmPrcTime_v1_00.hpp
		hmBasic後継ファイルの一つ
		時間制御関数群
			プロセスタイムを取得
			inline u_int prctime();
			ストップウォッチを動かす Bufを変えることで、異なるストップウォッチとなる。
			inline u_int stopwatch(u_int &Buf);
			一時停止用関数
			inline void delay(u_int ms);
	hmGate_v1_00.hpp
		gatestreamの後継ファイル
		itfGateのbasic_nonbuf_streambuf準拠
			Gateさえあれば、簡単にstream化可能
	hmBit_v1_00.hpp
		hmBasic後継ファイルの一つ
		bit演算用関数群
			bittest:指定bitの値取得
			bitput:指定bitを指定の値に
			bitset:指定bitをon
			bitclear:指定bitをoff
			bitxor:指定bitをxor
	hmAlgorithm_v1_00.hpp
		hmBasic後継ファイルの一つ
		数値algorithm関数群
			sort_2,sort_3 2,3変数のソート
			max_3,med_3,min_3 3変数の各順位取り出し
			positive_mod 負を出さない余り 正数の間でループ
			sel 条件式の真偽で戻り値が変わる関数
			hm::rourette weight_acsで重みづけしたルーレット選択
	basic_operator_v1_01.hpp
		hmLib_v3仕様に対応
		class basic_operator内にすべてのクラスを以降
		一部のoperatorの実装とbasic_operatorクラスの継承だけで、複数のoperatorが使用可能
		以下、使用可能operator []内は、内部での実装内容
			++X & incriment = X++
			--X & decriment = X--
			X+=Y & forward_add = X+Y
			X+=Y & bi_add = X+Y,Y+X
			X+=X & add = X+X
			X-=Y & forward_sub = X-Y
			X-=Y & -X & bi_sub = X-Y,Y-X[-(X-Y)]
			X-=X & sub = X-X
			X==Y & forward_equal = X!=Y
			X==Y & bi_equal = X!=Y,Y==X,Y!=X
			X==X & equal = X!=X
			X<Y & forward_compare = X>Y
			X<Y & bi_compare = X>Y,Y<X,Y>X
			X<X & compare = X>X
			X<Y & X==Y & forward_equalcompare = X>Y,X!=Y,X<=Y,X>=Y
			X<Y & X==Y & bi_equalcompare = X>Y,Y<X,Y>X,X!=Y,Y==X,Y!=X,X<=Y,X>=Y,Y<=X,Y>=X
			X<X & X==X & equalcompare =  X>X,X!=Y,X<=X,X>=X
	basic_iterator_v1_01.hpp
		iterator基本セットを大幅簡略化。basic_operator依存ファイル化。
		機能を整理
			iteratorタグを調べる、hm::iterator_tag_discriminater
			必要なtypedef,basic_operatorがすでに定義してある、basic_iterator群
				いずれも、継承して使用　独自関数、変数は一切ない
				前方向iterator basic_forward_iterator
				両方向iterator basic_bidirectional_iterator
				ランダムiterator basic_random_access_iterator
	algorithm.hpp 廃止
		後継はhmAlgorithm.hppとhmFuncIterator.hppに移行
	hmFuncIterator_v1_00.hpp
		algorithm.hppの後継ファイル
			self_iteraotr
				参照が自身(itretorとして)を返すiterator
			acs_iterator
				Accessorを通じて、iteratorの中身のさす要素にアクセスするiterator
				Accessorは、あるクラスを引数に、そのメンバ変数を戻り値とするようなファンクタ
	array_v1_04
		hmLib_v3に対応　名前空間をhmstdからshmに変更
		hmstd::basic_iterator.hppの仕様変更に対応
		assign=>create,clear=>releaseへと関数名変更
	arraydeque廃止
		dequeの名が誤解を招き、arrayloopの機能も持つcircular.hppが出来たため、廃止。
		circular.hppのbicircularが同様の機能を持つ。
	circular_v1_00.hpp
		循環バッファ用コンテナ
			bicircular
				前後から挿入、削除ができるタイプ
			circular
				後込め、先出し式循環バッファ
			circular_stack
				先込め、先出し式循環バッファ
	arrayloop 廃止
		後継はcircular.hppのcircular
	arraylist_v1_04.hpp
		list状データを高速で確保、削除可能な固定長配列
			内部では、arrayでデータ配列を、dequeで未使用データを管理
		可変長は、vectorlist.hppを使用
	vectorlist_v1_02.hpp
		hmLib_v3に準拠
		高速でメモリ確保可能なlist。
			vector形式でのメモリ確保、list形式での仕様
			高頻度での挿入、削除用クラス
	hmPos_v3_02.hpp
		二次元上の位置x,yへアクセス可能なクラス
		hmLib_v3に対応
		クラス名をhmPointからhm::PPositionへ変更
			Pointの名義は、pointorと紛らわしいため
			hm::SPositionを3次元用とする予定
		defaultのstd::ostream用出力形式を定義"(x,y)"
	hmRSGate_v1_03.hpp
		hmLib_v3に対応
		RS232c形式のCom接続用クラス
	hmDir_v1_02
		ディレクトリ制御用クラス hm::Directory
			フォルダの生成、削除や、移動が簡単に可能
			現在の作業場所をsetCurrentDirectory,getCurrentDirectoryで設定可能
	hmTime_v1_02.hpp
		hmLib_v3に対応
		ms単位まで保持するシステム時間用クラスhm::Time
		hm::Timeを日時型へに変換する日付クラスhm::Date
	hmColor_v1_01.hpp
		色関係制御用クラス
			class RGBClr;	//RGB型カラー	r:赤強度 g:緑強度 b:青強度 を指定
			class HSVClr;	//HSV型カラー	hue:色相 sat:彩度 vlu:強度 を指定
			class HLSClr;	//HLS型カラー	hue:色相 lgt:彩度 sat:明度 を指定
			class RGBAClr;//RGB型カラー	r:赤強度 g:緑強度 b:青強度 a:透過度 を指定
			class HSVAClr;//HSV型カラー	hue:色相 sat:彩度 vlu:強度 a:透過度 を指定
			class HLSAClr;//HLS型カラー	hue:色相 lgt:彩度 sat:明度 a:透過度 を指定
	hmBufGate_v1_03
		他のGateのバッファとして機能するGate
			有限バッファクラス BufGate
			無限バッファクラス InfBufGate
	hmRect_v1_02
		二点を保持する、長方形型クラス
			&,|で、範囲の共通部分を出すことが可能
	hmAlign_v1_01.hpp
		方向を示すクラス、hm::AlignQuad
		hm::aliq内の変数を使って、位置指定可能
	hmDebug_v1_01.h/cpp
		debug時用便利ツール
			debug窓に出力するostream hm::debugout
			debug時のみ出力するflow hm::debugflow
			debug時のみ出力するcout hm::debug_cout
			debug時のみ出力するfstream hm::debug_fout
	hmObject廃止
		Objectの概念変更のため（純粋に描画可能な物体、として再定義）
		後継はhmBody.hpp
v2_09_05
	hmRand_v2_04
		randNormの引数名をvarからsdへと変更
	basic_iterator_v1_00追加
		iterator基本作成キット
	basic_operator_v1_00追加
		operator基本作成キット
	algorithm_v1_02
		iterator生成用クラス群をbasic_iteratorに統一し、basic_iterator.hppへと移動
	array_v1_03
		iteratorにhmstd::basic_iteratorを使用
	arraydeque_v1_03
		iteratorにhmstd::basic_iteratorを使用
	arraylist_v1_03
		iteratorにhmstd::basic_iteratorを使用
	diarray_v1_00追加
		二次元iterator作成
v2_09_04
	algorithm_v1_01追加
		basic_random_access_iteratorにdistanceを追加
v2_09_03
	hmDir_v1_01
		str()とoperator()を統合
	hmBasic_v1_07
		system関数を追加
v2_09_02
	algorithm_v1_00追加
		STL系用のalgorithm
	array_v1_02
		iteratorにhm::basic_random_access_iteratorを使用
	arraydeque_v1_02
		iteratorにhm::basic_random_access_iteratorを使用
		iteratorがバッファ上のデータの先頭のアドレスを保持するように変更
	arraylist_v1_02
		iteratorにhm::basic_bidirectional_iteratorを使用
	hmRand_v2_03
		hmRandを関数オブジェクトとして使用可能に変更
		hm::getGlobalRand追加
	vectprlist_v1_01
		メモリ確保のふるまいを、vectorに準拠(追加確保は行うが、解放は自動で行わない)
	hmDebug_v1_00追加
		debug窓に出力するostream hm::dout
		debug時のみ出力するflow hm::debugflow
		debug時のみ出力するcout hm::d_cout
v2_09_01
	dxObject_v2_06
		dxOのObjectアクセスクラス名をaccessorに変更
		dxO::accessorのアクセス演算子をiteratorに準拠
	arraylist_v1_01
		iteratorをSTL型に準拠
	arraydeque_v1_01
		iteratorをSTL型に準拠
	array_v1_01
		iteratorをSTL型に準拠
v2_09_00
	array_v1_00追加
		STL準拠の固定長配列クラス
	arraydeque_v1_00追加
		STL準拠のdeque型固定長配列クラス
		メモリの自己確保を行わないため、高速
	arrayloop_v1_00追加
		STL準拠のloop型固定長配列クラス
		環形構造を持ち、環に要素を追加するタイプ
	arraylist_v1_00追加
		STL準拠のlist型固定長配列クラス
		毎回メモリ確保を行わず、固定配列上にデータを置く
		任意の位置の要素の挿入/削除が高速
	vectorlist_v1_00追加
		STL準拠のlist型vectorクラス
		自ら要素数の調整を行うarraylist
		任意の位置の要素の挿入/削除が高速
v2_08_07
	hmRand_v2_02
		hmRand::gen_randNormを追加。正規乱数生成を倍速化。
	hmBasic_v1_6
		modの致命的なバグを修正
	dideque_v1_01
		関数名を一部変更
v2_08_06
	bufgate_v1_02
		ibufcltrgateを内部クラスに追加
v2_08_05
	hmBasic_v1_5
		str,c_strをhmに追加
	hmRect_v1_01
		check関数を追加
	dxPad_v1_00追加
v2_08_04
	dxArea_v2_04
		dxAreaが描画範囲を制限していなかった問題を修正
	dxObject_v2_05
		dxArObjectが描画範囲を制限していた問題を修正
v2_08_03
	dxArea_v2_03
		dxArObjectを継承する形に修正
			dxObjectの更新に準拠
	dxObject_v2_04
		dxFnを描画関数オブジェクトとして特化
		dxFnObjectをdxFnに特化
		dxArObjectを追加
v2_08_02
	hmLib.cpp
		RSgate関連のインクルードバグを修正
	csvflow_v1_01
		ocsvflow,icsvflowのコンストラクタの記述バグを修正
	dxArea_v2_02
		dxObjectとhmoBoxを継承する形に修正
			dxBasicObject直接継承では、dxSubObjectとdxObjectにしか対応しないdxo.drawを使用できないため
		basic_dxMoveAreaType::AtoF,FtoA,setViewPosにバグがあったのを修正
	dxObject_v2_03
		dxReObjectを追加
v2_08_01
	hmTime_v1_01.hpp
		hmTime::setを追加
		hmDate::setを追加
	include_verにおいて、テスト版(_vX.hpp)をTEST_HMLIBでインクルード可能に変更
v2_08_00
	dxObject_v2_02
		関数ポインタ用関数オブジェクトdxFn追加
		dxFnObjectを関数オブジェクト用に改良
	dxArea_v2_01
		関数オブジェクトに対応
v2_07_02
	dideque.hpp v1_0
		新規追加
	include_verにおいて、USE_とVER_に修正
	全てのラッパーインクルードファイルの拡張子をhppに変更
	hmBasic_v1_4
		mod追加
	hmTime_v1_00.hpp
		winTimeを継承する形で新規追加
	winTimeを非推奨版に変更
	dxBasic_v2_05
		winTime.hppを廃止し、hmTime.hppに対応
v2_07_01
	hmstreambuf.hpp v1_1
		NBでstreamを使用すると、エラーになるバグを修正
	iologgate.hpp v1_0
		新規追加
v2_07_00
	include_verガードを仕様変更　一つのライブラリで複数バージョンが管理可能に
v2_06_00
	hmDirを追加
	dxBasic v2_4
		hmBasic.h v1_3に対応
v2_05_02
	RSgate.hpp v1_1
		namespace hmstdに加入
	hmBasic v1_3
		char* getTmpStrAdress()をhmに移動
		int getTmpStrAdressSize()を追加
v2_05_01
	dxBasic.h v2_3
		dx::resetDrawScreen実行時にNowScreenが変更されなかったバグを修正
	dxColorSet.h v1_1
		dxHLS型で新規に作成
v2_05_00
	hmBasic.h v1_2
		bitxorの追加
	hmAlign.hppをhmAlign.hとhmAlign.cppに分離
v2_04_00
	include_verガードを追加
v2_03_02
	hmBasic.h v1_1
		min3,max3,med3の追加
	dxBasic.h v2_2
		dxFont::make(...)が正常に機能しなかったバグを修正
	toolにmakeFontNameList.exeを追加
v2_03_01
	hmAlign.hpp v1_1
		PintVecを追加
			AliのPintベクトルを返す
		ALI::allを追加
		インクルードガードを追加
	hmRand.h v2_1
		hm::rand(int min,int max)で正常に表示されないバグを修正
v2_03_00
	hmRand.h v2_0
		乱数生成関数を追加
			最小、最大を指定できる乱数
			32bit,64bit両者対応の生の乱数
			最小、最大を指定できる浮動乱数
			正規乱数
		メンバ関数から正規乱数を削除
		hmRand hm::randを削除
v2_02_00
	dxBasic.h v2_1
		dxGraphが画像保存できないバグを修正
		描画可能なグラフィックハンドルを作る		dxBasicScreen,dxScreenの追加
		dx::setDrawScreenの引数をdxBasicScreenに変更
v2_01_00
	dxObject.hpp v2_0
		draw関数のactiveフラグの位置をdxDModeの前に修正
	dxArea.hpp v2_0
		dxObject.hpp v2.0に対応するように修正
v2_00_02
	hmBuffer.hpp->bufgate.hppへ変更
	bufgate.hpp v1_1
		getc()実行時にデータがなければ、データが来るまで待機するように変更
		bufのsize取得系関数の追加
v2_00_01
	dxObject.hpp
		dxoProgress追加
		dxoBasicButがactiveでない時も押せたバグを修正
	hmBuffer.hpp追加
	RSgate.hpp追加
		winRSフォルダも同時に追加
		winrs.cppの自動Include機能追加
	tool/resource.rcを追加
v2_00_00
	リリース
*/
#ifndef INC_HMLIB
#	define INC_HMLIB 30000
#	define VER_HMLIB INC_HMLIB
#
#	ifndef OUT_USE_HMLIB
#		define OUT_USE_HMLIB 1
#	endif
#
#	ifndef OUT_CHG_HMLIB
#		define OUT_CHG_HMLIB 1
#	endif
#
#	if 1<=OUT_USE_HMLIB
#		pragma message("=== hmLib(v3_00_00) ===")
#	endif
#
#	ifdef USE_HMLIB
#		if USE_HMLIB>30000
#			error hmLib : 非対応のバージョンです。v3_00_00より後は、使用できません。
#		elif USE_HMLIB==30000
#			if 1<=OUT_USE_HMLIB
#				pragma message("hmLib : v3_00_00")
#			endif
#		elif USE_HMLIB==20905
#			if 1<=OUT_USE_HMLIB
#				pragma message("hmLib : v2_09_05")
#			endif
#		elif USE_HMLIB==20904
#			if 1<=OUT_USE_HMLIB
#				pragma message("hmLib : v2_09_04")
#			endif
#		elif USE_HMLIB==20903
#			if 1<=OUT_USE_HMLIB
#				pragma message("hmLib : v2_09_03")
#			endif
#		elif USE_HMLIB==20902
#			if 1<=OUT_USE_HMLIB
#				pragma message("hmLib : v2_09_02")
#			endif
#		elif USE_HMLIB==20901
#			if 1<=OUT_USE_HMLIB
#				pragma message("hmLib : v2_09_01")
#			endif
#		elif USE_HMLIB==20900
#			if 1<=OUT_USE_HMLIB
#				pragma message("hmLib : v2_09_00")
#			endif
#		elif USE_HMLIB==20807
#			if 1<=OUT_USE_HMLIB
#				pragma message("hmLib : v2_08_07")
#			endif
#		elif USE_HMLIB==20806
#			if 1<=OUT_USE_HMLIB
#				pragma message("hmLib : v2_08_06")
#			endif
#		elif USE_HMLIB==20805
#			if 1<=OUT_USE_HMLIB
#				pragma message("hmLib : v2_08_05")
#			endif
#		elif USE_HMLIB==20804
#			if 1<=OUT_USE_HMLIB
#				pragma message("hmLib : v2_08_04")
#			endif
#		elif USE_HMLIB==20803
#			if 1<=OUT_USE_HMLIB
#				pragma message("hmLib : v2_08_03")
#			endif
#		elif USE_HMLIB==20802
#			if 1<=OUT_USE_HMLIB
#				pragma message("hmLib : v2_08_02")
#			endif
#		elif USE_HMLIB==20801
#			if 1<=OUT_USE_HMLIB
#				pragma message("hmLib : v2_08_01")
#			endif
#		elif USE_HMLIB==20800
#			if 1<=OUT_USE_HMLIB
#				pragma message("hmLib : v2_08_00")
#			endif
#		elif USE_HMLIB==20702
#			if 1<=OUT_USE_HMLIB
#				pragma message("hmLib : v2_07_02")
#			endif
#		elif USE_HMLIB==20701
#			if 1<=OUT_USE_HMLIB
#				pragma message("hmLib : v2_07_01")
#			endif
#		elif USE_HMLIB==20700
#			if 1<=OUT_USE_HMLIB
#				pragma message("hmLib : v2_07_00")
#			endif
#		elif USE_HMLIB==20600
#			if 1<=OUT_USE_HMLIB
#				pragma message("hmLib : v2_06_00")
#			endif
#		elif USE_HMLIB==20502
#			if 1<=OUT_USE_HMLIB
#				pragma message("hmLib : v2_05_02")
#			endif
#		elif USE_HMLIB==20501
#			if 1<=OUT_USE_HMLIB
#				pragma message("hmLib : v2_05_01")
#			endif
#		elif USE_HMLIB==20500
#			if 1<=OUT_USE_HMLIB
#				pragma message("hmLib : v2_05_00")
#			endif
#		elif USE_HMLIB==20400
#			if 1<=OUT_USE_HMLIB
#				pragma message("hmLib : v2_04_00")
#			endif
#		elif USE_HMLIB==20302
#			if 1<=OUT_USE_HMLIB
#				pragma message("hmLib : v2_03_02")
#			endif
#		elif USE_HMLIB==20301
#			if 1<=OUT_USE_HMLIB
#				pragma message("hmLib : v2_03_01")
#			endif
#		elif USE_HMLIB==20300
#			if 1<=OUT_USE_HMLIB
#				pragma message("hmLib : v2_03_00")
#			endif
#		elif USE_HMLIB==20200
#			if 1<=OUT_USE_HMLIB
#				pragma message("hmLib : v2_02_00")
#			endif
#		elif USE_HMLIB==20100
#			if 1<=OUT_USE_HMLIB
#				pragma message("hmLib : v2_01_00")
#			endif
#		elif USE_HMLIB==20002
#			if 1<=OUT_USE_HMLIB
#				pragma message("hmLib : v2_00_02")
#			endif
#		elif USE_HMLIB==20001
#			if 1<=OUT_USE_HMLIB
#				pragma message("hmLib : v2_00_01")
#			endif
#		elif USE_HMLIB==20000
#			if 1<=OUT_USE_HMLIB
#				pragma message("hmLib : v2_00_00")
#			endif
#		elif USE_HMLIB<20000
#			error hmLib : 非対応のバージョンです。v2_00_00より前は、使用できません。
#		else
#			error hmLib : 非定義のバージョンです。そのようなバージョンは存在しません。
#		endif
#
#		define CAN_USE_HMLIB
#	else
#		error hmLib : hmLibVer.hppがインクルードされていません
#	endif
#	
#	ifdef CHG_HMLIB
#		if 1<=OUT_CHG_HMLIB
#			pragma message("chg_hmLib : ########: hmLibの互換性をテストします。")
#		endif
#		define CAN_CHG_HMLIB
#	endif
#	//windows.hのmin,maxを使わせない
#	define NOMINMAX
#	//古いunsafeな関数群使用による警告回避
#	define _CRT_SECURE_NO_DEPRECATE 1
#endif
