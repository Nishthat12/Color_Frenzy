/* This files provides address values that exist in the system */

#define BOARD                 "DE1-SoC"

/* Memory */
#define DDR_BASE              0x00000000
#define DDR_END               0x3FFFFFFF
#define A9_ONCHIP_BASE        0xFFFF0000
#define A9_ONCHIP_END         0xFFFFFFFF
#define SDRAM_BASE            0xC0000000
#define SDRAM_END             0xC3FFFFFF
#define FPGA_ONCHIP_BASE      0xC8000000
#define FPGA_ONCHIP_END       0xC803FFFF
#define FPGA_CHAR_BASE        0xC9000000
#define FPGA_CHAR_END         0xC9001FFF

/* Cyclone V FPGA devices */
#define LEDR_BASE             0xFF200000
#define HEX3_HEX0_BASE        0xFF200020
#define HEX5_HEX4_BASE        0xFF200030
#define SW_BASE               0xFF200040
#define KEY_BASE              0xFF200050
#define JP1_BASE              0xFF200060
#define JP2_BASE              0xFF200070
#define PS2_BASE              0xFF200100
#define PS2_DUAL_BASE         0xFF200108
#define JTAG_UART_BASE        0xFF201000
#define JTAG_UART_2_BASE      0xFF201008
#define IrDA_BASE             0xFF201020
#define TIMER_BASE            0xFF202000
#define AV_CONFIG_BASE        0xFF203000
#define PIXEL_BUF_CTRL_BASE   0xFF203020
#define CHAR_BUF_CTRL_BASE    0xFF203030
#define AUDIO_BASE            0xFF203040
#define VIDEO_IN_BASE         0xFF203060
#define ADC_BASE              0xFF204000

/* Cyclone V HPS devices */
#define HPS_GPIO1_BASE        0xFF709000
#define HPS_TIMER0_BASE       0xFFC08000
#define HPS_TIMER1_BASE       0xFFC09000
#define HPS_TIMER2_BASE       0xFFD00000
#define HPS_TIMER3_BASE       0xFFD01000
#define FPGA_BRIDGE           0xFFD0501C

/* ARM A9 MPCORE devices */
#define   PERIPH_BASE         0xFFFEC000    // base address of peripheral devices
#define   MPCORE_PRIV_TIMER   0xFFFEC600    // PERIPH_BASE + 0x0600

/* Interrupt controller (GIC) CPU interface(s) */
#define MPCORE_GIC_CPUIF      0xFFFEC100    // PERIPH_BASE + 0x100
#define ICCICR                0x00          // offset to CPU interface control reg
#define ICCPMR                0x04          // offset to interrupt priority mask reg
#define ICCIAR                0x0C          // offset to interrupt acknowledge reg
#define ICCEOIR               0x10          // offset to end of interrupt reg
/* Interrupt controller (GIC) distributor interface(s) */
#define MPCORE_GIC_DIST       0xFFFED000    // PERIPH_BASE + 0x1000
#define ICDDCR                0x00          // offset to distributor control reg
#define ICDISER               0x100         // offset to interrupt set-enable regs
#define ICDICER               0x180         // offset to interrupt clear-enable regs
#define ICDIPTR               0x800         // offset to interrupt processor targets regs
#define ICDICFR               0xC00         // offset to interrupt configuration regs


#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


// 0xF81F is Purple
// 0X0400 is Yellow
// 0X001F is Blue
// 0XF800 is Dark Green
// 0X07E0 is Light Green
// 0XF800 is Red
// 0XFFFF is White

char seg7[10] =	{0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110, 
                        0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01100111};

int score=0;
int high_score = 0;
int lives = 3;
int ball_count = 0;


void draw_line(int x0,int y0, int x1, int y1, short int color);

void draw_triangle1(short int color);
void draw_triangle2(short int color);
void draw_triangle3(short int color);
void draw_triangle4(short int color);

void draw_square(int short Top,int short Right, int short Bottom, int short Left);
void clear_square();

void right_shift_triangle_colors(short int *arr);
void left_shift_triangle_colors(short int *arr);

void circleBres(int xc, int yc, int r,short int color);
void drawCircle(int xc, int yc, int x, int y,short int color);
void draw_filledCircle(int xc, int yc, int r, int short color);
void erase_filledCircle(int xc, int yc, int r);

void plot_box(int x_box, int y_box, int short color);
void draw_score_digit(int x, int y, int num, int short color);

void erase_old_score();
void draw_score(int short color);

void erase_old_high_score();
void draw_high_score(int short color);

void erase_old_lives();
void draw_lives(int short color);

void erase_pause();
void draw_pause(int short color);

void gameover(int score);

void swap(int *x,int *y);
void toString(char str[], int num);
void write_pixel(int x, int y, short int line_color);
void write_char(int x, int y, char c);
void clear_screen();


char get_jtag(volatile int *JTAG_UART_ptr){
    int data;
    data = *(JTAG_UART_ptr);
    if (data & 0x00008000) // check RVALID
        return ((char)data & 0xFF);
    else
        return ('\0');
}


int game(){

    short int colors[4]={0xE81F,0xFEA0,0xFA06,0x6CE0}; //getting the Pink, Yellow, Red and Green
    short int triangle_colors[4]={0xE81F,0xFEA0,0xFA06,0x6CE0};

    int dp[10]={8,8,8,10,12,14,16,13,15,10};
    int dp_random=dp[rand()%10];
    dp_random = dp_random + ball_count;

    int random_fast = rand()%5;
    if(random_fast == 0){
        dp_random = dp_random*1.5;
    }
    int dp_total=0;


    while(1){

        if(lives == 0){
            return 0;
        }

        int short ball_color = colors[rand()%4];
        int side = 0;
        side = rand()%4;        //0 is Top, 1 is Right, 2 is Bottom, 3 is Left

        if(side == 0){
            while(dp_total < 50){      //Top triangle is at y=60, hence we check till 50. (Radius of Ball is 10) --> Min is 0
                while(1){
                    if(dp_total > 0){
                        int i1;
                        for(i1 =0; i1<10000000; i1++){}                                    
                        erase_filledCircle(160,10+dp_total-dp_random,10);
                    }

                    draw_score(0xf800);
                    draw_high_score(0xf800);
                    draw_lives(0xf800);

                    volatile int *JTAG_UART_ptr = (int *)0xFF201000;
                    char c = get_jtag(JTAG_UART_ptr);
                    int i2;
                    for(i2 =0; i2<10000000; i2++){}

                    if(c == 0x64){
                        right_shift_triangle_colors(triangle_colors);
                        clear_square();                                
                    }

                    if(c == 0x61){
                        left_shift_triangle_colors(triangle_colors);
                        clear_square();                                   
                    } 

                    if(c == 0x1B){
                        draw_pause(0xf800);
                        while(1){
                            volatile int *JTAG_UART_ptr = (int *)0xFF201000;
                            char c2 = get_jtag(JTAG_UART_ptr);
                            if(c2 == 0X0A){
                                erase_pause();
                                break;
                            }
                            if(c2 == 0X1B){
                                return 0;
                            }
                        }
                    }

                    draw_square(triangle_colors[0],triangle_colors[1],triangle_colors[2],triangle_colors[3]);
                    draw_filledCircle(160,10+dp_total,10,ball_color);
                    dp_total=dp_total+dp_random;
                    break;                                                        
                }
            }
            erase_filledCircle(160,10+dp_total-dp_random,10);
        }


        else if(side == 1){
            while(dp_total < 90){      //Right triangle is at x=220, hence we check till 90. (Radius of Ball is 10) --> Max is 320
                while(1){
                    if(dp_total > 0){
                        int i1;
                        for(i1 =0; i1<10000000; i1++){}                                    
                        erase_filledCircle(310-dp_total+dp_random,120,10);
                    }

                    draw_score(0xf800);
                    draw_high_score(0xf800);
                    draw_lives(0xf800);

                    volatile int *JTAG_UART_ptr = (int *)0xFF201000;
                    char c = get_jtag(JTAG_UART_ptr);
                    int i2;
                    for(i2 =0; i2<10000000; i2++){}

                    if(c == 0x64){
                        right_shift_triangle_colors(triangle_colors);
                        clear_square();                                
                    }

                    if(c == 0x61){
                        left_shift_triangle_colors(triangle_colors);
                        clear_square();                                   
                    }

                    if(c == 0x1B){
                        draw_pause(0xf800);
                        while(1){
                            volatile int *JTAG_UART_ptr = (int *)0xFF201000;
                            char c2 = get_jtag(JTAG_UART_ptr);
                            
                            if(c2 == 0X0A){
                                erase_pause();
                                break;
                            }
                            if(c2 == 0X1B){
                                return 0;
                            }
                        }

                    }

                    draw_square(triangle_colors[0],triangle_colors[1],triangle_colors[2],triangle_colors[3]);
                    draw_filledCircle(310-dp_total,120,10,ball_color);
                    dp_total=dp_total+dp_random;
                    break;                                                        
                }
            }
            erase_filledCircle(310-dp_total+dp_random,120,10);
        }

        else if(side == 2){
            while(dp_total < 50){      //Bottom triangle is at y=180, hence we check till 50. (Radius of Ball is 10) --> Max is 240
                while(1){
                    if(dp_total > 0){
                        int i1;
                        for(i1 =0; i1<10000000; i1++){}                                    
                        erase_filledCircle(160,230-dp_total+dp_random,10);
                    }

                    draw_score(0xf800);
                    draw_high_score(0xf800);
                    draw_lives(0xf800);

                    volatile int *JTAG_UART_ptr = (int *)0xFF201000;
                    char c = get_jtag(JTAG_UART_ptr);
                    int i2;
                    for(i2 =0; i2<10000000; i2++){}

                    if(c == 0x64){
                        right_shift_triangle_colors(triangle_colors);
                        clear_square();                                
                    }

                    if(c == 0x61){
                        left_shift_triangle_colors(triangle_colors);
                        clear_square();                                   
                    } 

                    if(c == 0x1B){
                        draw_pause(0xf800);
                        while(1){
                            volatile int *JTAG_UART_ptr = (int *)0xFF201000;
                            char c2 = get_jtag(JTAG_UART_ptr);

                            if(c2 == 0X0A){
                                erase_pause();
                                break;
                            }
                            if(c2 == 0X1B){
                                return 0;
                            }
                        }
                    }

                    draw_square(triangle_colors[0],triangle_colors[1],triangle_colors[2],triangle_colors[3]);
                    draw_filledCircle(160,230-dp_total,10,ball_color);
                    dp_total=dp_total+dp_random;
                    break;                                                        
                }
            }
            erase_filledCircle(160,230-dp_total+dp_random,10);
        } 


        else if(side == 3){
            while(dp_total < 90){      //Left triangle is at x=100, hence we check till 90. (Radius of Ball is 10) --> Min is 0

                while(1){
                    if(dp_total > 0){
                        int i1;
                        for(i1 =0; i1<10000000; i1++){}                                    
                        erase_filledCircle(10+dp_total-dp_random,120,10);
                    }

                    draw_score(0xf800);
                    draw_high_score(0xf800);
                    draw_lives(0xf800);

                    volatile int *JTAG_UART_ptr = (int *)0xFF201000;
                    char c = get_jtag(JTAG_UART_ptr);
                    int i2;
                    for(i2 =0; i2<10000000; i2++){}

                    if(c == 0x64){
                        right_shift_triangle_colors(triangle_colors);
                        clear_square();                                
                    }

                    if(c == 0x61){
                        left_shift_triangle_colors(triangle_colors);
                        clear_square();                                   
                    }

                    if(c == 0x1B){
                        draw_pause(0xf800);
                        while(1){
                            volatile int *JTAG_UART_ptr = (int *)0xFF201000;
                            char c2 = get_jtag(JTAG_UART_ptr);

                            if(c2 == 0X0A){
                                erase_pause();
                                break;
                            }
                            if(c2 == 0X1B){
                                return 0;
                            }
                        }
                    }

                    draw_square(triangle_colors[0],triangle_colors[1],triangle_colors[2],triangle_colors[3]);
                    draw_filledCircle(10+dp_total,120,10,ball_color);
                    dp_total=dp_total+dp_random;
                    break;                                                        
                }
            }
            erase_filledCircle(10+dp_total-dp_random,120,10);
        }

        erase_old_score();
        erase_old_high_score();
        erase_old_lives();


        if((side==0)&&(triangle_colors[0]==ball_color)){
            if(random_fast == 0){
                score = score+3;
            }
            else{
                score++;
            }
        }
        else if((side==1)&&(triangle_colors[1]==ball_color)){
            if(random_fast == 0){
                score = score+3;
            }
            else{
                score++;
            }
        }
        else if ((side==2)&&(triangle_colors[2]==ball_color)){
            if(random_fast == 0){
                score = score+3;
            }
            else{
                score++;
            }
        }
        else if ((side==3)&&(triangle_colors[3]==ball_color)){
            if(random_fast == 0){
                score = score+3;
            }
            else{
                score++;
            }
        }
        else{
            lives = lives-1;
        }

        if(score>high_score){
            high_score = score;
        }

        if (score == 1000){
            score=0;
        }



        ball_count = ball_count + 1;

        int  temp;
        volatile int * HEX3_HEX0_ptr = (int *) HEX3_HEX0_BASE;
        volatile int * HEX5_HEX4_ptr = (int *) HEX5_HEX4_BASE;

        *HEX3_HEX0_ptr = 0;								// clear the display
        *HEX5_HEX4_ptr = 0;								// clear the display

        temp = score;

        temp = temp % 100;
        int sec = score / 100;
        *HEX5_HEX4_ptr = seg7[lives] << 8;
        *HEX3_HEX0_ptr |= seg7[sec] << 16;

        int temp2 = temp % 10;
        *HEX3_HEX0_ptr |= seg7[temp / 10] << 8;
        *HEX3_HEX0_ptr |= seg7[temp2];

        dp_random=dp[rand()%10];
        dp_random = dp_random + ball_count;
        int random_fast = rand()%5;
        if(random_fast == 0){
            dp_random = dp_random*1.5;
        }
        dp_total=0;

    }
}



int main(){

    clear_screen();
    while(1){
		clear_screen();
        int x, y;
        for (y = 0; y < 240; ++y){
            for (x = 0; x < 320; ++x){
                write_pixel(x, y, 0X18b4);
            }
        }
        
	   char* c1 = "COLOR FRENZY!";
	   int xc1 = 35;
	   while (*c1) {
		 write_char(xc1, 20, *c1);
		 xc1++;
		 c1++;
	   }
	   
	   char* c2 = "PRESS ENTER TO PLAY";
	   int xc2 = 30;
	   while (*c2) {
		 write_char(xc2, 40, *c2);
		 xc2++;
		 c2++;
	   }

	   char* c3 = "Made by: Deepanker & Nishtha";
	   int xc3 = 50;
	   while (*c3) {
		 write_char(xc3, 50, *c3);
		 xc3++;
		 c3++;
	   }

        score = 0;
        lives = 3;
        ball_count = 0;

        volatile int * HEX3_HEX0_ptr = (int *) HEX3_HEX0_BASE;
        volatile int * HEX5_HEX4_ptr = (int *) HEX5_HEX4_BASE;
        *HEX3_HEX0_ptr = 0;								// clear the display
        *HEX5_HEX4_ptr = 0;								// clear the display

        while(1){
            volatile int *JTAG_UART_ptr = (int *)0xFF201000;
            char c = get_jtag(JTAG_UART_ptr);
            int i;
            for(i =0; i<10000000; i++){}

            if(c == 0x0a){
                clear_screen();

                if(!game()){
					if(lives == 0){
						clear_screen();
						gameover(score);
						while(1){
							volatile int *JTAG_UART_ptr = (int *)0xFF201000;
							char c = get_jtag(JTAG_UART_ptr);
							int io;
							for(io =0; io<10000000; io++){}
							
							if(c == 0X1B){
								break;
							}
						}
					}	
                    break;
                }
            }
        }
    }


}






void draw_line(int x0,int y0, int x1, int y1, short int color){
    bool is_steep = abs(y1-y0) > abs(x1-x0); //check if magnitude greater than one of slope
    
    //if magnitude of slope greater than 1, switch x and y so we can iterrate through y
    if(is_steep){
        swap(&x0,&y0);
        swap(&x1,&y1);
    }
    
    //make it easier to draw (make it so the direction of the slope doesnt matter)
    if(x0>x1){
        swap(&x0,&x1);
        swap(&y0,&y1);
    }
    
    //process to tell when to iterate y
    int deltax=x1-x0;
    int deltay=abs(y1-y0);
    int error=-(deltax/2);
    int y=y0; //start iteration from the first y value;
    
    int y_step=0;
    
    //if line should be going up or down, y_step is different for each
    if(y0<y1){
        y_step=1;
    }
    else{
        y_step=-1;
    }
    //interate through x (which could also represent the y value)
    int x;
    for(x=x0; x<=x1; x++){
        if(is_steep)
            write_pixel(y,x,color);
        else
            write_pixel(x,y,color);
        
        //only increase the y value once we reach error is greater than 0.
        error=error+deltay;
        if(error>=0){
            y=y+y_step;
            error=error-deltax;	
        }
    }
}

//Top Triangle
void draw_triangle1(short int color){
    int i;
    for(i=0; i<60; i++){
        draw_line(100+i,60+i,220-i,60+i,color);
        
    }

}

//Right Triangle
void draw_triangle2(short int color){
    int i;
    for(i=0; i<60; i++){
        draw_line(220-i,60+i,220-i,180-i,color);
        
    }

}

//Bottom Triangle
void draw_triangle3(short int color){
    int i;
    
    for(i=0; i<60; i++){
        draw_line(100+i,180-i,220-i,180-i,color);
        
    }

}

//Left Triangle
void draw_triangle4(short int color){
    int i;
    
    for(i=0; i<60; i++){
        draw_line(100+i,60+i,100+i,180-i,color);
        
    }

}

void draw_square(int short Top,int short Right, int short Bottom, int short Left){

    draw_triangle1(Top);
    draw_triangle2(Right);
    draw_triangle3(Bottom);
    draw_triangle4(Left);
}

void clear_square(){

    draw_triangle1(0X18b4);
    draw_triangle2(0X18b4);
    draw_triangle3(0X18b4);
    draw_triangle4(0X18b4);
}


void right_shift_triangle_colors(short int *arr){
    short int t0=arr[0];
    short int t1=arr[1];
    short int t2=arr[2];
    short int t3=arr[3];
    
    arr[0]=t3;
    arr[1]=t0;
    arr[2]=t1;
    arr[3]=t2;
}

void left_shift_triangle_colors(short int *arr){
    short int t0=arr[0];
    short int t1=arr[1];
    short int t2=arr[2];
    short int t3=arr[3];
    
    arr[0]=t1;
    arr[1]=t2;
    arr[2]=t3;
    arr[3]=t0;
}

//BRESENHAM'S CIRCLE DRAWING ALGORITHM

void circleBres(int xc, int yc, int r,short int color){ 
    int x = 0, y = r; 
    int d = 3 - 2 * r; 
    drawCircle(xc, yc, x, y,color); 
    while (y >= x){ 
        x++;
        if (d > 0){ 
            y--;  
            d = d + 4 * (x - y) + 10; 
        } 
        else
            d = d + 4 * x + 6; 

        drawCircle(xc, yc, x, y, color);   
    } 
}

void drawCircle(int xc, int yc, int x, int y,short int color){ 
    write_pixel(xc+x, yc+y, color); 
    write_pixel(xc-x, yc+y, color); 
    write_pixel(xc+x, yc-y, color); 
    write_pixel(xc-x, yc-y, color); 
    write_pixel(xc+y, yc+x, color); 
    write_pixel(xc-y, yc+x, color); 
    write_pixel(xc+y, yc-x, color); 
    write_pixel(xc-y, yc-x, color); 
}

void draw_filledCircle(int xc, int yc, int r, int short color){
    int i;
    for(i=0;i<=r;i++){
        circleBres(xc, yc, i, color);
    }
}

void erase_filledCircle(int xc, int yc, int r){
    int i;
    for(i=0;i<=r;i++){
        circleBres(xc, yc, i, 0X18b4);        //Our Background
    }
}


//plots a 3x3 box
void plot_box(int x_box, int y_box, int short color){
	for(int x=0; x<3;x++){
		for(int y=0; y<3;y++){
			write_pixel(x_box+x,y_box+y,color);
		}
	}
}

void draw_score_digit(int x, int y, int num, int short color){
	if(num == 1){
		plot_box(x, y+6, color);
		plot_box(x+3, y+3, color);
		plot_box(x+6, y, color);
		plot_box(x+6, y+3, color);
		plot_box(x+6, y+6, color);
		plot_box(x+6, y+9, color);
		plot_box(x+6, y+12, color);
		plot_box(x+6, y+15, color);
		plot_box(x+6, y+18, color);
	}
	else if(num == 2){
		plot_box(x, y+3, color);
		plot_box(x, y+15, color);
		plot_box(x, y+18, color);
		plot_box(x+3, y, color);
		plot_box(x+3, y+12, color);
		plot_box(x+3, y+18, color);
		plot_box(x+6, y, color);
		plot_box(x+6, y+9, color);
		plot_box(x+6, y+18, color);
		plot_box(x+9, y+3, color);
		plot_box(x+9, y+6, color);
		plot_box(x+9, y+18, color);
	}
	else if(num == 3){
		plot_box(x, y+3, color);
		plot_box(x, y+15, color);
		plot_box(x+3, y, color);
		plot_box(x+3, y+9, color);
		plot_box(x+3, y+18, color);
		plot_box(x+6, y, color);
		plot_box(x+6, y+9, color);
		plot_box(x+6, y+18, color);
		plot_box(x+9, y+3, color);
		plot_box(x+9, y+6, color);
		plot_box(x+9, y+12, color);
		plot_box(x+9, y+15, color);
	}
	else if(num == 4){
		plot_box(x, y+9, color);
		plot_box(x, y+12, color);
		plot_box(x+3, y+6, color);
		plot_box(x+3, y+12, color);
		plot_box(x+6, y+3, color);
		plot_box(x+6, y+12, color);
		plot_box(x+9, y, color);
		plot_box(x+9, y+3, color);
		plot_box(x+9, y+6, color);
		plot_box(x+9, y+9, color);
		plot_box(x+9, y+12, color);
		plot_box(x+9, y+15, color);
		plot_box(x+9, y+18, color);
	}
	else if(num == 5){
		plot_box(x, y, color);
		plot_box(x, y+3, color);
		plot_box(x, y+6, color);
		plot_box(x, y+15, color);
		plot_box(x+3, y, color);
		plot_box(x+3, y+6, color);
		plot_box(x+3, y+18, color);
		plot_box(x+6, y, color);
		plot_box(x+6, y+6, color);
		plot_box(x+6, y+18, color);
		plot_box(x+9, y, color);
		plot_box(x+9, y+9, color);
		plot_box(x+9, y+12, color);
		plot_box(x+9, y+15, color);
	}
	else if(num == 6){
		plot_box(x, y+3, color);
		plot_box(x, y+6, color);
		plot_box(x, y+9, color);
		plot_box(x, y+12, color);
		plot_box(x, y+15, color);
		plot_box(x+3, y, color);
		plot_box(x+3, y+9, color);
		plot_box(x+3, y+18, color);
		plot_box(x+6, y, color);
		plot_box(x+6, y+9, color);
		plot_box(x+6, y+18, color);
		plot_box(x+9, y+3, color);
		plot_box(x+9, y+12, color);
		plot_box(x+9, y+15, color);
	}
	else if(num == 7){
		plot_box(x, y, color);
		plot_box(x+3, y, color);
		plot_box(x+3, y+12, color);
		plot_box(x+3, y+15, color);
		plot_box(x+3, y+18, color);
		plot_box(x+6, y, color);
		plot_box(x+6, y+9, color);
		plot_box(x+9, y, color);
		plot_box(x+9, y+3, color);
		plot_box(x+9, y+6, color);	
	}
	else if(num == 8){
		plot_box(x, y+3, color);
		plot_box(x, y+6, color);
		plot_box(x, y+12, color);
		plot_box(x, y+15, color);
		plot_box(x+3, y, color);
		plot_box(x+3, y+9, color);
		plot_box(x+3, y+18, color);
		plot_box(x+6, y, color);
		plot_box(x+6, y+9, color);
		plot_box(x+6, y+18, color);
		plot_box(x+9, y+3, color);
		plot_box(x+9, y+6, color);
		plot_box(x+9, y+12, color);
		plot_box(x+9, y+15, color);
	}
	else if(num == 9){
		plot_box(x, y+3, color);
		plot_box(x, y+6, color);
		plot_box(x, y+15, color);
		plot_box(x+3, y, color);
		plot_box(x+3, y+9, color);
		plot_box(x+3, y+18, color);
		plot_box(x+6, y, color);
		plot_box(x+6, y+9, color);
		plot_box(x+6, y+18, color);
		plot_box(x+9, y+3, color);
		plot_box(x+9, y+6, color);
		plot_box(x+9, y+9, color);
		plot_box(x+9, y+12, color);
		plot_box(x+9, y+15, color);
	}
	else if(num == 0){
		plot_box(x, y+3, color);
		plot_box(x, y+6, color);
		plot_box(x, y+9, color);
		plot_box(x, y+12, color);
		plot_box(x, y+15, color);
		plot_box(x+3, y, color);
		plot_box(x+3, y+18, color);
		plot_box(x+6, y, color);
		plot_box(x+6, y+18, color);
		plot_box(x+9, y+3, color);
		plot_box(x+9, y+6, color);
		plot_box(x+9, y+9, color);
		plot_box(x+9, y+12, color);
		plot_box(x+9, y+15, color);
	}
}


void erase_old_score(){
	draw_score_digit(240, 10, (score%1000 - score%100)/100, 0X18b4);
	draw_score_digit(255, 10, (score%100 - score%10)/10, 0X18b4);
	draw_score_digit(270, 10, score%10, 0X18b4);
}

void draw_score(int short color){
	draw_score_digit(240, 10, (score%1000 - score%100)/100, color);
	draw_score_digit(255, 10, (score%100 - score%10)/10, color);
	draw_score_digit(270, 10, score%10, color);
}

void draw_high_score(int short color){
	draw_score_digit(30, 10, (high_score%1000 - high_score%100)/100, color);
	draw_score_digit(45, 10, (high_score%100 - high_score%10)/10, color);
	draw_score_digit(60, 10, high_score%10, color);
}

void erase_old_high_score(){
	draw_score_digit(30, 10, (high_score%1000 - high_score%100)/100, 0X18b4);
	draw_score_digit(45, 10, (high_score%100 - high_score%10)/10, 0X18b4);
	draw_score_digit(60, 10, high_score%10, 0X18b4);
}

void erase_old_lives(){
	draw_score_digit(30, 210, lives, 0X18b4);
}

void draw_lives(int short color){
	draw_score_digit(30, 210, lives, color);
}

void erase_pause(){
    draw_pause(0X18b4);
}

void draw_pause(int short color){
    int x = 115;
    int y = 20;
    
    //P
    plot_box(x,y,color);
    plot_box(x,y+3,color);
    plot_box(x,y+6,color);
    plot_box(x,y+9,color);
    plot_box(x,y+12,color);
    plot_box(x,y+15,color);

    plot_box(x+3,y,color);
    plot_box(x+6,y,color);
    plot_box(x+9,y,color);

    plot_box(x+9,y+3,color);
    plot_box(x+9,y+6,color);
    plot_box(x+9,y+9,color);

    plot_box(x+6,y+9,color);
    plot_box(x+3,y+9,color);
    plot_box(x,y+9,color);

    //A
    x=x+15;
    plot_box(x,y,color);
    plot_box(x,y+3,color);
    plot_box(x,y+6,color);
    plot_box(x,y+9,color);
    plot_box(x,y+12,color);
    plot_box(x,y+15,color);

    plot_box(x+3,y,color);
    plot_box(x+6,y,color);
    plot_box(x+9,y,color);

    plot_box(x+9,y+3,color);
    plot_box(x+9,y+6,color);
    plot_box(x+9,y+9,color);
    plot_box(x+9,y+12,color);
    plot_box(x+9,y+15,color);

    plot_box(x+6,y+9,color);
    plot_box(x+3,y+9,color);
    plot_box(x,y+9,color);

    //U
    x = x+15;
    plot_box(x,y,color);
    plot_box(x,y+3,color);
    plot_box(x,y+6,color);
    plot_box(x,y+9,color);
    plot_box(x,y+12,color);
    plot_box(x,y+15,color);

    plot_box(x+3,y+15,color);
    plot_box(x+6,y+15,color);
    plot_box(x+9,y+15,color);  

    plot_box(x+9,y,color);
    plot_box(x+9,y+3,color);
    plot_box(x+9,y+6,color);
    plot_box(x+9,y+9,color);
    plot_box(x+9,y+12,color);
    plot_box(x+9,y+15,color);

    //S
    x = x+15;
    plot_box(x,y,color);
    plot_box(x+3,y,color);
    plot_box(x+6,y,color);
    plot_box(x+9,y,color); 

    plot_box(x,y+3,color);
    plot_box(x,y+6,color);

    plot_box(x+3,y+6,color);
    plot_box(x+6,y+6,color);
    plot_box(x+9,y+6,color);

    plot_box(x+9,y+6,color);
    plot_box(x+9,y+9,color);
    plot_box(x+9,y+12,color);
    plot_box(x+9,y+15,color);

    plot_box(x,y+15,color);
    plot_box(x+3,y+15,color);
    plot_box(x+6,y+15,color);

    //E
    x=x+15;
    plot_box(x,y,color);
    plot_box(x+3,y,color);
    plot_box(x+6,y,color);
    plot_box(x+9,y,color); 

    plot_box(x,y,color);
    plot_box(x,y+3,color);
    plot_box(x,y+6,color);
    plot_box(x,y+9,color);
    plot_box(x,y+12,color);
    plot_box(x,y+15,color);

    plot_box(x,y+6,color);
    plot_box(x+3,y+6,color);
    plot_box(x+6,y+6,color);
    plot_box(x+9,y+6,color);

    plot_box(x,y+15,color);
    plot_box(x+3,y+15,color);
    plot_box(x+6,y+15,color);
    plot_box(x+9,y+15,color);

    //D
    x=x+15;
    plot_box(x,y,color);
    plot_box(x,y+3,color);
    plot_box(x,y+6,color);
    plot_box(x,y+9,color);
    plot_box(x,y+12,color);
    plot_box(x,y+15,color);

    plot_box(x,y,color);
    plot_box(x+3,y,color);
    plot_box(x+6,y,color);
    plot_box(x+9,y,color);

    plot_box(x+9,y+3,color);
    plot_box(x+9,y+6,color);
    plot_box(x+9,y+9,color);
    plot_box(x+9,y+12,color);
    plot_box(x+9,y+15,color);

    plot_box(x,y+15,color);
    plot_box(x+3,y+15,color);
    plot_box(x+6,y+15,color);
    plot_box(x+9,y+15,color);

}

void gameover(int score){

	   char* c1 = "GAME OVER!";
	   int xc1 = 35;
	   while (*c1) {
		 write_char(xc1, 20, *c1);
		 xc1++;
		 c1++;
	   }
	   
	   char* c2 = "PRESS ESCAPE TO RETURN TO MAIN MENU";
	   int xc2 = 22;
	   while (*c2) {
		 write_char(xc2, 40, *c2);
		 xc2++;
		 c2++;
	   }
	   
	   char* c3 = "YOUR SCORE: ";
	   int xc3 = 31;
	   while (*c3) {
		 write_char(xc3, 30, *c3);
		 xc3++;
		 c3++;
	   }
		
		char str[10];
		toString(str,score);
		char* c4 = str;
	   int xc4 = 44;
	   while (*c4) {
		 write_char(xc4, 30, *c4);
		 xc4++;
		 c4++;
	   }
}



void swap(int *x,int *y){
    int temp=*x;
    *x=*y;
    *y=temp;
}

void toString(char str[], int num)
{
    int i, rem, len = 0, n;
 
    n = num;
    while (n != 0)
    {
        len++;
        n /= 10;
    }
    for (i = 0; i < len; i++)
    {
        rem = num % 10;
        num = num / 10;
        str[len - (i + 1)] = rem + '0';
    }
    str[len] = '\0';
}

void clear_screen(){
    int x, y;
    char* c = "";
    for (x=0; x<=319; x++){
        for(y=0; y<=239; y++){
            write_pixel(x,y, 0x18b4);
        } 
    }

    for(x=0; x<80; x++){
            for(y=0; y<60; y++){
                write_char(x,y,*c);
            }
        }
}

//for actual hardware
void write_pixel(int x, int y, short int line_color){
	*(short int *)(0x08000000 + (y << 10) + (x << 1)) = line_color;
}

//for cpulator
//void write_pixel(int x, int y, short int color) {
//volatile short *vga_addr=(volatile short*)(0x08000000 + (y<<10) + (x<<1));
//*vga_addr=color;
//}

void write_char(int x, int y, char c) {
volatile char * character_buffer = (char *) (0x09000000 + (y<<7) + x);
*character_buffer = c;
}
