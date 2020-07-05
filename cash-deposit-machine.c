#include<reg51.h>

sbit cancel = P3^0 ;
sbit start = P0^0;      //
sbit error_account_does_not_exist = P0^1;  //
sbit error_pin_incorrect = P0^2;  //
sbit error_amount_round = P0^4;  //
sbit finished = P0^7;  //
sbit zero = P1^0;   //                                                                              ^    ^  ^ ^ ^
sbit one = P1^1;    //                                                                              |    |  | | |
sbit two = P1^2;    //                                                                       continue    |  | | start
sbit three = P1^3;   //                                                                                  |  | error : account does not exist
sbit four = P1^4;   //                                                                                   |  error : Pin incorrect
sbit five = P1^5;   //                                                                                   error : amount not in times of 100    
sbit six = P1^6;    //
sbit seven = P1^7;  // 
sbit eight = P2^0;  // 
sbit nine = P2^1;  //
void Delay(void);  //

int which_bit()
{
	if(P1 == 0x01) return 0;//                                                       9 8              7 6 54 3 21 0               ok.        ^
	if(P1 == 0x02) return 1;//                                                                                                               |
	if(P1 == 0x04) return 2;//                                                                                                               cancel
	if(P1 == 0x08) return 3;//
	if(P1 == 0x10) return 4;
	if(P1 == 0x20) return 5;
	if(P1 == 0x40) return 6;
	if(P1 == 0x80) return 7;
	if(P2 == 0x01) return 8;
	if(P2 == 0x02) return 9;
	return -1;
}

short int global_account_numbers[2][5] = 0;
short int global_pin_numbers[2][2] = 0;
short int account_number[5] = 0;
short int pin_number[2] = 0;	
int number_of_accounts = 0;
int account_position = -1;
int temp;
int global_amounts[2] = 0;
int amount[10] = 0;

void add_account()
{
	int i=0;
	for(i=0;i<5;i++)
	{
		global_account_numbers[number_of_accounts][i] = account_number[i];
	}
	for(i=0;i<2;i++)
	{
		global_pin_numbers[number_of_accounts][i] = pin_number[i];
	}
	number_of_accounts++;
}


int account_present()
{
	int i=0,j=0;
	for(i=0;i<number_of_accounts;i++)
	{
		for(j=0;j<5;j++)
		{
			if(account_number[j] != global_account_numbers[i][j]) break;
		}
		if(j == 5) 
			{
				account_position = i;
				return 1;
			}
	}
	return 0;
}


int pin_correct()
{
	int i=0,j=0;
	for(i=0;i<2;i++)
	{
		if(global_pin_numbers[account_position][i] != pin_number[i]) return 0;
  }
	return 1;
}


void display(int m1)
{
	int m2 = 0,i=9;
	while(m1 != 0)
	{
		amount[i] = m1%10;
		m1 = m1/10;
		i--;
	}
	i++;
	while(i < 10)
	{
		switch(amount[i])
		{
			case 0 : zero = 1;break;
			case 1 : one = 1;break;
			case 2 : two = 1;break;
			case 3 : three = 1;break;
			case 4 : four = 1;break;
			case 5 : five = 1;break;
			case 6 : six = 1;break;
			case 7 : seven = 1;break;
			case 8 : eight = 1;break;
			case 9 : nine = 1;break;
	 }
	Delay();
  P1 = P2 = 0x00;
	Delay();
  i++; 
 }
}

void main()
{
	int account_number_count = 0,pin_number_count=0,i=0,value=0,sec = 0;
	error_amount_round=0;
	account_number[0] = 1,account_number[1] = 2,account_number[2] = 3,account_number[3] = 4,account_number[4] = 5;
	pin_number[0] = 9,pin_number[1] = 7;
	add_account();
	account_number[0] = 6,account_number[1] = 7,account_number[2] = 8,account_number[3] = 9,account_number[4] = 0;
	pin_number[0] = 8,pin_number[1] = 7;
	error_pin_incorrect = 0;
	
	add_account();
  P0 = P1 = P2 = P3 = 0x00;
	
again :
	while(start == 0);
	value = temp = 0;
	error_account_does_not_exist = 0;
	error_pin_incorrect = 0;
	error_amount_round = 0;
	account_number_count = 0;
	TR0 = 0;TF0 = 0;
	while(account_number_count < 5)
	{
		sec = 0;
		TMOD = 0x01;           // Timer0 mode1
		TH0=0xDC;              
		TL0=0x00;
		TF0 = 0;
		TR0 = 0;
		while(P1 == 0x00 && P2 == 0x00 && cancel == 0)
		{
			if(TF0 == 1)
			{
				TR0 = 0;
				TF0 = 0;
				if(sec == 10000)
				{
					P3 = 0xFF;
					Delay();
					Delay();
					TR0 = TF0 = 0;
					P3 = P2 = P1 = P0 = 0x00;
				  goto again;
				}
			}
			if(TR0 == 0)
			{
				sec++;
				TMOD = 0x01;           // Timer0 mode1
				TH0=0xDC;              
				TL0=0x00;
				TR0 = 1;  
			}
    }
    TR0 = TF0 = 0;		
		if(cancel == 1)
		{
			P1 = P2 = P3 = P0 = 0x00;
			goto again;
		}
		account_number[account_number_count] = which_bit();
		P1 = P2 = 0x00;
		account_number_count++;
	}
	if(! account_present())
	{
		P0 = P1 = P2 = P3 = 0x00;
		error_account_does_not_exist = 1;
		start = 0;
		goto again;
	}
	P3 = 0xF0;
	i=0;
	while(i<2)
	{
		pin_number_count = 0;
		while(pin_number_count < 2)
		{
			sec = 0;
		  TMOD = 0x01;           // Timer0 mode1
		  TH0=0xDC;              
		  TL0=0x00;
		  TF0 = 0;
		  TR0 = 0;
			
			while(P1 == 0x00 && P2 == 0x00 && cancel == 0)
			{
				if(TF0 == 1)
				{
					TR0 = 0;
					TF0 = 0;
					if(sec == 10000)
					{
						P3 = 0xFF;
						Delay();
						Delay();
						TR0 = TF0 = 0;
						P3 = P2 = P1 = P0 = 0x00;
						goto again;
					}
				}
				if(TR0 == 0)
				{
					sec++;
					TMOD = 0x01;           // Timer0 mode1
					TH0=0xDC;              
					TL0=0x00;
					TR0 = 1;  
				}
			}
			TR0 = TF0 = 0;
			if(cancel == 1)
			{
				P1 = P2 = P3 = P0 = 0x00;
				goto again;
			}
			P3 = 0x00;
			pin_number[pin_number_count] = which_bit();
			P1 = P2 = 0x00;
			pin_number_count++;
		}
		if(! pin_correct())
		{
			error_pin_incorrect = 1;
			if(i == 1) { start = 0; goto again; }
		}
		else
		{
			error_pin_incorrect = 0;
			break;
		}
		i++;
	}
  i=0;	
	P3 = 0xF0;
	deposit_again:	finished = 0;
	temp = 0;
	value = 0;
	while(finished == 0)
	{
		  sec = 0;
		  TMOD = 0x01;           // Timer0 mode1
		  TH0=0xDC;              
		  TL0=0x00;
		  TF0 = 0;
		  TR0 = 0;
		
		while(P1 == 0x00 && P2 == 0x00 && finished == 0 && cancel == 0)
		{
			 if(TF0 == 1)
				{
					TR0 = 0;
					TF0 = 0;
					if(sec == 10000)
					{
						P3 = 0xFF;
						Delay();
						Delay();
						TR0 = TF0 = 0;
						P3 = P2 = P1 = P0 = 0x00;
						goto again;
					}
				}
				if(TR0 == 0)
				{
					sec++;
					TMOD = 0x01;           // Timer0 mode1
					TH0=0xDC;              
					TL0=0x00;
					TR0 = 1;  
				}	
		}
		TR0 = TF0 = 0;
		
		if(cancel == 1)
		{
			P1 = P2 = P3 = P0 = 0x00;
			goto again;
		}
		P3 = 0x00;
		if(finished == 1) break;
		value = which_bit();
    error_amount_round = 0;
		P1 = P2 = 0x00;
		temp = temp*10 + value;
  }
  if(temp%100 != 0)
  {
		    error_amount_round = 1;
    		if(i < 2) goto deposit_again;
		    start = 0;
		    goto again;
  }
	display(temp);
	global_amounts[account_position] += temp;
	P3 = 0xF0;
	Delay();
	Delay();
	P3 = 0x00;
	Delay();
	Delay();
	finished = 0;
	display(global_amounts[account_position]);
	P3 = 0xF0;
	Delay();
	Delay();
	P3 = 0x00;
	start = finished = 0;
	goto again;
}

void Delay()
{
	int i;
	for(i=0;i<(500);i++)
	{
		TMOD = 0x01;           // Timer0 mode1
		TH0=0xDC;              
		TL0=0x00;
		TR0 = 1;               // timer0 start
		while (TF0 == 0);      // check overflow condition
		TR0 = 0;               // Stop Timer
		TF0 = 0;               // Clear flag
  }
}
