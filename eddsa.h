/* eddsa.h

   Copyright (C) 2014 Niels Möller

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

#ifndef NETTLE_EDDSA_H
#define NETTLE_EDDSA_H

#include "nettle-types.h"

#include "bignum.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Name mangling */
#define _eddsa_compress _nettle_eddsa_compress
#define _eddsa_compress_itch _nettle_eddsa_compress_itch
#define _eddsa_decompress _nettle_eddsa_decompress
#define _eddsa_decompress_itch _nettle_eddsa_decompress_itch

#define ED25519_KEY_SIZE 32

struct ecc_curve;

mp_size_t
_eddsa_compress_itch (const struct ecc_curve *ecc);
void
_eddsa_compress (const struct ecc_curve *ecc, uint8_t *r, mp_limb_t *p,
		 mp_limb_t *scratch);

mp_size_t
_eddsa_decompress_itch (const struct ecc_curve *ecc);
int
_eddsa_decompress (const struct ecc_curve *ecc, mp_limb_t *p,
		   const uint8_t *cp,
		   mp_limb_t *scratch);

#ifdef __cplusplus
}
#endif

#endif /* NETTLE_EDDSA_H */
