// Programm last abwechselnd die beiden LEDs auf dem
// Application Board im Sekunden Rhythmus blinken
// C-Control Pro Mega

void main(void)

{
    Port_DataDirBit(23,PORT_OUT);
    ADC_Set(ADC_VREF_VCC,0);

    float W;
    float D;

    while(true)  // Endlosschleife
    {

        W=ADC_Read()+1;
        D=0.8;

        do {
            D=(D+W/D)/2;
        } while((D*D-W)*(D*D-W)<0.00001);

        Port_WriteBit(23,PORT_ON);
        AbsDelay(2*(D-1));

        Port_WriteBit(23,PORT_OFF);
        AbsDelay(2*(D-1));
    }






}