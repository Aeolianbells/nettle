/* fat-arm.c

   Copyright (C) 2015 Niels Möller

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

#if HAVE_CONFIG_H
# include "config.h"
#endif

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nettle-types.h"

#include "aes-internal.h"
#include "fat-setup.h"

struct arm_features
{
  /* /proc/cpuinfo "CPU Architecture" doesn't correspond exactly to
     ARM architecture version, but it's good enough for our purposes.
     Will be set to 5, 6, 7 or 8. */
  unsigned arch_version;
  int have_neon;
};

static void
get_arm_features (struct arm_features *features)
{
  FILE *f;
  char line[200];
  int seen_arch = 0;
  int seen_features = 0;

  features->arch_version = 5;
  features->have_neon = 0;

  f = fopen ("/proc/cpuinfo", "r");
  if (!f)
    return;
  while (seen_features + seen_arch < 2
	 && fgets (line, sizeof(line), f))
    {
      char *sep;
      char *p;
      sep = strchr (line, ':');
      if (!sep)
	continue;
      for (p = sep; p - line > 0 && p[-1] == '\t'; p--)
	;

      *p = '\0';
      p = sep+1;

      if (strcmp (line, "Features") == 0)
	{
	  features->have_neon = (strstr (p, " neon ") != NULL);
	  seen_features = 1;
	}
      else if (strcmp (line, "CPU architecture") == 0)
	{
	  /* Don't use strtol, since it's locale dependent. */
	  while (p[0] == ' ')
	    p++;
	  if (p[0] > '5' && p[0] <= '9')
	    features->arch_version = p[0] - '0';
	  else if (strcmp (p, "AArch64") == 0)
	    features->arch_version = 8;
	  seen_arch = 1;	  
	}
    }
  if (features->arch_version >= 8)
    {
      /* Neon is not required, and maybe not listed in feature flags */
      features->have_neon = 1;
    }
  fclose (f);
}

DECLARE_FAT_FUNC(_nettle_aes_encrypt, aes_crypt_internal_func)
DECLARE_FAT_FUNC_VAR(aes_encrypt, aes_crypt_internal_func, arm)
DECLARE_FAT_FUNC_VAR(aes_encrypt, aes_crypt_internal_func, armv6)

DECLARE_FAT_FUNC(_nettle_aes_decrypt, aes_crypt_internal_func)
DECLARE_FAT_FUNC_VAR(aes_decrypt, aes_crypt_internal_func, arm)
DECLARE_FAT_FUNC_VAR(aes_decrypt, aes_crypt_internal_func, armv6)

static void CONSTRUCTOR
fat_init (void)
{
  static volatile int initialized = 0;
  struct arm_features features;
  int verbose;

  if (initialized)
    return;

  get_arm_features (&features);

  verbose = getenv (ENV_VERBOSE) != NULL;
  if (verbose)
    fprintf (stderr,
	     "libnettle: cpu arch: %u, neon: %s\n",
	     features.arch_version, features.have_neon ? "yes" : "no");

  if (features.arch_version >= 6)
    {
      if (verbose)
	fprintf (stderr, "libnettle: enabling armv6 code.\n");
      _nettle_aes_encrypt_vec = _nettle_aes_encrypt_armv6;
      _nettle_aes_decrypt_vec = _nettle_aes_decrypt_armv6;
    }
  else
    {
      if (verbose)
	fprintf (stderr, "libnettle: not enabling armv6 code.\n");
      _nettle_aes_encrypt_vec = _nettle_aes_encrypt_arm;
      _nettle_aes_decrypt_vec = _nettle_aes_decrypt_arm;
    }
  /* FIXME: Needs memory barrier, to enforce store ordering. */
  initialized = 1;
}
  
DEFINE_FAT_FUNC(_nettle_aes_encrypt, void,
		(unsigned rounds, const uint32_t *keys,
		 const struct aes_table *T,
		 size_t length, uint8_t *dst,
		 const uint8_t *src),
		(rounds, keys, T, length, dst, src))

DEFINE_FAT_FUNC(_nettle_aes_decrypt, void,
		(unsigned rounds, const uint32_t *keys,
		 const struct aes_table *T,
		 size_t length, uint8_t *dst,
		 const uint8_t *src),
		(rounds, keys, T, length, dst, src))
