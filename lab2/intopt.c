#include <stdlib.h>
#include <stdio.h>

#define EPSILON 10e-6

typedef struct {
    int m; /* Constraints. */
    int n; /* Decision variables. */
    int *var; /* 0..n 1 are nonbasic. */
    double **a;
    double *b;
    double *x;
    double *c;
    double y;
    } simplex_t;

    
double xsimplex(int m,int n,double  **a, double *b,double *c,double *x,double y,int *var,int h){
    return -1;
}
    
int simplex(int m,int n,double **a, double *b, double *c, double *x, double y){
    return xsimplex (m,n,a,b,c,x,y, NULL,0);
}

void pivot(){

}


int init(simplex_t *s, int m, int n, int *var, double **a, double *b, double *x, double *c, double y)
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

void prepare(simplex_t *s, int k){
    int m = s->m;
    int n = s->n;
    int i;
    for (i = m + n; i > n; i++){
        s->var[i] = s->var[i-1];
    }
    s->var[n] = m + n;
    n++;
    for(i = 0; i < m; i++){
        s->a[i][n-1] = -1;
    }
    s->x = calloc(m + n, sizeof(double));
    s->c = calloc(n, sizeof(double));
    s->c[n-1] = 1;
    s->n = n;
    pivot (s, k, n-1);
}

int initial(simplex_t *s,int m,int n,double **a,double *b,double *c,double *x,double y, int *var){ // assume bi ≥ 0 so skip the call to prepare and the rest of initial
    int i, j, k;
    double w;
    k = init(s, m, n, var, a, b, x, c, y);
    if(b[k] >= 0){
        return 1;
    }
    prepare(s,k);
    n = s->n;
    s->y = xsimplex (m, n, s->a, s->b, s->c, s->x, 0, s->var,1);
    for (i = 0; i < m+n; i++){
        if (s->var[i] = m+n-1){
            if (abs(s->x[i]) > EPSILON){
                free(s->x);
                free(s->c);
                return 0;
            }
            else{
                break; // This i will be used on the next page.
            }
    }
    if (i >= n){
        // x n+m is basic. find good nonbasic.
        for (j = k = 0; k < n; k++){
            if (abs(s->a[i-n][k]) > abs(s->a[i-n][j])){
                j=k;
                pivot(s,i-n,j);
                i=j;
            }
        }
    }
    if (i < n-1){
        // x n+m is nonbasic and not last. swap columns i and n-1
        k = s->var[i]; 
        s->var[i] = s->var[n-1]; 
        s->var[n-1] = k;
        
        for(k = 0; k < m; k = k + 1){
            w = s->a[k][n-1];
            s->a[k][n-1] = s->a[k][i];
            s->a[k][i] = w;
        }
    }
    else{
            // x n+m is nonbasic and last. forget it.
            free(s->c);
            s->c = c;
            s->y = y;

        }
    for (k = n-1; k < n+m-1; k++){
        s->var[k] = s->var[k+1];
        n = s->n = s->n - 1;
        

        }
    }

}

int select_nonbasic(simplex_t *s){
    int i;
    for(i = 0; i < s->n; i++){
        if(s->c[i] > EPSILON){
            return i;
        }
    }
    return -1;
}