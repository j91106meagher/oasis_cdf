/*
 * oasis_read.c
 *
 *  Created on: Apr 29, 2014
 *      Author: jmeagher
 */

#include "oasis_includes.h"

int read_it(items_type *it, char * it_fn, char * data_path) {
	char * fn;
	char hdr[100];
	FILE * fp;
	long n;

	asprintf(&fn,"%s%s",data_path,it_fn);
	fp = fopen(fn,"r");
	fscanf(fp,"%s\n",hdr);
	printf("file header: %s\n",hdr);
	fscanf(fp,"%ld",&n);
	(*it).n = n;

	(*it).ITEM_ID = gsl_vector_long_alloc((*it).n);
	(*it).AREAPERIL_ID = gsl_vector_long_alloc((*it).n);
	(*it).VULNERABILITY_ID = gsl_vector_long_alloc((*it).n);
	(*it).TIV = gsl_vector_alloc((*it).n);
	(*it).GROUP_ID = gsl_vector_long_alloc((*it).n);

	{
		int i;
		long ITEM_ID, AREAPERIL_ID;
		long VULNERABILITY_ID, GROUP_ID;
		double TIV;
		printf("ef.n %ld\n",(*it).n);
		for (i=0;i<(*it).n;i++) {
			fscanf(fp,"%ld, %ld, %ld, %lf, %ld\n",&ITEM_ID, &AREAPERIL_ID, &VULNERABILITY_ID, &TIV, &GROUP_ID);
			gsl_vector_long_set((*it).ITEM_ID,i,ITEM_ID);
			gsl_vector_long_set((*it).AREAPERIL_ID,i,AREAPERIL_ID);
			gsl_vector_long_set((*it).VULNERABILITY_ID,i,VULNERABILITY_ID);
			gsl_vector_set((*it).TIV,i,TIV);
			gsl_vector_long_set((*it).GROUP_ID,i,GROUP_ID);
		}
	}
	fclose(fp);

	return(0);
}


int read_ef(ef_type *ef, char * ef_fn, char * data_path) {
	char * fn;
	FILE * fp;
	long n;
	long num_events;

	asprintf(&fn,"%s%s",data_path,ef_fn);
	fp = fopen(fn,"r");

	fscanf(fp,"%ld,%ld\n",&n,&num_events);
	(*ef).n = n;
	(*ef).num_events = num_events;
	printf("n %ld num_events %ld\n",(*ef).n,(*ef).num_events);

	(*ef).event_start = gsl_vector_long_alloc((*ef).num_events);
	(*ef).event_stop = gsl_vector_long_alloc((*ef).num_events);

	{
		int i;
		long event_start, event_stop;
		for (i=0;i<(*ef).num_events;i++) {
			fscanf(fp,"%ld, %ld\n",&event_start, &event_stop);
			gsl_vector_long_set((*ef).event_start,i,event_start);
			gsl_vector_long_set((*ef).event_stop,i,event_stop);

		}
	}

	(*ef).EVENT_ID = gsl_vector_long_alloc((*ef).n);
	(*ef).AREA_PERIL_ID = gsl_vector_long_alloc((*ef).n);
	(*ef).INTENSITY_BIN_INDEX = gsl_vector_long_alloc((*ef).n);
	(*ef).PROB = gsl_vector_alloc((*ef).n);

	{
			int i;
			long EVENT_ID, AREA_PERIL_ID;
			long INTENSITY_BIN_INDEX;
			double PROB;
			printf("ef.n %ld\n",(*ef).n);
			for (i=0;i<(*ef).n;i++) {
				fscanf(fp,"%ld, %ld, %ld, %lf\n",&EVENT_ID, &AREA_PERIL_ID, &INTENSITY_BIN_INDEX, &PROB);
				gsl_vector_long_set((*ef).EVENT_ID,i,EVENT_ID);
				gsl_vector_long_set((*ef).AREA_PERIL_ID,i,AREA_PERIL_ID);
				gsl_vector_long_set((*ef).INTENSITY_BIN_INDEX,i,INTENSITY_BIN_INDEX);
				gsl_vector_set((*ef).PROB,i,PROB);
			}
		}

	fclose(fp);
	return(0);
}

int read_vm(vm_type *vm, char * vm_fn, char * data_path) {
	char * fn;
	char hdr[100];
	FILE * fp;
	long nv, ni, nj;

	asprintf(&fn,"%s%s",data_path,vm_fn);
	fp = fopen(fn,"r");
	fscanf(fp,"%s\n",hdr);
	printf("file header: %s\n",hdr);
	fscanf(fp,"%ld,%ld,%ld",&nv,&ni,&nj);
	(*vm).nv = nv;
	(*vm).ni = ni;
	(*vm).nj = nj;
	printf("nv %ld ni %ld, nj %ld\n",(*vm).nv,(*vm).ni,(*vm).nj);

	const unsigned int RANK = 3;
	size_t DIMENSION[RANK];
	DIMENSION[0] = (*vm).nv;
	DIMENSION[1] = (*vm).ni;
	DIMENSION[2] = (*vm).nj;

	(*vm).VULNERABILITY_ID = marray_long_alloc(RANK, DIMENSION);
	(*vm).DAMAGE_BIN_INDEX = marray_long_alloc(RANK, DIMENSION);
	(*vm).INTENSITY_BIN_INDEX = marray_long_alloc(RANK, DIMENSION);
	(*vm).PROB = marray_alloc(RANK, DIMENSION);

	{
		int i;
		int j;
		long vi,dbi, ibi;
		double prob;
		for (i=0;i<(*vm).ni;i++) {
			for (j=0;j<(*vm).nj;j++) {
				fscanf(fp,"%ld,%ld,%ld,%lf\n",&vi,&dbi,&ibi,&prob);
				size_t INDICES[3] = {0,i,j};
				marray_long_set((*vm).VULNERABILITY_ID,INDICES,vi);
				marray_long_set((*vm).DAMAGE_BIN_INDEX,INDICES,dbi);
				marray_long_set((*vm).INTENSITY_BIN_INDEX,INDICES,ibi);
				marray_set((*vm).PROB,INDICES,prob);
			}
		}
	}
	fclose(fp);
	printf("done vm read\n");
	printf("vm.ni.n %ld, vm.nj %ld\n",(*vm).ni,(*vm).nj);

	return(0);
};

int read_ddb(ddb_type *ddb, char * ddb_fn, char * data_path) {
	char * fn;
	char hdr[100];
	FILE * fp;
	long n;

	asprintf(&fn,"%s%s",data_path,ddb_fn);
	fp = fopen(fn,"r");
	fscanf(fp,"%s\n",hdr);
	printf("file header: %s\n",hdr);
	fscanf(fp,"%ld",&n);
	(*ddb).n = n;
	printf("n: %ld\n",(*ddb).n);

	(*ddb).BIN_INDEX = gsl_vector_long_alloc((*ddb).n);
	(*ddb).BIN_FROM = gsl_vector_alloc((*ddb).n);
	(*ddb).BIN_TO = gsl_vector_alloc((*ddb).n);
	(*ddb).INTERPOLATION = gsl_vector_alloc((*ddb).n);
	(*ddb).INTERVAL_TYPE = gsl_vector_long_alloc((*ddb).n);

	{
		int i;
		long bi,it;
		double bf,bt,ip;
		for (i=0;i<(*ddb).n;i++) {
			fscanf(fp,"%ld,%lf,%lf,%lf,%ld\n",&bi,&bf,&bt,&ip,&it);
			gsl_vector_long_set((*ddb).BIN_INDEX,i,bi);
			gsl_vector_set((*ddb).BIN_FROM,i,bf);
			gsl_vector_set((*ddb).BIN_TO,i,bt);
			gsl_vector_set((*ddb).INTERPOLATION,i,ip);
			gsl_vector_long_set((*ddb).INTERVAL_TYPE,i,it);
		}
	}
	fclose(fp);
	printf("done ddb read\n");
	printf("ddb_l.n %ld\n",(*ddb).n);

	printf("returning form ddb_read\n");
return(0);
}


