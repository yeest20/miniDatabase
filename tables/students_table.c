/*************************************************************************
	> File Name: students_table.c
	> Author:YEE 
	> Mail:15509992895@163.com 
	> Created Time: Fri 30 Aug 2024 07:03:45 PM CST
 ************************************************************************/

#include<stdio.h>
#include<database.h>

// 表信息
#define head " %5s| %15s| %5s| %5s| %7s|\n", "id", "name", "age","class", "height"
#define print {\
    printf(" %5d| %15s| %5d| %5d| %7.2lf|\n",\
            id,\
            data->name,\
            data->age,\
            data->class,\
            data->height);\
}
#define add {\
    printf("new item:(name age class height)\n");\
    printf("input: ");\
    scanf("%s%d%d%lf", \
            data->name,\
            &(data->age),\
            &(data->class),\
            &(data->height));\
}
static const char *table_name ="student table";
static const char *table_file = "./data/student_data.dat";

typedef struct Student{
    char name[20];
    int age;
    int class;
    double height;
}table_data;
#include "table_unit.t"




