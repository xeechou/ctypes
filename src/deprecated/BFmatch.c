/* Brute Force algorithm */
#include <string.h>
#include <stdio.h>

/* bfmatch : match pattern *p in *s, return match index i of *s */
int bfmatch(char *s, char *p) {
	int i, j;
	for (i = 0; i <= strlen(s) - strlen(p); i++) {
		for (j = 0; j <= strlen(p)-1; j++) {
			if (s[i+j] == p[j]);
			else
				break;
		}
		if (j == strlen(p))
			return i;
	}
	return -1;
}

int main() {
	char *s = "12345123456";
	char *p = "123456";
	int i = bfmatch(s, p);
	printf("%d\n", i);
}
