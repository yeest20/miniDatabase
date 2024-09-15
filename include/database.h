/*************************************************************************
	> File Name: database.h
	> Author:YEE 
	> Mail:15509992895@163.com 
	> Created Time: Fri 30 Aug 2024 03:37:24 PM CST
 ************************************************************************/


#ifndef _DATABASE_H
#define _DATABASE_H

struct Database;
struct table_data;
// InitTable_T: 表初始化入内存的函数
typedef void (*InitTable_T)(struct Database *);

// 界面枚举
enum OP_TYPE{
    CHOOSE_TABLE,
    TABLE_USAGE,
    OP_END,
    LIST_TABLE,
    ADD_TABLE,
    MODIFY_TABLE,
    DELETE_TABLE
};

// 所有表信息结构
struct TableInfo{
    const char *table_name;
    InitTable_T init_table;
};

// 表中数据链表结构
struct table_data{
    void *data;
    long offset;
    struct table_data *next;
};

// 表结构
struct Database{
    const char *table_name;
    const char *table_file;
    FILE *table;
    size_t (*getDataSize)();
    struct table_data head;
    void (*printData)(void *, int);
    void (*printTableHeader)();
    void (*scanfData)(void *);
};



// 保证全程序唯一的表
extern struct Database db;


//函数

void run_database();
//// 注册表函数
void register_table(const char *, InitTable_T);

#endif
