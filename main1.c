#include<reg51.h>
#define lcd_data P2
//adc
sbit read = P0^3;
sbit write = P0^4;
sbit intr = P0^5;
unsigned char adc();
unsigned char getvalue,conv;
int i,temp;
// lcd 
sbit rs = P0^0;
sbit rw = P0^1;
sbit en = P0^2;
//led
sbit red = P3^2;
sbit blue = P3^6;
sbit green = P3^7;
//switch
sbit tempdis = P3^3;
sbit adcvalue = P3^4;
sbit maxmin = P3^5;
//buz
sbit buz = P0^7;

//temp
short int maxtemp=0;
short int mintemp =100;
short int lasttemp = -1;
unsigned char temphistory[24];
short int index = 0;
short int count =0;
//mode
unsigned char mode = 0;
unsigned char lastmode = 0;
unsigned char firsttime;
//func
void Data(unsigned char a);
void cmd(unsigned char b);
void lcd_init();
void show(unsigned char *s);
void delayms(short int ms);
void indi(short int temp);
void uartinit();
void uartsend(unsigned char c);
void sendstr(unsigned char *se);
void sendpc();
void storetemp();
void sendhistory();


void main()
{
	lcd_init();
	uartinit();
	red = 0;
	green = 0;
	blue = 0;
	buz = 0;
	adc();
	delayms(10);
	getvalue = adc();      
	temp = (getvalue*100)/255;
	lasttemp = temp;      
	firsttime = 1; 
	while(1)
	{
		getvalue = adc();
		temp = (getvalue*100)/255;
		indi(temp);
		if (temp > maxtemp)
		{
    maxtemp = temp;
		}
		if (temp < mintemp)
		{
			mintemp = temp;
		}
		if(tempdis == 0 && adcvalue ==1 && maxmin ==1)
		{
			mode =1;
		}
		else if(adcvalue == 0 && tempdis == 1&&maxmin ==1)
		{
			mode = 2;
		}
    else if(adcvalue == 1 && tempdis == 1&&maxmin ==0)
    {  
		mode = 3;
		}
		else 
		{
			mode =0;
		}
		if(mode != lastmode)
    {
      cmd(0x01); 
      lastmode = mode;
    }
		if(mode ==1)
		{
			
			cmd(0x80);
			show("Temp:");
			cmd(0x87);
			Data((temp/100)+48);
			Data(((temp/10)%10)+48);
			Data((temp%10)+48);
			Data(0xDF);
			Data('C');
			delayms(1);
		}
		else if (mode ==2)
		{
			cmd(0x80);
			show("RAW ADC VALUE :");
			cmd(0xC0);
			Data((getvalue/100)+48);  
      Data(((getvalue/10)%10)+48);  
      Data((getvalue%10)+48);  
      delayms(1);
        }
		else if (mode == 3)
	{
    cmd(0x80);
    show("MAX: ");
    Data((maxtemp/100)+48);
    Data(((maxtemp/10)%10)+48);
    Data((maxtemp%10)+48);
    Data(0xDF);
    Data('C');

    cmd(0xC0);
    show("MIN: ");
    Data((mintemp/100)+48);
    Data(((mintemp/10)%10)+48);
    Data((mintemp%10)+48);
    Data(0xDF);
    Data('C');
	}
		else
		{
			cmd(0x80);
			show("no button pressed");
			
		}
		if (temp>lasttemp+0.5 || temp<lasttemp-0.5 ){
			sendpc();
			storetemp();
			lasttemp = temp;
		}
		delayms(10);
		if (mode == 3)
		{
			sendhistory();
			while(1)
			{
				if(!(adcvalue == 1 && tempdis == 1 && maxmin == 0)) {
				break;
			}
		}
			
		}
	}
}

//lcd

void lcd_init()
{
		cmd(0x38);
    cmd(0x0e);
    cmd(0x06);
    cmd(0x01);
    cmd(0x80);
}

void Data(unsigned char a)
{
	lcd_data = a;
	rs = 1;
	rw = 0;
	en =1;
	delayms(5);
	en =0;
}
void cmd(unsigned char b)
{
	lcd_data = b;
	rs = 0;
	rw = 0;
	en =1;
	delayms(5);
	en =0;
}

void show(unsigned char *s)
{
	while(*s)
	{
		Data(*s++);
	}
}

void delayms(ms)
{
		int i, j;
    for(i=0;i<ms; i++){
        for(j=0; j<1275; j++);
			{
			}
		}	
}

//adc 
unsigned char adc()
{
	write =0;
	delayms(1);
	write = 1;
	while(intr == 1);
	read = 1;
	delayms(1);
	read =0;
	delayms(1);
	conv = P1;
	read =1 ;
	
	return conv;
}
	
//led

void indi(temp)
{
	
		if (temp >=35)
		{
			cmd(0xC0);
			if(mode ==1)
			{
				show("HIGH TEMPERATURE");
			}
			red = 1;
			green = 0;
			blue = 0;
			buz = 1;
		}
		else if(temp>=25 && temp<35)
			{
			cmd(0xC0);
				if(mode == 1)
				{
					show("ROOM TEMPERATURE");
				}
			green= 1;
			red = 0;
			blue = 0;
			buz = 0;
			}
		else if (temp<25)
		{
			cmd(0xC0);
			if(mode ==1)
			{
				show("LOW TEMPERATURE");
			}
			blue  = 1;
			red = 0;
			green = 0;
			buz = 0;
		}
}
	
void uartinit() {
    TMOD = 0x20;  
    TH1 = TL1 = 0xFD;  
    TR1 = 1;      
    SCON = 0x50;  
}

void uartsend(char c) {
    SBUF = c;
    while(TI==0);
    TI = 0;
}

void sendstr(char *se) {
    while(*se) {uartsend(*se++);}
}

void sendpc()
{
	if (firsttime)
	{
		maxtemp = mintemp = temp;
		sendstr("System Started\r\n");
		firsttime =0;
	}
	sendstr("current temperature: ");
	uartsend((temp/100)+48);
  uartsend(((temp/10)%10)+48);
  uartsend((temp%10)+48);
  sendstr("°C\r\n");
	sendstr("max temperature: ");
  uartsend((maxtemp/100)+48);
  uartsend(((maxtemp/10)%10)+48);
	uartsend((maxtemp%10)+48);
	sendstr("°C\r\n");
  sendstr("min temperature: ");
  uartsend((mintemp/100)+48);
  uartsend(((mintemp/10)%10)+48);
  uartsend((mintemp%10)+48);
  sendstr("°C\r\n"); 
}


void storetemp()
{
	temphistory[index++] = temp;
	if(index>=24)
	{
		index =0;
	}
	if (count <24)
	{
		count++;
	}
}

void sendhistory() {
    short int i;
    sendstr("Temperature History:\r\n");
    for (i = 0; i < count ; i++) {
        uartsend((temphistory[i] / 100) + 48);
        uartsend(((temphistory[i] / 10) % 10) + 48);
        uartsend((temphistory[i] % 10) + 48);
        sendstr("°C  ");
    }
		sendstr("\r\n");
}