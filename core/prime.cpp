
#include <stdio.h>
#include <string.h>
#include <vector>
#include <limits.h>
#include <math.h>
#include <errno.h>
#include <algorithm>
#include <functional>

using namespace std;

void print_prime(unsigned long &pr)
{
    printf("%d, ", pr);
}

static bool check_devided(vector<unsigned long>::const_iterator its, 
    vector<unsigned long>::const_iterator ite, 
    unsigned long &num, unsigned long sqrt_num)
{
    while(its != ite && *its <= sqrt_num) {
        if(!(num % (*its))) {
            return false;
        }
        
        its++;
    }
    
    return true;
}

static vector<unsigned long> g_prime_list;

void get_prime_between(vector<unsigned long> *plist, unsigned long start, unsigned long end)
{
	unsigned long tmp = 0;
	static unsigned long sqrt_num = 10;
	static unsigned long add1_num = 121;

	if(g_prime_list.empty()) {
		unsigned long primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 
				41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97};

	    g_prime_list.reserve(1000);
	    g_prime_list.assign(primes, primes+sizeof(primes)/sizeof(primes[0]));
	}

	if(start > end) {
		start ^= end;
		end   ^= start;
		start ^= end;
	}

    tmp = (*g_prime_list.rbegin())+2;
    if(tmp > end) {
        goto done;
    }

	for(; tmp <= end; tmp += 2) {
		if(add1_num <= tmp) {
		    add1_num += (sqrt_num<<1) + 1; //y+=2x+1
		    sqrt_num++;
		}
		
		if(check_devided(g_prime_list.begin()+1, g_prime_list.end(), tmp, sqrt_num)) {
		    g_prime_list.push_back(tmp);
		}
	}
	
	//for_each(g_prime_list.begin(), g_prime_list.end(), print_prime);
	//puts("");

done:
	if(plist == NULL) {
		return ;
	}

    vector<unsigned long>::iterator it;
    it = lower_bound(g_prime_list.begin(), g_prime_list.end(),start);
    plist->assign(it, lower_bound(it, g_prime_list.end(),end));
}

int isprime(unsigned long num)
{
	if(num <= *g_prime_list.rbegin()) {
	    return binary_search(g_prime_list.begin(), g_prime_list.end(), num);
	}

	unsigned long sqrt_num= (unsigned long)sqrt(num);
	get_prime_between(NULL, 1, sqrt_num);
	return check_devided(g_prime_list.begin(), g_prime_list.end(), num, sqrt_num);
}

/*
int main(int argc, char *argv[])
{
    get_prime_between(NULL, 1, 1000);
#define TEST_PRIME(n) printf("%d is%s a prime\n", n, isprime(n)?"":"n't")
    if(argc <= 1) {
        TEST_PRIME(12);
        TEST_PRIME(13);
        TEST_PRIME(15);
        TEST_PRIME(47);
        TEST_PRIME(49);
        TEST_PRIME(112);
        TEST_PRIME(121);
        TEST_PRIME(199);
        TEST_PRIME(997);
        TEST_PRIME(991);
        TEST_PRIME(1322);
        TEST_PRIME(12122);
        TEST_PRIME(39601);
    } else {
        unsigned long tmp = 0;
        char* tp = NULL;
        for(int index = 1; index < argc; index++)
        {
            tmp = strtoul(argv[index], &tp, 10);
            if(*tp != 0) {
                printf("%s, isn't a number or out of range.\n", argv[index]);
            } else if (tmp == ULONG_MAX && errno == ERANGE) {
                printf("%s, is out of range.\n", argv[index]);
            } else {
                TEST_PRIME(tmp);
            }
        }
        
        puts("Done\n");
        
    }
#undef TEST_PRIME

	//vector<unsigned long> list;
	//get_prime_between(&list, 10, 10000);
	//for_each(list.begin(), list.end(), print_prime);
	//puts("");
	return 0;
}
*/