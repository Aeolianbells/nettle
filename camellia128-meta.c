/* camellia128-meta.c */

/* nettle, low-level cryptographics library
 *
 * Copyright (C) 2010, 2013, 2014 Niels Möller
 *  
 * The nettle library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at your
 * option) any later version.
 * 
 * The nettle library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with the nettle library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02111-1301, USA.
 */

#if HAVE_CONFIG_H
# include "config.h"
#endif

#include <assert.h>

#include "nettle-meta.h"

#include "camellia.h"

const struct nettle_cipher nettle_camellia128 =
  { "camellia128", sizeof(struct camellia128_ctx),
    CAMELLIA_BLOCK_SIZE, CAMELLIA128_KEY_SIZE,
    (nettle_set_key_func *) camellia128_set_encrypt_key,
    (nettle_set_key_func *) camellia128_set_decrypt_key,
    (nettle_crypt_func *) camellia128_crypt,
    (nettle_crypt_func *) camellia128_crypt
  };
