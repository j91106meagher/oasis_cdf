/*
 * oasis_cdf.c
 *
 *  Created on: Apr 29, 2014
 *      Author: jmeagher
 */

#include <stdio.h>
#include <stdlib.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <omp.h>

#include "oasis_includes.h"



int main(void) {
	char * data_path = "/home/jmeagher/mpi_code/oasis_c/data/";
	char * ddb_fn = "DictDamageBin_Model_JM.csv";
	char * vm_fn = "VersionVulnerability_Model_DBorder2.csv";
	char * ef_fn = "VersionEventFootprint_Model_JMorder2_C.csv";
	char * it_fn = "Items1_Model.csv";



	int status;

	ddb_type ddb;
	vm_type vm;
	ef_type ef;
	items_type it;

	status = read_ddb(&ddb, ddb_fn, data_path);

	printf("ddb.n %ld\n",ddb.n);
	printf("finished OK\n");

	status = read_vm(&vm, vm_fn, data_path);
	printf("done read_vm\n");

	status = read_ef(&ef, ef_fn, data_path);
	printf("done read_ef\n");

	status = read_it(&it, it_fn, data_path);
	printf("it.n %ld\n",it.n);
	printf("done read_it\n");

	const long num_rn = 100;
	long ei = 0;
	long item_i = 1;
	gsl_vector * ds;

	marray *ef_item_ds;
	const unsigned int RANK = 3;
	size_t DIMENSION[RANK];
	DIMENSION[0] = ef.num_events;
	DIMENSION[1] = it.n;
	DIMENSION[2] = num_rn;

	ef_item_ds = marray_alloc(3, DIMENSION);
	int ds_i;
	size_t INDICES[3];
	int nthreads;
	omp_set_num_threads(2);

	for (ei=0;ei<ef.num_events;ei++) {
		printf("ei %ld\n",ei);
#pragma omp parallel
		{
			int tid;
			nthreads = omp_get_num_threads();
#pragma omp for private(item_i,ds,INDICES,ds_i)
			for (item_i=0;item_i<it.n;item_i++) {
				tid = omp_get_thread_num();
				printf("item_i %ld tid %d\n",item_i,tid);
				ds = calc_cdf_ds(ei,item_i,&ef,&it,&vm,&ddb,num_rn);
				for (ds_i=0;ds_i<num_rn;ds_i++) {
					INDICES[0] = ei;
					INDICES[1] = item_i;
					INDICES[2] = ds_i;
					marray_set(ef_item_ds,INDICES,gsl_vector_get(ds,ds_i));
				}
				gsl_vector_free(ds);
			}
		}
	}

	printf("done calc_cdf_ds\n");
	printf("nthreads %d\n",nthreads);

	marray_free(ef_item_ds);
	//need to write free subroutines

	return(0);
}



