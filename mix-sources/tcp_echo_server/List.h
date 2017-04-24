#ifndef COORD_LIST_H
#define COORD_LIST_H
 typedef void *ItemHandle;	//定义数据项类型  
typedef struct node *PNode;	//定义节点指针  
//节点的定义  
typedef struct node  {
	ItemHandle handle;	//数据域  
	PNode next;		//链域  
} Node, *SList;
 extern int SL_Creat(SList * p_list, int size);
extern int SL_Add(SList list, ItemHandle handle);
extern int SL_Insert(SList list, int pos, ItemHandle handle);
extern int SL_Gethandle(SList list, int pos, ItemHandle * p_handle);
extern int SL_Delete(SList list, int pos, ItemHandle * p_handle);
extern int SL_Sethandle(SList list, int pos, ItemHandle handle);
extern int SL_Find(SList list, int *pos, ItemHandle handle);
extern int SL_Empty(SList list);
extern int SL_Size(SList list);
extern int SL_Clear(SList * p_list);

#endif
