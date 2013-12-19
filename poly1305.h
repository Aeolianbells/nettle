/* poly1305-aes.h
 *
 * Poly1305 message authentication code.
 */

/* nettle, low-level cryptographics library
 *
 * Copyright (C) 2013 Nikos Mavrogiannopoulos
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

#ifndef NETTLE_POLY1305_H_INCLUDED
#define NETTLE_POLY1305_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

/* Low level functions/macros for the poly1305 construction. */

#include "nettle-types.h"

struct poly1305_ctx {
  /* Key, 128-bit value and some cached multiples. */
  union
  {
    uint32_t r32[6];
    uint64_t r64[3];
  } r;
  uint32_t s32[3];
  /* State, represented as words of 26, 32 or 64 bits, depending on
     implementation. */
  /* High bits first, to maintain alignment. */
  uint32_t hh;
  union
  {
    uint32_t h32[4];
    uint64_t h64[2];
  } h;

  uint8_t nonce[16];
  uint8_t block[16];
  unsigned index;
};

/* All-in-one context, with cipher, and state. Cipher must have a 128-bit block */
#define POLY1305_CTX(type) \
{ struct poly1305_ctx pctx; type cipher; }

#define poly1305_set_key nettle_poly1305_set_key
#define poly1305_set_nonce nettle_poly1305_set_nonce
#define poly1305_update nettle_poly1305_update
#define poly1305_block nettle_poly1305_block
#define poly1305_digest nettle_poly1305_digest

void poly1305_set_key(struct poly1305_ctx *ctx, const uint8_t key[16]);
void poly1305_set_nonce (struct poly1305_ctx *ctx, const uint8_t * nonce);
void poly1305_block (struct poly1305_ctx *ctx, const uint8_t m[16]);
void poly1305_update (struct poly1305_ctx *ctx, size_t size, const uint8_t *data);
void poly1305_digest (struct poly1305_ctx *ctx,
		      size_t length, uint8_t *digest, const uint8_t *s);

#define POLY1305_SET_KEY(ctx, set_key, key)	\
  do {						\
    poly1305_set_key(&(ctx)->pctx, (key+16));	\
    (set_key)(&(ctx)->cipher, 16, (key));	\
    (ctx)->pctx.index = 0;			\
  } while (0)

#define POLY1305_SET_NONCE(ctx, data)		\
  poly1305_set_nonce(&(ctx)->pctx, (data))

#define POLY1305_DIGEST(ctx, encrypt, length, digest)		\
  do { 								\
    uint8_t _ts[16]; 						\
    (encrypt)(&(ctx)->cipher, 16, _ts, (ctx)->pctx.nonce);	\
    poly1305_digest (&(ctx)->pctx, (length), (digest), _ts); 	\
    INCREMENT (16, (ctx)->pctx.nonce); 				\
    (ctx)->pctx.index = 0; 					\
  } while(0);



#ifdef __cplusplus
}
#endif

#endif /* NETTLE_POLY1305_H_INCLUDED */
