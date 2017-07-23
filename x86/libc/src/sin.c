#ifdef __TBUILD
#include "../include/math.h"
#include <stdio.h>
#include <stdlib.h>
#else
#include <math.h>
#endif
#ifndef __TBUILD
double sin(double x){
#else
double sinT(double x){
#endif
	double v = x;
	if (v >= (M_PI * 2))
		while(v >= (M_PI * 2))
			v-=M_PI*2;
	else if(v <= -1 * (M_PI * 2))
		while(v <= -1 * (M_PI * 2))
			v+=M_PI*2;
	double ret = 0;
	int i = 0;
	while(i < 6){
		#ifndef __TBUILD
		ret+=((ipow(-1,i)))/((unsigned int)(fact(2*i + 1))) * (double)(ipow(v,2 * i + 1));
		#else
		ret+=((double)ipow(-1,i))/((unsigned int)(factT(2*i + 1))) * (double)(ipow(v,2 * i + 1));
		//printf("%f %u %f\n",ipow(-1,i),factT(2*i + 1),ipow(v,2*i+1));
		//printf("%f\n",ret);
		#endif
		i++;
	}
	#ifdef __TBUILD
	printf("%f\n",ret);
	#endif
	return ret;
}
