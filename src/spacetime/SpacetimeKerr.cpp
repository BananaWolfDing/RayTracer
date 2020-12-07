#include "SpacetimeKerr.h"

#include <cassert>
#include <boost/math/tools/roots.hpp>
#include <glm/gtx/norm.hpp>

#include "../util/de.h"

namespace
{
	inline float
	norm_squared(glm::vec3 const& x)
	{
		return glm::dot(x, x);
	}
	inline float
	vec_max(glm::vec4 const& x)
	{
		return std::max(x.w, std::max(std::max(x.x, x.y), x.z));
	}

	inline float
	kerr_schild_r(glm::vec4 const& x, float const horizon, float const a)
	{
		
		float const top1 = x.x*x.x + x.y*x.y;
		float const top2 = x.z*x.z;
		float const nominal_r = std::sqrt(top1+top2);
		if (nominal_r < 1e-5f)
		{
#ifdef DEBUG_GEODESIC
			std::cerr << "kerr_schild_r NaN1\n";
#endif
			return std::nanf("");
		}

		auto target = [&](float r) {
			float const r2 = r*r;
			return top1 / (r2 + a*a) + top2 / r2 - 1.f;
		};
		auto tol = [&](float min, float max) { return std::abs(max - min) < 1e-3; };
		if (target(1e-5f) < 0.f)
		{
#ifdef DEBUG_GEODESIC
			std::cerr << "kerr_schild_r NaN2\n";
#endif
			return std::nanf("");
		}
		auto result = boost::math::tools::bisect(target, /*horizon*/ 1e-5f, nominal_r+1.f, tol);
		return 0.5f * (result.first + result.second);
	}

	inline glm::vec3 
	kerr_schild_drdx(glm::vec4 const& x, float r, float const a)
	{
		float const r2 = r*r;
		float const r2a2 = r2 + a*a;
		float const r2a2_r = r + a*a/r;

		// Use Implicit Function Theorem to compute derivative of r w.r.t. x


		// Jacobian of definition of r in Kerr-Schild coordinates w.r.t r,
		// multipled by r
		float const jfr_r = -2 * (x.x*x.x + x.y*x.y) / (r2a2_r * r2a2_r)
			- 2 * x.z*x.z / r2;

		return -glm::vec3(
					2 * x.x / r2a2_r,
					2 * x.y / r2a2_r,
					2 * x.z
				) / jfr_r;
	}

	inline glm::vec4
	kerr_schild_k(glm::vec4 const& x, float r, float const a,
			glm::vec3 const& drdx,
			glm::vec4* const dkdx, glm::vec4* const dkdy, glm::vec4* const dkdz)
	{
		float const r2 = r*r;
		float const a2 = a*a;
		float const r2a2 = r2+a2;
		float const r2a22 = r2a2 * r2a2;
		float const r_r2a2 = r/r2a2;
		float const a2r2_r2a2 = (a2 - r2) / r2a2;
		// Partial derivative dk/dr
		glm::vec3 const pdkdr = glm::vec3(
				// [x(a^2 - r^2) -2ary] / (r^2+a^2)^2
				( a2r2_r2a2*x.x - 2*a*r_r2a2*x.y ) / r2a2,
				( a2r2_r2a2*x.y + 2*a*r_r2a2*x.x ) / r2a2,
				-x.z/r2
			);

		glm::vec3 const pdkdx = glm::vec3(
				r / r2a2,
				- a / r2a2,
				0
			);
		glm::vec3 const pdkdy = glm::vec3(
				a / r2a2,
				r / r2a2,
				0
			);
		glm::vec3 const pdkdz = glm::vec3(
				0,
				0,
				1 / r
			);

		// Chain rule d_mu k = pd_mu k + pd_r k * d_mu r
		*dkdx = glm::vec4(pdkdr * drdx.x + pdkdx, 0.f);
		*dkdy = glm::vec4(pdkdr * drdx.y + pdkdy, 0.f);
		*dkdz = glm::vec4(pdkdr * drdx.z + pdkdz, 0.f);

		return {
			(r * x.x + a * x.y) / r2a2,
			(r * x.y - a * x.x) / r2a2,
			x.z / r,
			1.f
		};
	}

	inline float
	kerr_schild_f(glm::vec4 const& x,
	              float const r, float const a, float const gm2,
								glm::vec3 const& drdx,
	              glm::vec3* const dfdx)
	{
		float const r2 = r * r;
		float const r3 = r2 * r;
		float const r4 = r2 * r2;
		float const a2 = a * a;
		float const z2 = x.z * x.z;
		float const denom_r = r3 + a2 * z2 / r;
		float const denom_r_2 = denom_r * denom_r;

		float const f = gm2 * r2 / denom_r;

		// gm2 * (3a^2r^2z^2 - r^6) / ((a^2z^2+r^4)^2)
		float const pdfdr = gm2 * (3 * a2 * z2 - r4) / denom_r_2;
		// -gm2 * (2a^2r^3 z) / (a^2z^2+r^4)^2
		float const pdfdz = -gm2 * 2 * a2 * x.z * r / denom_r_2;
		*dfdx = glm::vec3(
			pdfdr * drdx.x,
			pdfdr * drdx.y,
			pdfdr * drdx.z + pdfdz
		);

		return f;
	}

	inline glm::mat4x4
	mirror(glm::vec4 a, glm::vec4 b)
	{
		glm::mat4x4 result = glm::outerProduct(a, b);
		return result + glm::transpose(result);
	}

	/**
	 * Calculate the contravariant tensor g^{i,j}
	 */
	inline glm::mat4x4
	kerr_schild_contravariant_g(float const f, glm::vec4 const& k)
	{
		// Instead of computing the inverse directly we can use Sherman-Morrison
		// formula. This is because g is the sum of "identity" plus a rank 1
		// matrix.


		// See Visser, The Kerr spacetime: A brief introduction, Equation (45)
		// The vector k is null w.r.t. both Minkowski and Kerr metrics, so
		// Sherman-Morrison formula's denominator is equal to 1.

		glm::vec4 const k2(k.x, k.y, k.z, -k.w);
		glm::mat4 result = -glm::outerProduct(k2, k2);
		result[0][0] += 1.f;
		result[1][1] += 1.f;
		result[2][2] += 1.f;
		result[3][3] -= 1.f;

		return result;
	}
}
SpacetimeKerr::SpacetimeKerr(glm::vec3 position, float spin, float rs, float c,
		float epsilon, bool eulerSolver,
		int maxSteps, float initialStepSize)
	: Spacetime(maxSteps, initialStepSize)
	, position(position)
	, spin(spin)
	, rs(rs)
	, c(c)
	, epsilon(epsilon)
	, eulerSolver(eulerSolver)
{
	assert(0 < c);
	assert(0 <= rs);

	float const a = kerr_a();
	horizon = 0.5 * (rs + std::sqrt(rs * rs - 4 * a * a));
}


Ray SpacetimeKerr::geodesic(Ray const& ray, float* const h)
{
	assert(std::isfinite(*h));
	/*
	 * Use Runge-Kutta-Felhberg (RK4-5) Method to calculate the next step of the
	 * ray.
	 */

	glm::vec3 const o = ray.getOrigin() - this->position;
	glm::vec3 const d = ray.getDirection();

#if 1
	// Switch the order of z and y axes
	glm::vec4 o4(o.x, o.z, o.y, 0.f);
	// The direction must be light-like, so the spacetime interval ds = 0
	// Under the Minkowski metric, this vector has length 0.
	glm::vec4 d4(d.x, d.z, d.y, glm::l2Norm(d));

	float const a = kerr_a();
	float const r = kerr_schild_r(o4, horizon, a);
	if (std::isnan(r))
	{
#ifdef DEBUG_GEODESIC
		std::cerr << "Kerr:geodesic r=NaN\n";
#endif
		// Stop the tracer.
		*h = std::nanf("");
		return ray;
	}
	if (r <= 0.5 * (horizon + std::sqrt(horizon*horizon - 4*a*a)))
	{
#ifdef DEBUG_GEODESIC
		std::cerr << "Kerr:geodesic Fell into BH\n";
		*h = std::nanf("");
		return ray;
#endif
	}

	if (eulerSolver)
	{
		glm::vec4 const a4 = dx2ds(o4, d4);
		o4 += d4 * *h;
		d4 += a4 * *h;
	}
	else
	{
		runge_kutta_fehlberg(
			[this](glm::vec4 const& x, glm::vec4 const& dx){ return this->dx2ds(x, dx); },
			[](glm::vec4 const& x){ return vec_max(glm::abs(x)); },
			epsilon, o4, d4, *h);
	}
#ifdef DEBUG_GEODESIC
	if (std::isnan(*h))
	{
		std::cerr << "Kerr:geodesic h=NaN\n";
	}
#endif

	// Re-parametrise to prevent direction from blowing up
	glm::vec3 d3(d4.x, d4.z, d4.y);
	float const d3norm = glm::l2Norm(d3);
	if (!eulerSolver)
	{
		*h *= d3norm;
		*h = std::max(std::min(*h, 1.f), 1e-3f);
	}
	d3 /= d3norm;

	Ray result(glm::vec3(o4.x, o4.z, o4.y) + this->position, d3);
	return result;
#else
	glm::vec4 dx(d.x, d.z, d.y, glm::l2Norm(d));
	glm::vec4 dx2 = dx2ds(glm::vec4(o.x, o.z, o.y, 0.f),  dx);
	glm::vec3 dx2_3(dx2.x, dx2.z, dx2.y);
	

	Ray result(o + *h * d, d + *h * dx2_3);
	return result;
#endif
}


glm::vec4 SpacetimeKerr::dx2ds(glm::vec4 const& x, glm::vec4 const& dxds) const
{
	// Write the metric in Kerr-Schild form because Boyer-Lindquist form is
	// hard to handle
	
	// Solve for r in the range [horizon, x^2+y^2+z^2]
	float const a = kerr_a();
	float const r = kerr_schild_r(x, horizon, a);
	glm::vec3 const drdx = kerr_schild_drdx(x, r, a);

	glm::vec4 dkd1, dkd2, dkd3;
	glm::vec4 const k = kerr_schild_k(x, r, a, drdx, &dkd1, &dkd2, &dkd3);

	glm::vec3 dfdx;
	float const f = kerr_schild_f(x, r, a, /*2GM=*/rs * c * c, drdx, &dfdx);

#ifdef DEBUG_GEODESIC
	if (std::isnan(r) || std::isnan(f))
	{
			std::cerr << "Kerr:dx2ds NaN\n";
	}
#endif

	glm::mat4x4 const ktk = glm::outerProduct(k, k);

	// The metric tensor is given by g = eta + f*k*k^T
	
	/*
	glm::mat4x4 g = f * ktk;
	// Add the Minkowski tensor
	g[0][0] += 1.f;
	g[1][1] += 1.f;
	g[2][2] += 1.f;
	g[3][3] -= 1.f;
	*/

	glm::mat4x4 const gInv = kerr_schild_contravariant_g(f, k);

	glm::mat4x4 const dgdx[] = {
		f * mirror(k, dkd1) + dfdx.x * ktk,
		f * mirror(k, dkd2) + dfdx.y * ktk,
		f * mirror(k, dkd3) + dfdx.z * ktk,
		glm::mat4x4(), // the metric is static w.r.t. time
	};

	// Compute the Christoffel symbol
	glm::vec4 dx2ds;

	for (int beta = 0; beta < 4; ++beta)
	{
		dx2ds[beta] = 0.f;
#pragma unroll
		for (int alpha = 0; alpha < 4; ++alpha)
		{
#pragma unroll
			for (int nu = 0; nu < 4; ++nu)
			{
				float christoffel = 0.f;
#pragma unroll
				for (int mu = 0; mu < 4; ++mu)
				{
					christoffel += gInv[mu][beta] *
						( dgdx[alpha][mu][nu]
						+ dgdx[nu][mu][alpha]
						- dgdx[mu][alpha][nu] );
				}
				christoffel *= 0.5;

				dx2ds[beta] -= christoffel * dxds[alpha] * dxds[nu];
			}
		}
	}

	return dx2ds;
}
