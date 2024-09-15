/*************************************************************************
	> File Name: database.c
	> Author:YEE 
	> Mail:15509992895@163.com 
	> Created Time: Fri 30 Aug 2024 03:38:30 PM CST
 ************************************************************************/

#include<stdio.h>
#include<database.h>
#include<stdlib.h>
#include<string.h>

struct Database db;
// 选择表层面操作
////存储表信息
struct TableInfo tables[100];
int table_cnt = 0;
////定义注册表
void register_table(const char *table_name,
    InitTable_T init_table){
    tables[table_cnt].table_name = table_name;
    tables[table_cnt].init_table = init_table;
    table_cnt += 1;
    return;
}
////获取数据链表节点
static struct table_data *getNewTableData(
        void *data,
        long offset){
    struct table_data *p = (struct table_data *)malloc(sizeof(struct table_data));
    p->data = malloc(db.getDataSize());//重新开辟空间
    memcpy(p->data, data, db.getDataSize());
    p->offset = offset;
    p->next = NULL;
    return p;
}

////下载表数据
static void load_table_data(){
    char buff[db.getDataSize()];
    struct table_data *p = &(db.head);
    int data_cnt = 0;
    while(1){
        long offset = ftell(db.table);
        if(fread(buff, db.getDataSize(), 1, db.table) == 0) break;
        p->next = getNewTableData(buff, offset);
        p = p->next;
        data_cnt += 1;
    }
    printf("load data success : %d\n", data_cnt);
    return;

    }
////打开表
static void open_table(){
    if(db.table == NULL){
        printf("can't open file:%s.....\n", db.table_file);
        exit(1);
    }
    load_table_data();
    return;

}
////清空表
static void clear_table(){
    struct table_data *p = db.head.next, *q;
    while(p){
        q = p->next;
        free(p->data);
        free(p);
        p = q;
    }
    return;
}
//// 关闭表
static void close_table(){
    clear_table(); 
    if(db.table == NULL) return;
    fclose(db.table);
    return;
}
//// 选择表
static enum OP_TYPE choose_table(){
    printf("0: quit\n");
    for(int i = 1; i < table_cnt + 1; i++){
        printf("%d : %s\n", i, tables[i - 1].table_name);
    }

    int ope;
    do{
        printf("input: ");
        scanf("%d", &ope);
    }while(ope < 0 || ope > table_cnt);

    if(ope == 0) return OP_END;
    else if(ope < table_cnt + 1) {
        clear_table();
        tables[ope - 1].init_table(&db);//选择数据库
        db.table = fopen(db.table_file,"rb+");
        open_table();
        fclose(db.table);
        return TABLE_USAGE;
    }
    return TABLE_USAGE;
}
// 表层面信息
//// 选择功能
static enum OP_TYPE table_usage(){
    printf("%s\n", db.table_name);
    printf("0: back\n");
    printf("1: list table\n");
    printf("2: add one item to table\n");
    printf("3: modify an item in table\n");
    printf("4: delete an item from table\n");
    
    int ope = -1;
    do{
        printf("input: ");
        scanf("%d", &ope);
        if(ope == 0) return CHOOSE_TABLE;
        if(ope == 1) return LIST_TABLE;
        if(ope == 2) return ADD_TABLE;
        if(ope == 3) return MODIFY_TABLE;
        if(ope == 4) return DELETE_TABLE;
    }while(ope < 0 || ope > 4);
    return TABLE_USAGE;
}
//// 打印表
static enum OP_TYPE list_table(){
    struct table_data *p = db.head.next;
    db.printTableHeader();
    int id = 0;
    while(p){
        db.printData(p->data, id);
        p = p->next;
        id += 1;
    }
    return TABLE_USAGE;
}
//// 添加数据
static void addOneItem(void *buff){
    fseek(db.table, 0, SEEK_END);
    long offset = ftell(db.table);
    struct table_data *p = &(db.head);
    while(p->next) p = p->next;
    p->next = getNewTableData(buff, offset);
    fwrite(buff, db.getDataSize(), 1, db.table);
    fflush(db.table);
    p = p->next;
    return;
}
static enum OP_TYPE add_table(){
    db.table = fopen(db.table_file, "rb+");
    char buff[db.getDataSize()];
    db.scanfData(buff);
    addOneItem(buff);
    printf("add success............\n");
    fclose(db.table);
    return TABLE_USAGE;
}
//// 修改数据
static int __list_table(){
    struct table_data *p = db.head.next;
    db.printTableHeader();
    int id = 0;
    while(p){
        db.printData(p->data, id);
        p = p->next;
        id += 1;
    }
    return id;
}
static void modifyOneItem(void *buff, int id){
    struct table_data *p = db.head.next;
    for(int i = 0; i < id; i++){
        p = p->next;
    }
    memcpy(p->data, buff, db.getDataSize());
    fseek(db.table, p->offset, SEEK_SET);
    fwrite(p->data, db.getDataSize(), 1, db.table);
    fflush(db.table);
    return;
}
static enum OP_TYPE modify_table(){
    db.table = fopen(db.table_file, "rb+");
    int n = __list_table();
    
    int modify_id = -1;
    do{
    printf("input modify id: ");
    scanf("%d", &modify_id);
    }while(modify_id < 0 || modify_id > n - 1);
    
    char buff[db.getDataSize()];
    db.scanfData(buff);
    modifyOneItem(buff, modify_id);
    printf("modity success.........\n");
    fclose(db.table);
    return TABLE_USAGE;
}
//// 删除数据
static void addItems(){
    fseek(db.table, 0, SEEK_SET);
    struct table_data *p = db.head.next;
    while(p){
        fwrite(p->data, db.getDataSize(), 1, db.table);
        p->offset = ftell(db.table);
        p = p->next;
    }
    return;
}

static void destroyData(struct table_data* p){
    free(p->data);
    free(p);
    return;
} 
static void clearTableData(){
    fclose(db.table);
    db.table = fopen(db.table_file, "wb");
    fclose(db.table);
    db.table = fopen(db.table_file, "rb+");
    return;
}
static void restorTableData()
{
    clearTableData();
    addItems();
    return;
}
void deleteOneItem(int id){
    struct table_data *p = &(db.head), *q;
    for(int i = 0; i < id; i++) p = p->next;
    q = p->next;
    p->next = q->next;
    destroyData(q);
    restorTableData();

    return;
}

static enum OP_TYPE delete_table(){
    db.table = fopen(db.table_file, "rb+");
    int n = __list_table();
    int delete_id = -1;
    do{
        printf("delete_id: ");
        scanf("%d", &delete_id);
    }while(delete_id < 0 || delete_id > n - 1);
    char confirm[100];
    printf("Confirm?(y/n):");
    scanf("%s", confirm);
    if(confirm[0] != 'y') return TABLE_USAGE;
    
    deleteOneItem(delete_id);
    printf("delete success.........\n");
    fclose(db.table);

    return TABLE_USAGE;
}

//run函数
enum OP_TYPE run(enum OP_TYPE status){
    switch(status){
        case CHOOSE_TABLE:{
           return choose_table();
        }break;
        case TABLE_USAGE:{
            return  table_usage();
        }break;
        case LIST_TABLE:{
            return list_table();
        }break;
        case ADD_TABLE:{
            return add_table();
        }break;
        case MODIFY_TABLE:{
            return modify_table();
        }break;
        case DELETE_TABLE:{
            return delete_table();
        }break;
        default : {
            printf("ERROR....\nUNKOWN status:%d\n", status);
        }break; 
    }

    return OP_END;
}



// run_database函数
void run_database(){
    enum OP_TYPE status = CHOOSE_TABLE;    
    while(1){
        status = run(status);
        if(status == OP_END) break;
    }

    return;
}

