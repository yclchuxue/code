#include <stdio.h>

int getmid(int* a, int left, int right);
void quicksort(int* a, int left, int right);

int main()
{
	int num[] = { 5,2,7,8 };
	int len = sizeof(num) / sizeof(num[0]);
	quicksort(num, 0, len - 1);
	for (int i = 0; i < len; i++)
	{
		printf("%d\t", num[i]);
	}
	
	return 0;
}

int getmid(int* a, int left, int right)
{
	int mid = a[left];
	while (left < right)
	{
		while (a[right] >= mid && left < right)
			right--;
		a[left] = a[right];
		while (a[left] <= mid && left < right)
			left++;
		a[right] = a[left];
	}
	a[left] = mid;
	return left;
}

void quicksort(int* a, int left, int right)
{
	if (left < right)
	{
		int get = getmid(a, left, right);
		quicksort(a, left, get - 1);
		quicksort(a, get + 1, right);
	}
}
