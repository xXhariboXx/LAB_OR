#include <stdio.h>
#include <string.h>

void doSth (unsigned int n){
	while (n)
		n --;
}
void header (char * s){
	unsigned i;
	putchar ('\n');
	printf ("%s\n", s);
	for (i = 0; i < strlen(s); i ++)
		putchar ('-');
	putchar ('\n');
	fflush(stdout);
}
void footer (void){
	unsigned i;

	putchar ('\n');
	for (i = 0; i < 80; i ++)
		putchar ('=');
	fflush(stdout);
}
void printArr (const char * txtBefore, int a[], const int cnt, const char * txtAfter){
	int i;
	printf ("%s", txtBefore);
	for (i = 0; i < cnt; i ++)
		printf ("%d ", a[i]);
	printf ("%s", txtAfter);
	return;
}
