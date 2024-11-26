#include "stdio.h"
#include "math.h"

static int i=-1;

static double Theta1=0;
static double Theta2=0;
static double Theta3=0;
static double Theta4=0;
static double Theta5=0;
static double Theta6=0;

static double Buffer_Theta1=0;
static double Buffer_Theta2=0;
static double Buffer_Theta3=0;
static double Buffer_Theta4=0;
static double Buffer_Theta5=0;
static double Buffer_Theta6=0;

static double Buffer_Theta1_PWM=0;
static double Buffer_Theta2_PWM=0;
static double Buffer_Theta3_PWM=0;
static double Buffer_Theta4_PWM=0;
static double Buffer_Theta5_PWM=0;
static double Buffer_Theta6_PWM=0;

static unsigned int Theta1_PWM_Value=0x00;
static unsigned int Theta1_PWM_Value_HIGH= 0x00;
static unsigned int Theta1_PWM_Value_LOW= 0x00;
static unsigned int Theta2_PWM_Value=0x00;
static unsigned int Theta2_PWM_Value_HIGH=0x00;
static unsigned int Theta2_PWM_Value_LOW=0x00;
static unsigned int Theta3_PWM_Value=0x00;
static unsigned int Theta3_PWM_Value_HIGH=0x00;
static unsigned int Theta3_PWM_Value_LOW=0x00;
static unsigned int Theta4_PWM_Value=0x00;
static unsigned int Theta4_PWM_Value_HIGH=0x00;
static unsigned int Theta4_PWM_Value_LOW=0x00;
static unsigned int Theta5_PWM_Value=0x00;
static unsigned int Theta5_PWM_Value_HIGH=0x00;
static unsigned int Theta5_PWM_Value_LOW=0x00;
static unsigned int Theta6_PWM_Value=0x00;
static unsigned int Theta6_PWM_Value_HIGH=0x00;
static unsigned int Theta6_PWM_Value_LOW=0x00;

static double L1=10.5;
static double L2=10.5;
static double L5=14.5;
static double L6=16;

static double px=23.59326673973660579; //17.67766952966368811
static double py=0;
static double pz=15.75;

static double Alpha=0;
static double Gamma=0;
static double Beta=0;
static double r=0;
static double l3=0;
static double pi=3.141593;

static double Servo_Degree_Decimal[206]={205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,
					 230,231,232,233,234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,
					 255,256,257,258,259,260,261,262,263,264,265,266,267,268,269,270,271,272,273,274,2755,276,277,278,279,
					 280,281,282,283,284,285,286,287,288,289,290,291,292,293,294,295,296,297,298,299,300,301,302,303,304,
				 	 305,306,307,308,309,310,311,312,313,314,315,316,317,318,319,320,321,322,323,324,325,326,327,328,329,
					 330,331,332,333,334,335,336,337,338,339,340,341,342,343,344,345,346,347,348,349,350,351,352,353,354,
					 355,356,357,358,359,360,361,362,363,364,365,366,367,368,369,370,371,372,373,374,375,376,377,378,379,
					 380,381,382,383,384,385,386,387,388,389,390,391,392,393,394,395,396,397,398,399,400,401,402,403,404,
					 405,406,407,408,409,410};

static unsigned int Servo_Degree_Hexadecimal[206]={0xCC,0xCD,0xCE,0xCF,0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF,0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,
						   0xE6,0xE7,0xE8,0xE9,0xEA,0xEB,0xEC,0xED,0xEE,0xEF,0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFB,0xFC,0xFD,0xFE,
						   0xFF,0x100,0x101,0x102,0x103,0x104,0x105,0x106,0x107,0x108,0x109,0x10A,0x10B,0x10C,0x10D,0x10E,0x10F,0x110,0x111,0x112,0x113,
						   0x114,0x115,0x116,0x117,0x118,0x119,0x11A,0x11B,0x11C,0x11D,0x11E,0x11F,0x120,0x121,0x122,0x123,0x124,0x125,0x126,0x127,0x128,
						   0x129,0x12A,0x12B,0x12C,0x12D,0x12E,0x12F,0x130,0x131,0x132,0x133,0x134,0x135,0x136,0x137,0x138,0x139,0x13A,0x13B,0x13C,0x13D,
						   0x13E,0x13F,0x140,0x141,0x142,0x143,0x144,0x145,0x146,0x147,0x148,0x149,0x14A,0x14B,0x14C,0x14D,0x14E,0x14F,0x150,0x151,0x152,
						   0x153,0x154,0x155,0x156,0x157,0x158,0x159,0x15A,0x15B,0x15C,0x15D,0x15E,0x15F,0x160,0x161,0x162,0x163,0x164,0x165,0x166,0x167,
						   0x168,0x169,0x16A,0x16B,0x16C,0x16D,0x16E,0x16F,0x170,0x171,0x172,0x173,0x174,0x175,0x176,0x177,0x178,0x179,0x17A,0x17B,0x17C,
						   0x17D,0x17E,0x17F,0x180,0x181,0x182,0x183,0x184,0x185,0x186,0x187,0x188,0x189,0x18A,0x18B,0x18C,0x18D,0x18E,0x18F,0x190,0x191,
						   0x192,0x193,0x194,0x195,0x196,0x197,0x198,0x199};

static double Servo_Degree[121]={-60,-59,-58,-57,-56,-55,-54,-53,-52,-51,-50,-49,-48,-47,-46,-45,-44,-43,-42,-41,-40,-39,
				 -38,-37,-36,-35,-34,-33,-32,-31,-30,-29,-28,-27,-26,-25,-24,-23,-22,-21,-20,-19,-18,-17,
				 -16,-15,-14,-13,-12,-11,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,7,8,9,10,11,
				  12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,
				  40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60};

static double PWM_Value[121]= {1.000, 1.008333, 1.016666, 1.025, 1.033333, 1.041666, 1.05, 1.058333, 1.066666, 1.075, 1.083333, 1.091666, 1.1, 1.108333,
                               1.116666, 1.125, 1.133333, 1.141666, 1.15, 1.158333, 1.166666, 1.175, 1.183333, 1.191666, 1.2, 1.208333, 1.216666, 1.225,
                               1.233333, 1.241666, 1.25, 1.258333, 1.266666, 1.275, 1.283333, 1.291666, 1.3, 1.308333, 1.316666, 1.325, 1.333333, 1.341666,
                               1.35, 1.358333, 1.366666, 1.375, 1.383333, 1.391666, 1.4, 1.408333, 1.416666, 1.425, 1.433333, 1.441666, 1.45, 1.458333, 1.466666,
                               1.475, 1.483333, 1.491666, 1.5, 1.508333, 1.516666, 1.525, 1.533333, 1.541666, 1.55, 1.558333, 1.566666, 1.575, 1.583333, 1.591666,
                               1.6, 1.608333, 1.616666, 1.625, 1.633333, 1.641666, 1.65, 1.658333, 1.666666, 1.675, 1.683333, 1.691666, 1.7, 1.708333, 1.716666,
                               1.725, 1.733333, 1.741666, 1.75, 1.758333, 1.766666, 1.775, 1.783333, 1.791666, 1.8, 1.808333, 1.816666, 1.825, 1.833333, 1.841666,
                               1.85, 1.858333, 1.866666, 1.875, 1.883333, 1.891666, 1.9, 1.908333, 1.916666, 1.925, 1.933333, 1.941666, 1.95, 1.958333, 1.966666,
                               1.975, 1.983333, 1.991666, 2.000};


int main(){

        r=sqrt(pow((px),2)+pow(py,2));
		Alpha=atan2((pz-L1),r);
		l3=sqrt(pow((pz-L1),2)+pow(r,2));
		Gamma=acos((pow(l3,2)+pow(L2,2)-pow(L5,2))/(2*L2*l3));
		Beta=acos((pow(L2,2)+pow(L5,2)-pow(l3,2))/(2*L2*L5));
		Theta1=atan2(py,px);
		Theta2=-(Alpha+Gamma);
		Theta3=pi-Beta;
		Theta5= atan2(sqrt(round(cos(Theta2 + Theta3))), round((sin(Theta2 + Theta3))));
		Theta4=atan2(0,cos(Theta2 + Theta3)/sin(Theta5));
		Theta6=atan2((-sin(Theta1)*cos(Theta2 + Theta3))/sin(Theta5),-(-cos(Theta1)*cos(Theta2 + Theta3))/sin(Theta5));

        printf("r=%f\n",r);
		printf("Alpha=%f\n",Alpha);
        printf("l3=%f\n",l3);
        printf("Gamma=%f\n",Gamma);
        printf("Beta=%f\n",Beta);
        printf("Theta1=%f\n",Theta1);
        printf("Theta2=%f\n",Theta2);
        printf("Theta3=%f\n",Theta3);
        printf("Theta5=%f\n",Theta5);
        printf("Theta4=%f\n",Theta4);
        printf("Theta6=%f\n",Theta6);

        do{
			i++;
			Buffer_Theta1=Servo_Degree[i];
            Buffer_Theta1_PWM=PWM_Value[i];
		}while(!(round(Theta1*180/pi)==Servo_Degree[i]));
        i=0;

         do{
			i++;
            Theta1_PWM_Value&=~ 0x1FF;
			Theta1_PWM_Value |= Servo_Degree_Hexadecimal[i];
		}while(!(round((Buffer_Theta1_PWM/20)*4096)==Servo_Degree_Decimal[i]));
        i=0;

        Theta1_PWM_Value_HIGH |= Theta1_PWM_Value & 0x100 ;
        Theta1_PWM_Value_LOW |= Theta1_PWM_Value & 0xFF;

        do{
			i++;
			Buffer_Theta2=Servo_Degree[i];
            Buffer_Theta2_PWM=PWM_Value[i];
		}while(!(round(((Theta2)*180/pi))==Servo_Degree[i]));

        do{
			i++;
            Theta2_PWM_Value&=~ 0x1FF;
			Theta2_PWM_Value |= Servo_Degree_Hexadecimal[i];
		}while(!(round((Buffer_Theta1_PWM/20)*4096)==Servo_Degree_Decimal[i]));
        i=0;

        Theta2_PWM_Value_HIGH |= Theta2_PWM_Value & 0x100 ;
        Theta2_PWM_Value_LOW |= Theta2_PWM_Value & 0xFF;

        printf("Buffer_Theta1=%f\n",Buffer_Theta1);
        printf("Buffer_Theta1_PWM=%f\n",Buffer_Theta1_PWM);
        printf("Theta1_PwM_Value=%x\n", Theta1_PWM_Value);
        printf("Theta1_PwM_Value_HIGH=%x\n",Theta1_PWM_Value_HIGH>>8);
        printf("Theta1_PwM_Value_LOW=%x\n",Theta1_PWM_Value_LOW);
        printf("Theta2_PwM_Value_HIGH=%x\n",Theta2_PWM_Value_HIGH>>8);
        printf("Theta2_PwM_Value_LOW=%x\n",Theta2_PWM_Value_LOW);

        printf("Buffer_Theta2=%f\n",Buffer_Theta2);
        printf("Buffer_Theta2_PWM=%f\n",Buffer_Theta2_PWM);
/*
        do{
			i++;
			Buffer_Theta2=Servo_Degree[i];
            Buffer_Theta2_PWM=PWM_Value[i];
		}while(!(round(Theta2*180/pi)==Servo_Degree[i]));

        do{
			i++;
			Buffer_Theta3=Servo_Degree[i];
            Buffer_Theta3_PWM=PWM_Value[i];
		}while(!(round(Theta3*180/pi)==Servo_Degree[i]));

        do{
			i++;
			Buffer_Theta4=Servo_Degree[i];
            Buffer_Theta4_PWM=PWM_Value[i];
		}while(!(round(Theta4*180/pi)==Servo_Degree[i]));

        do{
			i++;
			Buffer_Theta5=Servo_Degree[i];
            Buffer_Theta5_PWM=PWM_Value[i];
		}while(!(round(Theta5*180/pi)==Servo_Degree[i]));
        do{
			i++;
			Buffer_Theta6=Servo_Degree[i];
            Buffer_Theta6_PWM=PWM_Value[i];
		}while(!(round(Theta6*180/pi)==Servo_Degree[i]));

        printf("Buffer_Theta1=%f",Buffer_Theta1);
        printf("Buffer_Theta1_PWM=%f",Buffer_Theta1_PWM);
        printf("Buffer_Theta2=%f",Buffer_Theta2);
        printf("Buffer_Theta2_PWM=%f",Buffer_Theta2_PWM);
        printf("Buffer_Theta3=%f",Buffer_Theta3);
        printf("Buffer_Theta3_PWM=%f",Buffer_Theta3_PWM);
        printf("Buffer_Theta4=%f",Buffer_Theta4);
        printf("Buffer_Theta4_PWM=%f",Buffer_Theta4_PWM);
        printf("Buffer_Theta5=%f",Buffer_Theta5);
        printf("Buffer_Theta5_PWM=%f",Buffer_Theta5_PWM);
        printf("Buffer_Theta6=%f",Buffer_Theta6);
        printf("Buffer_Theta6_PWM=%f",Buffer_Theta6_PWM);
*/
}
