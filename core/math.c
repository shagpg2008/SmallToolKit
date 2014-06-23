
unsigned long gcd(unsigned long num1, unsigned long num2)
{
	unsigned long r = 0;

	while(num2 > 0) {
		r = num1 % num2;
		num1 = num2;
		num2 = r;
	} 

	return num1;
}

unsigned long lcm(unsigned long num1, unsigned long num2)
{
	return num1 * num2 / gcd(num1, num2);
}

unsigned long mod(unsigned long num1, unsigned long num2)
{
	return num1 % num2;
}

unsigned long lim(unsigned long num1, unsigned long num2)
{
	int tmp = (num2 + num1 - 1) / num1;
	return tmp * num1;
}

#ifdef NUM_CML
#include <stdio.h>
int main(int argc, char *argv[])
{
	printf("gcd(1,2)=%d\n", gcd(1,2));
	printf("gcd(3,2)=%d\n", gcd(3,2));
	printf("gcd(4,2)=%d\n", gcd(4,2));
	printf("lcm(1,2)=%d\n", lcm(1,2));
	printf("lcm(3,2)=%d\n", lcm(3,2));
	printf("lcm(4,2)=%d\n", lcm(4,2));
	printf("lim(1,2)=%d\n", lim(1,2));
	printf("lim(3,2)=%d\n", lim(3,2));
	printf("lim(2,3)=%d\n", lim(2,3));
	return 0;
}
#endif