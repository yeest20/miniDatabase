/*************************************************************************
	> File Name: students_table.c
	> Author:YEE 
	> Mail:15509992895@163.com 
	> Created Time: Fri 30 Aug 2024 07:03:45 PM CST
 ************************************************************************/

#include<stdio.h>
#include<database.h>

// 函数声明集
static void init_table(struct Database *);
static size_t getDataSize();
static void printData(void *, int);
static void printTableHeader();
static void scanfData(void *);

// 构造函数，选择表层面提供所有表信息
__attribute__((constructor))
static void __register_table(){
    register_table(table_name, init_table);
    return;
}
//初始化表信息to操作
void init_table(struct Database *db){
    db->table_name = table_name;
    db->table_file = table_file;
    db->getDataSize = getDataSize;
    db->printData = printData;
    db->printTableHeader = printTableHeader;
    db->scanfData = scanfData;
    return;
}
// 获取表每条数据大小
size_t getDataSize(){
    return sizeof(table_data);
}

// 打印表头
void printTableHeader(){
    int len = 0;
    len += printf(head);
    for(int i = 0; i < len; i++) printf("=");
    printf("\n");
}

// 打印数据
void printData(void *__data, int id){
    table_data *data = (table_data*)(__data);
    print;
    return;
}

// 添加一条数据
void scanfData(void *__data){
    table_data *data = (table_data *)(__data);
    add;
    return;
}





