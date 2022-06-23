#include <REGX52.H>
	#include <stdio.H>
	//#include <intrins.H>

	sbit CS  = P1^0;
	sbit SI  = P1^1;
	sbit SCK = P1^2;

//----------------------------------------------------------------------------------;
		unsigned int count_timer1_interrupt;/* counrets */
		unsigned char count_timer0_interrupt;/* counrets */
		unsigned char clock_sec,clock_min,clock_chas;
//----------------------------------------------------------------------------------;
		void preset_T0isr (void){ // auto_reload,1sec/1024
		  TH0 = 0xA4; 
		  TL0 = 0xA4; 
		  TMOD |= 0x2;
		  TR0 = 1; 
		  ET0 = 1; 
		  EA = 1;
			
}
//----------------------------------------------------------------------------------;		
void preset_T1isr (void){ // auto_reload,1sec/1024
		  TH1 = 0xA4; 
		  TL1 = 0xA4; 
		  TMOD |= 0x22;
		  TR1 = 1; 
		  ET1 = 1; 
		  EA = 1;
} 		
//----------------------------------------------------------------------------------;
		//void delay(unsigned int val){
		//   while(val != 0)val--; 
		//}   
//----------------------------------------------------------------------------------;	
		
		void shift_out8(unsigned char _data) {
			char count_shift,SPI_time = 10;    	
      for (count_shift = 0; count_shift < 8; count_shift++) {   
        //delay(SPI_time/2); 
         if (_data & 0x80) SI =1; else SI =0; 
             _data<<=1;                                  
             //delay(SPI_time/2); 
             SCK = 1;          
             //delay(SPI_time);   
             SCK = 0;        
//***********************************************************;		
		/*	unsigned char count_shift;
				for ( count_shift = 0; count_shift < 8; count_shift++ ) {
						SCK = 0;
						delay(5);
						_data = _crol_ ( _data, 1 );
						SI = _data & 0x1;
						delay(5);
						SCK = 1;
						delay(5); 
				}	 */
//***********************************************************;
			}
		}
		
//----------------------------------------------------------------------------------;
    unsigned char delenie (unsigned char val, unsigned char dec) {
  	 dec = val / 10; 
	    return dec; 
}
//----------------------------------------------------------------------------------; 
    unsigned char ostatok (unsigned char val, unsigned char ed) {
  		ed = val % 10;
		   return ed;
}
//----------------------------------------------------------------------------------; 
	  void print_LED ( unsigned char sec, unsigned char min, unsigned char chas ) {
			 unsigned char code mass_number [10] = {0x3f, 0x6, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x7, 0x7f, 0x6f}; /*a,b,c,d,e,f,g   0,1,2,3,4,5,6,7,8,9*/
			 unsigned char code mass_stolb [7] = {1, 2, 4, 8, 16, 32};/* clocs katods */
//***********************************************************;
			 unsigned char ss_dec, ss_ed, mm_dec, mm_ed, cc_dec, cc_ed;/* clocs segment */
			 unsigned char count_data_line, position_stolb;
//***********************************************************;
				ss_dec = delenie(sec,ss_dec); 
				mm_dec = delenie(min,mm_dec);
				cc_dec = delenie(chas,cc_dec);
				ss_ed = ostatok(sec,ss_ed);
				mm_ed = ostatok(min,mm_ed);
				cc_ed = ostatok(chas,cc_ed);
//***********************************************************;		
						switch(count_data_line) { 
								case 0: 
												 cc_dec = ~mass_number[cc_dec]; 
												 position_stolb = mass_stolb[count_data_line]; 
												 CS = 0; 
												 shift_out8(position_stolb);
												 shift_out8(cc_dec);
												 CS = 1; 
												 break;
								case 1: 
												 cc_ed  = ~mass_number[cc_ed] ^ 128;
												 position_stolb = mass_stolb[count_data_line];
												 CS = 0;
												 shift_out8(position_stolb);
												 shift_out8(cc_ed);
												 CS = 1;
												 break;
								case 2: 
												 mm_dec = ~mass_number[mm_dec];
												 position_stolb = mass_stolb[count_data_line];
												 CS = 0; 
												 shift_out8(position_stolb); 
												 shift_out8(mm_dec); 
												 CS = 1;
												 break;
								case 3:
												 mm_ed  = ~mass_number[mm_ed] ^ 128;
												 position_stolb = mass_stolb[count_data_line];
												 CS = 0;
												 shift_out8(position_stolb);
												 shift_out8(mm_ed);
												 CS = 1;
												 break; 
								case 4: 
												 ss_dec = ~mass_number[ss_dec];
												 position_stolb = mass_stolb[count_data_line];
												 CS = 0; 
												 shift_out8(position_stolb);
												 shift_out8(ss_dec);
												 CS = 1;
												 break;   				
								case 5: 
												ss_ed  = ~mass_number[ss_ed];   
												position_stolb = mass_stolb[count_data_line];
												CS = 0;
												shift_out8(position_stolb);
												shift_out8(ss_ed);
												CS = 1;
												break;  
						}
											count_data_line++;  
												 if(count_data_line > 5){
													 count_data_line = 0;
		}
}
		
void chasi_hod (void){
	       			 
    clock_sec++;
							        
				if( clock_sec > 59) { clock_sec = 0;clock_min++;}             
				if (clock_min > 59){clock_min = 0;clock_sec = 0;clock_chas++;}
				if (clock_chas > 23) {clock_chas= 0;clock_min = 0;clock_sec = 0;}
	      
			
				          
}

//----------------------------------------------------------------------------------;	
void T0isr(void) interrupt 1 using 0{
	  TF0 = 0;
	  count_timer0_interrupt--;
						if(count_timer0_interrupt == 0){ 
								print_LED(clock_sec,clock_min ,clock_chas);
								  count_timer0_interrupt = 10; 
				}  
}
void T1isr(void) interrupt 3 using 1{
	  TF1 = 0;
	  count_timer1_interrupt--;
	         
	           if(count_timer1_interrupt == 0){ 
								 chasi_hod();
							   count_timer1_interrupt = 10000; 
								 
						   
							}	  
				}  

//----------------------------------------------------------------------------------;	
void main(void){
	  count_timer0_interrupt = 10;
	  count_timer1_interrupt = 10000;
	  P1 |= ~0x6;
	  PT1 = 1;
	  PT0 = 0;
	  preset_T0isr();
	  preset_T1isr ();
    
			while(1){	  
			 	if(P2_0 == 0){
					    TR0 = 0;
							clock_min++;
					     TR0 = 1;
						 }
	}
}	