#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#if defined(TEST) || defined(DEBUG)
#define _debug(exp) printf exp
#else
#define _debug(exp)
#endif

enum {
	TAG_RIGHT = 0,
	TAG_ADD,
	TAG_SUB,
	TAG_DIV,
	TAG_MUL,
	TAG_POW,
	TAG_LEFT
};

static void calc_number(double result[], int index, char operate)
{
	int target = index - 1;
	
	switch (operate) {
		case TAG_ADD: {
			result[target] += result[index];
			_debug(("\t{+}:=%f\n", result[target]));
			break;
		} 
		case TAG_SUB: {
			result[target] -= result[index];
			_debug(("\t{-}:=%f\n", result[target]));
			break;
		} 
		case TAG_DIV: {
			_debug(("\t{/}:=%f/%f\t", result[target], result[index]));
			result[target] /= result[index];
			_debug(("\t{/}:=%f\n", result[target]));
			break;
		} 
		case TAG_MUL: {
			result[target] *= result[index];
			_debug(("\t{*}:=%f\n", result[target]));
			break;
		} 
		case TAG_POW: {
			result[target] = pow(result[target], result[index]);
			_debug(("\t{^}:=%f\n", result[target]));
			break;
		}
		default: {
			break;
		}
	}
}

double eval(const char *s)
{
	char *tmp = NULL;
	const char simple[7]={')','+','-','/','*','^','('};
#ifdef TEST
	char t[100] = {0};
	int k = 0;
#endif
	int j = 0;
	char stack[100] = {0};
	double result[100] = {0};
	int top = -1, ReTop = -1;
	int require_number = 1;

	while(*s != 0) {
		while(*s == ' ' || *s == '\t')s++;
		if(*s == 0) { 
			break;
		}
		
		j = 0;
		while(simple[j] != *s && j < 7) {
			j++;
		}
		
		if(j >= 7 || (require_number && (*s == '+' || *s == '-'))) {
			if(!require_number) {
				_debug(("expect operator:%c", *s));
				return -1;
			}
			
			result[++ReTop] = strtod(s, &tmp);
#ifdef TEST
			memcpy(t+k, s, tmp - s);
			k += tmp - s;
#endif
			s = tmp;
			require_number = 0;
			continue;
		}
		
		if(require_number && *s != '(' && *s != ')') {
			_debug(("expect number:%s", s));
			return -1;
		}
#ifdef TEST
		t[k++] = ' ';
#endif
		require_number = 1;
		
		if(top == -1 || stack[top] < j)	{
			stack[++top] = j==6 ? 0 : j;
			
			s++;
			continue;
		}

		while(top != -1 && stack[top] >= j) {
			if(j == 0 && stack[top] == 0) {
				break;
			}
#ifdef TEST
			t[k++]=simple[stack[top]];
			t[k++]=' ';
#endif
			calc_number(result, ReTop--, stack[top--]);
		}
		
		if(j != 0) {
			stack[++top] = j;
		} else {
			top--;
		}
		
		s++;
	}
	
	while(top != -1) {
#ifdef TEST
		t[k++] = simple[stack[top]];
		t[k++] = ' ';
#endif
		calc_number(result, ReTop--, stack[top--]);
	}
#ifdef TEST
	t[k]='\0';
	_debug(("RP:%s\n", t));
#endif
	return result[ReTop];
}

#ifdef TEST
int main(int argc, char *argv[])
{
	int index = 0;
	for(index = 1; index < argc; index++) {
		printf("Eval(%s):%f\n", argv[index], eval(argv[index]));
	}
}
#endif

