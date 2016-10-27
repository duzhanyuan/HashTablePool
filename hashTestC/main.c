#include "HashTable.h"
#include "ShareMemory.h"

//gcc -o Test ShareMemory.c HashTable.c main.c
//0����ͨ�ڴ棬1�ǹ����ڴ�
#define MEMORY_TYPE 0

//hash���ڴ������ȫ��ָ��
_Hash_Table_Cell* g_Hash_Table_Cell;

static void Test_Insert(const char* pKey, int nCount)
{
	printf("[Test_Insert]****************.\n");
	int nPos = GetHashTablePos(pKey, g_Hash_Table_Cell, nCount, EM_INSERT);
	if(-1 != nPos)
	{
		printf("[Test_Insert]nPos=%d, key=%s.\n", nPos, g_Hash_Table_Cell[nPos].m_szKey);
	}
	else
	{
		printf("[Test_Insert]nPos=%d, key=%s.\n", nPos, pKey);
	}	
}

static void Test_Select(const char* pKey, int nCount)
{
	printf("[Test_Select]****************.\n");
	int nPos = GetHashTablePos(pKey, g_Hash_Table_Cell, nCount, EM_SELECT);
	if(-1 != nPos)
	{
		printf("[main select]nPos=%d, key=%s.\n", nPos, g_Hash_Table_Cell[nPos].m_szKey);
	}
	else
	{
		printf("[main select]nPos=%d, key=%s.\n", nPos, pKey);
	}			
}

static void Test_Delete(const char* pKey, int nCount)
{
	printf("[Test_Delete]****************.\n");
	int nPos = DelHashTablePos(pKey, g_Hash_Table_Cell, nCount);
	if(-1 != nPos)
	{
		printf("[main delete]nPos=%d, key=%s.\n", nPos, pKey);
	}
	else
	{
		printf("[main delete]nPos=%d, key=%s.\n", nPos, pKey);
	}		
}

//���Ի����ַ���key��o(1)�����У�ȷʵ�������ڹ����ڴ���ȥ������
int main()
{
	prepareCryptTable();
	
	//��Ҫ���ڴ���С
	int nKeySize   = 20;
	int nValueSize = 100;
	int nPoolCount = 3;
	size_t nArraySize = (sizeof(_Hash_Table_Cell) + nKeySize + nValueSize)*nPoolCount;

	char* pBase = NULL;
	if(MEMORY_TYPE == 1)
	{
		//���Թ����ڴ�
		int nCreate = 0;
		shm_id obj_shm_id;
		shm_key obj_shm_key = 3001; 
		pBase = Open_Share_Memory_API(obj_shm_key, nArraySize, &obj_shm_id, &nCreate);
		g_Hash_Table_Cell = (_Hash_Table_Cell* )pBase;
		printf("[main](%d)USED SHAER MEMORY.\n", MEMORY_TYPE);
	}
	else
	{
		//���Ե�ǰ�ڴ�
		pBase = (char* )malloc(nArraySize);
		g_Hash_Table_Cell = (_Hash_Table_Cell* )pBase;
		printf("[main](%d)USED LOCAL MEMORY.\n", MEMORY_TYPE);
	}	
	
	//��ʼ������
	int i = 0;
	int nKeyBase   = (sizeof(_Hash_Table_Cell) * nPoolCount);
	int nValueBase = nKeyBase + nKeySize * nPoolCount;
	printf("[main]nKeyBase = %d, nValueBase = %d.\n", nKeyBase, nValueBase);
	for(i = 0; i < nPoolCount; i++)
	{
		Init_Hash_Table_Cell(&g_Hash_Table_Cell[i]);
		Set_Hash_Table_Key_Cell(&g_Hash_Table_Cell[i], (char* )g_Hash_Table_Cell + nKeyBase + nKeySize * i, nKeySize);
		Set_Hash_Table_Value_Cell(&g_Hash_Table_Cell[i], (char* )g_Hash_Table_Cell + nValueBase + nValueSize * i, nValueSize);
	}
	printf("[main]Init Finish(%d),size=%d.\n", nPoolCount, nArraySize);
	
	Test_Insert("shiqiang", nPoolCount);
	Test_Insert("changsong", nPoolCount);
	Test_Insert("liuwei", nPoolCount);
	Test_Delete("changsong", nPoolCount);
	Test_Insert("anthony", nPoolCount);
	
	Test_Select("shiqiang", nPoolCount);
	Test_Select("changsong", nPoolCount);
	Test_Select("liuwei", nPoolCount);
	Test_Select("anthony", nPoolCount);	
	
	Test_Insert("shiqiang", nPoolCount);
	Test_Insert("changsong", nPoolCount);
	Test_Insert("liuwei", nPoolCount);	
	
	//����ڴ�ռ�
	if(MEMORY_TYPE == 1)
	{
		shm_key obj_shm_key = 3001; 
		Delete_Share_Memory_API(obj_shm_key);
	}
	else
	{
		free(pBase);
	}
	
	return 0;
}
