/*
 * logger.c
 *
 * Copyright (c) Zhou Peng <lockrecv@qq.com>
 */

#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdarg.h>

#include "logger.h"

static FILE *logger_cache = NULL;
static int tty_cache = 0;

static const char *strnow(char *buffer, size_t bufsize)
{
	static const char *timefmt = "[%Y-%m-%d %H:%M:%S]";

	time_t now;
	struct tm *timeinfo;

	time(&now);
	timeinfo = localtime(&now);
	strftime(buffer, bufsize, timefmt, timeinfo);
	return buffer;
}

static int logger(const char *color, const char *level,
		  const  char *fmt, va_list args)
{
	static const char *end = "\e[00m";
	static const int bufsize = 32;

	int cnt = 0;
	char buffer[bufsize];

	if (!logger_cache)
		return cnt;

	if (tty_cache)
		cnt += fprintf(logger_cache, "%s", color);
	cnt += fprintf(logger_cache, "%s", strnow(buffer, bufsize));
	cnt += fprintf(logger_cache, "%s", level);
	cnt += vfprintf(logger_cache, fmt, args);
	if (tty_cache)
		cnt += fprintf(logger_cache, "%s", end);
	return cnt;
}

int logger_open(const char *logfile)
{
	FILE *log = fopen(logfile, "a+");
	if (!log)
		return 0;
	int fd = fileno(log);
	tty_cache = isatty(fd);
	logger_cache = log;
	return 1;
}

int logger_close()
{
	if (!logger_cache) {
		fclose(logger_cache);
		return 1;
	} else
		return 0;
}

int logger_info(const char *fmt, ...)
{
	static const char *blue = "\e[00;36m";
	static const char *info = "[info] ";

	int cnt;
	va_list args;

	va_start(args, fmt);
	cnt = logger(blue, info, fmt, args);
	va_end(args);

	return cnt;
}

int logger_warning(const char *fmt, ...)
{
	static const char *red = "\e[00;33m";
	static const char *warning = "[warning] ";

	int cnt;
	va_list args;

	va_start(args, fmt);
	cnt = logger(red, warning, fmt, args);
	va_end(args);

	return cnt;
}

int logger_error(const char *fmt, ...)
{
	static const char *red = "\e[00;31m";
	static const char *error = "[error] ";

	int cnt;
	va_list args;

	va_start(args, fmt);
	cnt = logger(red, error, fmt, args);
	va_end(args);

	return cnt;
}
