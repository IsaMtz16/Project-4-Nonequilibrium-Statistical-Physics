#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

extern float Random(void);
#define NormRANu (2.3283063671E-10)
extern void ini_ran(int SEMILLA);
unsigned int irr[256];
unsigned int ir1;
unsigned char ind_ran, ig1, ig2, ig3;

#define PI 3.14159265
#define N 100
#define L 10
#define Nt 100000

typedef struct{
    double x, y;
} Vector;

Vector epsilon[N]={0};
Vector epsilon0[N]={0};
Vector r[N]={0};
Vector r0[N]={0};
Vector rc[N]={0};
Vector F[N]={0};

float Random(void)    // generar numero aleatorio
{
    float r;
    ig1=ind_ran-24;
    ig2=ind_ran-55;
    ig3=ind_ran-61;
    irr[ind_ran]=irr[ig1]+irr[ig2];
    ir1=(irr[ind_ran]^irr[ig3]);
    ind_ran++;
    r=ir1*NormRANu;
    return r;
}

void ini_ran(int SEMILLA)
{
    int INI,FACTOR,SUM,i;
    srand(SEMILLA);
    INI=SEMILLA;
    FACTOR=67397;
    SUM=7364893;
    for(i=0; i<256; i++)
    {
        INI=(INI*FACTOR+SUM);
        irr[i]=INI;
    }
    ind_ran=ig1=ig2=ig3=0;
}

void Box_Muller (double *g1, double *g2)
{
    double d1, d2;
    d1=sqrt(-2.0*log(Random()));
    d2=2.0*PI*Random();
    *g1=-d1*cos(d2);
    *g2=-d1*sin(d2);
}

void PBC(double *x){
    if(*x>(L/2)){
        *x=*x-L;
    }
    if(*x<-(L/2)){
        *x=*x+L;
    }

}

void inicializar_ruido(double D,int tau){
    double a;
    double b;
    int i;
    for(i=0;i<N;i++){
        a=Random();
        b=Random();
        if(a<0.5){
            epsilon[i].x=sqrt(D/(double)tau);
            epsilon0[i].x=sqrt(D/(double)tau);
        }
        else{
            epsilon[i].x=-sqrt(D/(double)tau);
            epsilon0[i].x=-sqrt(D/(double)tau);
        }
        if(b<0.5){
            epsilon[i].y=sqrt(D/(double)tau);
            epsilon0[i].y=sqrt(D/(double)tau);
        }
        else{
            epsilon[i].y=-sqrt(D/(double)tau);
            epsilon[i].y=-sqrt(D/(double)tau);
        }

    }

}

inicializar_r(){
    int i;
    for(i=0;i<N;i++){
        r[i].x=0;
        r[i].y=0;

        rc[i].x=0;
        rc[i].y=0;

        r0[i].x=0;
        r0[i].y=0;
    }
}

double Mean_square_displacement(int i){
    double dx,dy;
    dx=r[i].x-r0[i].x;
    dy=r[i].y-r0[i].y;
    return (pow(dx,2)+pow(dy,2));
}

double delta2(int tau,double t,double D){
    return (4*D*(t+(double)tau*(exp(-t/(double)tau)-1)));
}

void Euler_ruido(int tau,double h,double D){
    int i;
    double g1, g2;
    for(i=0;i<N;i++){
        Box_Muller(&g1,&g2);
        epsilon[i].x=epsilon[i].x*(1-h/(double)tau)+sqrt(2*D*h/(double)(tau*tau))*g1;
        epsilon[i].y=epsilon[i].y*(1-h/(double)tau)+sqrt(2*D*h/(double)(tau*tau))*g2;
    }

}

void Euler_posicion(double gamma,double h){
    int i;
    double aux;
    for(i=0;i<N;i++){
        r[i].x=r[i].x+h*(F[i].x+epsilon[i].x)/gamma;
        r[i].y=r[i].y+h*(F[i].y+epsilon[i].y)/gamma;

        rc[i].x=rc[i].x+h*(F[i].x+epsilon[i].x)/gamma;
        rc[i].y=rc[i].y+h*(F[i].y+epsilon[i].y)/gamma;

        PBC(&rc[i].x);
        PBC(&rc[i].y);

    }
}

double correlacion(){
    double dx, dy;
    double aux=0;
    int i;
    for(i=0;i<N;i++){
        dx=epsilon[i].x*epsilon0[i].x;
        dy=epsilon[i].x*epsilon0[i].x;
        aux=aux+dx+dy;
    }
    return aux/(double)N;

}

int main(){
    ini_ran(time(NULL));
    double h=0.001;
    int valores_tau[3]={1,10,100};
    int tau=1;
    double D=4;
    double gamma=1;
    double MSD;
    int i,j,t;
    double delta_2;
    double c;

    for(j=0;j<3;j++){
        tau=valores_tau[j];
        FILE*f;
        char filename[40];
        sprintf(filename,"Correlacion_tau%d.txt",tau);
        f=fopen(filename,"w");
        t=0;
        inicializar_ruido(D,tau);
        inicializar_r();
        for(i=0;i<Nt;i++){
            Euler_ruido(tau,h,D);
            Euler_posicion(gamma,h);
            c=correlacion();
            fprintf(f,"%lf       %lf\n",i*h,c);

        }
        fclose(f);

    }



    return 0;

}
