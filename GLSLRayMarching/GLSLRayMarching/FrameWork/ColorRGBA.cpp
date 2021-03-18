//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#include "System.h"
#include "ColorRGBA.h"

const ColorRGBA ColorRGBA::Black(0.0f, 0.0f, 0.0f, 1.0f);
const ColorRGBA ColorRGBA::Blue(0.0f, 0.0f, 0.5f, 1.0f);
const ColorRGBA ColorRGBA::Green(0.0f, 0.5f, 0.0f, 1.0f);
const ColorRGBA ColorRGBA::Cyan(0.0f, 0.5f, 0.5f, 1.0f);
const ColorRGBA ColorRGBA::Red(0.5f, 0.0f, 0.0f, 1.0f);
const ColorRGBA ColorRGBA::Mangenta(0.5f, 0.0f, 0.5f, 1.0f);
const ColorRGBA ColorRGBA::Brown(0.5f, 0.5f, 0.0f, 1.0f);
const ColorRGBA ColorRGBA::Grey(0.5f, 0.5f, 0.5f, 1.0f);
const ColorRGBA ColorRGBA::BrightBlue(0.0f, 0.0f, 1.0f, 1.0f);
const ColorRGBA ColorRGBA::BrightGreen(0.0f, 1.0f, 0.0f, 1.0f);
const ColorRGBA ColorRGBA::BrightCyan(0.0f, 1.0f, 1.0f, 1.0f);
const ColorRGBA ColorRGBA::BrightRed(1.0f, 0.0f, 0.0f, 1.0f);
const ColorRGBA ColorRGBA::BrightMangenta(1.0f, 0.0f, 1.0f, 1.0f);
const ColorRGBA ColorRGBA::Yellow(1.0f, 1.0f, 0.0f, 1.0f);
const ColorRGBA ColorRGBA::White(1.0f, 1.0f, 1.0f, 1.0f);

ColorRGBA::ColorRGBA(float r_, float g_, float b_, float a_)
{
	m[0] = r_;
	m[1] = g_;
	m[2] = b_;
	m[3] = a_;
}

ColorRGBA::ColorRGBA(const ColorRGBA& rgba_)
{
	*this = rgba_;
}

ColorRGBA& ColorRGBA::operator = (const ColorRGBA& rgba)
{
	m[0] = rgba[0];
	m[1] = rgba[1];
	m[2] = rgba[2];
	m[3] = rgba[3];

	return *this;
}

ColorRGBA::~ColorRGBA()
{
}

bool ColorRGBA::operator == (const ColorRGBA& rgba) const
{
	return ::MemCmp((const void*)(m), (const void*)(rgba.m), sizeof(m[0]) * 4) == 0;
}

bool ColorRGBA::operator < (const ColorRGBA& rgba) const
{
	return ::MemCmp((const void*)(m), (const void*)(rgba.m), sizeof(m[0]) * 4) < 0;
}

ColorRGBA ColorRGBA::operator+ (const ColorRGBA& rkC) const
{
	return ColorRGBA(m[0] + rkC.m[0], m[1] + rkC.m[1], m[2] + rkC.m[2], m[3]);
}

ColorRGBA ColorRGBA::operator* (float fScalar) const
{
	return ColorRGBA(m[0] * fScalar, m[1] * fScalar, m[2] * fScalar, m[3]);
}

unsigned int ColorRGBA::toInteger() const
{
	unsigned int rval = ((unsigned int)((m[3] * 255)) << 24) | (((unsigned int)(m[2] * 255)) << 16) |
		((unsigned int)((m[1] * 255)) << 8) | (((unsigned int)(m[0] * 255)));

	return rval;
}

/*
void ColorRGBA::read(InputStream& is)
{
	is >> m[0] >> m[1] >> m[2] >> m[3];
}

void ColorRGBA::write(OutputStream& os) const
{
	os << m[0] << m[1] << m[2] << m[3];
}
*/