#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

//g++ -o Test main.cpp

typedef enum 
{
	EM_SELECT = 0,
	EM_INSERT,
}EM_HASH_STATE;

//hash��ṹ
typedef struct 
{
	char  m_cExists;                       //��ǰ���Ƿ��Ѿ�ʹ��,1�Ѿ�ʹ�ã�0û�б�ʹ��
	char* m_szKey;                         //��ǰ��keyֵ��û����Ϊ��
	int   m_nKeySize;                      //��ǰkey���ݳ���
	int   m_nNextKeyIndex;                 //������Ϣ����������г�ͻ,��¼��һ����ͻ������λ��
	int   m_nProvKeyIndex;                 //������Ϣ����������г�ͻ,��¼��һ����ͻ������λ��
	unsigned long m_uHashA;                //�ڶ��ε�hashkeyֵ
	unsigned long m_uHashB;                //�����ε�hashkeyֵ 
	char* m_szValue;                       //��ǰ������ָ��
	int   m_nValueSize;                    //��ǰ�����峤��  
}_Hash_Table_Cell;

char cryptTable[1280];

//������Կ
void prepareCryptTable();

//��ʼ��һ��p_Hash_Table_Cell�ڵ�
void Init_Hash_Table_Cell(_Hash_Table_Cell* p_Hash_Table_Cell);

//��ʼ��һ��hash����key�ռ�
void Set_Hash_Table_Key_Cell(_Hash_Table_Cell* p_Hash_Table_Cell, char* pKey, int nKeySize);

//��ʼ��һ��hash����value�ռ�
void Set_Hash_Table_Value_Cell(_Hash_Table_Cell* p_Hash_Table_Cell, char* pValue, int nValueSize);

//����һ��hash�����ڴ�
void Clear_Hash_Table_Value_Cell(_Hash_Table_Cell* p_Hash_Table_Cell);

//����һ��p_Hash_Table_Cell�ڵ�
void Clear_Hash_Table_Cell(_Hash_Table_Cell* p_Hash_Table_Cell);

unsigned long HashString(const char* pKey, unsigned long dwHashType);

int GetLastClashKey(_Hash_Table_Cell *lpTable, int nCount, int nStartIndex, const char *lpszString, unsigned long uHashA, unsigned long uHashB, EM_HASH_STATE emHashState);

int GetHashTablePos(const char *lpszString, _Hash_Table_Cell *lpTable, int nCount, EM_HASH_STATE emHashState);

int DelHashTablePos(const char *lpszString, _Hash_Table_Cell *lpTable, int nCount);

#endif
