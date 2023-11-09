#include <stdio.h>
#define N 5
int main()
{
    int arr[N][N], count = 1, len = N, dir = 0, step = 1, a = 0, b = 0;
    while (count <= N * N)
    {
        if (dir == 0)
        {
            for (int i = 0; i < len; a += step, i++)
            {
                arr[a][b] = count;
                printf("%d ", arr[a][b]);
                count++;

            }
            
            if (step == 1)
            {
                b++;
            }
            else
            {
                b--;
            }
            dir = 1;
            
            len --;
        }
        else
        {
            for (int i= 0; i < len; b += step, i++)
            {
                arr[a][b] = count;
                printf("%d ", arr[a][b]);
                count++;
            
            }
            if (step == 1)
            {
                step = -1;
                a--;
            }
            else
            {
                step = 1;
                a++;
            }
            dir = 0;
            
        }
        
    }
    printf("\n");
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf("%d ", arr[j][i]);
        }
        printf("\n");
    }
    return 0;
}
