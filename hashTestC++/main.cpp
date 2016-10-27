#include "HashTable.h"
#include "ShareMemory.h"

//g++ -o Test ShareMemory.cpp HashTable.cpp main.cpp
//0����ͨ�ڴ棬1�ǹ����ڴ�
#define MEMORY_TYPE 0

//hash���ڴ������ȫ��ָ��
_Hash_Table_Cell* g_Hash_Table_Cell;

static void Test_Insert(CHashTable& objHashTable, const char* pKey)
{
	printf("[Test_Insert]****************.\n");
	int nPos = objHashTable.Add_Hash_Data(pKey, pKey);
	if(-1 != nPos)
	{
		printf("[Test_Insert]nPos=%d, key=%s.\n", nPos, pKey);
	}
	else
	{
		printf("[Test_Insert]nPos=%d, key=%s.\n", nPos, pKey);
	}	
}

static void Test_Select(CHashTable& objHashTable, const char* pKey)
{
	printf("[Test_Select]****************.\n");
	char* pValue = objHashTable.Get_Hash_Box_Data(pKey);
	if(NULL != pValue)
	{
		printf("[main select]key=%s, value=%s.\n", pKey, pValue);
	}
	else
	{
		printf("[main select]key=%s.\n", pKey);
	}			
}

static void Test_Delete(CHashTable& objHashTable, const char* pKey)
{
	printf("[Test_Delete]****************.\n");
	int nPos = objHashTable.Del_Hash_Data(pKey);
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
	//��Ҫ���ڴ���С
	int nKeySize   = 20;
	int nValueSize = 100;
	int nPoolSize  = 3;
	size_t nArraySize = (sizeof(_Hash_Table_Cell) + nKeySize + nValueSize) * nPoolSize;

	if(MEMORY_TYPE == 1)
	{
		//���Թ����ڴ�
		bool blCreate = false;
		shm_id obj_shm_id;
		shm_key obj_shm_key = 3001; 
		g_Hash_Table_Cell = (_Hash_Table_Cell* )Open_Share_Memory_API(obj_shm_key, nArraySize, obj_shm_id, blCreate);
		printf("[main](%d)USED SHAER MEMORY.\n", MEMORY_TYPE);
	}
	else
	{
		//���Ե�ǰ�ڴ�
		g_Hash_Table_Cell = (_Hash_Table_Cell* )new char[nArraySize];
		printf("[main](%d)USED LOCAL MEMORY.\n", MEMORY_TYPE);
	}	
	
	//hash�س�ʼ��
	CHashTable objHashTable;
	objHashTable.Set_Base_Addr((char* )g_Hash_Table_Cell, nPoolSize);
	objHashTable.Set_Base_Key_Addr((char* )g_Hash_Table_Cell + sizeof(_Hash_Table_Cell) * nPoolSize, 
																	nKeySize * nPoolSize, nKeySize);
	objHashTable.Set_Base_Value_Addr((char* )g_Hash_Table_Cell + (sizeof(_Hash_Table_Cell) + nKeySize) * nPoolSize, 
																	nValueSize * nPoolSize, nValueSize);
	printf("[main]Init Finish(%d),size=%d.\n", nPoolSize, objHashTable.Get_Size());
	
	Test_Insert(objHashTable, "shiqiang");
	Test_Insert(objHashTable, "changsong");
	Test_Insert(objHashTable, "liuwei");
	Test_Delete(objHashTable, "changsong");
	Test_Insert(objHashTable, "anthony");
	
	Test_Select(objHashTable, "shiqiang");
	Test_Select(objHashTable, "changsong");
	Test_Select(objHashTable, "liuwei");
	Test_Select(objHashTable, "anthony");	
	
	Test_Insert(objHashTable, "shiqiang");
	Test_Insert(objHashTable, "changsong");
	Test_Insert(objHashTable, "liuwei");	
	
	//����ڴ�ռ�
	if(MEMORY_TYPE == 1)
	{
		shm_key obj_shm_key = 3001; 
		Delete_Share_Memory_API(obj_shm_key);
	}
	else
	{
		delete (char* )g_Hash_Table_Cell;
	}
	
	return 0;
}
