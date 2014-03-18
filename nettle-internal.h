/* nettle-internal.h
 *
 * Things that are used only by the testsuite and benchmark, and
 * subject to change.
 */

/* nettle, low-level cryptographics library
 *
 * Copyright (C) 2002, 2014 Niels Möller
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

#ifndef NETTLE_INTERNAL_H_INCLUDED
#define NETTLE_INTERNAL_H_INCLUDED

#include "nettle-meta.h"

/* Temporary allocation, for systems that don't support alloca. Note
 * that the allocation requests should always be reasonably small, so
 * that they can fit on the stack. For non-alloca systems, we use a
 * fix maximum size, and abort if we ever need anything larger. */

#if HAVE_ALLOCA
# define TMP_DECL(name, type, max) type *name
# define TMP_ALLOC(name, size) (name = alloca(sizeof (*name) * (size)))
#else /* !HAVE_ALLOCA */
# define TMP_DECL(name, type, max) type name[max]
# define TMP_ALLOC(name, size) \
  do { if ((size) > (sizeof(name) / sizeof(name[0]))) abort(); } while (0)
#endif 

/* Arbitrary limits which apply to systems that don't have alloca */
#define NETTLE_MAX_HASH_BLOCK_SIZE 128
#define NETTLE_MAX_HASH_DIGEST_SIZE 64
#define NETTLE_MAX_SEXP_ASSOC 17
#define NETTLE_MAX_CIPHER_BLOCK_SIZE 32

/* Doesn't quite fit with the other algorithms, because of the weak
 * keys. Weak keys are not reported, the functions will simply crash
 * if you try to use a weak key. */

extern const struct nettle_cipher nettle_des;
extern const struct nettle_cipher nettle_des3;

extern const struct nettle_cipher nettle_blowfish128;

extern const struct nettle_cipher nettle_unified_aes128;
extern const struct nettle_cipher nettle_unified_aes192;
extern const struct nettle_cipher nettle_unified_aes256;

/* Stream ciphers treated as aead algorithms with no authentication. */
extern const struct nettle_aead nettle_chacha;
extern const struct nettle_aead nettle_salsa20;
extern const struct nettle_aead nettle_salsa20r12;

/* Glue to openssl, for comparative benchmarking. Code in
 * examples/nettle-openssl.c. */
extern const struct nettle_cipher nettle_openssl_aes128;
extern const struct nettle_cipher nettle_openssl_aes192;
extern const struct nettle_cipher nettle_openssl_aes256;
extern const struct nettle_cipher nettle_openssl_blowfish128;
extern const struct nettle_cipher nettle_openssl_des;
extern const struct nettle_cipher nettle_openssl_cast128;

extern const struct nettle_hash nettle_openssl_md5;
extern const struct nettle_hash nettle_openssl_sha1;

#endif /* NETTLE_INTERNAL_H_INCLUDED */
