#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "../src/helpers.h"
//#include <helpers.h>

int main(int argc, char *argv[])
{
	assert(abs(3.1) == 3.1);
	assert(abs(-3.1415926) == 3.1415926);
	assert(step(1.0, 2.0) == 0.0);
	assert(step(1.1, 1.1) == 1.0);
	assert(step(1.1, 1.0) == 1.0);

	const char *str_arr[] = {"this", "that", "you", "me"};
	assert(numof(str_arr) == 4);
	assert(clamp(3.0, 1.0, 2.0) == 2.0);
	assert(clamp(1.2, 1.0, 2.0) == 1.2);
	fprintf(stderr, "%f\n", mix(0.3, 4.0, 5.0));
	assert(mix(0.3, 4.0, 5.0) == 4.30);
	assert(mix(-1.0, 4.0, 5.0) == 4.0);
	assert(mix(3.0, 3.0, 5.0) == 5.0);
	assert(smoothstep(2.0, 1.0, 3.0) == 0.5);
	assert((smoothstep(1.8, 1.0, 3.0) - 0.35200 < 0.00001));
	return 0;
}
