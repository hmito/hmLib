#ifndef HMLIB_UPDATE_INC
#define HMLIB_UPDATE_INC	30303
#define HMLIB_UPDATE_STR	"v3_03_03"
#ifdef HMLIB_UPDATE
#	undef HMLIB_UPDATE
#endif
#define HMLIB_UPDATE HMLIB_UPDATE_INC
#
#ifndef HMLIB_CONFIG_INC
#	include "hmLib_config.h"
#endif
#
#define _HMLIB_FN_VER_TO_NUM(VER) #VER
#define HMLIB_FN_VER_TO_NUM(VER) _HMLIB_FN_VER_TO_NUM(VER)
#
#if HMLIB_OUT > 0
#	pragma message("=== hmLib_update_"HMLIB_UPDATE_STR" ===")
#	ifndef HMLIB_VERSION
#		pragma message("hmLib_update : #error : hmLib_versionがincludeされていません。")
#	else
#		ifdef HMLIB_UPGRADE
#			pragma message("hmLib_update : update  "HMLIB_FN_VER_TO_NUM(HMLIB_UPDATE))
#			pragma message("hmLib_update : version "HMLIB_FN_VER_TO_NUM(HMLIB_VERSION))
#			pragma message("hmLib_update : upgrade "HMLIB_FN_VER_TO_NUM(HMLIB_UPGRADE))
#		else
#			pragma message("hmLib_update : update  "HMLIB_FN_VER_TO_NUM(HMLIB_UPDATE))
#			pragma message("hmLib_update : version "HMLIB_FN_VER_TO_NUM(HMLIB_VERSION))
#		endif
#
//hmLib_v3_00_00
#if HMLIB_VERSION<30000
#		if HMLIB_OUT>0 && HMLIB_VERSION<30000
#			pragma message("hmLib_update : #error   : 非対応バージョンです。hmLib_v2のchg_hmLib機能をご利用ください。")
#		endif
#endif
#
//hmLib_v3_01_01
#if HMLIB_VERSION<30101
#pragma message("hmLib_update : ===hmLib_v3_01===") 
/*
	algorithm.hpp v1_04
		ランダム選択、ルーレット選択で指定範囲から抜き出す、以下の6関数を追加。
		//ランダム選択
		template<class InputIterator,class OutputIterator>
		OutputIterator random_sample(InputIterator Begin,InputIterator End,OutputIterator Out);
		//ランダム選択 OutputIteratorの範囲を埋めるまで
		template<class InputIterator,class OutputIterator>
		OutputIterator random_sample(InputIterator Begin,InputIterator End,OutputIterator OutBegin,OutputIterator OutEnd);
		//ランダム選択 n個分
		template<class InputIterator,class OutputIterator>
		OutputIterator random_sample(InputIterator Begin,InputIterator End,OutputIterator Out,unsigned int n);
		//ルーレット選択
		template<class InputIterator,class FnRealValue,class OutputIterator>
		OutputIterator roulette_sample(InputIterator Begin,InputIterator End,FnRealValue Value,OutputIterator Out);
		//ルーレット選択 OutputIteratorの範囲を埋めるまで
		template<class InputIterator,class FnRealValue,class OutputIterator>
		OutputIterator roulette_sample(InputIterator Begin,InputIterator End,FnRealValue Value,OutputIterator OutBegin,OutputIterator OutEnd);
		//ルーレット選択 n個分
		template<class InputIterator,class FnRealValue,class OutputIterator>
		OutputIterator roulette_sample(InputIterator Begin,InputIterator End,FnRealValue Value,OutputIterator Out,unsigned int n);
*/
#		ifdef HMLIB_ALGORITHM_INC
#			ifdef HMLIB_UPGRADE
#				if HMLIB_OUT>2 && 30101<=HMLIB_UPGRADE
#					pragma message("hmLib_update : #inform  : [algorithm.hpp] roulette_sampleが、v1_05(v3_01_01)で追加されました。")
#				endif
#			else
#				if HMLIB_OUT>2
#					pragma message("hmLib_update : #inform  : [algorithm.hpp] roulette_sampleが、v1_05(v3_01_01)で追加されました。")
#				endif
#			endif
#		endif
/*
	lab/bio/agent.hpp v1_00
		適応度、形質などを一律で制御できる、agentクラスとその付属機能を新設
*/
#		ifdef HMLIB_AGENT_INC
#			ifdef HMLIB_UPGRADE
#				if HMLIB_OUT>2 && 30101<=HMLIB_UPGRADE
#					pragma message("hmLib_update : #inform  : [agent.hpp] hmLib/labにv3_01_01で追加されました。")
#				endif
#			else
#				if HMLIB_OUT>2
#					pragma message("hmLib_update : #inform  : [agent.hpp] hmLib/labにv3_01_01で追加されました。")
#				endif
#			endif
#		endif
#endif
#
//hmLib_v3_01_02
#if HMLIB_VERSION<30102
/*
	algorithm.hpp v1_05
		ランダム選択を行う以下のクラスと関数を追加。
		//ルーレット選択クラス
		template<typename InputIterator,typename fnAssess>
		class roulette_sampler{
		public:
			roulette_sampler(InputIterator Begin_,InputIterator End_,fnAssess FnAssess_);
			OutputIterator operator()();
		};
		//ルーレット選択クラスのビルダー
		template<typename InputIterator,typename fnAssess>
		roulette_sampler<InputIterator,fnAssess> build_roulette_sampler(InputIterator Begin_,InputIterator End_,fnAssess FnAssess_);
*/
#		ifdef HMLIB_ALGORITHM_INC
#			ifdef HMLIB_UPGRADE
#				if HMLIB_OUT>2 && 30102<=HMLIB_UPGRADE
#					pragma message("hmLib_update : #inform  : [algorithm.hpp] random_samplerが、v1_05(v3_01_02)で追加されました。")
#				endif
#			else
#				if HMLIB_OUT>2
#					pragma message("hmLib_update : #inform  : [algorithm.hpp] random_samplerが、v1_05(v3_01_02)で追加されました。")
#				endif
#			endif
#		endif
/*
	lab/parameter.hpp v0_00
		class parameterizerに従って、変数の入出力およびクラス名等を管理・補助するクラス。
*/
#		ifdef HMLIB_PARAMETER_INC
#			ifdef HMLIB_UPGRADE
#				if HMLIB_OUT>2 && 30102<=HMLIB_UPGRADE
#					pragma message("hmLib_update : #inform  : [parameter.hpp] hmLib/labにv3_01_02で追加されました。")
#				endif
#			else
#				if HMLIB_OUT>2
#					pragma message("hmLib_update : #inform  : [parameter.hpp] hmLib/labにv3_01_02で追加されました。")
#				endif
#			endif
#		endif
#endif
#
//hmLib_v3_01_03
#if HMLIB_VERSION<30103
/*
	algorithm.hpp v1_06
		class random_samplerの調整
*/
#		ifdef HMLIB_ALGORITHM_INC
#			ifdef HMLIB_UPGRADE
#				if HMLIB_OUT>2 && 30101<=HMLIB_VERSION && 30103<=HMLIB_UPGRADE
#					pragma message("hmLib_update : #inform  : [algorithm.hpp] random_samplerの致命的なバグが、v1_06(v3_01_03)で修正されています。")
#				elif HMLIB_OUT>1 && 30101<=HMLIB_UPGRADE
#					pragma message("hmLib_update : #warning : [algorithm.hpp] random_samplerに致命的なバグがあります。v1_06以上(v3_01_03以上)に更新してください。")
#				endif
#			else
#				if HMLIB_OUT>1 && 30101<=HMLIB_VERSION
#					pragma message("hmLib_update : #warning : [algorithm.hpp] random_samplerに致命的なバグがあります。v1_06以上(v3_01_03以上)に更新してください。")
#				endif
#			endif
#		endif
/*
	lab/parameter.hpp v1_00
		メタ関数周りのバグを取り除き。使用可能状態
*/
#		ifdef HMLIB_PARAMETER_INC
#			ifdef HMLIB_UPGRADE
#				if HMLIB_OUT>2 && 30102<=HMLIB_VERSION && 30103<=HMLIB_UPGRADE
#					pragma message("hmLib_update : #inform  : [parameter.hpp] メタ関数周辺の致命的なバグが、v1_00(v3_01_03)で修正されています。")
#				elif HMLIB_OUT>1 && 30102<=HMLIB_UPGRADE
#					pragma message("hmLib_update : #warning : [parameter.hpp] メタ関数周辺に致命的なバグがあります。v1_00以上(v3_01_03以上)に更新してください。")
#				endif
#			else
#				if HMLIB_OUT>1 && 30102<=HMLIB_VERSION
#					pragma message("hmLib_update : #warning : [parameter.hpp] メタ関数周辺に致命的なバグがあります。v1_00以上(v3_01_03以上)に更新してください。")
#				endif
#			endif
#		endif
#endif
#
#//hmLib_v3_01_04
#if HMLIB_VERSION<30104
/*
	algorithm.hpp v1_07
		class roulette_samplerの初期化でBeginを不正に書き換えていたのを修正
*/
#		ifdef HMLIB_ALGORITHM_INC
#			ifdef HMLIB_UPGRADE
#				if HMLIB_OUT>2 && 30101<=HMLIB_VERSION && 30104<=HMLIB_UPGRADE
#					pragma message("hmLib_update : #inform  : [algorithm.hpp] random_samplerの致命的なバグが、v1_07(v3_01_04)で修正されています。")
#				elif HMLIB_OUT>1 && 30101<=HMLIB_UPGRADE
#					pragma message("hmLib_update : #warning : [algorithm.hpp] random_samplerに致命的なバグがあります。v1_07以上(v3_01_04以上)に更新してください。")
#				endif
#			else
#				if HMLIB_OUT>1 && 30101<=HMLIB_VERSION
#					pragma message("hmLib_update : #warning : [algorithm.hpp] random_samplerに致命的なバグがあります。v1_07以上(v3_01_04以上)に更新してください。")
#				endif
#			endif
#		endif
/*
	lab/config_vc.hpp v1_00
		VisualStudio特有のエラー表示問題を解決するヘッダー
*/
#		ifdef HMLIB_CONFIG_VC_INC
#			ifdef HMLIB_UPGRADE
#				if HMLIB_OUT>2 && 30104<=HMLIB_UPGRADE
#					pragma message("hmLib_update : #inform  : [config_vc.hpp] hmLib/labにv3_01_04で追加されました。")
#				endif
#			else
#				if HMLIB_OUT>2
#					pragma message("hmLib_update : #inform  : [config_vc.hpp] hmLib/labにv3_01_04で追加されました。")
#				endif
#			endif
#		endif
#endif
#
//hmLib_v3_02_00
#if HMLIB_VERSION<30199
#pragma message("hmLib_update : ===hmLib_v3_02===")
/*
	parameter.hpp v1_01
		クラス名、役割を変更
		hmLib/labからhmLibへ移動
*/
#		ifdef HMLIB_PARAMETER_INC
#			ifdef HMLIB_UPGRADE
#				if HMLIB_OUT>0 && 30102<=HMLIB_VERSION && 30199<=HMLIB_UPGRADE
#					pragma message("hmLib_update : #error   : [parameter.hpp] クラス名と必要な関数の種類が、v1_01(v3_02_00)で変更されています。")
#				elif HMLIB_OUT>2 && 30102<=HMLIB_UPGRADE
#					pragma message("hmLib_update : #inform  : [parameter.hpp] クラス名と必要な関数の種類が、v1_01(v3_02_00)で変更されています。")
#				endif
#			else
#				if HMLIB_OUT>2 && 30102<=HMLIB_VERSION
#					pragma message("hmLib_update : #inform  : [parameter.hpp] クラス名と必要な関数の種類が、v1_01(v3_02_00)で変更されています。")
#				endif
#			endif
#		endif
/*
	agent.hpp 廃止
*/
#		ifdef HMLIB_AGENT_INC
#			ifdef HMLIB_UPGRADE
#				if HMLIB_OUT>0 && 30101<=HMLIB_VERSION 30199<=HMLIB_UPGRADE
#					pragma message("hmLib_update : #error   : [agent.hpp] hmLibからv3_02_00/v3_01で廃止されました。")
#				elif HMLIB_OUT>2 && 30101<=HMLIB_UPGRADE
#					pragma message("hmLib_update : #inform  : [agent.hpp] hmLibからv3_02_00/v3_01で廃止されます。")
#				endif
#			else
#				if HMLIB_OUT>2 && 30101<=HMLIB_VERSION
#					pragma message("hmLib_update : #inform  : [agent.hpp] hmLibからv3_02_00/v3_01で廃止されます。")
#				endif
#			endif
#		endif
/*
	lociset.hpp 廃止
*/
#		ifdef HMLIB_LOCISET_INC
#			ifdef HMLIB_UPGRADE
#				if HMLIB_OUT>0 && 30000<=HMLIB_VERSION && 30199<=HMLIB_UPGRADE
#					pragma message("hmLib_update : #error   : [lociset.hpp] hmLibからv3_02_00/v3_01で廃止されました。")
#				elif HMLIB_OUT>2 && 30000<=HMLIB_UPGRADE
#					pragma message("hmLib_update : #inform  : [lociset.hpp] hmLibからv3_02_00/v3_01で廃止されます。")
#				endif
#			else
#				if HMLIB_OUT>2 && 30000<=HMLIB_VERSION
#					pragma message("hmLib_update : #inform  : [lociset.hpp] hmLibからv3_02_00/v3_01で廃止されます。")
#				endif
#			endif
#		endif
#endif
//hmLib_v3_02_01
#if HMLIB_VERSION<30201
/*
	filterbuf.hpp v1_01
		labにv1_01を追加。関数名等を変更。
*/
#		ifdef HMLIB_FILTERBUF_INC
#			ifdef HMLIB_UPGRADE
#				if HMLIB_OUT>2 && 30201<=HMLIB_UPGRADE
#					pragma message("hmLib_update : #inform   : [filterbuf.hpp] labにv1_01がv3_02_01で追加されました。関数名等が変更されています。")
#				endif
#			else
#				if HMLIB_OUT>2
#					pragma message("hmLib_update : #inform  : [filterbuf.hpp] labにv1_01がv3_02_01で追加されました。関数名等が変更されています。")
#				endif
#			endif
#		endif
/*
	exception.hpp v1_01
		labにv1_01を追加。クラス名等を変更。
*/
#		ifdef HMLIB_EXCEPTION_INC
#			ifdef HMLIB_UPGRADE
#				if HMLIB_OUT>2 && 30201<=HMLIB_UPGRADE
#					pragma message("hmLib_update : #inform   : [exception.hpp] labにv1_01がv3_02_01で追加されました。クラス名等が変更されています。")
#				endif
#			else
#				if HMLIB_OUT>2
#					pragma message("hmLib_update : #inform  : [exception.hpp] labにv1_01がv3_02_01で追加されました。クラス名等が変更されています。")
#				endif
#			endif
#		endif
/*
	streambuf_interface.hpp v1_00
		labにv1_00を追加。streambuf_pattern.hppの後継。
*/
#		ifdef HMLIB_STREAMBUFINTERFACE_INC
#			ifdef HMLIB_UPGRADE
#				if HMLIB_OUT>2 && 30201<=HMLIB_UPGRADE
#					pragma message("hmLib_update : #inform   : [streambuf_interface.hpp] labにv1_00がv3_02_01で追加されました。streambuf_pattern.hppの後継です。")
#				endif
#			else
#				if HMLIB_OUT>2
#					pragma message("hmLib_update : #inform  : [streambuf_interface.hpp] labにv1_00がv3_02_01で追加されました。streambuf_pattern.hppの後継です。")
#				endif
#			endif
#		endif
//hmLib_v3_02_02
/*
	byte_base.hpp v1_01
		standard_bytetype.hppが廃止されていたので、hmLib_configを参照する形に修正。
*/
#		ifdef HMLIB_BYTEBASETYPE_INC
#			ifdef HMLIB_UPGRADE
#				if HMLIB_OUT>2 && 30201<=HMLIB_UPGRADE
#					pragma message("hmLib_update : #inform  : [bytebase_type.hpp] includeファイルに不備があったのがv1_01(v3_02_01)で修正されています。")
#				elif HMLIB_OUT>1
#					pragma message("hmLib_update : #warning : [bytebase_type.hpp] includeファイルに不備があります。v1_01以上(v3_02_01以上)に更新してください。")
#				endif
#			else
#				if HMLIB_OUT>1
#					pragma message("hmLib_update : #warning : [bytebase_type.hpp] includeファイルに不備があります。v1_01以上(v3_02_01以上)に更新してください。")
#				endif
#			endif
#		endif
#endif
//hmLib_v3_03_00
#if HMLIB_VERSION<30300
#pragma message("hmLib_update : ===hmLib_v3_03===")
/*
	hmCLibと統合
*/
#			ifdef HMLIB_UPGRADE
#				if HMLIB_OUT>2 && 30300<=HMLIB_UPGRADE
#					pragma message("hmLib_update : #inform  : v3_03_00でhmCLibと統合しました。C言語から一部のヘッダファイルも利用できます。")
#					pragma message("hmLib_update : #inform  : 拡張子の意味がC++対応版がhpp,C/C++対応版がhに、v3_03_00で変更されています。")
#					pragma message("hmLib_update : #inform  : v3_03_00からhmLibのパスの指定が不要になりました。相対パスでlibファイルは設定します。")
#				endif
#			else
#				if HMLIB_OUT>2
#					pragma message("hmLib_update : #inform  : v3_03_00でhmCLibと統合しました。C言語から一部のヘッダファイルも利用できます。")
#					pragma message("hmLib_update : #inform  : 拡張子の意味がC++対応版がhpp,C/C++対応版がhに、v3_03_00で変更されています。")
#					pragma message("hmLib_update : #inform  : v3_03_00からhmLibのパスの指定が不要になりました。相対パスでlibファイルは設定します。")
#				endif
#			endif
/*
	byte_base.hpp v1_02
		拡張子をhに変更。
		C言語との共通化を行う。
		hmLib_xxxスタイルに型名を統一
*/
#		ifdef HMLIB_BYTEBASETYPE_INC
#			ifdef HMLIB_UPGRADE
#				if HMLIB_OUT>0 && 30300<=HMLIB_UPGRADE
#					pragma message("hmLib_update : #error   : [bytebase_type.h] 拡張子がhppからhに、v1_02(v3_03_00)変更されます。")
#				elif HMLIB_OUT>2
#					pragma message("hmLib_update : #inform  : [bytebase_type.h] 拡張子がhppからhに、v1_02(v3_03_00)変更されます。")
#				endif
#			else
#				if HMLIB_OUT>2
#					pragma message("hmLib_update : #inform  : [bytebase_type.h] 拡張子がhppからhに、v1_02(v3_03_00)変更されます。")
#				endif
#			endif
#		endif
/*
	random_v2_06
		拡張子をhppに変更。
*/
#		ifdef HMLIB_RANDOM_INC
#			ifdef HMLIB_UPGRADE
#				if HMLIB_OUT>0 && 30300<=HMLIB_UPGRADE
#					pragma message("hmLib_update : #error   : [random.hpp] 拡張子がhからhppに、v2_06(v3_03_00)変更されます。")
#				elif HMLIB_OUT>2
#					pragma message("hmLib_update : #inform  : [random.hpp] 拡張子がhからhppに、v2_06(v3_03_00)変更されます。")
#				endif
#			else
#				if HMLIB_OUT>2
#					pragma message("hmLib_update : #inform  : [random.hpp] 拡張子がhからhppに、v2_06(v3_03_00)変更されます。")
#				endif
#			endif
#		endif
/*
	align_v1_01
		拡張子をhppに変更。
*/
#		ifdef HMLIB_ALIGN_INC
#			ifdef HMLIB_UPGRADE
#				if HMLIB_OUT>0 && 30300<=HMLIB_UPGRADE
#					pragma message("hmLib_update : #error   : [allign.hpp] 拡張子がhからhppに、v1_01(v3_03_00)変更されます。")
#				elif HMLIB_OUT>2
#					pragma message("hmLib_update : #inform  : [allign.hpp] 拡張子がhからhppに、v1_01(v3_03_00)変更されます。")
#				endif
#			else
#				if HMLIB_OUT>2
#					pragma message("hmLib_update : #inform  : [allign.hpp] 拡張子がhからhppに、v1_01(v3_03_00)変更されます。")
#				endif
#			endif
#		endif
/*
	csvstream_v1_01
		拡張子をhppに変更。
*/
#		ifdef HMLIB_ALIGN_INC
#			ifdef HMLIB_UPGRADE
#				if HMLIB_OUT>0 && 30300<=HMLIB_UPGRADE
#					pragma message("hmLib_update : #error   : [csvstream.hpp] 拡張子がhからhppに、v1_01(v3_03_00)変更されます。")
#				elif HMLIB_OUT>2
#					pragma message("hmLib_update : #inform  : [csvstream.hpp] 拡張子がhからhppに、v1_01(v3_03_00)変更されます。")
#				endif
#			else
#				if HMLIB_OUT>2
#					pragma message("hmLib_update : #inform  : [csvstream.hpp] 拡張子がhからhppに、v1_01(v3_03_00)変更されます。")
#				endif
#			endif
#		endif
/*
	comgate_v1_04
		拡張子をhppに変更。
*/
#		ifdef HMLIB_ALIGN_INC
#			ifdef HMLIB_UPGRADE
#				if HMLIB_OUT>0 && 30300<=HMLIB_UPGRADE
#					pragma message("hmLib_update : #error   : [comgate.hpp] 拡張子がhからhppに、v1_04(v3_03_00)変更されます。")
#				elif HMLIB_OUT>20
#					pragma message("hmLib_update : #inform  : [comgate.hpp] 拡張子がhからhppに、v1_04(v3_03_00)変更されます。")
#				endif
#			else
#				if HMLIB_OUT>2
#					pragma message("hmLib_update : #inform  : [comgate.hpp] 拡張子がhからhppに、v1_04(v3_03_00)変更されます。")
#				endif
#			endif
#		endif
/*
	cdeque_v1_00
		固定長配列を用いたdeque。
*/
#		ifdef HMLIB_CDEQUE_INC
#			ifdef HMLIB_UPGRADE
#				if HMLIB_OUT>2 && 30300<=HMLIB_UPGRADE
#					pragma message("hmLib_update : #inform  : [cdeque.hpp] v1_00がv3_03_00で追加されました。")
#				endif
#			else
#				if HMLIB_OUT>2
#					pragma message("hmLib_update : #inform  : [cdeque.hpp] v1_00がv3_03_00で追加されました。")
#				endif
#			endif
#		endif
/*
	cqueue_v1_01
		固定長配列を用いたqueue。内部的にはcdequeのラッピング。
*/
#		ifdef HMLIB_CQUEUE_INC
#			ifdef HMLIB_UPGRADE
#				if HMLIB_OUT>2 && 30300<=HMLIB_UPGRADE
#					pragma message("hmLib_update : #inform  : [cqueue.hpp] v1_01がv3_03_00で追加されました。")
#				endif
#			else
#				if HMLIB_OUT>2
#					pragma message("hmLib_update : #inform  : [cqueue.hpp] v1_01がv3_03_00で追加されました。")
#				endif
#			endif
#		endif
/*
	cstack_v1_00
		固定長配列を用いたstack。内部的にはcdequeのラッピング。
*/
#		ifdef HMLIB_CSTACK_INC
#			ifdef HMLIB_UPGRADE
#				if HMLIB_OUT>2 && 30300<=HMLIB_UPGRADE
#					pragma message("hmLib_update : #inform  : [cstack.hpp] v1_00がv3_03_00で追加されました。")
#				endif
#			else
#				if HMLIB_OUT>2
#					pragma message("hmLib_update : #inform  : [cstack.hpp] v1_00がv3_03_00で追加されました。")
#				endif
#			endif
#		endif
/*
	cstring_v1_01
		固定長配列を用いたstring。
*/
#		ifdef HMLIB_CSTRING_INC
#			ifdef HMLIB_UPGRADE
#				if HMLIB_OUT>2 && 30300<=HMLIB_UPGRADE
#					pragma message("hmLib_update : #inform  : [cstring.hpp] v1_01がv3_03_00で追加されました。")
#				endif
#			else
#				if HMLIB_OUT>2
#					pragma message("hmLib_update : #inform : [cstring.hpp] v1_01がv3_03_00で追加されました。")
#				endif
#			endif
#		endif
/*
	exception.hpp v1_01
		クラス名等をnormalが無いバージョンに変更。labから格上げ。
*/
#		ifdef HMLIB_EXCEPTION_INC
#			ifdef HMLIB_UPGRADE
#				if HMLIB_OUT>0 && 30300<=HMLIB_UPGRADE
#					pragma message("hmLib_update : #error   : [exception.hpp] クラス名等がv1_01(v3_03_00)で変更されています。")
#				elif HMLIB_OUT>2
#					pragma message("hmLib_update : #inform  : [exception.hpp] クラス名等がv1_01(v3_03_00)で変更されています。")
#				endif
#			else
#				if HMLIB_OUT>2
#					pragma message("hmLib_update : #inform  : [exception.hpp] クラス名等がv1_01(v3_03_00)で変更されています。")
#				endif
#			endif
#		endif
/*
	gate_v1_01
		exception_v1_01に合わせて内部での使用関数名を変更。
*/
#		ifdef HMLIB_GATE_INC
#			ifdef HMLIB_UPGRADE
#				if HMLIB_OUT>3 && 30300<=HMLIB_UPGRADE
#					pragma message("hmLib_update : #memo    : [gate.hpp] exception_v1_01にv1_01(v3_01_02)で対応しました。")
#				endif
#			else
#				if HMLIB_OUT>3
#					pragma message("hmLib_update : #memo    : [gate.hpp] exception_v1_01にv1_01(v3_01_02)で対応しました。")
#				endif
#			endif
#		endif
#endif
//hmLib_v3_03_01
#if HMLIB_VERSION<30301
/*
	config_vc_v1_01/121204 hmIto
		v3_02_01以降、誤って廃止されていたのを復活
		拡張子をhppからhに変更
		インクルードガードマクロの不要なアンダーバーを消去
*/
#		ifdef HMLIB_CONFIGVC_INC
#			ifdef HMLIB_UPGRADE
#				if HMLIB_OUT>0 && 30301<=HMLIB_UPGRADE
#					pragma message("hmLib_update : #error   : [config_vc.h] 拡張子がhppからhに、v1_01(v3_03_01)で変更されます。")
#				elif HMLIB_OUT>2
#					pragma message("hmLib_update : #inform  : [config_vc.h] 拡張子がhppからhに、v1_01(v3_03_01)で変更されます。")
#				endif
#			else
#				if HMLIB_OUT>2 && HMLIB_VERSION<30301
#					pragma message("hmLib_update : #inform  : [config_vc.h] 拡張子がhppからhに、v1_01(v3_03_01)で変更されます。")
#				endif
#			endif
#			ifdef HMLIB_UPGRADE
#				if HMLIB_OUT>2 && 30301<=HMLIB_UPGRADE
#					pragma message("hmLib_update : #inform  : [config_vc.h] インクルードガードの不備が、v1_01(v3_03_01)で修正されます。")
#				elif HMLIB_OUT>1
#					pragma message("hmLib_update : #warning : [config_vc.h] インクルードガードの不備が、v1_01(v3_03_01)で修正されます。")
#				endif
#			else
#				if HMLIB_OUT>1
#					pragma message("hmLib_update : #warning : [config_vc.h] インクルードガードの不備が、v1_01(v3_03_01)で修正されます。")
#				endif
#			endif
#		endif
#endif
//hmLib_v3_01_02
#if HMLIB_VERSION<30102
/*
lab/filterbuf_v1_02/130101 hmIto
	iostreamに対応
*/
/*
cstring:v1_02/130105 hmIto
	format関数を追加
		変数を初期化できる destructで必要な処理を行わない
	is_construct関数を追加
		constructされているかどうかを判定できる
		formatされていることが条件
	move関数を追加
		一方のポインタからもう一方のポインタにオブジェクト自体を移動する
	swap関数を追加
		二つのポインタの中身を入れ替える
*/
/*
cdeque_v1_02/130105 hmIto
	format関数を追加
		変数を初期化できる destructで必要な処理を行わない
	is_construct関数を追加
		constructされているかどうかを判定できる
		formatされていることが条件
	move関数を追加
		一方のポインタからもう一方のポインタにオブジェクト自体を移動する
	swap関数を追加
		二つのポインタの中身を入れ替える
*/
/*
cqueue_v1_02/130105 hmIto
	format関数を追加
		変数を初期化できる destructで必要な処理を行わない
	is_construct関数を追加
		constructされているかどうかを判定できる
		formatされていることが条件
	move関数を追加
		一方のポインタからもう一方のポインタにオブジェクト自体を移動する
	swap関数を追加
		二つのポインタの中身を入れ替える
*/
/*
cstack_v1_02/130105 hmIto
	format関数を追加
		変数を初期化できる destructで必要な処理を行わない
	is_construct関数を追加
		constructされているかどうかを判定できる
		formatされていることが条件
	move関数を追加
		一方のポインタからもう一方のポインタにオブジェクト自体を移動する
	swap関数を追加
		二つのポインタの中身を入れ替える
*/
#endif
//hmLib_v3_01_03
#if HMLIB_VERSION<30103
/*
algorithm:v1_08/130109
	roulette_samplerの高速化
		内部で配列の形で保持しておくことで、繰り返し時の動作を高速化
		std::lower_boundを利用する事で高速化
*/
#endif
//hmLib_vX
#		if HMLIB_OUT>0 && HMLIB_UPDATE<HMLIB_VERSION
#			pragma message("hmLib_update : #error   : 非対応バージョンです。hmLib_updateのバージョンを更新してください。")
#		endif
#	endif
#
#endif
#
#endif
