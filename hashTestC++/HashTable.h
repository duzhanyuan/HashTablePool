#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdio.h>
#include <ctype.h>
#include <string.h>

enum EM_HASH_STATE
{
	EM_SELECT = 0,
	EM_INSERT,
};

//hash��ṹ
struct _Hash_Table_Cell 
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
	
	_Hash_Table_Cell()
	{
		Init();
	}
	
	void Init()
	{
		m_cExists       = 0;
		m_nKeySize      = 0;
		m_nValueSize    = 0;
		m_uHashA        = 0;
		m_uHashB        = 0;
		m_nNextKeyIndex = -1;
		m_nProvKeyIndex = -1;
		m_szKey         = NULL;
		m_szValue       = NULL;		
	}
	
	void Set_Key(char* pKey, int nKeySize)
	{
		m_szKey         = pKey;
		m_nKeySize      = nKeySize;
	}
	
	void Set_Value(char* pValue, int nValueSize)
	{
		m_szValue       = pValue;
		m_nValueSize    = nValueSize;
	}
	
	void Clear()
	{
		m_cExists       = 0;
		m_uHashA        = 0;
		m_uHashB        = 0;
		m_nNextKeyIndex = -1;
		m_nProvKeyIndex = -1;		
		if(NULL != m_szKey)	
		{
			memset(m_szKey, 0, m_nKeySize);		
		}
		if(NULL != m_szValue)
		{
			memset(m_szValue, 0, m_nValueSize);
		}
	}  
};

//hashTable��
class CHashTable
{
public:
	CHashTable();
	~CHashTable();
	
	//����һ����֪���ڴ������(�����ʼ������)
	void Set_Base_Addr(char* pData, int nCount);
	//����һ��Hash Key�������(�����ʼ������)
	void Set_Base_Key_Addr(char* pData, int nSize, int nKeySize);
	//����һ��Hash Value�ĵ�ַ��(�����ʼ������)
	void Set_Base_Value_Addr(char* pData, int nSize, int nValueSize);
	
	//�õ���ǰ������ܸ���
	int Get_Count();
	//�õ����������ڴ��С
	size_t Get_Size();
	
	//���һ��Hash���ݿ�
	int Add_Hash_Data(const char* pKey, const char* pValue);
	//���һ������ӳ���Ӧ��ֵ
	char* Get_Hash_Box_Data(const char* pKey);
	//����һ��hash���ݿ�
	int Del_Hash_Data(const char* pKey);
	
private:
	void prepareCryptTable();
	unsigned long HashString(const char* pKey, unsigned long dwHashType);
	int GetLastClashKey(int nStartIndex, const char *lpszString, unsigned long uHashA, unsigned long uHashB, EM_HASH_STATE emHashState);
	int GetHashTablePos(const char *lpszString, EM_HASH_STATE emHashState);	
	int DelHashTablePos(const char *lpszString);
	
private:
	_Hash_Table_Cell* m_lpTable;	
	char              m_cryptTable[1280];
	int               m_nCount;
	int               m_nKeySize;
	int               m_nValueSize;
};


#endif
