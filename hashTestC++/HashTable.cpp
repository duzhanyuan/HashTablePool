#include "HashTable.h"

CHashTable::CHashTable()
{
	m_lpTable    = NULL;
	m_nCount     = 0;
	m_nKeySize   = 0;
	m_nValueSize = 0;
	
	memset(m_cryptTable, 0, sizeof(m_cryptTable));
	
	//��ʼ���������ݴ�
	prepareCryptTable();
}

CHashTable::~CHashTable()
{
}

void CHashTable::Set_Base_Addr(char* pData, int nCount)
{
	m_lpTable = (_Hash_Table_Cell*)pData;
	m_nCount  = nCount;
	for(int i = 0; i < m_nCount; i++)
	{
		m_lpTable[i].Init();
	}
}

void CHashTable::Set_Base_Key_Addr(char* pData, int nSize, int nKeySize)
{
	m_nKeySize    = nKeySize;
	int nCurrSize = 0;
	for(int i = 0; i < m_nCount; i++)
	{
		nCurrSize = nKeySize * (i + 1);
		if(nCurrSize <= nSize)
		{
			m_lpTable[i].Set_Key(pData + (nKeySize * i), nKeySize);
		}
		else
		{
			return;
		}
	}	
}

void CHashTable::Set_Base_Value_Addr(char* pData, int nSize, int nValueSize)
{
	m_nValueSize = nValueSize;
	int nCurrSize = 0;
	for(int i = 0; i < m_nCount; i++)
	{
		nCurrSize = nValueSize * (i + 1);
		if(nCurrSize <= nSize)
		{
			m_lpTable[i].Set_Value(pData + (nValueSize * i), nValueSize);
		}
		else
		{
			return;
		}
	}		
}

//������Կ
void CHashTable::prepareCryptTable()
{ 
  unsigned long seed = 0x00100001, index1 = 0, index2 = 0, i;
  for(index1 = 0; index1 < 0x100; index1++)
  { 
    for(index2 = index1, i = 0; i < 5; i++, index2 += 0x100)
    { 
      unsigned long temp1, temp2;
      seed = (seed * 125 + 3) % 0x2AAAAB;
      temp1 = (seed & 0xFFFF) << 0x10;
      seed = (seed * 125 + 3) % 0x2AAAAB;
      temp2 = (seed & 0xFFFF);
      if(index2 > 1280)
      {
      	printf("[prepareCryptTable]index2=%u.\n", (unsigned int)index2);
      }
      m_cryptTable[index2] = (temp1 | temp2); 
    } 
  } 
}

//����key��Ӧ��hash��ֵ
unsigned long CHashTable::HashString(const char* pKey, unsigned long dwHashType)
{
	unsigned char *key = (unsigned char *)pKey;
	unsigned long seed1 = 0x7FED7FED, seed2 = 0xEEEEEEEE;
	int ch;
	while(*key != 0)
	{
		ch = toupper(*key++);
		seed1 = m_cryptTable[(dwHashType << 8) + ch] ^ (seed1 + seed2);
		seed2 = ch + seed1 + seed2 + (seed2 << 5) + 3;
	}
	
	//printf("[HashString]seed1=%lu.\n", seed1);
	return seed1;
}

//����֪�ĳ�ͻ������Ѱ�����һ��
int CHashTable::GetLastClashKey(int nStartIndex, const char *lpszString, unsigned long uHashA, unsigned long uHashB, EM_HASH_STATE emHashState)
{
	int nMaxIndex = m_nCount;
	int nRunCount = 0;
	
	//����֪������Ѱ�����һ��
	while(true)
	{
		if(nRunCount > nMaxIndex - 1)
		{
			return -1;
		}
		
		//printf("[GetLastClashKey](%s) curr(%d) next(%d)-->.\n", lpszString, nStartIndex, m_lpTable[nStartIndex].m_nNextKeyIndex);
		if(m_lpTable[nStartIndex].m_nNextKeyIndex == -1)
		{
			//�жϵ�ǰ�Ƿ��ǵ�ǰ����
			if(uHashA == m_lpTable[nStartIndex].m_uHashA && uHashB == m_lpTable[nStartIndex].m_uHashB)
			{
				//�ҵ���ƥ��Ķ��󣬷���֮
				/*
				printf("[GetLastClashKey][%d](%s).\n", 
											nStartIndex, m_lpTable[nStartIndex].m_szKey);
				*/
				printf("[GetLastClashKey]Search Count=%d.\n", nRunCount);			
				return nStartIndex;
			}			
			
			//����ǲ��룬��ȥ�ҿ��࣬������ǣ���ֱ�ӷ���
			if(emHashState == EM_INSERT)
			{
				//�ҵ�������ĩβ
				//��ʼѰ�ҿ����λ��
				//����ҿ���
				for(int i = nStartIndex + 1; i < m_nCount; i++)
				{
					if(m_lpTable[i].m_cExists == 0)
					{
						m_lpTable[i].m_cExists = 1;
						m_lpTable[i].m_uHashA  = uHashA;
						m_lpTable[i].m_uHashB  = uHashB;
						if(NULL != m_lpTable[i].m_szKey)
						{
							sprintf(m_lpTable[i].m_szKey, "%s", lpszString);	
						}
						//��¼������Ϣ
						m_lpTable[nStartIndex].m_nNextKeyIndex = i;
						m_lpTable[i].m_nProvKeyIndex           = nStartIndex;
						//printf("[GetLastClashKey](%s) <--prov(%d) next(%d)-->.\n", lpszString, nStartIndex, i);
						return i;
					}
				}
				
				//��ǰ�ҿ���
				for(int i = 0; i <= nStartIndex - 1; i++)
				{
					if(m_lpTable[i].m_cExists == 0)
					{
						m_lpTable[i].m_cExists = 1;
						m_lpTable[i].m_uHashA  = uHashA;
						m_lpTable[i].m_uHashB  = uHashB;
						if(NULL != m_lpTable[i].m_szKey)
						{
							sprintf(m_lpTable[i].m_szKey, "%s", lpszString);					
						}	
						//��¼������Ϣ
						m_lpTable[nStartIndex].m_nNextKeyIndex = i;
						m_lpTable[i].m_nProvKeyIndex           = nStartIndex;
						//printf("[GetLastClashKey](%s) <--prov(%d) next(%d)-->.\n", lpszString, nStartIndex, i);
						return i;
					}							
				}
			}
			
			return -1;
		}
		else
		{
			//����Ѱ��
			if(uHashA == m_lpTable[nStartIndex].m_uHashA && uHashB == m_lpTable[nStartIndex].m_uHashB)
			{
				//�ҵ���ƥ��Ķ��󣬷���֮
				printf("[GetLastClashKey]Search Count=%d.\n", nRunCount);				
				return nStartIndex;
			}
			
			/*
			printf("[GetLastClashKey][%d](%s) <--prov(%d) next(%d)-->.\n", 
							nStartIndex, m_lpTable[nStartIndex].m_szKey, 
							m_lpTable[nStartIndex].m_nProvKeyIndex, m_lpTable[nStartIndex].m_nNextKeyIndex);
			*/
			nStartIndex = m_lpTable[nStartIndex].m_nNextKeyIndex;	
		}
		
		nRunCount++;
	}
}

//�õ�hashָ����λ��
int CHashTable::GetHashTablePos(const char *lpszString, EM_HASH_STATE emHashState)
{
	const int HASH_OFFSET = 0, HASH_A = 1, HASH_B = 2;
	unsigned long uHash = HashString(lpszString, HASH_OFFSET);
	unsigned long uHashA = HashString(lpszString, HASH_A);
	unsigned long uHashB = HashString(lpszString, HASH_B);
	unsigned long uHashStart = uHash % m_nCount, uHashPos = uHashStart;
	
	if (m_lpTable[uHashPos].m_cExists == 0)
	{
		if(EM_INSERT == emHashState)
		{
			m_lpTable[uHashPos].m_cExists = 1;
			m_lpTable[uHashPos].m_uHashA  = uHashA;
			m_lpTable[uHashPos].m_uHashB  = uHashB;
			if(NULL != m_lpTable[uHashPos].m_szKey)
			{
				sprintf(m_lpTable[uHashPos].m_szKey, "%s", lpszString);
			}
			return (int)uHashPos;
		}
		else
		{
			return (int)uHashPos;
		}
				
	}
	else if(m_lpTable[uHashPos].m_uHashA == uHashA && m_lpTable[uHashPos].m_uHashB == uHashB)	
	{
		//�������hashֵ��ȣ���ֱ�ӷ���
		return (int)uHashPos;
	}
	else
	{
		return GetLastClashKey((int)uHashStart, lpszString, uHashA, uHashB, emHashState);
	}
	
	//printf("[GetHashTablePos]no find.\n");
	return -1; //Error value
}

int CHashTable::DelHashTablePos(const char *lpszString)
{
	int nPos = GetHashTablePos(lpszString, EM_SELECT);
	if(-1 == nPos)
	{
		return -1;
	}
	else
	{
		if(-1 != m_lpTable[nPos].m_nProvKeyIndex)
		{
			m_lpTable[m_lpTable[nPos].m_nProvKeyIndex].m_nNextKeyIndex = m_lpTable[nPos].m_nNextKeyIndex;
		}
		
		if(-1 != m_lpTable[nPos].m_nNextKeyIndex)
		{
			m_lpTable[m_lpTable[nPos].m_nNextKeyIndex].m_nProvKeyIndex = m_lpTable[nPos].m_nProvKeyIndex;
		}
		m_lpTable[nPos].Clear();
		
		return nPos;
	}	
}

int CHashTable::Get_Count()
{
	return (int)m_nCount;
}

size_t CHashTable::Get_Size()
{
	return (sizeof(_Hash_Table_Cell) + m_nKeySize + m_nValueSize) * m_nCount;
}

int CHashTable::Add_Hash_Data(const char* pKey, const char* pValue)
{
	if(NULL == m_lpTable)
	{
		//û���ҵ������ڴ�
		return -1;
	}
	
	int nPos = GetHashTablePos(pKey, EM_INSERT);
	if(-1 == nPos)
	{
		//�����ڴ�����
		return -1;
	}
	else
	{
		if(NULL != m_lpTable[nPos].m_szValue)
		{
			sprintf(m_lpTable[nPos].m_szValue, "%s", pValue);
		}
		return nPos;
	}
}

char* CHashTable::Get_Hash_Box_Data(const char* pKey)
{
	if(NULL == m_lpTable)
	{
		//û���ҵ������ڴ�
		return NULL;
	}	
	
	int nPos = GetHashTablePos(pKey, EM_SELECT);
	if(-1 == nPos)
	{
		//û���ҵ�
		return NULL;
	}
	else
	{
		return m_lpTable[nPos].m_szValue;
	}	
}

int CHashTable::Del_Hash_Data(const char* pKey)
{
	return DelHashTablePos(pKey);
}

