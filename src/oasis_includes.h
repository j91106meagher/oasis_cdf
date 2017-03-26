/*
 * oasis_includes.h
 *
 *  Created on: Apr 29, 2014
 *      Author: jmeagher
 */

#include <stdio.h>
#include <stdlib.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <marray/marray.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_interp.h>

typedef struct ddb_type{
	long n;
	gsl_vector_long * BIN_INDEX;
	gsl_vector * BIN_FROM;
	gsl_vector * BIN_TO;
	gsl_vector * INTERPOLATION;
	gsl_vector_long * INTERVAL_TYPE;
} ddb_type;

typedef struct vm_type{
	long nv;
	long ni;
	long nj;
	marray_long * VULNERABILITY_ID;
	marray_long * DAMAGE_BIN_INDEX;
	marray_long * INTENSITY_BIN_INDEX;
	marray * PROB;
} vm_type;

typedef struct ef_type{
	long n;
	long num_events;
	gsl_vector_long * event_start;
	gsl_vector_long * event_stop;
	gsl_vector_long * EVENT_ID;
	gsl_vector_long * AREA_PERIL_ID;
	gsl_vector_long * INTENSITY_BIN_INDEX;
	gsl_vector * PROB;
} ef_type;

typedef struct items_type{
	long n;
	gsl_vector_long * ITEM_ID;
	gsl_vector_long * AREAPERIL_ID;
	gsl_vector_long * VULNERABILITY_ID;
	gsl_vector * TIV;
	gsl_vector_long * GROUP_ID;
}items_type;



int read_it(items_type *it, char * it_fn, char * data_path);

int read_ef(ef_type *ef, char * ef_fn, char * data_path);

int read_ddb(ddb_type *ddb, char * ddb_fn, char * data_path);

int read_vm(vm_type *vm, char * vm_fn, char * data_path);

gsl_vector * calc_cdf_ds(long ei, long item_i,ef_type *ef, items_type *it, vm_type *vm, ddb_type *ddb, long num_rn);

double interp_lin(double x, gsl_vector *xd, gsl_vector *yd, long n,double ymin,double ymax);
