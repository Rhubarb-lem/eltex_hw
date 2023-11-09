#include <stdio.h>
#define N 7
int main()
{
    int arr[N][N], count = 1, len = N, dir = 0, step = 1, a = 0, b = 0;
    while (count <= N * N)
    {
        if (dir == 0)
        {
            for (int i = 0; (i < len) && (count <= N * N); a += step, i++)
            {
                arr[a][b] = count;
                count++;
            }
            a -= step;
            if (step == 1)
            {
                b++;
            }
            else
            {
                b--;
            }
            dir = 1;
            len--;
        }
        else
        {
            for (int i = 0; (i < len) && (count <= N * N); b += step, i++)
            {
                arr[a][b] = count;
                count++;
            }
            b -= step;
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