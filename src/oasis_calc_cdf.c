/*
 * oasis_calc_cdf.c
 *
 *  Created on: May 2, 2014
 *      Author: jmeagher
 */

#include "oasis_includes.h"

gsl_vector * calc_cdf_ds(long ei, long item_i,ef_type *ef, items_type *it, vm_type *vm, ddb_type *ddb, long num_rn) {

	long v_i = 0;
	long j;
	long event_start;
	long event_stop;
	int k;
	gsl_vector_long * cdf_count;
	gsl_vector * cdf_prob;
	size_t INDICES[3];

	event_start = gsl_vector_long_get(ef->event_start,ei);
	event_stop = gsl_vector_long_get(ef->event_stop,ei);
	//printf("ei %ld, start %ld stop %ld\n",ei,event_start,event_stop);

	cdf_prob = gsl_vector_alloc(vm->ni);
	cdf_count = gsl_vector_long_alloc(vm->ni);

	for (j=event_start;j<=event_stop;j++) {

		if (gsl_vector_long_get(it->AREAPERIL_ID,item_i) == gsl_vector_long_get(ef->AREA_PERIL_ID,j)) {
			printf("it->AREAPERIL_ID %ld ef->AREAPERIL %ld item_i %ld j %ld\n",gsl_vector_long_get(it->AREAPERIL_ID,item_i),gsl_vector_long_get(ef->AREA_PERIL_ID,j),item_i,j);

			for (k=0;k<vm->ni;k++) {
				double c;
				INDICES[0] = v_i;
				INDICES[1] = k;
				INDICES[2] = gsl_vector_long_get(ef->INTENSITY_BIN_INDEX,j);
				//printf("k %d INDICES %d %d %d\n",k,INDICES[0],INDICES[1],INDICES[2]);
				//printf("vm->PROB %lf el->PROB %lf\n",marray_get(vm->PROB,INDICES),gsl_vector_get(ef->PROB,j));
				c = marray_get(vm->PROB,INDICES) * gsl_vector_get(ef->PROB,j);
				c = c + gsl_vector_get(cdf_prob,k);
				gsl_vector_set(cdf_prob,k,c);
				gsl_vector_long_set(cdf_count,k,gsl_vector_long_get(cdf_count,k)+1);
				//printf("k %d cdf_prob %lf c %lf count %ld\n",k,gsl_vector_get(cdf_prob,k),c,gsl_vector_long_get(cdf_count,k));
			}
		}
	}

	gsl_vector * cdf;
	cdf = gsl_vector_alloc(vm->ni);
	gsl_vector *ds;
	ds = gsl_vector_alloc(num_rn);

	if (gsl_vector_isnull(cdf_prob) == 1) {
		//printf("cdf_prob sum zero\n");
	}
	else {
		//printf("vm->ni %ld\n",vm->ni);
		gsl_vector_set(cdf,0,gsl_vector_get(cdf_prob,0));
		int i;
		for (i=1;i<vm->ni;i++) {
			gsl_vector_set(cdf,i,(gsl_vector_get(cdf,i-1)+gsl_vector_get(cdf_prob,i)));
			//printf("i %d cdf %lf cdf_prob %lf\n",i,gsl_vector_get(cdf,i),gsl_vector_get(cdf_prob,i));
		}

		gsl_rng_env_setup();
		const gsl_rng_type * T;
		double rnd;
		gsl_rng * r;
		T = gsl_rng_default;
		r = gsl_rng_alloc(T);

		//for (i=0;i<vm->ni;i++) {
		//	printf("i %d cdf %lf ddb %lf\n",i,gsl_vector_get(cdf,i),gsl_vector_get(ddb->INTERPOLATION,i));
		//}

		double y;

		for (i=0;i<num_rn;i++) {
			rnd = gsl_rng_uniform(r);
			y = interp_lin(rnd, cdf, ddb->INTERPOLATION,vm->ni,0.0,1.0);
			gsl_vector_set(ds,i,y);
			//printf("rnd %lf y %lf\n",rnd,y);

		}
	}

	gsl_vector_free(cdf_prob);
	gsl_vector_long_free(cdf_count);
	gsl_vector_free(cdf);

	return(ds);
}

double interp_lin(double x, gsl_vector *xd, gsl_vector *yd, long n,double ymin,double ymax) {
	double y;
	double t;
	int ii;

	if (x <= gsl_vector_get(xd,0)) {
					t = (x - gsl_vector_get(xd,0) ) / (gsl_vector_get(xd,1) - gsl_vector_get(xd,0));
					y = (1.0 - t) * gsl_vector_get(yd,0) + t * gsl_vector_get(yd,1);
				}
				else if (gsl_vector_get(xd,n -1) <= x) {
					t = (x - gsl_vector_get(xd,n -2) / (gsl_vector_get(xd,n -1) - gsl_vector_get(xd,n -2)));
					y = (1.0 - t) * gsl_vector_get(yd,n -2) + t * gsl_vector_get(yd,n -1);
				}
				else {
					for (ii=1;ii<n;ii++) {
						if (gsl_vector_get(xd,ii-1) <= x && x <= gsl_vector_get(xd,ii)) {
							t = (x - gsl_vector_get(xd,ii-1)) / (gsl_vector_get(xd,ii) - gsl_vector_get(xd,ii-1));
							y = (1.0 - t) * gsl_vector_get(yd,ii -1) + t * gsl_vector_get(yd,ii);
						}
					}
				}
	if (y < ymin) y = ymin;
	if (y > ymax) y = ymax;

	return(y);
}

