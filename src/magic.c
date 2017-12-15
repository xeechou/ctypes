
int abs(int a)
{
	int s = a >> sizeof(int) * 8 - 1;	//s == -1 if a < 0, else 0
	a ^= s;					//any number ^ -1 is one's complement
	a -= s;					//a - (-1) if a is one's complement
}

/* return a - (a - b) if a < b, else return a - 0 */
int max(int a, int b)
{
	int diff = a - b;
	int dsign = diff >> sizeof(int) * 8 - 1;
	return a - (diff & dsign);
}

/* return b + (a - b) if a < b, else return b + 0 */
int min(int a, int b)
{
	int diff = a - b;
	int dsign = diff >> sizeof(int) * 8 - 1;
	return b + (diff & dsign);
}

#define _max((a), (b)) ((a) > (b)) ? (a) : (b)
#define _min((a), (b)) ((a) > (b)) ? (b) : (a)

/* conditional increment */

#define _conditional_incre((x), (cond), (inc))	(x) += -(cond)  & (inc)

/* increase pointer if condition */
/* if (a > b)
 *	*ptr++ = value
 * now becomes
 *
 * *ptr = value
 * ptr += (a > b)
 */
