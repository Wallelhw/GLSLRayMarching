///////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2016, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk			 //
//																				 //
// Author : Mac Lin									                             //
// Module : Magnum Engine v0.7.0												 //
// Date   : 14/Jun/2020											                 //
//																				 //
///////////////////////////////////////////////////////////////////////////////////
#include "Platform.h"
#include "FileIO.h"

#if defined(_WIN32) ||  defined(_WIN64)
#include <Windows.h>
#elif defined(__ANDROID__ )
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#elif ( defined(TARGET_OS_IPHONE) || defined(TARGET_IPHONE_SIMULATOR) )
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#endif

#define FILEIO_USE_FOPEN

typedef struct
{
#if defined (FILEIO_USE_FOPEN)
	FILE* handle;
#elif defined(_WIN32) ||  defined(_WIN64)
	HANDLE		handle;
	OVERLAPPED	overlapped;
#elif defined(__ANDROID__ )
	int			handle;
#elif ( defined(TARGET_OS_IPHONE) || defined(TARGET_IPHONE_SIMULATOR) )
	int			handle;
#endif
	unsigned int position;
	unsigned int length;
}FileDesc;

FileIO::FileIO(const std::string& path_)
	: IO(path_)
	, user(0)
{
#if defined (FILEIO_USE_FOPEN)
	user = (void*)(new FileDesc);
	Assert(user);

	FileDesc* fd = ((FileDesc*)user);

	fd->handle = 0;
	fd->position = 0;
	fd->length = 0;
#elif defined(_WIN32) ||  defined(_WIN64)
	user = (void*)(new FileDesc);
	Assert(user);

	FileDesc* fd = ((FileDesc*)user);

	fd->handle = INVALID_HANDLE_VALUE;
	fd->position = 0;
	fd->length = 0;
#elif defined(__ANDROID__ )
	user = (void*)(new FileDesc);
	Assert(user);

	FileDesc* fd = ((FileDesc*)user);

	fd->handle = -1;
	fd->position = 0;
	fd->length = 0;
#elif ( defined(TARGET_OS_IPHONE) || defined(TARGET_IPHONE_SIMULATOR) )
	user = (void*)(new FileDesc);
	Assert(user);

	FileDesc* fd = ((FileDesc*)user);

	fd->handle = -1;
	fd->position = 0;
	fd->length = 0;
#endif
}

FileIO::~FileIO()
{
	Close();

	delete ((FileDesc*)user);
}

bool FileIO::Open(AccessMode mode)
{
	IO::Open(mode);

#if defined (FILEIO_USE_FOPEN)
	FileDesc* fd = ((FileDesc*)user);
	Assert(fd->handle == 0);

	if (IsReadEnabled() && !IsWriteEnabled())
		fd->handle = fopen(path.c_str(), "rb");
	else if (!IsReadEnabled() && IsWriteEnabled())
		fd->handle = fopen(path.c_str(), "wb");
	else if (IsReadEnabled() && IsWriteEnabled())
		fd->handle = fopen(path.c_str(), "rb+wb");

	if (!fd->handle)
		return false;

	fd->position = 0;

	::fseek(fd->handle, 0, SEEK_END);
	fd->length = ::ftell(fd->handle);
	::fseek(fd->handle, 0, SEEK_SET);
#elif defined(_WIN32) ||  defined(_WIN64)
	FileDesc* fd = ((FileDesc*)user);
	Assert(fd->handle == INVALID_HANDLE_VALUE);

	DWORD dwDesiredAccess = 0;
	if (IsReadEnabled() && !IsWriteEnabled())
		dwDesiredAccess |= GENERIC_READ;
	else if (!IsReadEnabled() && IsWriteEnabled())
		dwDesiredAccess |= GENERIC_WRITE;
	else if (IsReadEnabled() && IsWriteEnabled())
		dwDesiredAccess |= (GENERIC_READ | GENERIC_WRITE);

	DWORD dwCreationDisposition = 0;
	if (IsReadEnabled() && !IsWriteEnabled())
		dwCreationDisposition |= OPEN_EXISTING;
	else if (!IsReadEnabled() && IsWriteEnabled())
		dwCreationDisposition |= CREATE_ALWAYS;
	else if (IsReadEnabled() && IsWriteEnabled())
		dwCreationDisposition |= (OPEN_EXISTING | CREATE_ALWAYS);

	fd->handle = CreateFile(path.c_str(), dwDesiredAccess, 0, 0, dwCreationDisposition,
		FILE_ATTRIBUTE_NORMAL, 0);
#elif defined(__ANDROID__ )
	FileDesc* fd = ((FileDesc*)user);
	Assert(fd->handle == -1);

	int flag = O_SYNC;
	if (IsReadEnabled() && !IsWriteEnabled())
		flag |= O_RDONLY;
	else if (!IsReadEnabled() && IsWriteEnabled())
		flag |= (O_WRONLY | O_CREAT);
	else if (IsReadEnabled() && IsWriteEnabled())
		flag |= (O_RDWR | O_CREAT);

	fd->handle = ::open(path.text(), flag, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
#elif ( defined(TARGET_OS_IPHONE) || defined(TARGET_IPHONE_SIMULATOR) )
	FileDesc* fd = ((FileDesc*)user);
	Assert(fd->handle == -1);

	int flag = O_SYNC;
	if (IsReadEnabled() && !IsWriteEnabled())
		flag |= O_RDONLY;
	else if (!IsReadEnabled() && IsWriteEnabled())
		flag |= (O_WRONLY | O_CREAT);
	else if (IsReadEnabled() && IsWriteEnabled())
		flag |= (O_RDWR | O_CREAT);

	fd->handle = ::open(path.text(), flag, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
#endif
	return IsOpened();
}

bool FileIO::OpenForAsync(AccessMode mode)
{
	IO::OpenForAsync(mode);

#if defined (FILEIO_USE_FOPEN)
	FileDesc* fd = ((FileDesc*)user);
	Assert(fd->handle == 0);

	if (IsReadEnabled() && !IsWriteEnabled())
		fd->handle = fopen(path.c_str(), "rb");
	else if (!IsReadEnabled() && IsWriteEnabled())
		fd->handle = fopen(path.c_str(), "wb");
	else if (IsReadEnabled() && IsWriteEnabled())
		fd->handle = fopen(path.c_str(), "rb+wb");

	if (!fd->handle)
		return false;

	fd->position = 0;

	::fseek(fd->handle, 0, SEEK_END);
	fd->length = ::ftell(fd->handle);
	::fseek(fd->handle, 0, SEEK_SET);
#elif defined(_WIN32) ||  defined(_WIN64)
	FileDesc* fd = ((FileDesc*)user);
	Assert(fd->handle == INVALID_HANDLE_VALUE);

	DWORD dwDesiredAccess = 0;
	if (IsReadEnabled() && !IsWriteEnabled())
		dwDesiredAccess |= GENERIC_READ;
	else if (!IsReadEnabled() && IsWriteEnabled())
		dwDesiredAccess |= GENERIC_WRITE;
	else if (IsReadEnabled() && IsWriteEnabled())
		dwDesiredAccess |= (GENERIC_READ | GENERIC_WRITE);

	DWORD dwCreationDisposition = 0;
	if (IsReadEnabled() && !IsWriteEnabled())
		dwCreationDisposition |= OPEN_EXISTING;
	else if (!IsReadEnabled() && IsWriteEnabled())
		dwCreationDisposition |= CREATE_ALWAYS;
	else if (IsReadEnabled() && IsWriteEnabled())
		dwCreationDisposition |= (OPEN_EXISTING | CREATE_ALWAYS);

	fd->handle = CreateFile(path.c_str(), dwDesiredAccess, 0, 0, dwCreationDisposition,
		FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING, 0);
#elif defined(__ANDROID__ )
	FileDesc* fd = ((FileDesc*)user);
	Assert(fd->handle == -1);

	int flag = O_NONBLOCK;
	if (IsReadEnabled() && !IsWriteEnabled())
		flag |= O_RDONLY;
	else if (!IsReadEnabled() && IsWriteEnabled())
		flag |= (O_WRONLY | O_CREAT);
	else if (IsReadEnabled() && IsWriteEnabled())
		flag |= (O_RDWR | O_CREAT);

	fd->handle = ::open(path.text(), flag, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	//int opt = 1;
	//Assert(ioctl(fd->handle, FIONBIO, &opt)!=-1);
#if defined(O_NONBLOCK)
	/* Fixme: O_NONBLOCK is defined but broken on SunOS 4.1.x and AIX 3.2.5. */
	int flags = 0;
	if (-1 == (flags = fcntl(fd->handle, F_GETFL, 0)))
		flags = 0;

	fcntl(fd->handle, F_SETFL, flags | O_NONBLOCK);
#else
	/* Otherwise, use the old way of doing it */
	flags = 1;
	ioctl(fd->handle, FIONBIO, &flags);
#endif
#elif ( defined(TARGET_OS_IPHONE) || defined(TARGET_IPHONE_SIMULATOR) )
	FileDesc* fd = ((FileDesc*)user);
	Assert(fd->handle == -1);

	int flag = O_NONBLOCK;
	if (IsReadEnabled() && !IsWriteEnabled())
		flag |= O_RDONLY;
	else if (!IsReadEnabled() && IsWriteEnabled())
		flag |= (O_WRONLY | O_CREAT);
	else if (IsReadEnabled() && IsWriteEnabled())
		flag |= (O_RDWR | O_CREAT);

	fd->handle = ::open(path.text(), flag, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	//int opt = 1;
	//Assert(ioctl(fd->handle, FIONBIO, &opt)!=-1);
#endif
	return IsOpened();
}

bool FileIO::IsAsyncFinished()
{
#if defined (FILEIO_USE_FOPEN)
	FileDesc* fd = ((FileDesc*)user);
	Assert(fd->handle != 0);

	return true;
#elif defined(_WIN32) ||  defined(_WIN64)
	FileDesc* fd = ((FileDesc*)user);
	Assert(fd->handle != INVALID_HANDLE_VALUE);

	if (asyncMode)
	{
		DWORD numberOfBytesTransferred = 0;
		if (GetOverlappedResult(fd->handle, &fd->overlapped, &numberOfBytesTransferred, FALSE))
		{
			Info("Async Operation Ended Sucessfully %d\n", numberOfBytesTransferred);
			return true;
		}
		else
		{
			int lastError = GetLastError();
			if (lastError == ERROR_IO_INCOMPLETE)
			{
				Info("IO_INCOMPLETE File Reading...%d\n", numberOfBytesTransferred);
				return false;
			}
			else if (lastError == ERROR_IO_PENDING)
			{
				Info("IO_PENDING File Reading...%d\n", numberOfBytesTransferred);
				return false;
			}
			else
			{
				Error("Unhandled Error..%d\n", lastError);
				return false;
			}
		}
	}
	else
	{
		return true;
	}
#elif defined(__ANDROID__ )
	FileDesc* fd = ((FileDesc*)user);
	Assert(fd->handle != -1);

	return true;
#elif ( defined(TARGET_OS_IPHONE) || defined(TARGET_IPHONE_SIMULATOR) )
	FileDesc* fd = ((FileDesc*)user);
	Assert(fd->handle != -1);

	return true;
#endif
}

void FileIO::WaitAsyncFinished()
{
#if defined (FILEIO_USE_FOPEN)
	FileDesc* fd = ((FileDesc*)user);
	Assert(fd->handle != 0);
#elif defined(_WIN32) ||  defined(_WIN64)
	FileDesc* fd = ((FileDesc*)user);
	Assert(fd->handle != INVALID_HANDLE_VALUE);

	if (asyncMode)
	{
		DWORD numberOfBytesTransferred = 0;
		if (GetOverlappedResult(fd->handle, &fd->overlapped, &numberOfBytesTransferred, TRUE))
		{
			::Info("Async Operation Ended Sucessfully %d\n", numberOfBytesTransferred);
		}
	}
	else
	{
		// sync mode always finished
	}

#elif defined(__ANDROID__ )
	FileDesc* fd = ((FileDesc*)user);
	Assert(fd->handle != -1);
#elif ( defined(TARGET_OS_IPHONE) || defined(TARGET_IPHONE_SIMULATOR) )
	FileDesc* fd = ((FileDesc*)user);
	Assert(fd->handle != -1);
#endif
}

bool FileIO::Close()
{
#if defined (FILEIO_USE_FOPEN)
	FileDesc* fd = ((FileDesc*)user);
	if (fd->handle == 0)
		return false;

	fclose(fd->handle);
	fd->handle = 0;
	fd->position = 0;
	fd->length = 0;
#elif defined(_WIN32) ||  defined(_WIN64)
	FileDesc* fd = ((FileDesc*)user);
	if (fd->handle == INVALID_HANDLE_VALUE)
		return false;

	CloseHandle(fd->handle);
	fd->handle = INVALID_HANDLE_VALUE;
#elif defined(__ANDROID__ )
	FileDesc* fd = ((FileDesc*)user);
	if (fd->handle == -1)
		return false;

	::close(fd->handle);
	fd->handle = -1;

	//struct stat info;
	//if(stat(path, &info) == 0)
	//{
	//	debug("info.st_mode = %x", info.st_mode);
	//}
#elif ( defined(TARGET_OS_IPHONE) || defined(TARGET_IPHONE_SIMULATOR) )
	FileDesc* fd = ((FileDesc*)user);
	if (fd->handle == -1)
		return false;

	::close(fd->handle);
	fd->handle = -1;

	//struct stat info;
	//if(stat(path, &info) != 0)
//		return false;
//	else
		//return true;
#endif

	return true;
}

bool FileIO::IsOpened() const
{
#if defined (FILEIO_USE_FOPEN)
	FileDesc* fd = ((FileDesc*)user);

	return fd->handle != 0;
#elif defined(_WIN32) ||  defined(_WIN64)
	FileDesc* fd = ((FileDesc*)user);

	return fd->handle != INVALID_HANDLE_VALUE;
#elif defined(__ANDROID__ )
	FileDesc* fd = ((FileDesc*)user);

	return fd->handle != -1;
#elif ( defined(TARGET_OS_IPHONE) || defined(TARGET_IPHONE_SIMULATOR) )
	FileDesc* fd = ((FileDesc*)user);

	return fd->handle != -1;
#endif
}

int FileIO::Tell() const
{
#if defined (FILEIO_USE_FOPEN)
	FileDesc* fd = ((FileDesc*)user);
	Assert(fd->handle != 0);

	return fd->position;
#elif defined(_WIN32) ||  defined(_WIN64)
	FileDesc* fd = ((FileDesc*)user);
	Assert(fd->handle != INVALID_HANDLE_VALUE);

	return SetFilePointer(fd->handle, 0, 0, FILE_CURRENT);
#elif defined(__ANDROID__ )
	FileDesc* fd = ((FileDesc*)user);
	Assert(fd->handle != -1);

	return lseek(fd->handle, 0, SEEK_CUR);
#elif ( defined(TARGET_OS_IPHONE) || defined(TARGET_IPHONE_SIMULATOR) )
	FileDesc* fd = ((FileDesc*)user);
	Assert(fd->handle != -1);

	return lseek(fd->handle, 0, SEEK_CUR);
#endif
}

int FileIO::Seek(SeekDef seek, int offset)
{
#if defined (FILEIO_USE_FOPEN)
	FileDesc* fd = ((FileDesc*)user);
	Assert(fd->handle != 0);

	switch (seek)
	{
	default:
	case BEGIN:
	{
		bool rval = ::fseek(fd->handle, offset, SEEK_SET) != 0;
		if (rval)
		{
			fd->position = 0 + offset;
		}
		return rval;
	}

	case CURRENT:
	{
		bool rval = ::fseek(fd->handle, offset, SEEK_CUR) != 0;
		if (rval)
		{
			fd->position = fd->position + offset;
		}
		return rval;
	}

	case END:
	{
		bool rval = ::fseek(fd->handle, offset, SEEK_END) != 0;
		if (rval)
		{
			fd->position = fd->length - offset;
		}
		return rval;
	}
	};
#elif defined(_WIN32) ||  defined(_WIN64)
	FileDesc* fd = ((FileDesc*)user);
	Assert(fd->handle != INVALID_HANDLE_VALUE);

	switch (seek)
	{
	default:
	case BEGIN:
		return SetFilePointer(fd->handle, offset, 0, FILE_BEGIN);

	case CURRENT:
		return SetFilePointer(fd->handle, offset, 0, FILE_CURRENT);

	case END:
		return SetFilePointer(fd->handle, offset, 0, FILE_END);
	};
#elif defined(__ANDROID__ )
	FileDesc* fd = ((FileDesc*)user);
	Assert(fd->handle != -1);

	switch (seek)
	{
	default:
	case BEGIN:
		return lseek(fd->handle, offset, SEEK_SET);

	case CURRENT:
		return lseek(fd->handle, offset, SEEK_CUR);

	case END:
		return lseek(fd->handle, offset, SEEK_END);
	};
#elif ( defined(TARGET_OS_IPHONE) || defined(TARGET_IPHONE_SIMULATOR) )
	FileDesc* fd = ((FileDesc*)user);
	Assert(fd->handle != -1);

	switch (seek)
	{
	default:
	case BEGIN:
		return lseek(fd->handle, offset, SEEK_SET);

	case CURRENT:
		return lseek(fd->handle, offset, SEEK_CUR);

	case END:
		return lseek(fd->handle, offset, SEEK_END);
	};
#endif
}

int FileIO::Read(void* buf, int size)
{
	Assert(IsReadEnabled() && "File was not opened in read mode");

#if defined (FILEIO_USE_FOPEN)
	FileDesc* fd = ((FileDesc*)user);
	Assert(fd->handle != 0);

	int rval = fread(buf, 1, size, fd->handle);
	fd->position = fd->position + size;
	return rval;
#elif defined(_WIN32) ||  defined(_WIN64)
	FileDesc* fd = ((FileDesc*)user);
	Assert(fd->handle != INVALID_HANDLE_VALUE);

	if (!asyncMode)
	{
		DWORD dwReadSize = 0;
		if (ReadFile(fd->handle, buf, size, &dwReadSize, 0))
			return dwReadSize;
		else
			return -1;
	}
	else
	{
		Assert((size % 2048) == 0);  // make sure is multiple of 2048
		memset(&fd->overlapped, 0, sizeof(fd->overlapped));
		if (ReadFile(fd->handle, buf, size, 0, &fd->overlapped))
		{
			::Info("File Read Sucessfully\n");
			return size;
		}
		else
		{
			int lastError = GetLastError();
			if (lastError == ERROR_IO_PENDING)
			{
				::Info("IO Pending..%d\n", lastError);
				return 0;
			}
			else
			{
				::Error("Unhandled Error..%d\n", lastError);
				return -1;
			}
		}
	}
#elif defined(__ANDROID__ )
	FileDesc* fd = ((FileDesc*)user);
	Assert(fd->handle != -1);

	if (!asyncMode)
	{
		return ::Read(fd->handle, buf, size);
	}
	else
	{
		int rval = ::read(fd->handle, buf, size);
		if (rval > 0)
		{
			debug("File Read Sucessfully\n");
			return rval;
		}
		else
		{
			int lastError = errno;
			if (lastError == EWOULDBLOCK)
			{
				debug("Info: IO Pending..%d\n", lastError);
				return 0;
			}
			else
			{
				debug("Error: Unhandled Error..%d\n", lastError);
				return -1;
			}
		}
	}
#elif ( defined(TARGET_OS_IPHONE) || defined(TARGET_IPHONE_SIMULATOR) )
	FileDesc* fd = ((FileDesc*)user);
	Assert(fd->handle != -1);

	if (!asyncMode)
	{
		return ::read(fd->handle, buf, size);
	}
	else
	{
		int rval = ::read(fd->handle, buf, size);
		if (rval > 0)
		{
			debug("File Read Sucessfully\n");
			return rval;
		}
		else
		{
			int lastError = errno;
			if (lastError == EWOULDBLOCK)
			{
				debug("Info: IO Pending..%d\n", lastError);
				return 0;
			}
			else
			{
				debug("Error: Unhandled Error..%d\n", lastError);
				return -1;
			}
		}
	}
#endif
}

int FileIO::Read(unsigned char* uc)
{
	return Read(uc, sizeof(unsigned char));
}

int FileIO::Read(char* c)
{
	return Read(c, sizeof(char));
}

int FileIO::Read(unsigned short* us)
{
	return Read(us, sizeof(unsigned short));
}

int FileIO::Read(short* s)
{
	return Read(s, sizeof(short));
}

int FileIO::Read(unsigned int* ui)
{
	return Read(ui, sizeof(unsigned int));
}

int FileIO::Read(int* i)
{
	return Read(i, sizeof(int));
}

int FileIO::Read(unsigned long* ul)
{
	return Read(ul, sizeof(unsigned long));
}

int FileIO::Read(long* l)
{
	return Read(l, sizeof(long));
}

int FileIO::Read(float* f)
{
	return Read(f, sizeof(float));
}

int FileIO::Read(double* d)
{
	return Read(d, sizeof(double));
}

int FileIO::Write(const void* buf, int size)
{
	Assert(IsWriteEnabled() && "File was not opened in write mode");

#if defined (FILEIO_USE_FOPEN)
	FileDesc* fd = ((FileDesc*)user);
	Assert(fd->handle != 0);

	int rval = fwrite(buf, 1, size, fd->handle);
	fd->position = fd->position + size;
	return rval;
#elif defined(_WIN32) ||  defined(_WIN64)
	FileDesc* fd = ((FileDesc*)user);
	Assert(fd->handle != INVALID_HANDLE_VALUE);

	if (!asyncMode)
	{
		DWORD dwWriteSize = 0;
		if (WriteFile(fd->handle, buf, size, &dwWriteSize, 0))
			return dwWriteSize;
		else
			return -1;
	}
	else
	{
		memset(&fd->overlapped, 0, sizeof(fd->overlapped));
		if (WriteFile(fd->handle, buf, size, 0, &fd->overlapped))
		{
			::Info("File Write Sucessfully\n");
			return size;
		}
		else
		{
			int lastError = GetLastError();
			if (lastError == ERROR_IO_PENDING)
			{
				::Info("IO Pending..%d\n", lastError);
				return 0;
			}
			else
			{
				::Error("Unhandled Error..%d\n", lastError);
				return -1;
			}
		}
	}
#elif defined(__ANDROID__ )
	FileDesc* fd = ((FileDesc*)user);
	Assert(fd->handle != -1);

	if (!asyncMode)
	{
		return ::Write(fd->handle, buf, size);
	}
	else
	{
		int rval = ::Write(fd->handle, buf, size);
		if (rval > 0)
		{
			debug("File Write Sucessfully\n");
			return rval;
		}
		else
		{
			int lastError = errno;
			if (lastError == EWOULDBLOCK)
			{
				debug("Info: IO Pending..%d\n", lastError);
				return 0;
			}
			else
			{
				debug("Error: Unhandled Error..%d\n", lastError);
				return -1;
			}
		}
	}
#elif ( defined(TARGET_OS_IPHONE) || defined(TARGET_IPHONE_SIMULATOR) )
	FileDesc* fd = ((FileDesc*)user);
	Assert(fd->handle != -1);

	if (!asyncMode)
	{
		return ::Write(fd->handle, buf, size);
	}
	else
	{
		int rval = ::Write(fd->handle, buf, size);
		if (rval > 0)
		{
			debug("File Write Sucessfully\n");
			return rval;
		}
		else
		{
			int lastError = errno;
			if (lastError == EWOULDBLOCK)
			{
				debug("Info: IO Pending..%d\n", lastError);
				return 0;
			}
			else
			{
				debug("Error: Unhandled Error..%d\n", lastError);
				return -1;
			}
		}
	}
#endif
}

int FileIO::Write(const unsigned char* uc)
{
	return Write(uc, sizeof(unsigned char));
}

int FileIO::Write(const char* c)
{
	return Write(c, sizeof(char));
}

int FileIO::Write(const unsigned short* us)
{
	return Write(us, sizeof(unsigned short));
}

int FileIO::Write(const short* s)
{
	return Write(s, sizeof(short));
}

int FileIO::Write(const unsigned int* ui)
{
	return Write(ui, sizeof(unsigned int));
}

int FileIO::Write(const int* i)
{
	return Write(i, sizeof(int));
}

int FileIO::Write(const unsigned long* ul)
{
	return Write(ul, sizeof(unsigned long));
}

int FileIO::Write(const long* l)
{
	return Write(l, sizeof(long));
}

int FileIO::Write(const float* f)
{
	return Write(f, sizeof(float));
}

int FileIO::Write(const double* d)
{
	return Write(d, sizeof(double));
}

int FileIO::GetSize() const
{
#if defined (FILEIO_USE_FOPEN)
	FileDesc* fd = ((FileDesc*)user);
	Assert(fd->handle != 0);

	return fd->length;
#elif defined(_WIN32) ||  defined(_WIN64)
	FileDesc* fd = ((FileDesc*)user);
	Assert(fd->handle != INVALID_HANDLE_VALUE);

	return GetFileSize(fd->handle, 0);
#elif defined(__ANDROID__ )
	FileDesc* fd = ((FileDesc*)user);
	Assert(fd->handle != -1);

	struct stat info;
	if (stat(path, &info) != 0)
		return 0;
	else
		return info.st_size;
#elif ( defined(TARGET_OS_IPHONE) || defined(TARGET_IPHONE_SIMULATOR) )
	FileDesc* fd = ((FileDesc*)user);
	Assert(fd->handle != -1);

	struct stat info;
	if (stat(path, &info) != 0)
		return 0;
	else
		return info.st_size;
#endif
}

bool FileIO::Exists(const std::string& path_)
{
#if defined (FILEIO_USE_FOPEN)
	FileIO f(path_);
	f.Open(IO::READ);

	return f.IsOpened();
#elif defined(_WIN32) ||  defined(_WIN64)
	FileIO f(path_);
	f.Open(IO::READ);

	return f.IsOpened();
#elif defined(__ANDROID__ )
	struct stat info;
	if (stat(path, &info) != 0)
		return false;
	else
		return true;
#elif ( defined(TARGET_OS_IPHONE) || defined(TARGET_IPHONE_SIMULATOR) )
	struct stat info;
	if (stat(path, &info) != 0)
		return false;
	else
		return true;
#endif
}

int FileIO::GetSize(const std::string& path_)
{
#if defined (FILEIO_USE_FOPEN)
	FileIO f(path_);
	f.Open(IO::READ);

	return f.GetSize();
#elif defined(_WIN32) ||  defined(_WIN64)
	FileIO f(path_);
	f.Open(IO::READ);

	return f.GetSize();
#elif defined(__ANDROID__ )
	struct stat info;
	if (stat(path, &info) != 0)
		return -1;
	else
		return info.st_size;
#elif ( defined(TARGET_OS_IPHONE) || defined(TARGET_IPHONE_SIMULATOR) )
	struct stat info;
	if (stat(path, &info) != 0)
		return -1;
	else
		return info.st_size;
#endif
}

bool FileIO::Load(const std::string& path_, void* buf, int size, int offset)
{
	FileIO f(path_);
	f.Open(IO::READ);
	f.Seek(IO::BEGIN, offset);

	return f.Read(buf, size);
}