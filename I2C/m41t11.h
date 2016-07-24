/*************************************************************************
    > File Name: m41t11.h
    > Author: ZHAOCHAO
    > Mail: 479680168@qq.com 
    > Created Time: Sun 24 Jul 2016 04:36:50 AM PDT
 ************************************************************************/

/*
 * FILE: m41t11.h
 * 提供设置、读取RTC芯片m41t11的函数
 */
struct rtc_time {
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_wday;
    int tm_mday;
	int tm_mon;
	int tm_year;
    int cs;
};
    
int m41t11_get_datetime(struct rtc_time *dt);
int m41t11_set_datetime(struct rtc_time *dt);

