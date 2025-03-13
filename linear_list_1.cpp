#include <iostream>
#define LIST_INIT_SIZE 100
//线性表初始分配空间
#define LIST_INCREMENT 10
//线性表存储空间分配增量
#define ElemType Book

#define OK 1 
#define ERROR 0
typedef int Status;

//用存储图书信息为例
typedef struct Book{
    char no[20];    //图书ISBN
    char name[50];  //图书名称
    float price;    //图书价格
};
//定义线性表结构
typedef struct SqList{
    ElemType *elem;
    int length;
    int listsize;//当前分配的存储容量，以ElemType为单位
};


//初始化
Status InitList(SqList &L)
{
    L.elem = new ElemType[LIST_INIT_SIZE];
    if(!L.elem)
    {
        return ERROR;
    }
    L.length = 0;
    return OK;
}
//销毁
Status DestroyList(SqList &L)
{
    if(L.elem)
    {
        free(L.elem);
    }
    L.elem = NULL;
    return OK;
}

//插入
Status ListInsert(SqList &L, int i, ElemType e)
{
//在顺序线性表的第i个位置插入新的元素e
//应分清位置和下标，两个极限值对应首位和末尾+1
    if(i<1 || i>L.length+1) 
    {
        return ERROR;
    }
    
    if(L.length>=L.listsize)
    {
        ElemType *newbase = (ElemType *)realloc(L.elem, (L.listsize + LIST_INCREMENT) * sizeof(ElemType));
        if(!newbase)
        {
            return ERROR;
        }
        L.elem = newbase;             //更新基址
        L.listsize += LIST_INCREMENT; //更新容量
    }
    ElemType* q = L.elem+i-1;         //q指向第i个位置
    
    for(ElemType *p = L.elem+L.length-1; p>=q; p--)
    {
        *(p+1) = *p;    
    }
    *q = e;
    L.length++;

    return OK;
}

//取值
Status GetElem(SqList L, int i, ElemType &e)
{
    if(i<1 || i>L.length)
    {
        return ERROR;
    }
    e = L.elem[i-1];

}

//顺序表的查找*
int LocateElem(SqList L, ElemType e)
{
    for(int i = 0; i<L.length; i++)
    {
        if(L.elem[i] == e)
        {
            return i+1;
        }
    }
    return 0;
}

//删除
Status ListDelete(SqList &L, int i, ElemType &e)
{
    if(i<1 || i>L.length)
    {
        return ERROR;                 //删除位置不合法
    }

    ElemType *p = L.elem+i-1;        //p指向要删除的元素
    e = *p;
    ElemType *q = L.elem+L.length-1; //q指向最后一个元素
    for(++p; p<=q; ++p)
    {
        *(p-1) = *p;                 //将后面的元素前移
    }
    L.length--;                      //表长度减1
    
    return OK;
}

//查找元素
int LocateElem_Sq(SqList L, ElemType e,
    Status (*compare)(ElemType, ElemType)) {
    // 在顺序表L中查询第一个值与e满足compare()的元素的位序
    // 若存在，则返回它的位序，否则返回0
    int i = 1;            // 位序从1开始
    ElemType *p = L.elem; // p的初值为第1元素的存储位置
    while(i <=L.length && !(*compare)(*p++, e)) 
    {
        ++i;
    }
    if(i<=L.length)
    {
        return i;
    }

    else
    {
        return 0;
    }
    
}

//非递减的有序表La和Lb归并为Lc ppt
void MergeList(SqList La, SqList Lb, SqList &Lc)
{
    InitList(Lc);
    int i = 1;
    int j = 1;
    int k = 0;

    int La_len = La.length;
    int Lb_len = Lb.length;

    ElemType ai, bj;

    while ((i <= La_len) && (j <= Lb_len)) 
    {
        GetElem(La, i, ai); 
        GetElem(Lb, j, bj);

        if (ai.no <= bj.no) 
        { 
            ListInsert(Lc, ++k, ai); ++i; 
        }
        else 
        { 
            ListInsert(Lc, ++k, bj); ++j; 
        }
        }
        while (i<=La_len) // 若 La 不空
        { 
            GetElem(La, i++, ai); ListInsert(Lc, ++k, ai);
        }
        while (j<=Lb_len) // 若 Lb 不空
        { 
            GetElem(Lb, j++, bj); ListInsert(Lc, ++k, bj); 
        }

}

//非递减的有序表La和Lb归并为Lc 教材
void MergeList_Sq(SqList La, SqList Lb, SqList &Lc)
{
    Lc.length = La.length + Lb.length;
    Lc.listsize = Lc.length;
    Lc.elem = new ElemType[Lc.listsize];
    
    ElemType *pc = Lc.elem;
    ElemType *pa = La.elem;
    ElemType *pb = Lb.elem;

    ElemType *pa_last = La.elem + La.length - 1;// 指向 La 最后一个元素
    ElemType *pb_last = Lb.elem + Lb.length - 1;// 指向 Lb 最后一个元素\

    while ((pa<=pa_last) && (pb<=pb_last)) //LA和LB都没到达表尾
    {
        if(pa->no <= pb->no)               //取小元素放到LC的表尾
        {
            *pc = *pa;
            ++pc;
        }
        else
        {
            *pc = *pb;
            ++pc;
        }
        
        while(pa <= pa_last)
        {
            *pc = *pa;
            ++pc;
        }
        while(pb <= pb_last)
        {
            *pc = *pb;
            ++pc;
        }
    }
}

    