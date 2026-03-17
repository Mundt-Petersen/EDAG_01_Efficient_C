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

int initial(simplex_t *s, int m, int n, double **a, double *b, double *c, double *x, double y, int *var)
{ // assume bi ≥ 0 so skip the call to prepare and the rest of initial
    int i, j, k;
    double w;
    k = init(s, m, n, var, a, b, x, c, y);
    if (b[k] >= 0)
    {
        return 1;
    }

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




int main(void)
{
    int m;
    int n;
    double **a;
    double *b;
    double *c;
    double *x;
    size_t i;
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
