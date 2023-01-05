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
#define Nt 1000000

typedef struct{
    double x, y;
} Vector;

Vector epsilon[N]={0};
Vector r[N]={0};
Vector r0[N]={0};
Vector rc[N]={0};
Vector F[N]={0};
double e[N]={0};

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
        }
        else{
            epsilon[i].x=-sqrt(D/(double)tau);
        }
        if(b<0.5){
            epsilon[i].y=sqrt(D/(double)tau);
        }
        else{
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
    //dy=r[i].y-r0[i].y;
    return (dx*dx);
}

/*double delta2(int tau,double t,double D){
    return (4*D*(t+(double)tau*(exp(-t/(double)tau)-1)));
}*/


double displacement(int i){
    double aux;
    aux=(r[i].x-r0[i].x);
    return aux;
}



void valores_F(double f){
    int i;
    double a;
    for(i=0;i<N;i++){
        a=Random();
        if(a>0.5){
            e[i]=1;
        }
        else{
            e[i]=-1;
        }
        F[i].x=f*e[i];
        F[i].y=0;
    }
}

void Euler_ruido(int tau,double h,double D){
    int i;
    double g1, g2;
    for(i=0;i<N;i++){
        Box_Muller(&g1,&g2);
        epsilon[i].x=epsilon[i].x*(1-h/(double)tau)+g1*sqrt(2*D*h/(double)(tau*tau));
        epsilon[i].y=epsilon[i].y*(1-h/(double)tau)+g2*sqrt(2*D*h/(double)(tau*tau));
    }

}

void Euler_posicion(double gamma,double h,double *MSD,double *d){
    int i;
    *MSD=0;
    *d=0;
    double aux=0;
    double aux2=0;
    for(i=0;i<N;i++){
        r[i].x=r[i].x+h*(F[i].x+epsilon[i].x)/gamma;
        r[i].y=r[i].y+h*(F[i].y+epsilon[i].y)/gamma;

        rc[i].x=rc[i].x+h*(F[i].x+epsilon[i].x)/gamma;
        rc[i].y=rc[i].y+h*(F[i].y+epsilon[i].y)/gamma;

        PBC(&rc[i].x);
        PBC(&rc[i].y);

        aux=Mean_square_displacement(i);
        *MSD=*MSD+aux;
        aux2=displacement(i);
        *d=*d+aux2;
    }
}



int main(){
    ini_ran(time(NULL));
    double h=0.001;
    int valores_tau[10]={1,5,10,20,35,50,65,80,90,100};
    int tau=10;
    double f=10;
    double valores_f[5]={0,0.001,0.01,0.1,1};
    double D=4;
    double gamma=1;
    double MSD;
    int i,j;
    double d;
    double mu[10]={0};
    double D_[10]={0};

    FILE*f2;
    f2=fopen("mu_D_f=10.txt","w");
    for(j=0;j<10;j++){
        tau=valores_tau[j];



        inicializar_ruido(D,tau);
        inicializar_r();
        valores_F(f);
        for(i=0;i<Nt;i++){
            MSD=0;
            d=0;

            Euler_ruido(tau,h,D);
            Euler_posicion(gamma,h,&MSD,&d);
            MSD=MSD/(double)N;
            if(d<0){
                d=-d;
            }
            d=d/(double)N;
            mu[j]=d/(f*h*(double)i);
            D_[j]=MSD/(2*h*(double)i);




        }
        fprintf(f2,"%d       %lf     %lf\n",tau,mu[j],D_[j]);


    }
    fclose(f2);



    return 0;

}
