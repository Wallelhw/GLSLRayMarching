//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#ifndef _ColorRGBA_h_
#define _ColorRGBA_h_

#include "Platform.h"
#include "Maths.h"
#include "InputStream.h"
#include "OutputStream.h"

	class ColorRGBA
	{
	public:
		ColorRGBA(float r_ = 0, float g_ = 0, float b_ = 0, float a_ = 0);
		ColorRGBA(const ColorRGBA& rgba);
		ColorRGBA& operator = (const ColorRGBA& rgba);
		~ColorRGBA();

		float& R() { return m[0]; };
		float& G() { return m[1]; };
		float& B() { return m[2]; };
		float& A() { return m[3]; };

		const float& R() const { return m[0]; };
		const float& G() const { return m[1]; };
		const float& B() const { return m[2]; };
		const float& A() const { return m[3]; };

		operator float* () { return m; };
		operator const float* () const { return m; };

		float& operator [] (int i) { assert(i < 4); return m[i]; };
		const float& operator [] (int i) const { assert(i < 4); return m[i]; };

		bool operator == (const ColorRGBA& rgba) const;
		bool operator <  (const ColorRGBA& rgba) const;

		ColorRGBA operator+ (const ColorRGBA& rkC) const;
		ColorRGBA operator* (float fScalar) const;

		unsigned int toInteger() const;

		void Read(InputStream& is)
		{
			is >> m[0] >> m[1] >> m[2] >> m[3];
		}

		void Write(OutputStream& os) const
		{
			os << m[0] << m[1] << m[2] << m[3];
		}

		static const ColorRGBA Black;
		static const ColorRGBA Blue;
		static const ColorRGBA Green;
		static const ColorRGBA Cyan;
		static const ColorRGBA Red;
		static const ColorRGBA Mangenta;
		static const ColorRGBA Brown;
		static const ColorRGBA Grey;
		static const ColorRGBA BrightBlue;
		static const ColorRGBA BrightGreen;
		static const ColorRGBA BrightCyan;
		static const ColorRGBA BrightRed;
		static const ColorRGBA BrightMangenta;
		static const ColorRGBA Yellow;
		static const ColorRGBA White;
	private:
		float m[4];
	};

#endif