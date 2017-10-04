#include <math.h>
#ifndef __TBUILD
int fact(int n){
#else
int factT(int n){
#endif
	if(n == 0 || n == 1)
		return 1;
	#ifndef __TBUILD
	return n*fact(n-1);
	#else
	return n*factT(n-1);
	#endif
}
