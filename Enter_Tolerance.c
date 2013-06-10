int Enter_Tolerance() //Entered by CLOCK
{
  i++;
	if (i==1) //START BIT
	{	
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
		 	
	if((i>1) && (i<10)) //DATA BIT
	{
		data = data >> 1;
		bit = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2);
		if(bit > 0)
		{
			data = data | 0x80;
			parity = (1-parity);
		}
			EXTI_ClearITPendingBit(EXTI_Line0);
		}
		
	if(i==10) //PARITY BIT
	{
		//check parity, not necessary;
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
	
	if(i==11) //STOP BIT
	{
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2) == 1)
		{
			i = 0;
			if(data == 0x5A) //return valid temperature difference
			{
				EXTI_ClearITPendingBit(EXTI_Line0);
				USART_SendData (USART6, t_diff);
				return t_diff;
			}
			else if(data == 0x45)
			{
				t_diff = 0;
			}
			else if(data == 0x16)
			{
				t_diff = 1;
			}
			else if(data == 0x1E)
			{
				t_diff = 2;
			}
			else if(data == 0x26)
			{
				t_diff = 3;
			}
			else if(data == 0x25)
			{
				t_diff = 4;
			}
			else if(data == 0x2E)
			{
				t_diff = 5;
			}
			else if(data == 0x36)
			{
				t_diff = 6;
			}
			else if(data == 0x3D)
			{
				t_diff = 7;
			}
			else if(data == 0x3E)
			{
				t_diff = 8;
			}
			else if(data == 0x46)
			{
				t_diff = 9;
			}
		}
		EXTI_ClearITPendingBit(EXTI_Line0);
	}	
	if(i>11)
	{
		i=0;
	}
}

/* benötigte Variablen (global)
int bit=0;
int data=0;
int t_diff=0;
int i=0;
int parity=0;
/*
