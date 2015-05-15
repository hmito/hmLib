#ifndef HMLIB_CONFIG_INC
#define HMLIB_CONFIG_INC

//Define the output level at the compile of hmLib library files.
//	0: Nothing are output.
//	1: Only Error information are output.
//	2: Both Error and Warning information are output.
//	3: Error, Warning and noted information are output.
//	4: All information are output.
#define HMLIB_OUT 4

//Enable if the compiled library files rather than default library files are needed.
//	In enable case, all macros link to default lib files are disabled.
#define HMLIB_NOLIB

//Enable if you want to use macros write without "hmLib_" prefix, e.g. write hmLib_??? as just ???.
//	In enable case, all macros with "hmLib_" prefix are redefined that without the prefix.
//#define HMLIB_NOPREFIX

//Define the byte size of each types in this environment.
#define HMLIB_TYPE_SIZE_CHAR 1
#define HMLIB_TYPE_SIZE_SHORT 2
#define HMLIB_TYPE_SIZE_INT 4
#define HMLIB_TYPE_SIZE_LONG 4
#define HMLIB_TYPE_SIZE_LONG_LONG 8
#define HMLIB_TYPE_SIZE_FLOAT 4
#define HMLIB_TYPE_SIZE_DOUBLE 8
#define HMLIB_TYPE_SIZE_LONG_DOUBLE 8

#endif
