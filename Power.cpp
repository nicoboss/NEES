#include <iostream>
using namespace std;

float Power(float a, int x)
{
    if(x==0) return 1;
    if(x==1) return x;
    if(x<1) return false;

    int i;
    unsigned int z_erg;
    unsigned int z_temp;
    float p_erg;
    float p[100];
    float z_list[100];
    p[0]=a;
    z_list[0]=1;
    for(i=1;i<100;i++)
    {
        p[i]=p[i-1]*p[i-1];

        z_list[i]=z_list[i-1]+z_list[i-1];
        //cout << p[i] << " " << z_list[i] << endl;
        if(z_list[i]>x) goto Power_calculate;
    }
    return false;

    Power_calculate:;
    //cout << endl;
    z_erg=z_list[i-1];
    p_erg=p[i-1];
    //cout << p_erg << " & " << z_erg << endl;
    for(;i>0;i--)
    {
        z_temp=z_erg+z_list[i-1];
        if(z_temp<=x)
        {
            z_erg=z_temp;
            p_erg=p_erg*p[i-1];
            //cout << z_temp << endl;
            if(z_erg==x) break;
        }
    }
    //cout << i << endl;
    return p_erg;
}

int main()
{
    //float Basis;
    //Basis=cin.getline();
    //int Exponent;
    //Exponent=cin.getline();
    cout << Power(77,0) << endl;
    return 1;
}


