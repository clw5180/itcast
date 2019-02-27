#define _CRT_SECURE_NO_WARNINGS

#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/************************************************************************/
/* clw modify 20190226：for .cpp file using open, close or write function.
/************************************************************************/
#ifdef _WIN32
#include <io.h>
#pragma warning(disable: 4996)
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>  #clw note:for write(),read() and close()
#endif
/************************************************************************/

#include "itcastlog.h"

#define ITCAST_DEBUG_FILE_	"clw.log"
#define ITCAST_MAX_STRING_LEN 		10240

//Level类别
#define IC_NO_LOG_LEVEL			0
#define IC_DEBUG_LEVEL			1
#define IC_INFO_LEVEL			2
#define IC_WARNING_LEVEL		3
#define IC_ERROR_LEVEL			4

int  LogLevel[5] = { IC_NO_LOG_LEVEL, IC_DEBUG_LEVEL, IC_INFO_LEVEL, IC_WARNING_LEVEL, IC_ERROR_LEVEL };

//Level的名称
char ICLevelName[5][10] = { "NOLOG", "DEBUG", "INFO", "WARNING", "ERROR" };

static int ITCAST_Error_GetCurTime(char* strTime)
{
	struct tm*		tmTime = NULL;
	size_t			timeLen = 0;
	time_t			tTime = 0;

	tTime = time(NULL);
	tmTime = localtime(&tTime);
	//timeLen = strftime(strTime, 33, "%Y(Y)%m(M)%d(D)%H(H)%M(M)%S(S)", tmTime);
	timeLen = strftime(strTime, 33, "%Y.%m.%d %H:%M:%S", tmTime);

	return timeLen;
}

static int ITCAST_Error_OpenFile(int* pf)
{
	char	fileName[1024];

	memset(fileName, 0, sizeof(fileName));
#ifdef _WIN32
	//sprintf(fileName, "c:\\itcast\\%s",ITCAST_DEBUG_FILE_);
	sprintf(fileName, "./%s", ITCAST_DEBUG_FILE_); //clw modify：暂时改为在当前目录打印日志
#else
	sprintf(fileName, "%s/log/%s", getenv("HOME"), ITCAST_DEBUG_FILE_);
#endif

	*pf = open(fileName, O_WRONLY | O_CREAT | O_APPEND, 0666);
	if (*pf < 0)
	{
		return -1;
	}

	return 0;
}

static void ITCAST_Error_Core(const char *file, int line, int level, int status, const char *fmt, va_list args)
{
	char str[ITCAST_MAX_STRING_LEN];
	int	 strLen = 0;
	char tmpStr[64];
	int	 tmpStrLen = 0;
	int  pf = 0;

	//初始化
	memset(str, 0, ITCAST_MAX_STRING_LEN);
	memset(tmpStr, 0, 64);

	//加入LOG时间       
	tmpStrLen = ITCAST_Error_GetCurTime(tmpStr);
	tmpStrLen = sprintf(str, "[%s]  ", tmpStr);
	strLen = tmpStrLen;

	////加入LOG等级           clw note：个人使用意义不大，如有需要可以放开
	//tmpStrLen = sprintf(str+strLen, "[%s] ", ICLevelName[level]);
	//strLen += tmpStrLen;

	//加入LOG发生文件和所在行数   clw modify 20190226
	tmpStrLen = sprintf(str + strLen, "[%s(%d):] ", file, line);  //sprintf 返回以format为格式argument为内容组成的结果被写入buffer 的字节数，结束字符‘\0’不计入内。即，如果“Hello”被写入空间足够大的buffer后，函数sprintf 返回5
	strLen += tmpStrLen;

	//加入LOG日志信息
	tmpStrLen = vsprintf(str + strLen, fmt, args);
	strLen += tmpStrLen;

	//加入LOG函数返回值
	if (status != 0)       //clw note：默认如果return非0值，则代表发生了某一类错误
	{
		tmpStrLen = sprintf(str + strLen, ", return %d\n", status);
	}
	else
	{
		tmpStrLen = sprintf(str + strLen, " [SUCCESS]");
	}
	strLen += tmpStrLen;

	//打开LOG文件
	if (ITCAST_Error_OpenFile(&pf))
	{
		return;
	}

	//写入LOG文件
	write(pf, str, strLen);
	//IC_Log_Error_WriteFile(str);

	//关闭文件
	close(pf);

	return;
}


void ITCAST_LOG(const char *file, int line, int level, int status, const char *fmt, ...)
{
	va_list args;

	//判断是否需要写LOG
	//	if(level!=IC_DEBUG_LEVEL && level!=IC_INFO_LEVEL && level!=IC_WARNING_LEVEL && level!=IC_ERROR_LEVEL)
	if (level == IC_NO_LOG_LEVEL)
	{
		return;
	}

	//调用核心的写LOG函数
	va_start(args, fmt);
	ITCAST_Error_Core(file, line, level, status, fmt, args);
	va_end(args);

	return;
}

void LOG(const char *info, ...)
{
	char str[ITCAST_MAX_STRING_LEN];
	int	 strLen = 0;
	char tmpStr[64];
	int	 tmpStrLen = 0;
	int  pf = 0;

	//初始化
	memset(str, 0, ITCAST_MAX_STRING_LEN);
	memset(tmpStr, 0, 64);

	//加入LOG时间       
	tmpStrLen = ITCAST_Error_GetCurTime(tmpStr);
	tmpStrLen = sprintf(str, "%s  ", tmpStr);
	strLen = tmpStrLen;

	//加入LOG日志信息
	//clw note：可以把返回值信息用ret=%d的格式写到日志里面去，无须再添加一个参数！
	va_list args;
	va_start(args, info);
	tmpStrLen = vsprintf(str + strLen, info, args);
	va_end(args);
	strLen += tmpStrLen;
	tmpStrLen = sprintf(str + strLen, "\n");
	strLen += tmpStrLen;

	//打开LOG文件
	if (ITCAST_Error_OpenFile(&pf))
	{
		return;
	}

	//写入LOG文件
	write(pf, str, strLen);
	//IC_Log_Error_WriteFile(str);

	//关闭文件
	close(pf);
}

