//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#include "Platform.h"
#include "ID.h"

ID ID::Get()
{
	static unsigned int id = 0;

	ID r;
	r.id = +id;

	return r;
}

ID::ID()
: id(0)
{
}

ID::~ID()
{
}

ID& ID::operator = (const ID& other)
{
	id = other.id;

	return *this;
}

bool ID::operator < (const ID& other)
{
	return id < other.id;
}

bool ID::operator == (const ID& other)
{
	return id == other.id;
}