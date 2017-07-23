#include <math.h>
double ipow(double n,int p){
	double ret = 1;
	int i = 0;
	while(i < p){
		ret*=n;
		i++;
	}
	return ret;
}
