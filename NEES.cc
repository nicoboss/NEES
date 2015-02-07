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
    Sinusfaktor=2;


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

        if(SinPos>Pi || SinPos<-Pi)
        {
            SinStep=SinStep*-1;
        }

        SinPos=SinPos+SinStep;
        Faktor=(Sinus(SinPos)/Sinusfaktor)+1+(1/Sinusfaktor);

        ADC=ADC_Read();
        //Delay: Delay1*Faktor und Delay2*Faktor

        if(Modi==0) //Frequenzwahl
        {
            //4.16666667*1.00763766^ADC+4.16666667
            //0.99242023
            //4.16666667*D+4.16666667

            Delay=4.16666667*Wurzel(0.99242023,ADC)+4.16666667;

            Msg_WriteFloat(SinPos);
            Msg_WriteChar(10);
            Msg_WriteFloat(Faktor);
            Msg_WriteChar(10);
            Msg_WriteChar(13);

            Delay1=Delay*Faktor;
            Delay2=Delay/Faktor;

        } else if(Modi==1) { //Verhältnisswahl
            //1.8*1.00384247^ADC-0.8
            //0.996172235
            //1.8*D-0.8

            if(ADC<512)
            {
                Faktor=1.8*Wurzel(0.996172235,511-ADC)-0.8;
            } else {
                Faktor=1/(1.8*Wurzel(0.996172235,1023-ADC)-0.8);
            }

            Delay1=Delay*Faktor;
            Delay2=Delay/Faktor;

        } else if(Modi==2) { //Pitchwahl => Sinus
            if(ADC>940)
            {
                Sinusfaktor=0;
            } else {
                Sinusfaktor=0.642857*Wurzel(0.99661453923,ADC)-0.642857;
            }
        } else if(Modi==3) { //Random mode
            Delay=4.16666667*Wurzel(0.99242023,ADC)+4.16666667;
            Delay1=Delay*rand()/16384;
            Delay2=Delay*rand()/16384;
    } else { //Nichts Abfragen
    }

        Port_WriteBit(23,PORT_ON);
        AbsDelay(Delay1);

        Port_WriteBit(23,PORT_OFF);
        AbsDelay(Delay2);

        //Msg_WriteFloat(Faktor);
        //Msg_WriteChar(10);
        //Msg_WriteChar(13);

    }
}



float Sinus(float x) //Bogenmass! Achtung nur genau von -Pi bis Pi
{
    f1=x*x;
    f2=f1*x;
    f3=f2*f2*f2;
    f4=f3*f1;

    return(x-f2/6+(f2*f1)/120-(f3/f1)/5040+f3/362880-f4/39916800+(f4*f1)/6227020800);
    //return(x-f2/6+f2*f1/120)
}

float Wurzel(float W, float D)
{
    //do {
    int i;
    for(i=0;i<7;i++)
    {
        D=(D+W/D)/2;
    }

    //} while((D*D-W)*(D*D-W)<0.00001);
    return D;
}
