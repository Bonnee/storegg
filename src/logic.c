#include "logic.h"

//custom basic pow function for int parameters
int my_pow(int x, int y)
{
	int res = 1;
	for (int i = 0; i < y; i++)
		res *= x;
	return res;
}

//return the logarithm base 2
int Log2n(int n)
{
  return (n > 1) ? 1 + Log2n(n / 2) : 0;
}


double get_binary_digits(double n)
{
	return (double)(Log2n((int)n)+1);
}

//function that convert decimal num to binary writing into an array with a specified delay from first pos
void num_to_bin(int num, int n_bit, int ar_delay, int *array)
{
	for (int i = n_bit - 1; i >= 0; i--)
	{
		int num_pow = my_pow(2, i);
		if (num >= num_pow)
		{
			array[(n_bit - 1 - i) + ar_delay] = 1;
			num -= num_pow;
		}
		else
		{
			array[(n_bit - 1 - i) + ar_delay] = 0;
		}
	}
}

void calc_output(int *input, int *output, int max_egg, int max_out)
{

	//initialization
	for (int i = 0; i < max_out; i++)
	{
		output[i] = 0;
	}

	int num_eggs = 0;
	//count eggs
	for (int i = 0; i < max_egg; i++)
	{
		num_eggs += input[i];
	}

	int mag_min = 0;
	int start = max_egg;
	//count min eggs in storage
	for (int i = start; i < start + 2; i++)
	{
		mag_min += my_pow(2, 1 - (i - start)) * input[i];
	}

	// [bit 0-2]
	//number of eggs in binary
	num_to_bin(num_eggs, 3, 0, output);

	// [bit 3-4]
	//check to move eggs from storage
	if (num_eggs < max_egg && mag_min > 0)
	{
		int eggs_to_move = max_egg - num_eggs;
		if (eggs_to_move > mag_min) //not enough in storage
			eggs_to_move = mag_min;
		num_to_bin(eggs_to_move, 2, 3, output);
	}

	// [bit 5-7]
	//check to order eggs to storage
	if (mag_min < 3 && num_eggs + mag_min < max_egg)
	{
		int order_eggs; // know exactly num storage eggs and need some other
		if ( max_egg - mag_min - num_eggs >= 5 )
			order_eggs = 5;
		else // ordering a surplus egg if needed and possible
			order_eggs = max_egg - mag_min - num_eggs + 1;
		num_to_bin(order_eggs, 3, 5, output);
	}
}