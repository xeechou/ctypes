#include <stdio.h>

int link_error(int error) {
	switch error {
		case 0:
			break;
		case -2:
			printf("error: illegal delete position\n");
			return -1;
			break;
		case -1:
			printf("error: try to insert at illegal position\n");
			return -1;
			break;
		
	}
	
}
