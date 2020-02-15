#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "../src/helpers.h"
//#include <helpers.h>

int main(int argc, char *argv[])
{
	assert(ABS(3.1) == 3.1);
	assert(ABS(-3.1415926) == 3.1415926);
	assert(STEP(1.0, 2.0) == 0.0);
	assert(STEP(1.1, 1.1) == 1.0);
	assert(STEP(1.1, 1.0) == 1.0);

	const char *str_arr[] = {"this", "that", "you", "me"};
	assert(NUMOF(str_arr) == 4);
	assert(CLAMP(3.0, 1.0, 2.0) == 2.0);
	assert(CLAMP(1.2, 1.0, 2.0) == 1.2);
	fprintf(stderr, "%f\n", MIX(0.3, 4.0, 5.0));
	assert(MIX(0.3, 4.0, 5.0) == 4.30);
	assert(MIX(-1.0, 4.0, 5.0) == 4.0);
	assert(MIX(3.0, 3.0, 5.0) == 5.0);
	assert(SMOOTHSTEP(2.0, 1.0, 3.0) == 0.5);
	assert((SMOOTHSTEP(1.8, 1.0, 3.0) - 0.35200 < 0.00001));
	(void)str_arr;
	return 0;
}
