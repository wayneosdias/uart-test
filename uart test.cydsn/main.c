
#include "common.h"

/* ==================================================
 * Followed by project posted by 
 * Bob Marlowe TxRx20
 * http://www.cypress.com/?app=forum&id=2233&rID=63901

 * Provided As-is, without any warranties to perform
 * under license terms CREATIVE COMMONS - SHARE ALIKE
 *
 * ==================================================
*/

#define RxBufferSize	128     //size of the Rx circular buffer


  
CY_ISR_PROTO(MyRxInt);          // process Rx interrupt

uint8	IsCharReady(void);		// Returns TRUE when a char is ready
uint8	GetRxStr(void);			// Extract command string from buffer, non-blocking 


//#endif
/* [] END OF FILE */

uint8	RxBuffer[RxBufferSize];     // Rx circular buffer to hold all incoming command
uint8  *RxReadIndex	 = RxBuffer;    // pointer to position in RxBuffer to write incoming Rx bytes
uint8  *RxWriteIndex = RxBuffer;    // pointer to position in RxBuffer to read and process bytes

uint8  rbuff[RxBufferSize];
uint8  pbuff[RxBufferSize];

uint8   *RxStrIndex = rbuff;//RB.RxStr;      // pointer to command string buffer (processed messages)
//uint8   writecnt = 0;                        // each Rx command consists of: <byte command><string value><CR>



CY_ISR(MyRxInt) //interrupt on Rx byte received
{   
    Uart_ClearRxInterruptSource(Uart_INTR_RX_NOT_EMPTY ); //clear interrupt
    //move all available characters from Rx queue to RxBuffer
    char byte;
    while((byte = Uart_UartGetChar()) !=0 )
    {
        *RxWriteIndex++ = byte; 
		if (RxWriteIndex >= RxBuffer + RxBufferSize){
            RxWriteIndex = RxBuffer;  
        }
	}   

}

uint8 IsCharReady(void) 
{
	return !(RxWriteIndex == RxReadIndex);
}

uint8 GetRxStr(void){

    uint8 RxStr_flag = 0;
    static uint8 Ch;//static? 

    
    Ch = *RxReadIndex++;

    if (RxReadIndex >= RxBuffer + RxBufferSize) RxReadIndex = RxBuffer;
    if(Ch == 13) {//RxWriteIndex - RxReadIndex == 1 EOM_ETX && *(RxReadIndex-2) == EOM_DLE
        *RxStrIndex = 0;
        RxStr_flag++;
    }
    else{                  
        *RxStrIndex++ = Ch;     
    } 
        
    return RxStr_flag; 
}

int16 volts = 0;
int16 therms = 0;

int main(){
    CyGlobalIntEnable;  
    Uart_Start();
    Rx_Int_StartEx(MyRxInt);
	while(1){   
        if(IsCharReady()){
            if(GetRxStr()){
                sprintf((char*)pbuff,"echo, %s\r\n",(char*)rbuff);
                Uart_UartPutString((char*)pbuff);
            }
        }
    //        volts = ADC_SAR_CountsTo_mVolts(1,ADC_SAR_GetResult16(1));
//        sprintf(strMsg1,"volts=%d\r\n", volts);
//        UART_UartPutString(strMsg1);
//        CyDelay(1000);   
//        therms = ADC_SAR_CountsTo_mVolts(0,ADC_SAR_GetResult16(0));
//        sprintf(strMsg1,"therms=%d\r\n", therms);
//        UART_UartPutString(strMsg1);
	}   
}  


/* [] END OF FILE */

