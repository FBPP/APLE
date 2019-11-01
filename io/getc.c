#include "../apue.h"

int main()
{
	int c;

	while( (c = getc(stdin)) != EOF)
		if( putc(c, stdout) == EOF){
			perror("error putc:");
			exit(1);
		}


	if(ferror(stdin)){
		perror("error getc:");
		exit(1);
	}

	return 0;
}
