/* ecc-pp1-redc.c

   Copyright (C) 2013, 2014 Niels Möller

   This file is part of GNU Nettle.

   GNU Nettle is free software: you can redistribute it and/or
   modify it under the terms of either:

     * the GNU Lesser General Public License as published by the Free
       Software Foundation; either version 3 of the License, or (at your
       option) any later version.

   or

     * the GNU General Public License as published by the Free
       Software Foundation; either version 2 of the License, or (at your
       option) any later version.

   or both in parallel, as here.

   GNU Nettle is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received copies of the GNU General Public License and
   the GNU Lesser General Public License along with this program.  If
   not, see http://www.gnu.org/licenses/.
*/

/* Development of Nettle's ECC support was funded by the .SE Internet Fund. */

#if HAVE_CONFIG_H
# include "config.h"
#endif

#include <assert.h>

#include "ecc-internal.h"

/* Use that 1 = p + 1 (mod p), and that at least one low limb of p + 1
   is zero. */
void
ecc_pp1_redc (const struct ecc_curve *ecc, mp_limb_t *rp)
{
  unsigned i;
  mp_limb_t hi, cy;
  unsigned shift = ecc->p.size * GMP_NUMB_BITS - ecc->p.bit_size;
  mp_size_t k = ecc->p.redc_size;
  
  for (i = 0; i < ecc->p.size; i++)
    rp[i] = mpn_addmul_1 (rp + i + k,
			  ecc->p.redc_mpm1, ecc->p.size - k, rp[i]);
  hi = mpn_add_n (rp, rp, rp + ecc->p.size, ecc->p.size);
  if (shift > 0)
    {
      hi = (hi << shift) | (rp[ecc->p.size - 1] >> (GMP_NUMB_BITS - shift));
      rp[ecc->p.size - 1] = (rp[ecc->p.size - 1]
			   & (((mp_limb_t) 1 << (GMP_NUMB_BITS - shift)) - 1))
	+ mpn_addmul_1 (rp, ecc->p.B_shifted, ecc->p.size-1, hi);
	  
    }
  else
    {
      cy = cnd_sub_n (hi, rp, ecc->p.m, ecc->p.size);
      assert (cy == hi);      
    }
}
