#include <ctype.h>
int ispunc(int c){
	return c == '.' || c == '!' || c == '"' || c == '\'' || c == '?' || c == ';' || c == ':';
}
