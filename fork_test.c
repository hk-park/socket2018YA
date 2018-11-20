#include <stdio.h>

main()
{
	int	pid;
	int	a = 10;
	int	b = 20;

	a = a + 1;

	pid = fork();
	if (pid > 0) {
		a = a + 1;
		
		printf("PARENT");
	}
	else if (pid == 0) {
		b = b + 1;
		printf("CHILD");
	}
	printf("[%d] a=%d/b=%d\n", getpid(), a, b);
}

