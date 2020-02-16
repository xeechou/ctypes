#include <stdlib.h>
#include <stdio.h>
#include "../src/helpers.h"
//#include <helpers.h>

int main(int argc, char *argv[])
{
	ASSERT(ABS(3.1) == 3.1);
	ASSERT(ABS(-3.1415926) == 3.1415926);
	ASSERT(STEP(1.0, 2.0) == 0.0);
	ASSERT(STEP(1.1, 1.1) == 1.0);
	ASSERT(STEP(1.1, 1.0) == 1.0);

	const char *str_arr[] = {"this", "that", "you", "me"};
	ASSERT(NUMOF(str_arr) == 4);
	ASSERT(CLAMP(3.0, 1.0, 2.0) == 2.0);
	ASSERT(CLAMP(1.2, 1.0, 2.0) == 1.2);
	fprintf(stderr, "%f\n", MIX(0.3, 4.0, 5.0));
	ASSERT(MIX(0.3, 4.0, 5.0) == 4.30);
	ASSERT(MIX(-1.0, 4.0, 5.0) == 4.0);
	ASSERT(MIX(3.0, 3.0, 5.0) == 5.0);
	ASSERT(SMOOTHSTEP(2.0, 1.0, 3.0) == 0.5);
	ASSERT((SMOOTHSTEP(1.8, 1.0, 3.0) - 0.35200 < 0.00001));
	(void)str_arr;
	return 0;
}
