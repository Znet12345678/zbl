#include "lib.h"
enum INPUT_KEYS{
	A = 0x9E,
	B = 0xB0,
	C = 0xAE,
	D = 0xA0,
	E = 0x92,
	F = 0xA1,
	G = 0xA2,
	H = 0xA3,
	I = 0x97,
	J = 0xA4,
	K = 0xA5,
	L = 0xA6,
	M = 0xB2,
	N = 0xB1,
	O = 0x98,
	P = 0x99,
	Q = 0x90,
	R = 0x93,
	S = 0x9F,
	T = 0x94,
	U = 0x96,
	V = 0xAF,
	W = 0x91,
	X = 0xAD,
	Y = 0x95,
	Z = 0xAC,
	ONE = 0x02,
	TWO = 0x03,
	THREE = 0x04,
	FOUR = 0x05,
	FIVE = 0x06,
	SIX = 0x07,
	SEVEN = 0x08,
	EIGHT = 0x09,
	NINE = 0x0A,
	ZERO = 0x0B,
	PRD = 0x34,
	US = 0x0C,
	SPACE = 0x39,
	NL = 0x1C,
	BS = 0x35
};
void erase(void *buf,int n){
	char *pntr = (char*)buf;
	for(int i = 0; i < n;i++)
		pntr[i] = 0;
	buf = pntr;
}
char kgetc(){
	int i = 0;
	unsigned char sc;
		sc = inportb(0x60);
		if(sc == A){
			return 'a';
		}
		else if (sc == B){
			return 'b';
		}
		else if (sc == C){
                        return 'c';
                }

		else if (sc == D){
                        return 'd';
                }

		else if (sc == E){
                        return 'e';
                }

		else if (sc == F){
                        return 'f';
                }

		else if (sc == G){
                        return 'g';
                }

		else if (sc == H){
                        return 'h';
                }

		else if (sc == I){
                        return 'i';
                }

		else if (sc == J){
                        return 'j';
                }

		else if (sc == K){
                        return 'k';
                }

		else if (sc == L){
                        return 'l';
                }

		else if (sc == M){
                        return 'm';
                }

		else if (sc == N){
                        return 'n';
                }

		else if (sc == O){
                        return 'o';
                }

		else if (sc == P){
                        return 'p';
                }

		else if (sc == Q){
                        return 'q';
                }

		else if (sc == R){
                        return 'r';
                }

		else if (sc == S){
                        return 's';
                }

		else if (sc == T){
                        return 't';
                }

		else if (sc == U){
                        return 'u';
                }

		else if (sc == V){
                        return 'v';
                }

		else if (sc == W){
                        return 'w';
                }

		else if (sc == X){
                        return 'x';
                }

		else if (sc == Y){
                        return 'y';
                }

		else if (sc == Z){
                        return 'z';
                }
		else if(sc == NL){
			return '\n';
		}
		else if(sc == SPACE){
			return ' ';
		}
		else if(sc == 0x00){
			return '\001';
		}
		else if(sc == PRD)
			return '.';
		else if(sc >= ONE && sc < ZERO)
			return (sc - 1) + '0';
		else if(sc == ZERO)
			return '0';
		else if(sc == US)
			return '-';
		else if(sc == BS)
			return '/';
		else{
			return '\001';
		return '\001';	
	}
}
void silent_gets(char *buf){
char oldc = '\n';
        char c;
        int i = 0;
        while(1){
                if(!(inb(0x64) & 1)){
                        oldc = c;
                        continue;
                }c = kgetc();
                if(c == '\001')
                        continue;
                if(c == '\n')
                        break;
                buf[i] = c;
                i++;
        }

}
void gets(char *buf){
	char oldc = '\n';
	char c;
	int i = 0;
	while(1){
		if(!(inb(0x64) & 1)){
			oldc = c;
			continue;
		}c = kgetc();
		if(c == '\001')
			continue;
		if(c == '\n')
			break;
		t_putc(c);
		buf[i] = c;
		i++;
	}
}
