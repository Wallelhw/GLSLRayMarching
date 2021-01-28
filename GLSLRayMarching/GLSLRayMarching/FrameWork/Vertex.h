//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#ifndef _Vertex_h_
#define _Vertex_h_

#include "Plane3.h"
#include "Matrix4.h"
#include "Vector3.h"
#include "Vector2.h"

	class Vertex
	{
	public:
		Vertex(const Vector3& p_ = Vector3(0, 0, 0), const Vector3& n_ = Vector3(0, 0, 1), const Vector2& t_ = Vector2::Zero)
			: p(p_)
			, n(n_)
			, t(t_)
		{
		}

		~Vertex()
		{
		}

		Vertex(const Vertex& other)
			: p(other.p)
			, n(other.n)
			, t(other.t)
		{
		}

		Vertex& operator = (const Vertex& other)
		{
			this->p = other.p;
			this->n = other.n;
			this->t = other.t;

			return *this;
		}

		bool operator < (const Vertex& other) const
		{
			bool less = false;
			if (p < other.p)
			{
				less = true;
			}
			else if (p == other.p)
			{
				if (n < other.n)
				{
					less = true;
				}
				else if (n == other.n)
				{
					less = (t < other.t);
				}
			}

			return less;
		}

		bool operator == (const Vertex& other) const
		{
			return (p == other.p) && (n == other.n) && (t == other.t);
		}

		friend Vertex operator -(const Vertex& v)
		{
			return Vertex(-v.p, -v.n, -v.t);
		}

		friend Vertex operator + (const Vertex& v0, const Vertex& v1)
		{
			return Vertex(v0.p + v1.p, v0.n + v1.n, v0.t + v1.t);
		}

		friend Vertex operator - (const Vertex& v0, const Vertex& v1)
		{
			return Vertex(v0.p - v1.p, v0.n - v1.n, v0.t - v1.t);
		}

		friend Vertex operator* (const Vertex& v0, const Vertex& v1)
		{
			return Vertex(v0.p * v1.p, v0.n * v1.n, v0.t * v1.t);
		}

		friend Vertex operator* (const Vertex& v, float scalar)
		{
			return Vertex(scalar * v.p, scalar * v.n, scalar * v.t);
		}

		friend Vertex operator* (float scalar, const Vertex& v)
		{
			return Vertex(scalar * v.p, scalar * v.n, scalar * v.t);
		}

		friend Vertex operator / (const Vertex& v, float scalar)
		{
			Vertex quotient;

			quotient.p = v.p / scalar;
			quotient.n = v.n / scalar;
			quotient.t = v.t / scalar;

			return quotient;
		}

		Vertex& operator+= (const Vertex& v)
		{
			p += v.p;
			n += v.n;
			t += v.t;
			return *this;
		}

		Vertex& operator-= (const Vertex& v)
		{
			p -= v.p;
			n -= v.n;
			t -= v.t;
			return *this;
		}

		Vertex& operator*= (const Vertex& v)
		{
			p *= v.p;
			n *= v.n;
			t *= v.t;
			return *this;
		}

		Vertex& operator*= (float scalar)
		{
			p *= scalar;
			n *= scalar;
			t *= scalar;
			return *this;
		}

		Vertex& operator/= (float scalar)
		{
			p /= scalar;
			n /= scalar;
			t /= scalar;
			return *this;
		}

		void SetPosition(const Vector3& p_)
		{
			p = p_;
		}

		void SetNormal(const Vector3& n_)
		{
			n = n_;
		}

		void SetTexcoord(const Vector2& t_)
		{
			t = t_;
		}

		const Vector3& GetPosition() const
		{
			return p;
		}

		const Vector3& GetNormal() const
		{
			return n;
		}

		const Vector2& GetTexcoord() const
		{
			return t;
		}

		void Flip()
		{
			n = -n;
		}

		Vertex& operator *= (const Matrix4& m)
		{
			p = m * p;

			return *this;
		}

		friend Vertex operator * (const Matrix4& m, const Vertex& v)
		{
			Vertex result(m * v.p, v.n, v.t);

			return result;
		}

		void Read(InputStream& is)
		{
			is >> p >> n >> t;
		}

		void Write(OutputStream& os) const
		{
			os << p << n << t;
		}
	protected:
	private:

	public:
	protected:
	private:
		Vector3 p;
		Vector3 n;
		Vector2 t;
	};
#endif