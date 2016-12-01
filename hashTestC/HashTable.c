#include "HashTable.h"

void prepareCryptTable()
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
      	printf("[prepareCryptTable]index2=%d.\n", index2);
      }
      cryptTable[index2] = (temp1 | temp2); 
    } 
  } 
}

//��ʼ���ڴ���ز���
void Init_Hash_Table_Cell(_Hash_Table_Cell* p_Hash_Table_Cell)
{
	if(NULL != p_Hash_Table_Cell)
	{		
		p_Hash_Table_Cell->m_cExists       = 0;
		p_Hash_Table_Cell->m_nKeySize      = 0;
		p_Hash_Table_Cell->m_uHashA        = 0;
		p_Hash_Table_Cell->m_uHashB        = 0;
		p_Hash_Table_Cell->m_nNextKeyIndex = -1;
		p_Hash_Table_Cell->m_nProvKeyIndex = -1;
		p_Hash_Table_Cell->m_nValueSize    = 0;
		p_Hash_Table_Cell->m_szKey         = NULL;
		p_Hash_Table_Cell->m_szValue       = NULL;
	}
}

//��ʼ��һ��hash����key�ռ�
void Set_Hash_Table_Key_Cell(_Hash_Table_Cell* p_Hash_Table_Cell, char* pKey, int nKeySize)
{
	if(NULL != p_Hash_Table_Cell)
	{		
		p_Hash_Table_Cell->m_szKey         = pKey;
		p_Hash_Table_Cell->m_nKeySize      = nKeySize;
	}	
}

//��ʼ��һ��hash����value�ռ�
void Set_Hash_Table_Value_Cell(_Hash_Table_Cell* p_Hash_Table_Cell, char* pValue, int nValueSize)
{
	if(NULL != p_Hash_Table_Cell)
	{		
		p_Hash_Table_Cell->m_szValue       = pValue;
		p_Hash_Table_Cell->m_nValueSize    = nValueSize;
	}		
}

//�����ڴ���ز���
void Clear_Hash_Table_Cell(_Hash_Table_Cell* p_Hash_Table_Cell)
{
	if(NULL != p_Hash_Table_Cell)
	{
		p_Hash_Table_Cell->m_cExists       = 0;
		p_Hash_Table_Cell->m_uHashA        = 0;
		p_Hash_Table_Cell->m_uHashB        = 0;
		if(NULL != p_Hash_Table_Cell->m_szKey)
		{
			memset(p_Hash_Table_Cell->m_szKey, 0, p_Hash_Table_Cell->m_nKeySize);
		}
		if(NULL != p_Hash_Table_Cell->m_szValue)
		{
			memset(p_Hash_Table_Cell->m_szValue, 0, p_Hash_Table_Cell->m_nValueSize);
		}
	}	
}

//����key��Ӧ��hash��ֵ
unsigned long HashString(const char* pKey, unsigned long dwHashType)
{
	unsigned char *key = (unsigned char *)pKey;
	unsigned long seed1 = 0x7FED7FED, seed2 = 0xEEEEEEEE;
	int ch;
	while(*key != 0)
	{
		ch = toupper(*key++);
		seed1 = cryptTable[(dwHashType << 8) + ch] ^ (seed1 + seed2);
		seed2 = ch + seed1 + seed2 + (seed2 << 5) + 3;
	}
	
	//printf("[HashString]seed1=%lu.\n", seed1);
	return seed1;
}

//����֪�ĳ�ͻ������Ѱ�����һ��
int GetLastClashKey(_Hash_Table_Cell *lpTable, int nCount, int nStartIndex, const char *lpszString, unsigned long uHashA, unsigned long uHashB, EM_HASH_STATE emHashState)
{
	int nMaxIndex = nCount;
	int nRunCount = 0;
	
	//����֪������Ѱ�����һ��
	while(1)
	{
		if(nRunCount > nMaxIndex - 1)
		{
			return -1;
		}
		
		//printf("[GetLastClashKey](%s) curr(%d) next(%d)-->.\n", lpszString, nStartIndex, lpTable[nStartIndex].m_nNextKeyIndex);
		if(lpTable[nStartIndex].m_nNextKeyIndex == -1)
		{
			//�жϵ�ǰ�Ƿ��ǵ�ǰ����
			if(uHashA == lpTable[nStartIndex].m_uHashA && uHashB == lpTable[nStartIndex].m_uHashB)
			{
				//�ҵ���ƥ��Ķ��󣬷���֮
				/*
				printf("[GetLastClashKey][%d](%s).\n", 
											nStartIndex, lpTable[nStartIndex].m_szKey);
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
				int i;
				for(i = nStartIndex + 1; i < nCount; i++)
				{
					if(lpTable[i].m_cExists == 0)
					{
						lpTable[i].m_cExists = 1;
						lpTable[i].m_uHashA  = uHashA;
						lpTable[i].m_uHashB  = uHashB;
#ifndef WIN32
							sprintf(lpTable[i].m_szKey, "%s", lpszString);
#else
							sprintf_s(lpTable[i].m_szKey, lpTable[nStartIndex].m_nKeySize, "%s", lpszString);
#endif
						//��¼������Ϣ
						lpTable[nStartIndex].m_nNextKeyIndex = i;
						lpTable[i].m_nProvKeyIndex           = nStartIndex;
						//printf("[GetLastClashKey](%s) <--prov(%d) next(%d)-->.\n", lpszString, nStartIndex, i);
						return i;
					}
				}
				
				//��ǰ�ҿ���
				for(i = 0; i <= nStartIndex - 1; i++)
				{
					if(lpTable[i].m_cExists == 0)
					{
						lpTable[i].m_cExists = 1;
						lpTable[i].m_uHashA  = uHashA;
						lpTable[i].m_uHashB  = uHashB;
						if(NULL != lpTable[i].m_szKey)
						{
#ifndef WIN32
							sprintf(lpTable[i].m_szKey, "%s", lpszString);
#else
							sprintf_s(lpTable[i].m_szKey, lpTable[nStartIndex].m_nKeySize, "%s", lpszString);
#endif
						}						
						//��¼������Ϣ
						lpTable[nStartIndex].m_nNextKeyIndex = i;
						lpTable[i].m_nProvKeyIndex           = nStartIndex;
						//printf("[GetLastClashKey](%s) <--prov(%d) next(%d)-->.\n", lpszString, nStartIndex, i);
						return i;
					}							
				}
			}
			
			return -1;
		}
		else
		{
			if(emHashState == EM_INSERT)
			{
				if(lpTable[nStartIndex].m_cExists == 0)
				{
					lpTable[nStartIndex].m_cExists = 1;
					lpTable[nStartIndex].m_uHashA  = uHashA;
					lpTable[nStartIndex].m_uHashB  = uHashB;
					if(NULL != lpTable[nStartIndex].m_szKey)
					{
#ifndef WIN32
						sprintf(lpTable[nStartIndex].m_szKey, "%s", lpszString);
#else
						sprintf_s(lpTable[nStartIndex].m_szKey, lpTable[nStartIndex].m_nKeySize, "%s", lpszString);
#endif
					}	

					return nStartIndex;
				}
			}			
			
			//����Ѱ��
			if(uHashA == lpTable[nStartIndex].m_uHashA && uHashB == lpTable[nStartIndex].m_uHashB)
			{
				//�ҵ���ƥ��Ķ��󣬷���֮
				printf("[GetLastClashKey]Search Count=%d.\n", nRunCount);				
				return nStartIndex;
			}
			
			/*
			printf("[GetLastClashKey][%d](%s) <--prov(%d) next(%d)-->.\n", 
							nStartIndex, lpTable[nStartIndex].m_szKey, 
							lpTable[nStartIndex].m_nProvKeyIndex, lpTable[nStartIndex].m_nNextKeyIndex);
			*/
			nStartIndex = lpTable[nStartIndex].m_nNextKeyIndex;	
		}
		
		nRunCount++;
	}
}

//�õ�hashָ����λ��
int GetHashTablePos(const char *lpszString, _Hash_Table_Cell *lpTable, int nCount, EM_HASH_STATE emHashState)
{
	const int HASH_OFFSET = 0, HASH_A = 1, HASH_B = 2;
	unsigned long uHash = HashString(lpszString, HASH_OFFSET);
	unsigned long uHashA = HashString(lpszString, HASH_A);
	unsigned long uHashB = HashString(lpszString, HASH_B);
	unsigned long uHashStart = uHash % nCount, uHashPos = uHashStart;
	
	if (lpTable[uHashPos].m_cExists == 0)
	{
		if(EM_INSERT == emHashState)
		{
			lpTable[uHashPos].m_cExists = 1;
			lpTable[uHashPos].m_uHashA  = uHashA;
			lpTable[uHashPos].m_uHashB  = uHashB;
			sprintf(lpTable[uHashPos].m_szKey, "%s", lpszString);
			return (int)uHashPos;
		}
		else
		{
			return (int)uHashPos;
		}
				
	}
	else if(lpTable[uHashPos].m_uHashA == uHashA && lpTable[uHashPos].m_uHashB == uHashB)	
	{
		//�������hashֵ��ȣ���ֱ�ӷ���
		return (int)uHashPos;
	}
	else
	{
		return GetLastClashKey(lpTable, nCount, (int)uHashStart, lpszString, uHashA, uHashB, emHashState);
	}
	
	//printf("[GetHashTablePos]no find.\n");
	return -1; //Error value
}

//ɾ��һ��hash��ֵ��
int DelHashTablePos(const char *lpszString, _Hash_Table_Cell *lpTable, int nCount)
{
	int nPos = GetHashTablePos(lpszString, lpTable, nCount, EM_SELECT);
	if(-1 == nPos)
	{
		return -1;
	}
	else
	{
		if(-1 != lpTable[nPos].m_nProvKeyIndex)
		{
			lpTable[lpTable[nPos].m_nProvKeyIndex].m_nNextKeyIndex = lpTable[nPos].m_nNextKeyIndex;
		}
		
		if(-1 != lpTable[nPos].m_nNextKeyIndex)
		{
			lpTable[lpTable[nPos].m_nNextKeyIndex].m_nProvKeyIndex = lpTable[nPos].m_nProvKeyIndex;
		}

		Clear_Hash_Table_Cell(&lpTable[nPos]);
				
		return nPos;
	}
}
