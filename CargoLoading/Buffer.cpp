
//TODO : Project File Recovery and save class 
#include "Buffer.h"
#include <memory>

CBuffer::CBuffer(void)
{
	m_Data=NULL;
	m_Allocated=0;
	m_size=0;
	m_ReadLoc=0;
	InitBuffer();

}
void CBuffer::InitBuffer()
{
	m_Data=new unsigned char[INIT_BUFF_SIZE];
	m_Allocated=INIT_BUFF_SIZE;
	m_size=0;
	m_ReadLoc=0;

}

CBuffer::~CBuffer(void)
{
	ClearMem();
	
}
void CBuffer::ClearMem()
{
	if(m_Data)
	{
		delete [] m_Data;
		m_Data=NULL;
	}
}
void CBuffer::Reset()
{
	ClearMem();
	m_Data=new unsigned char[INIT_BUFF_SIZE];
	m_Allocated=INIT_BUFF_SIZE;
	m_size=0;
	m_ReadLoc=0;

}
void CBuffer::AddTail(int a)
{
	if(m_Allocated<=m_size+sizeof(int))
		ReAllocateMem();

	memcpy(m_Data+m_size,&a,sizeof(a));
	int b;
	memcpy(&b,m_Data+m_size,sizeof(int));
	if(b!=a)
	{
		//AfxMessageBox("Error in CBuffer::AddTail(int)");
	}

	m_size+=sizeof(int);
}
void CBuffer::AddTailCodedChar(char ch)
{
	int NewChar=(ch+128)+2*2*2*2+2*2*2*2*2+2*2*2*2*2*2*2;
	AddTail(NewChar);
	return;
}
void CBuffer::AddTail(char ch)
{
	if(m_Allocated<=m_size+sizeof(char))
		ReAllocateMem();

	memcpy(m_Data+m_size,&ch,sizeof(ch));
	char b;
	memcpy(&b,m_Data+m_size,sizeof(char));
	if(b!=ch)
	{
		//AfxMessageBox("Error in CBuffer::AddTail(ch)");
	}
	m_size+=sizeof(char);
}
void CBuffer::AddTail(double a)
{
	if(m_Allocated<=m_size+sizeof(double))
		ReAllocateMem();

	memcpy(m_Data+m_size,&a,sizeof(double));
	double b;
	memcpy(&b,m_Data+m_size,sizeof(double));
	if(b!=a)
	{
		//AfxMessageBox("Error in CBuffer::AddTail(double)");
	}

	m_size+=sizeof(double);

}
void CBuffer::AddTail(ushort a)
{
	if(m_Allocated<=m_size+sizeof(ushort))
		ReAllocateMem();

	memcpy(m_Data+m_size,&a,sizeof(ushort));
	ushort b;
	memcpy(&b,m_Data+m_size,sizeof(ushort));
	if(b!=a)
	{
		//AfxMessageBox("Error in CBuffer::AddTail(ushort)");
	}
	m_size+=sizeof(ushort);
}
void CBuffer::AddTail(long a)
{
	if(m_Allocated<=m_size+sizeof(long))
		ReAllocateMem();

	memcpy(m_Data+m_size,&a,sizeof(long));
	long b;
	memcpy(&b,m_Data+m_size,sizeof(long));
	if(b!=a)
	{
		//AfxMessageBox("Error in CBuffer::AddTail(long)");
	}

	m_size+=sizeof(long);
}
void CBuffer::AddTail(float a)
{
	if(m_Allocated<=m_size+sizeof(float))
		ReAllocateMem();

	memcpy(m_Data+m_size,&a,sizeof(a));
	float b;
	memcpy(&b,m_Data+m_size,sizeof(float));
	if(b!=a)
	{
		//AfxMessageBox("Error in CBuffer::AddTail(float)");
	}

	m_size+=sizeof(float);

}
void CBuffer::AddTail(QString st,bool bUseCodec)
{
	int size=st.length();
	AddTail(size);
	for(int i=0;i<size;i++)
	{
		ushort ch2=st.at(i).unicode();
		AddTail(ch2);
	}
}
void CBuffer::AddTail(void* pData,int length)
{
	if(m_Allocated<=m_size+length)
		ReAllocateMem();
	memcpy(m_Data+m_size,pData,length);
	m_size+=length;
}
int CBuffer::MoveHead()
{
	m_ReadLoc=0;
	return 0;
}
int CBuffer::GetHeadI()
{
	int b=0;
	if(m_ReadLoc+sizeof(int)>m_size)
		return b;
	memcpy(&b,m_Data+m_ReadLoc,sizeof(int));
	m_ReadLoc+=sizeof(int);
	return b;

}
char CBuffer::GetHeadCodedC()
{
	int NewChar=GetHeadI();
	int newchar2=NewChar-2*2*2*2-2*2*2*2*2-2*2*2*2*2*2*2-128;
	char ch=(char)newchar2;
	return ch;
}
char CBuffer::GetHeadC()
{
	char b=0;
	if(m_ReadLoc+sizeof(char)>m_size)
		return b;
	memcpy(&b,m_Data+m_ReadLoc,sizeof(char));
	m_ReadLoc+=sizeof(char);
	return b;

}
double CBuffer::GetHeadD()
{
	double b=0;
	if(m_ReadLoc+sizeof(double)>m_size)
		return b;
	memcpy(&b,m_Data+m_ReadLoc,sizeof(double));
	m_ReadLoc+=sizeof(double);
	return b;

}
float CBuffer::GetHeadF()
{
	float b=0;
	if(m_ReadLoc+sizeof(float)>m_size)
		return b;
	memcpy(&b,m_Data+m_ReadLoc,sizeof(float));
	m_ReadLoc+=sizeof(float);
	return b;

}
float CBuffer::GetHeadL()
{
	long b=0;
	if(m_ReadLoc+sizeof(long)>m_size)
		return b;
	memcpy(&b,m_Data+m_ReadLoc,sizeof(long));
	m_ReadLoc+=sizeof(long);
	return b;
}
ushort CBuffer::GetHeadU()
{
	ushort b=0;
	if(m_ReadLoc+sizeof(ushort)>m_size)
		return b;
	memcpy(&b,m_Data+m_ReadLoc,sizeof(ushort));
	m_ReadLoc+=sizeof(ushort);
	return b;

}
QString CBuffer::GetHeadST(bool bUseCodec)
{
	int size=GetHeadI();
	QString ret;
	if(size<=0)
		return ret;
	for(int i=0;i<size;i++)
	{
		QString tmp;
		char ch;
		ushort ch2;
		if(bUseCodec)
			ch=GetHeadCodedC();
		else
			//ch=GetHeadC();
			ch2=GetHeadU();
		//tmp=ch;
		QChar chq(ch2);
		tmp=chq;
		//tmp=(wchar)ch2;
		ret+=tmp;
	}
	return ret;
}
void* CBuffer::GetHeadBytes(int length)
{
	unsigned char* b=NULL;
	if(m_ReadLoc+length>m_size)
		return b;
	b=new unsigned char[length];
	memcpy(b,m_Data+m_ReadLoc,length);
	m_ReadLoc+=length;
	return b;
}
void CBuffer::ReAllocateMem()
{
	ReAllocateMem(INIT_BUFF_SIZE);
};
void CBuffer::ReAllocateMem(int size)
{
	int newSize=m_Allocated+size;
	unsigned char* newAllocated=new unsigned char[newSize];
	unsigned char* tmpData=new unsigned char[m_size];
	memcpy(tmpData,m_Data,m_size);
	m_Data=newAllocated;
	memcpy(m_Data,tmpData,m_size);
	m_Allocated=newSize;
}
void* CBuffer::GetBuffer()
{
	return (void*)m_Data;
}
int CBuffer::GetBufferLength()
{
	return m_size;
}
void CBuffer::SetBuffer(void* ptr,int size)
{
	InitBuffer();
	if(size>m_Allocated)
		ReAllocateMem(size);
	memcpy(m_Data,ptr,size);
	m_size=size;
}
