#include <stdio.h>
#include <stdlib.h>

#define NUMCPU 8
#define MAX 10000000

typedef struct __counter_t 
{
	long int value;
} counter_t;

void init(counter_t *c)
{
	c->value = 0;
}

void increment(counter_t *c)
{
	c->value++;
}

void decrement(counter_t *c)
{
	c->value--;
}

int get(counter_t *c)
{
	return c->value;
}

counter_t c_t;

int main(int argc, char **argv)
{
	for(int i = 0; i < (MAX * NUMCPU); i++)
	{
		increment(&c_t);
	}
	printf("[*] expected value: %d\n", MAX * NUMCPU);
	printf("[*] real value: %ld\n", c_t.value);
	return 0;

}

