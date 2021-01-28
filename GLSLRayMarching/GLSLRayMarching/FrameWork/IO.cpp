///////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2016, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk			 //
//																				 //
// Author : Mac Lin									                             //
// Module : Magnum Engine v0.7.0												 //
// Date   : 14/Jun/2020											                 //
//																				 //
///////////////////////////////////////////////////////////////////////////////////
#include "Platform.h"
#include "IO.h"

IO::IO(const std::string& path_)
	: path(path_)
	, accessMode(IO::READ)
{
}

IO::~IO()
{
}

bool IO::Open(AccessMode accessMode)
{
	assert(accessMode);

	this->accessMode = accessMode;
	this->asyncMode = false;

	return true;
}

bool IO::OpenForAsync(AccessMode accessMode)
{
	assert(accessMode);

	this->accessMode = accessMode;
	this->asyncMode = true;

	return true;
}

bool IO::IsReadEnabled() const
{
	return ((accessMode & IO::READ) != 0);
}

bool IO::IsWriteEnabled() const
{
	return ((accessMode & IO::WRITE) != 0);
}

const std::string& IO::GetPath() const
{
	return path;
}