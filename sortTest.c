#include <stdio.h>

void swap(int *a, int *b)
{
  int tmp;

  tmp = *a;
  *a = *b;
  *b = tmp;
}

/**
 * stupid sort for testing.
 */
void b_sort(int p[], int left, int right)
{
  int i, j;

  for (i = left; i <= right; i++) {
    for (j = left; j <= right - 1; j++) {
      if (p[j] > p[j+1])
        swap(&p[j], &p[j+1]);
    }
  }
}

int main()
{
  int data[] = {3, 2, 1, 4, 5, 100, 53, 59 ,532};
  int length = sizeof(data) / sizeof(data[0]);
  int i;

  b_sort(data, 0, length - 1);

  for (i = 0; i<length; i++) {
    printf("%d ", data[i]);
  }
  printf("\n");
}
