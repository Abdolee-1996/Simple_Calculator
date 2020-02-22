/*-------------------------------------------------------------------------------------
 |         [FILE NAME]     <keypd.c>    		 									   |
 | 		   [AUTHOR]        <Abdolee>                                                   |
 |         [DATE CREATED]  <Jan 15, 2020>											   |
 |         [DESCRIPTION]   <simple calculator MINI project, it capable to			   |
 | 	  	  	  	  	  	  	  get a negative numb and float>						   |
 |-------------------------------------------------------------------------------------*/
/* Defines to select the way of callng the Functions*/
#define ARR_OF_FUNC_METHOD                                     DISABLE
#define POINTER_TO_FUNC_METHOD								   ENABLE
#define FUNC_THAT_USE_POINTER_TO_FUNC_METHOD                   DISABLE


#include"../HAL/lcd.h"
#include"../HAL/keypad.h"

/*prototype of the calculator functions*/
float32 add(float32 a_numb1,float32 a_numb2);
float32 sub(float32 a_numb1,float32 a_numb2);
float32 mul(float32 a_numb1,float32 a_numb2);
float32 div(float32 a_numb1,float32 a_numb2);

/* function the reset the Calculator */
void restartCalculator(void);

/* function that uses PTR TO FUNC to call the functions*/
#if FUNC_THAT_USE_POINTER_TO_FUNC_METHOD
float32 calcFunc(float32 (*pointerToOpperationFunc)(float32, float32),float32 a_numb1,float32 a_numb2);
#endif

/* ptr to function that points to the calculator function*/
#if (POINTER_TO_FUNC_METHOD|| FUNC_THAT_USE_POINTER_TO_FUNC_METHOD)
float32 (*pointerToOpperationFunc)(float32, float32)=NULL_PTR;
#endif

/*ARR of function which contains that calculator functions*/
#if ARR_OF_FUNC_METHOD
float32 (*opperationFuncArr[4])(float32, float32)={add,sub,mul,div};
#endif

/*global variable that has the value of the keypad */
uint8 keypadKey=0;

uint8 enteredOperatiinFlag=FALSE;      //FLAGE to indicate if there is an operation symbol has been entered or not
uint8 firstNumberFlag=FALSE;           //FLAGE that rise when the first INT number for the operation is entered
uint8 newOperationFlag=FALSE;        //FLAGE that indicate if the mathematical operation is ended and need to make new one

/* variable that contain the ARR value at using this method*/
#if ARR_OF_FUNC_METHOD
uint8 operationArrNumb=0;
#endif


uint16 numb1=0;
uint16 numb2=0;
uint16 numbCntr=1; //a counter to build the decimal number properly


int main(void)
{
	/*KEYPAD and LCD INIT*/
	KEYPAD_Init();
	LCD_Init();

	while(1)
	{

		keypadKey=KEYPAD_ReadInt(); //wait to press the keypad

		/* this instruction check if the new operation flag is risen to clear the display to be ready to enter a new operation */
		newOperationFlag?LCD_SendCommand(CLEAR_DISPLAY),newOperationFlag=0:0;

		if(keypadKey =='K' ) //this key indicate to reset the calculator
		{
			restartCalculator();
			LCD_SendCommand(CLEAR_DISPLAY);
			newOperationFlag=FALSE;
		}
		else
		{
			if(keypadKey>=0 && keypadKey<=9)  // if the pressed key is a number
			{
				LCD_SendInt(keypadKey);

				if(!(enteredOperatiinFlag)) // and the operation flag is not risen, no operation entered
				{
					numb1=(numb1*10)+keypadKey; //build the first number
				}
				else if(enteredOperatiinFlag) // if an operation symbol is entered
				{
					numb2=(numb2*10)+keypadKey;  // build the second  number
				}
				firstNumberFlag=TRUE; // rise the flag as there a number entered
			}
			else if(firstNumberFlag) //must be a first number be entered, to pass this condition
			{
				if(keypadKey=='=') // if the equal symbol is entered
				{
					LCD_SendCommand(FORCE_TO_NEXT_LINE);
					LCD_SendChar('=');

					if(enteredOperatiinFlag) // if there is an operation symbol is entered before
					{
						/* calling the function, by different methods*/

						#if FUNC_THAT_USE_POINTER_TO_FUNC_METHOD
						LCD_SendFloat((*opperationFuncArr[opperationArrNumb])(numb1,numb2));

						#elif ARR_OF_FUNC_METHOD
						LCD_SendFloat(calcFunc(pointerToOpperationFunc,numb1,numb2));

						#elif POINTER_TO_FUNC_METHOD
						LCD_SendFloat((*pointerToOpperationFunc)(numb1,numb2));

						#endif
					}
					else
					{
						LCD_SendInt(numb1);
					}

					restartCalculator();
					newOperationFlag=TRUE; // the operation is finished so rise the flag
				}
				else if(!(enteredOperatiinFlag))  // as capable to enter a single symbol in one operation
				{
					numbCntr=1; // restart the counter to be able to build the second number after entering the operation symbol
					enteredOperatiinFlag=TRUE;  // rise the operation flag is entered to not enter this section again the operation end
					LCD_SendChar(keypadKey);

					if(keypadKey=='+')
					{

						#if ARR_OF_FUNC_METHOD
						opperationArrNumb=0;
						#elif (POINTER_TO_FUNC_METHOD|| FUNC_THAT_USE_POINTER_TO_FUNC_METHOD)
						pointerToOpperationFunc=add;
						#endif
					}
					else if(keypadKey=='-')
					{

						#if ARR_OF_FUNC_METHOD
						opperationArrNumb=1;
						#elif (POINTER_TO_FUNC_METHOD|| FUNC_THAT_USE_POINTER_TO_FUNC_METHOD)
						pointerToOpperationFunc=sub;
						#endif
					}
					else if(keypadKey=='*')
					{
						#if ARR_OF_FUNC_METHOD
						opperationArrNumb=2;
						#elif (POINTER_TO_FUNC_METHOD|| FUNC_THAT_USE_POINTER_TO_FUNC_METHOD)
						pointerToOpperationFunc=mul;
						#endif
					}
					else if(keypadKey=='/')
					{
						#if ARR_OF_FUNC_METHOD
						opperationArrNumb=3;
						#elif (POINTER_TO_FUNC_METHOD|| FUNC_THAT_USE_POINTER_TO_FUNC_METHOD)
						pointerToOpperationFunc=div;
						#endif
					}
				}
			}
		}
	}
return 0;

}



/*function that reset the flags and the numbers when reseting the calculator */
void restartCalculator(void)
{
	enteredOperatiinFlag=FALSE;
	firstNumberFlag=FALSE;

	numb1=FALSE;
	numb2=FALSE;
	numbCntr=TRUE;

}



#if FUNC_THAT_USE_POINTER_TO_FUNC_METHOD
float32 calcFunc(float32 (*pointerToOpperationFunc)(float32, float32),float32 a_numb1,float32 a_numb2)
{

	return ((*pointerToOpperationFunc)(a_numb1,a_numb2));
}
#endif

float32 add(float32 a_numb1,float32 a_numb2)
{
	return (a_numb1+a_numb2);
}



float32 sub(float32 a_numb1,float32 a_numb2)
{
	if(a_numb1<a_numb2)
	{
		LCD_SendChar('-');
		return (a_numb2-a_numb1);

	}
	return (a_numb1-a_numb2);
}



float32 mul(float32 a_numb1,float32 a_numb2)
{
	return (a_numb1*a_numb2);
}



float32 div(float32 a_numb1,float32 a_numb2)
{
	return (a_numb1/a_numb2);
}


