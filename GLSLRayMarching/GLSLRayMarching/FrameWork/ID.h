//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#ifndef _ID_h_
#define _ID_h_

#include "Platform.h"
#include "Frame3.h"

class ID
{
public:
	static ID Get();

	ID();
	~ID();

	ID& operator = (const ID& other);

	bool operator < (const ID& other);
	bool operator == (const ID& other);
private:
	unsigned int id;
};

#endif