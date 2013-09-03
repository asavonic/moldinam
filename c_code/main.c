#include <stdio.h>

int main(int argc, char const *argv[])
{
	float a = 50000.1;
	float b = 3.70089;

	float c_del_a = a/1000;
	float c_del_b = b/1000;
	float c_del = ( c_del_a   + c_del_b  );

	printf("%.25lf\n", c_del * 1000);
	float c = a + b;
	printf("%.25lf\n", c);

	return 0;
}