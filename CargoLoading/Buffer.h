#pragma once
#define INIT_BUFF_SIZE  10000
#include "QString"

#ifndef __BUFFER_H_
#define __BUFFER_H_


class CBuffer
{
public:
	CBuffer(void);
	~CBuffer(void);
public:
	unsigned char* m_Data;
	int m_size;
	int m_ReadLoc;
	int m_Allocated;
public:
	void AddTail(int a);
	void AddTail(char ch);
	void AddTailCodedChar(char ch);
	void AddTail(double a);
	void AddTail(float a);
	void AddTail(long a);
	void AddTail(ushort a);
	void AddTail(QString st,bool bUseCodec=false);
	void AddTail(void* pData,int length);
	void InitBuffer();
	void ClearMem();
	void Reset();

	int MoveHead();
	int GetHeadI();
	char GetHeadC();
	char GetHeadCodedC();
	double GetHeadD();
	float GetHeadF();
	float GetHeadL();
	ushort GetHeadU();
	QString GetHeadST(bool  bUseCodec=false);
	void* GetHeadBytes(int length);
	void* GetBuffer();
	int GetBufferLength();
	void SetBuffer(void* ptr,int size);


	void ReAllocateMem();
	void ReAllocateMem(int size);


};
#endif