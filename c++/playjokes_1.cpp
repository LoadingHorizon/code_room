/*******************************************************************************
 * 这段代码是同学要我帮他写的，需求就是对于类employee这个链表类进行排序
 * 
 * 该链表类含有头结点，当时想没想就动手写了，花了10分钟写完了，不过一直有错误由于
 *
 * 他代码比较恶心，调试困难（需要一堆的输入数据），花了一个小时的40分钟才找到一处
 *
 * 错误。
 *
 * 肯定有很多地方需要改善，只不过正好上传上来罢了，以为例子。
 * ****************************************************************************/

void insert(employee*  Head, employee *node)
{
    employee *ptr = Head->Next;
    employee *pre = Head;
    while(ptr != NULL)
    {
        if(ptr->m_Payment <= node->m_Payment)
        {
            pre->Next = node;
            node->Next = ptr;
            break;                                   //忘了写break
        }

        pre = ptr;
        ptr = ptr->Next;
    }
    if(ptr == NULL)
    {
        pre->Next = node;
    }
}

employee* employee::Sort(employee*  Head) 
{//我创建的是带头节点的链表。用直接插入法。 
	if((Head->Next==NULL)||(Head->Next->Next==NULL))//此步条件判断非常有价值。 
	{ 
		cout<<"数据节点数少于2个，不用排序！"<<endl; 
		return Head; 
	} 
	//从这里开始，我写的 
    employee *sorted = Head;
    employee *unsorted = Head->Next->Next;
    employee *sorting = NULL;
    sorted->Next->Next = NULL;

    while(unsorted != NULL)
    {
        sorting = unsorted;
        unsorted = unsorted->Next;
        sorting->Next = NULL;
        insert(sorted, sorting);
    }

	return Head; 
} 
