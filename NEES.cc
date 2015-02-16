#define SW2 26
#define math_Pi 3.14159265359

byte Modi;
word Delay;
word Delay1;
word Delay2;
word ADC;
float Faktor;
float SinPos;
float SinStep;
float Sinusfaktor;
float Pi;

float f1;
float f2;
float f3;
float f4;


void main(void)
{
    Port_DataDirBit(SW2,0);
    Port_WriteBit(SW2,1);

    Port_DataDirBit(23,PORT_OUT);
    ADC_Set(ADC_VREF_VCC,0);

    float W;
    float D;

    Pi=3.14159265359;
    Modi=0;
    SinPos=0;
    SinStep=Pi/77;
    Faktor=1;
    Sinusfaktor=1;


    while(1)
    {
        if(Port_ReadBit(SW2)==0)
        {
            AbsDelay(50);
            if(Port_ReadBit(SW2)==0)
            {
                Modi++;
                if(Modi>=4) Modi=0;
                Msg_WriteInt(Modi);
                Msg_WriteChar(10);
                Msg_WriteChar(13);
                AbsDelay(50);
                while(Port_ReadBit(SW2)==0);
            }
        }

        if(SinPos>Pi/2 || SinPos<-Pi/2)
        {
            SinStep=SinStep*-1;
        }

        SinPos=SinPos+SinStep;
        //Faktor=(Sinus(SinPos)/Sinusfaktor)+1+(1/Sinusfaktor); 2 1.5+0.5=2
        //Faktor=Sinusfaktor;

        ADC=ADC_Read();
        //Delay: Delay1*Faktor und Delay2*Faktor

        if(Modi==0) //Frequenzwahl
        {
            //4.16666667*1.00763766^ADC+4.16666667
            //0.99242023
            //4.16666667*D+4.16666667

            Delay=4.16666667*Power(1.0076376617191691,ADC)-4.16666667;

            //Msg_WriteFloat(Power(2.0,7));
            Msg_WriteChar(13);
            Msg_WriteFloat(SinPos);
            Msg_WriteChar(13);
            Msg_WriteFloat(Sinus(SinPos));
            Msg_WriteChar(13);
            Msg_WriteFloat(ADC);
            Msg_WriteChar(13);
            Msg_WriteFloat(Faktor);
            Msg_WriteChar(13);
            Msg_WriteFloat(Delay);
            Msg_WriteChar(13);

            Delay1=(Delay*Faktor)*(Sinus(SinPos)*Sinusfaktor+1);
            Delay2=(Delay/Faktor)*(Sinus(SinPos)*Sinusfaktor+1);

            Msg_WriteFloat(Delay1);
            Msg_WriteChar(13);
            Msg_WriteFloat(Delay2);
            Msg_WriteChar(10);
            Msg_WriteChar(13);



        } else if(Modi==1) { //Verhältnisswahl
            //1.8*1.00384247^ADC-0.8
            //0.996172235
            //1.8*D-0.8

            if(ADC<512)
            {
                Faktor=1.8*Power(1.003842473,511-ADC)-0.8;
            } else {
                Faktor=1/(1.8*Power(1.003842473,1023-ADC)-0.8);
            }

            Delay1=(Delay*Faktor)*(Sinus(SinPos)*Sinusfaktor+1);
            Delay2=(Delay/Faktor)*(Sinus(SinPos)*Sinusfaktor+1);

        } else if(Modi==2) { //Pitchwahl => Sinus
            if(ADC>940)
            {
                Sinusfaktor=0;
            } else {
                Sinusfaktor=0.642857*Power(0.996614539,ADC)-0.642857;
            }

            Msg_WriteFloat(SinPos);
            Msg_WriteChar(13);
            Msg_WriteFloat(Sinus(SinPos));
            Msg_WriteChar(13);
            Msg_WriteFloat(ADC);
            Msg_WriteChar(13);
            Msg_WriteFloat(Sinusfaktor);
            Msg_WriteChar(13);
            Msg_WriteFloat(Delay);
            Msg_WriteChar(13);

            Delay1=(Delay*Faktor)*(Sinus(SinPos)*Sinusfaktor+1);
            Delay2=(Delay/Faktor)*(Sinus(SinPos)*Sinusfaktor+1);

            Msg_WriteFloat(Delay1);
            Msg_WriteChar(13);
            Msg_WriteFloat(Delay2);
            Msg_WriteChar(10);
            Msg_WriteChar(13);


        } else if(Modi==3) { //Random mode
            Delay=0.33333333*Power(1.00135604637,ADC)-0.33333333;

            //The .0 is extreamly importent because outherwise it doesen uses floats!
            Delay1=Delay*(rand()/65536.0);
            Delay2=Delay*(rand()/65536.0);

            //Msg_WriteFloat(Delay1);
            //Msg_WriteChar(13);
            //Msg_WriteFloat(Delay2);
            //Msg_WriteChar(10);
            //Msg_WriteChar(13);

        } else { //Nichts Abfragen
        }

        Port_WriteBit(23,PORT_ON);
        AbsDelay(Delay1);

        Port_WriteBit(23,PORT_OFF);
        AbsDelay(Delay2);

    }
}



float Sinus(float x) //Based on Taylor's theorem    Bogenmass! Achtung nur genau von -Pi bis Pi
{
    f1=x*x;
    f2=f1*x;
    f3=f2*f2*f2;
    f4=f3*f1;

    return(x-f2/6+(f2*f1)/120-(f3/f1)/5040+f3/362880-f4/39916800+(f4*f1)/6227020800); //Remove the last part of the formulawill improve the speed much but the calculation will be more inaccurate.
    //return(x-f2/6+f2*f1/120)
}

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


float QWurzel(float W, float D) //Babylonian method or Heron's method works with the Heron's formula.
{
    //do {
    int i;
    for(i=0;i<7;i++) //A lover value then 7 will improve the speed much but the calculation will be more inaccurate.
    {
        D=(D+W/D)/2.0;
    }
    //} while((D*D-W)*(D*D-W)<0.00001);
    return D;
}
