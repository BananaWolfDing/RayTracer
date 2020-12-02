#ifndef UTIL_DE_H
#define UTIL_DE_H

#include <cmath>

#define A1 (0.f)
#define A2 (2/9.f)
#define A3 (1/3.f)
#define A4 (3/4.f)
#define A5 (1.f)
#define A6 (5/6.f)

#define B21 (2/9.f)
#define B31 (1/12.f)
#define B41 (69/128.f)
#define B51 (-17/12.f)
#define B61 (65/432.f)

#define B32 (1/4.f)
#define B42 (-243/128.f)
#define B52 (27/4.f)
#define B62 (-5/16.f)

#define B43 (135/64.f)
#define B53 (-27/5.f)
#define B63 (13/16.f)

#define B54 (16/15.f)
#define B64 (13/16.f)

#define B65 (5/144.f)

#define CH1 (47/450.f)
#define CH2 (0.f)
#define CH3 (12/25.f)
#define CH4 (32/225.f)
#define CH5 (1/30.f)
#define CH6 (6/25.f)

#define CT1 (-1/150.f)
#define CT2 (0.f)
#define CT3 (3/100.f)
#define CT4 (-16/75.f)
#define CT5 (-1/20.f)
#define CT6 (6/25.f)

template <typename V, typename F, typename Norm> inline void
runge_kutta_fehlberg(F f, Norm norm, float const epsilon, V& x, V& dx, float& h)
{
	V k1 = h * f(x + A1 * h, dx);
	V k2 = h * f(x + A2 * h, dx + B21 * k1);
	V k3 = h * f(x + A3 * h, dx + B31 * k1 + B32 * k2);
	V k4 = h * f(x + A4 * h, dx + B41 * k1 + B42 * k2 + B43 * k3);
	V k5 = h * f(x + A5 * h, dx + B51 * k1 + B52 * k2 + B53 * k3 + B54 * k4);
	V k6 = h * f(x + A6 * h, dx + B61 * k1 + B62 * k2 + B63 * k3 + B64 * k4 + B65 * k5);
	V dx_next = dx +              CH1 * k1 + CH2 * k2 + CH3 * k3 + CH4 * k4 + CH5 * k5 + CH6 * k6;
	float const trunc = std::max(1e-3f, norm(     CT1 * k1 + CT2 * k2 + CT3 * k3 + CT4 * k4 + CT5 * k5 + CT6 * k6));

#if 1
	x += dx * h;
	dx = dx_next;
	h *= 0.9f * std::pow(epsilon / trunc, 0.2f);
	h = std::max(std::min(h, 1000.f), 1e-2f);
#else
	h *= 0.9f * std::pow(epsilon / trunc, 0.2f);
	dx = dx_next;
	x += dx * h;
#endif
}

#endif // !UTIL_DE_H
