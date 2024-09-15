/*************************************************************************
	> File Name: students_table.c
	> Author:YEE 
	> Mail:15509992895@163.com 
	> Created Time: Fri 30 Aug 2024 07:03:45 PM CST
 ************************************************************************/

#include<stdio.h>
#include<database.h>

// 表信息
#define head " %5s| %10s| %5s| %10s|\n", "id", "name", "stuNo", "master"
#define print {\
    printf(" %5d| %10s| %5d| %10s|\n",\
            id,\
            data->name,\
            data->stuNo,\
            data->master);\
}
#define add {\
    printf("new item:(name stuNo master)\n");\
    printf("input: ");\
    scanf("%s%d%s", \
            data->name,\
            &(data->stuNo),\
            data->master);\
}
static const char *table_name ="class table";
static const char *table_file = "./data/class_data.dat";

typedef struct Class{
    char name[20];
    int stuNo;
    char master[20];
} table_data;

#include "table_unit.t"

