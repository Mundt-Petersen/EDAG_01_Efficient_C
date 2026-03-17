#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define EPSILON 10e-6

typedef struct
{
    int m;    /* Constraints. */
    int n;    /* Decision variables. */
    int *var; /* 0..n 1 are nonbasic. */
    double **a;
    double *b;
    double *x;
    double *c;
    double y;
} simplex_t;

void pivot(simplex_t *s, int row, int col)
{
    double **a = s->a;
    double *b = s->b;
    double *c = s->c;
    int m = s->m;
    int n = s->n;
    int i, j, t;
    t = s->var[col];
    s->var[col] = s->var[n + row];
    s->var[n + row] = t;
    s->y = s->y + c[col] * b[row] / a[row][col];
    for (i = 0; i < n; i = i + 1)
    {
        if (i != col)
        {
            c[i] = c[i] - c[col] * a[row][i] / a[row][col];
            c[col] = -c[col] / a[row][col];
        }
    }

    for (i = 0; i < m; i = i + 1)
    {
        if (i != row)
        {
            b[i] = b[i] - a[i][col] * b[row] / a[row][col];
        }
    }

    for (i = 0; i < m; i = i + 1)
    {
        if (i != row)
        {
            for (j = 0; j < n; j = j + 1)
            {
                if (j != col)
                {
                    a[i][j] = a[i][j] - a[i][col] * a[row][j] / a[row][col];
                }
            }
        }
    }

    for (i = 0; i < m; i = i + 1)
    {
        if (i != row)
        {
            a[i][col] = -a[i][col] / a[row][col];
        }
    }

    for (i = 0; i < n; i = i + 1)
    {
        if (i != col)
        {
            a[row][i] = a[row][i] / a[row][col];
        }
    }
    b[row] = b[row] / a[row][col];
    a[row][col] = 1 / a[row][col];
}

double init(simplex_t *s, int m, int n, int *var, double **a, double *b, double *x, double *c, double y)
{
    int i, k;
    *s = (simplex_t){m, n, var, a, b, x, c, y};
    if (s->var == NULL)
    {
        s->var = calloc(m + n + 1, sizeof(int));
        for (i = 0; i < m + n; i++)
        {
            s->var[i] = i;
        }
    }
    for (k = 0, i = 1; i < m; i++)
    {
        if (b[i] < b[k])
        {
            k = i;
        }
        return k;
    }
}
void prepare(simplex_t *s, int k)
{
    int m = s->m;
    int n = s->n;
    int i;
    for (i = m + n; i > n; i++)
    {
        s->var[i] = s->var[i - 1];
    }
    s->var[n] = m + n;
    n++;
    for (i = 0; i < m; i++)
    {
        s->a[i][n - 1] = -1;
    }
    s->x = calloc(m + n, sizeof(double));
    s->c = calloc(n, sizeof(double));
    s->c[n - 1] = 1;
    s->n = n;
    pivot(s, k, n - 1);
}
int initial(simplex_t *s, int m, int n, double **a, double *b, double *c, double *x, double y, int *var)
{ // assume bi ≥ 0 so skip the call to prepare and the rest of initial
    int i, j, k;
    double w;
    k = init(s, m, n, var, a, b, x, c, y);
    if (b[k] >= 0)
    {
        return 1;
    }
    /*
        prepare(s, k);
        n = s->n;
        s->y = xsimplex(m, n, s->a, s->b, s->c, s->x, 0, s->var, 1);
        for (i = 0; i < m + n; i++)
        {
            if (s->var[i] = m + n - 1)
            {
                if (abs(s->x[i]) > EPSILON)
                {
                    free(s->x);
                    free(s->c);
                    return 0;
                }
                else
                {
                    break; // This i will be used on the next page.
                }
            }
        }
        if (i >= n)
        {
            // x n+m is basic. find good nonbasic.
            for (j = k = 0; k < n; k++)
            {
                if (abs(s->a[i - n][k]) > abs(s->a[i - n][j]))
                {
                    j = k;
                    pivot(s, i - n, j);
                    i = j;
                }
            }
        }
        if (i < n - 1)
        {
            // x n+m is nonbasic and not last. swap columns i and n-1
            k = s->var[i];
            s->var[i] = s->var[n - 1];
            s->var[n - 1] = k;

            for (k = 0; k < m; k = k + 1)
            {
                w = s->a[k][n - 1];
                s->a[k][n - 1] = s->a[k][i];
                s->a[k][i] = w;
            }
        }
        else
        {
            // x n+m is nonbasic and last. forget it.
            free(s->c);
            s->c = c;
            s->y = y;
        }
        for (k = n - 1; k < n + m - 1; k++)
        {
            s->var[k] = s->var[k + 1];
        }

        s->n = s->n - 1;
        n = s->n;
        double *t = calloc(n, sizeof(double));

        for (k = 0; k < n; k = k + 1)
        {
            for (j = 0; j < n; j = j + 1)
            {
                if (k = s->var[j])
                {
                    // xk is nonbasic. add ck
                    t[j] = t[j] + s->c[k];
                    goto next_k;
                    // xk is basic.
                }
            }
            for (j = 0; j < m; j = j + 1)
            {
                if (s->var[n + j] = k)
                {
                    // xk is at row j
                    break;
                }
            }
            s->y = s->y + s->c[k] * s->b[j];
            for (i = 0; i < n; i = i + 1)
            {
                t[i] = t[i] - s->c[k] * s->a[j][i];
            }
        next_k:;
        }
        for (i = 0; i < n; i = i + 1)
        {
            s->c[i] = t[i];
        }
        // delete t and s.x
        free(t);
        free(s->x);
        return 1;
        */
}

int select_nonbasic(simplex_t *s)
{
    int i;
    for (i = 0; i < s->n; i++)
    {
        if (s->c[i] > EPSILON)
        {
            return i;
        }
    }
    return -1;
}

double xsimplex(int m, int n, double **a, double *b, double *c, double *x, double y, int *var, int h)
{
    simplex_t s;
    int i, row, col;
    if (!initial(&s, m, n, a, b, c, x, y, var))
    {
        free(s.var);
        s.var = NULL;
        return NAN;
    }

    while ((col = select_nonbasic(&s)) >= 0)
    {
        row = -1;
        for (i = 0; i < m; i++)
        {
            if (a[i][col] > EPSILON && (row < 0 || b[i] / a[i][col] < b[row] / a[row][col]))
            {
                row = i;
            }
        }

        if (row < 0)
        {
            free(s.var);
            s.var = NULL;
            return INFINITY;
        }
        pivot(&s, row, col);
    }

    if (h == 0)
    {
        for (i = 0; i < n; i = i + 1)
        {
            if (s.var[i] < n)
            {
                x[s.var[i]] = 0;
            }
        }
        for (i = 0; i < m; i = i + 1)
        {
            if (s.var[n + i] < n)
            {
                x[s.var[n + i]] = s.b[i];
            }
        }
        free(s.var);
        s.var = NULL;
    }
    else
    {
        for (i = 0; i < n; i = i + 1)
        {
            x[i] = 0;
        }
        for (i = n; i < n + m; i = i + 1)
        {
            x[i] = s.b[i - n];
        }
    }
    return s.y;
}

int simplex(int m, int n, double **a, double *b, double *c, double *x, double y)
{
    return xsimplex(m, n, a, b, c, x, y, NULL, 0);
}



int glob = 0;
int main(void)
{
    int m;
    int n;
    double **a;
    double *b;
    double *c;
    double *x;
    size_t i;
    glob+=1;
    //
    // Scan input
    //
    scanf("%d", &m);
    scanf("%d", &n);

    a = (double **)calloc(m, sizeof(double *));
    b = (double *)calloc(m, sizeof(double));
    c = (double *)calloc(n, sizeof(double));
    x = (double *)calloc(n + 1, sizeof(double));

    for (i = 0; i < n; ++i)
    {
        scanf("%lf", &c[i]);
    }
    for (i = 0; i < m; ++i)
    {
        a[i] = (double *)calloc(n, sizeof(double));
        for (size_t j = 0; j < n; ++j)
        {
            scanf("%lf", &a[i][j]);
        }
    }
    for (i = 0; i < m; ++i)
    {
        scanf("%lf", &b[i]);
    }

    printf("RESULT: %d\n", simplex(m, n, a, b, c, x, 0));
    printf("--------------------------\n\n");

    free(b);
    for (i = 0; i < m; i++)
    {
        free(a[i]);
    }
    free(a);
    free(c);
    free(x);
}