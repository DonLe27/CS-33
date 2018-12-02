#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <stdlib.h>

void* thread(void* vargp);
int L(int k);

struct arg{
	int label;
	int k;
};

int L(int k) //Leonardo function
{
	if ((k == 1) || (k == 0))
		return 1;
	int t1 = 1, t2 = 1;
	int nextTerm = 0;
	int i;
	for (i = 0; i < (k - 1); i++)
	{
		nextTerm = t1 + t2 + 1;
		t1 = t2;
		t2 = nextTerm;
	}
	return nextTerm;

}

int main(int argc, char **argv)
{
	int commandArg = atoi(argv[1]);
	if (commandArg == 1)
	{
		printf("[0]\n");
		exit(0);
	}
	pthread_t tid;
	printf("(0\n");
	struct arg root;
	root.k = commandArg;
	root.label = 0;
	pthread_create(&tid, NULL, thread, (void*)&root);
	pthread_join(tid, NULL);

	printf("0)\n");
	return 0;
}

void *thread(void *vargp)
{	
	struct arg* Parent = (struct arg*)vargp;
	struct arg* left = malloc(sizeof(struct arg) * 1);
	left->k = Parent->k - 1;
	left->label = Parent->label + 1;
	if (Parent->k <= 1) //Parent was leaf 
	{
		printf("[%d]\n", Parent->label);
		free(left);
		pthread_exit(NULL);
	}
	else
	{
		if (left->k > 1) //There are still descendents for left
			printf("(%d\n", left->label);
		pthread_t tid;
		pthread_create(&tid, NULL, thread, left);
		pthread_join(tid, NULL);
		if (left->k > 1)
			printf("%d)\n", left->label);
		free(left);
	}
	if (Parent->k - 2 >= 0) //Check if there is right descendent
	{
		struct arg* right = malloc(sizeof(struct arg) * 1);
		pthread_t tid;
		right->k = Parent->k - 2;
		right->label = Parent->label + L((Parent->k) - 1) + 1;
		if (right->k > 1) //Check if right has descendents
			printf("(%d\n", right->label);
		pthread_create(&tid, NULL, thread, right);
		pthread_join(tid, NULL);
		if (right->k > 1)
			printf("%d)\n", right->label);
		free(right);
	}
	return NULL;
}



