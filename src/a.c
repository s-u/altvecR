/*
 *  Copyright (C) 2018  Simon Urbanek
 *  Distributed under MIT license
 */

#include <R.h>
#include <Rinternals.h>
#include <R_ext/Rdynload.h>
#include <R_ext/Altrep.h>

static R_altrep_class_t altvecR_real_class;

static R_xlen_t altvecR_Length(SEXP sx)
{
    SEXP exp = PROTECT(lang2(Rf_install("get.len"), CAR(sx)));
    SEXP x = Rf_eval(exp, R_GlobalEnv);
    double d = (LENGTH(x) > 0) ? REAL(x)[0] : NA_REAL;
    UNPROTECT(1);
    if (LENGTH(x) != 1)
	Rf_warning("get.len() should return a scalar but has length %d", LENGTH(x));
    return d;
}

static void *altvecR_Dataptr(SEXP sx, Rboolean writeable)
{
    SEXP exp = PROTECT(lang2(Rf_install("get.all"), CAR(sx)));
    SEXP x = Rf_eval(exp, R_GlobalEnv);
    UNPROTECT(1);
    return DATAPTR(x);
}

static const void *altvecR_Dataptr_or_null(SEXP sx)
{
    SEXP exp = PROTECT(lang2(Rf_install("get.all.or.NULL"), CAR(sx)));
    SEXP x = Rf_eval(exp, R_GlobalEnv);
    UNPROTECT(1);
    return (x == R_NilValue) ? 0 : DATAPTR(x);
}

static double altvecR_real_Elt(SEXP sx, R_xlen_t i)
{
    SEXP exp = PROTECT(lang3(Rf_install("get.elt"), CAR(sx), ScalarReal((double)i)));
    SEXP x = Rf_eval(exp, R_GlobalEnv);
    double d = (LENGTH(x) > 0) ? REAL(x)[0] : NA_REAL;
    UNPROTECT(1);
    if (LENGTH(x) != 1)
	Rf_warning("get.elt() should return a scalar but has length %d", LENGTH(x));
    return d;
}

static
R_xlen_t altvecR_real_Get_region(SEXP sx, R_xlen_t i, R_xlen_t n, double *buf)
{
    SEXP exp = PROTECT(lang4(Rf_install("get.region"), CAR(sx), ScalarReal((double)i), ScalarReal((double)n)));
    SEXP x = Rf_eval(exp, R_GlobalEnv);
    R_xlen_t len = XLENGTH(x);
    if (len > n) len = n;
    memcpy(buf, REAL(x), sizeof(double) * len);
    UNPROTECT(1);
    return len;
}

static void InitRealClass(DllInfo *dll)
{
    R_altrep_class_t cls =
	R_make_altreal_class("altvecR_real", "altvecR", dll);
    altvecR_real_class = cls;

    R_set_altrep_Length_method(cls, altvecR_Length);

    R_set_altvec_Dataptr_method(cls, altvecR_Dataptr);
    R_set_altvec_Dataptr_or_null_method(cls, altvecR_Dataptr_or_null);

    R_set_altreal_Elt_method(cls, altvecR_real_Elt);
    R_set_altreal_Get_region_method(cls, altvecR_real_Get_region);
}

SEXP do_altvecR(SEXP sWhat) {
    return R_new_altrep(altvecR_real_class, sWhat, R_NilValue);
}

static const R_CallMethodDef CallEntries[] = {
    {"altvecR", (DL_FUNC) &do_altvecR, -1},
    {NULL, NULL, 0}
};

void R_init_altvecR(DllInfo *dll)
{
    InitRealClass(dll);

    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
}
