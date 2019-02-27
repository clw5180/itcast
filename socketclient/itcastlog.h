//written by wangbaoming1999@163.com
//20140323
//itcastlog.h 日志头文件

#ifndef _ITCAST_LOG_H_
#define _ITCAST_LOG_H_


#ifdef _WIN32 //clw add：自写一个只打印__FILE__中文件名的函数
#define filename(x) strrchr(x,'\\')?strrchr(x,'\\')+1:x
#else
#define filename(x) strrchr(x,'/')?strrchr(x,'/')+1:x
#endif

/*
#define IC_NO_LOG			    0
#define IC_DEBUG_LEVEL			1
#define IC_INFO_LEVEL			2
#define IC_WARNING_LEVEL		3
#define IC_ERROR_LEVEL			4;
*/

/************************************************************************/
/* 
const char *file：文件名称
int line：文件行号
int level：错误级别
		0 -- 没有日志
		1 -- debug级别
		2 -- info级别
		3 -- warning级别
		4 -- err级别
int status：错误码
const char *fmt：可变参数
*/
/************************************************************************/
//实际使用的Level
extern int  LogLevel[5];

//#ifdef __cplusplus
//extern "C" {
//#endif 
void ITCAST_LOG(const char *file, int line, int level,  int status/*错误码，即返回值ret*/, const char *fmt/*即输出的日志信息*/, ...);
void LOG(const char *info/*即输出的日志信息*/, ...);

//#ifdef __cplusplus
//}
//#endif


#endif
