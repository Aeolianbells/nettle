#include "testutils.h"

#if NETTLE_USE_MINI_GMP
void
test_main (void)
{
  SKIP();
}
#else /* ! NETTLE_USE_MINI_GMP */

static void
ref_mod (mp_limb_t *rp, const mp_limb_t *ap, const mp_limb_t *mp, mp_size_t mn)
{
  mp_limb_t q[mn + 1];
  mpn_tdiv_qr (q, rp, 0, ap, 2*mn, mp, mn);
}

#define MAX_ECC_SIZE (1 + 521 / GMP_NUMB_BITS)
#define MAX_SIZE (2*MAX_ECC_SIZE)
#define COUNT 50000

static void
test_curve (gmp_randstate_t rands, const struct ecc_curve *ecc)
{
  mp_limb_t a[MAX_SIZE];
  mp_limb_t m[MAX_SIZE];
  mp_limb_t ref[MAX_SIZE];
  mpz_t r;
  unsigned j;

  mpz_init (r);
  
  for (j = 0; j < COUNT; j++)
    {
      if (j & 1)
	mpz_rrandomb (r, rands, 2*ecc->size * GMP_NUMB_BITS);
      else
	mpz_urandomb (r, rands, 2*ecc->size * GMP_NUMB_BITS);

      mpz_limbs_copy (a, r, 2*ecc->size);

      ref_mod (ref, a, ecc->p, ecc->size);

      mpn_copyi (m, a, 2*ecc->size);
      ecc->modp (ecc, m);
      if (mpn_cmp (m, ecc->p, ecc->size) >= 0)
	mpn_sub_n (m, m, ecc->p, ecc->size);

      if (mpn_cmp (m, ref, ecc->size))
	{
	  fprintf (stderr, "ecc->modp failed: bit_size = %u\n",
		   ecc->bit_size);
	  gmp_fprintf (stderr, "a   = %Nx\n", a, 2*ecc->size);
	  gmp_fprintf (stderr, "m   = %Nx (bad)\n", m, ecc->size);
	  gmp_fprintf (stderr, "ref = %Nx\n", ref, ecc->size);
	  abort ();
	}

      if (ecc->Bmodp_size < ecc->size)
	{
	  mpn_copyi (m, a, 2*ecc->size);
	  ecc_generic_modp (ecc, m);
	  if (mpn_cmp (m, ecc->p, ecc->size) >= 0)
	    mpn_sub_n (m, m, ecc->p, ecc->size);

	  if (mpn_cmp (m, ref, ecc->size))
	    {
	      fprintf (stderr, "ecc_generic_modp failed: bit_size = %u\n",
		       ecc->bit_size);
	      gmp_fprintf (stderr, "a   = %Nx\n", a, 2*ecc->size);
	      gmp_fprintf (stderr, "m   = %Nx (bad)\n", m, ecc->size);
	      gmp_fprintf (stderr, "ref = %Nx\n", ref, ecc->size);
	      abort ();
	    }
	}

      ref_mod (ref, a, ecc->q, ecc->size);

      mpn_copyi (m, a, 2*ecc->size);
      ecc->modq (ecc, m);
      if (mpn_cmp (m, ecc->q, ecc->size) >= 0)
	mpn_sub_n (m, m, ecc->q, ecc->size);

      if (mpn_cmp (m, ref, ecc->size))
	{
	  fprintf (stderr, "ecc->modq failed: bit_size = %u\n",
		   ecc->bit_size);
	  gmp_fprintf (stderr, "a   = %Nx\n", a, 2*ecc->size);
	  gmp_fprintf (stderr, "m   = %Nx (bad)\n", m, ecc->size);
	  gmp_fprintf (stderr, "ref = %Nx\n", ref, ecc->size);
	  abort ();
	}
      if (ecc->Bmodq_size < ecc->size)
	{
	  mpn_copyi (m, a, 2*ecc->size);
	  ecc_generic_modq (ecc, m);
	  if (mpn_cmp (m, ecc->q, ecc->size) >= 0)
	    mpn_sub_n (m, m, ecc->q, ecc->size);

	  if (mpn_cmp (m, ref, ecc->size))
	    {
	      fprintf (stderr, "ecc_generic_modp failed: bit_size = %u\n",
		       ecc->bit_size);
	      gmp_fprintf (stderr, "a   = %Nx\n", a, 2*ecc->size);
	      gmp_fprintf (stderr, "m   = %Nx (bad)\n", m, ecc->size);
	      gmp_fprintf (stderr, "ref = %Nx\n", ref, ecc->size);
	      abort ();
	    }
	}
    }
  mpz_clear (r);
}

void
test_main (void)
{
  gmp_randstate_t rands;
  unsigned i;

  gmp_randinit_default (rands);
  
  for (i = 0; ecc_curves[i]; i++)
    test_curve (rands, ecc_curves[i]);

  test_curve (rands, &nettle_curve25519);
  gmp_randclear (rands);
}
#endif /* ! NETTLE_USE_MINI_GMP */
