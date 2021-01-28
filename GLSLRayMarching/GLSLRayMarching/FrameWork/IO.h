/////////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2016, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk			 //
//																				 //
// Author : Mac Lin									                             //
// Module : Magnum Engine v0.7.0												 //
// Date   : 14/Jun/2020											                 //
//																				 //
///////////////////////////////////////////////////////////////////////////////////
#ifndef _IO_h_
#define _IO_h_

#include "Platform.h"

	class IO
	{
	public:
		enum SeekDef
		{
			BEGIN = 0,
			CURRENT,
			END
		};

		enum AccessMode
		{
			READ = 0x01,
			WRITE = 0x02
		};

		IO(const std::string& path_);
		virtual ~IO();

		virtual bool Open(AccessMode mode);
		virtual bool OpenForAsync(AccessMode mode);
		virtual bool IsAsyncFinished() = 0;
		virtual void WaitAsyncFinished() = 0;
		virtual bool Close() = 0;
		virtual bool IsOpened() const = 0;

		virtual int Tell() const = 0;
		virtual int Seek(SeekDef from, int offset = 0) = 0;

		virtual int Read(void* buf, int size) = 0;
		virtual int Read(unsigned char* uc) = 0;
		virtual int Read(char* c) = 0;
		virtual int Read(unsigned short* us) = 0;
		virtual int Read(short* s) = 0;
		virtual int Read(unsigned int* ui) = 0;
		virtual int Read(int* i) = 0;
		virtual int Read(unsigned long* ui) = 0;
		virtual int Read(long* i) = 0;
		virtual int Read(float* f) = 0;
		virtual int Read(double* d) = 0;

		virtual int Write(const void* buf, int size) = 0;
		virtual int Write(const unsigned char* uc) = 0;
		virtual int Write(const char* c) = 0;
		virtual int Write(const unsigned short* us) = 0;
		virtual int Write(const short* s) = 0;
		virtual int Write(const unsigned int* ui) = 0;
		virtual int Write(const int* i) = 0;
		virtual int Write(const unsigned long* ui) = 0;
		virtual int Write(const long* i) = 0;
		virtual int Write(const float* f) = 0;
		virtual int Write(const double* d) = 0;

		virtual int GetSize() const = 0;

		bool IsReadEnabled() const;
		bool IsWriteEnabled() const;
		const std::string& GetPath() const;
	private:

	protected:
		std::string path;
		AccessMode accessMode;
		bool asyncMode;
	};

#endif