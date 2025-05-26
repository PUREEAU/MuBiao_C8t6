#include "Work.h"
#include "sys_tick.h"
#include "uart_servo_lite.h"
#include "OLED.h"
#include "Key.h"
Usart_DataTypeDef* servoUsart = &usart1;
#define ID_HoJo_X 1
#define ID_HoJo_Y 2

uint8_t Mode = 9;

//
#define change_Angle 2

float Add_X_coordinate = 0;
float Add_Y_coordinate = 0;
float curAngle_x = 0;
float curAngle_y = 0;
float Save_Star_Data_X[4]={0,0,0,0};
float Save_Star_Data_Y[4]={0,0,0,0};
uint8_t read_flag = 0;
float Back_central_x=0;
float Back_central_y=0;

//
uint8_t Mode3_Save_Data_flag = 0;


void OLED_Work(void)
{
    if(Mode == 0 )
    {
        OLED_Printf(1, 1, OLED_6X8, "%2.2f", Save_Star_Data_X[0]);
        OLED_Printf(48, 1, OLED_6X8, "%2.2f", Save_Star_Data_Y[0]);
    
        OLED_Printf(1, 10, OLED_6X8, "%2.2f", Save_Star_Data_X[1]);
        OLED_Printf(48, 10, OLED_6X8, "%2.2f", Save_Star_Data_Y[1]);
    
        OLED_Printf(1, 20, OLED_6X8, "%2.2f", Save_Star_Data_X[2]);
        OLED_Printf(48, 20, OLED_6X8, "%2.2f", Save_Star_Data_Y[2]);
    
        OLED_Printf(1, 30, OLED_6X8, "%2.2f", Save_Star_Data_X[3]);
        OLED_Printf(48, 30, OLED_6X8, "%2.2f", Save_Star_Data_Y[3]);
        
        OLED_Printf(1, 40, OLED_6X8, "%2.2f", Add_X_coordinate);
        OLED_Printf(48, 40, OLED_6X8, "%2.2f", Add_Y_coordinate);
        
    }
    if(Mode == 1 )
    {
        OLED_Printf(1, 1, OLED_6X8, "%2.2f", Save_Star_Data_X[0]);
        OLED_Printf(48, 1, OLED_6X8, "%2.2f", Save_Star_Data_Y[0]);
    
        OLED_Printf(1, 10, OLED_6X8, "%2.2f", Save_Star_Data_X[1]);
        OLED_Printf(48, 10, OLED_6X8, "%2.2f", Save_Star_Data_Y[1]);
    
        OLED_Printf(1, 20, OLED_6X8, "%2.2f", Save_Star_Data_X[2]);
        OLED_Printf(48, 20, OLED_6X8, "%2.2f", Save_Star_Data_Y[2]);
    
        OLED_Printf(1, 30, OLED_6X8, "%2.2f", Save_Star_Data_X[3]);
        OLED_Printf(48, 30, OLED_6X8, "%2.2f", Save_Star_Data_Y[3]);
        
        OLED_Printf(1, 40, OLED_6X8, "%2.2f", Add_X_coordinate);
        OLED_Printf(48, 40, OLED_6X8, "%2.2f", Add_Y_coordinate);
        
        
//        OLED_Printf(1, 50, OLED_6X8, "%d", Rx_1_Data);
    }
    
    OLED_Printf(90, 0, OLED_6X8, "Mode:%d", Mode);
    OLED_Update();
    SysTick_CountdownBegin(10);
}
void Mode_Change_Work(void)
{
    if(Mode == 9) // 模式一下读取当前舵机角度
    {
        
//        Add_X_coordinate = USL_GETPositionVal(servoUsart,1);
//        SysTick_DelayMs(10);
//        Add_Y_coordinate = USL_GETPositionVal(servoUsart,2);
//        SysTick_DelayMs(10);
        
        MeGet_GETPositionVal(servoUsart,ID_HoJo_X ,&Add_X_coordinate);
        SysTick_DelayMs(10);
        MeGet_GETPositionVal(servoUsart,ID_HoJo_Y ,&Add_Y_coordinate);
        SysTick_DelayMs(10);
        Mode=0;
    }
    if(Matrix_Key_GetDownNum() == 1)
    {
        Mode++;
        OLED_Clear();
        if(Mode == 4) Mode = 0;
    }
}


void Mode_1_Work(void)
{
    if(Mode == 0)
    {
        //修改舵机角度
        if( Matrix_Key_GetDownNum() == 9 )
        {
            Add_X_coordinate += change_Angle;
            MeSet_ServoAngle(servoUsart, ID_HoJo_X ,Add_X_coordinate , 100);
        }
        if( Matrix_Key_GetDownNum() == 11 )
        {
            Add_X_coordinate -= change_Angle;
            MeSet_ServoAngle(servoUsart, ID_HoJo_X ,Add_X_coordinate , 100);
        }
        if( Matrix_Key_GetDownNum() == 14  )
        {
            Add_Y_coordinate += change_Angle;
            MeSet_ServoAngle(servoUsart, ID_HoJo_Y ,Add_Y_coordinate , 100);
        }
        if( Matrix_Key_GetDownNum() == 6 )
        {
            Add_Y_coordinate -= change_Angle;
            MeSet_ServoAngle(servoUsart, ID_HoJo_Y ,Add_Y_coordinate , 100);
        }
        //保存舵机角度
        if( Matrix_Key_GetDownNum() == 10 )
        {
            MeGet_GETPositionVal(servoUsart,ID_HoJo_X ,&curAngle_x);
            SysTick_DelayMs(5);
            MeGet_GETPositionVal(servoUsart,ID_HoJo_Y ,&curAngle_y);
            SysTick_DelayMs(5);
            Save_Star_Data_X [read_flag]= curAngle_x;
            Save_Star_Data_Y [read_flag]= curAngle_y;
            read_flag++;
            if(read_flag>3)read_flag=0;
        }
        //模式1开始工作
        if( Matrix_Key_GetDownNum() == 16 )
        {
            Back_central_x = ( (Save_Star_Data_X[0] + Save_Star_Data_X[2]) + (Save_Star_Data_X[1] + Save_Star_Data_X[3]) ) / 4;
            Back_central_y = ( (Save_Star_Data_Y[0] + Save_Star_Data_Y[2]) + (Save_Star_Data_Y[1] + Save_Star_Data_Y[3]) ) / 4;
            MeSet_ServoAngle(servoUsart, ID_HoJo_X ,Back_central_x , 100);
            SysTick_DelayMs(5);
            MeSet_ServoAngle(servoUsart, ID_HoJo_Y ,Back_central_y , 100);
            SysTick_DelayMs(5);
        }
        
    }
}



void Arithmetic_Data(uint8_t start, uint8_t end) 
{
    // 确保start和end不越界（假设数组长度为N）
    // if (start >= N || end >= N) return;
    const int segments = 4; // 五等分，共5段
    for (int i = 0; i <= segments; i++) 
    { // 循环6次（0到5）
        float t = (float)i / (float)segments; // t范围0.0 ~ 1.0
        float X = Save_Star_Data_X[start] + t * (Save_Star_Data_X[end] - Save_Star_Data_X[start]);
        float Y = Save_Star_Data_Y[start] + t * (Save_Star_Data_Y[end] - Save_Star_Data_Y[start]);
        
        MeSet_ServoAngle(servoUsart, ID_HoJo_X ,X , 100);
        MeSet_ServoAngle(servoUsart, ID_HoJo_Y ,Y , 100);
        
        // 根据实际舵机运动时间调整延时，或使用异步控制
        SysTick_DelayMs(200);
    }
}

void Mode_2_Work(void)
{
    if(Mode == 1)
    {
        if( Matrix_Key_GetDownNum() == 16 )
        {
            Arithmetic_Data(0,1);
            Arithmetic_Data(1,2);
            Arithmetic_Data(2,3);
            Arithmetic_Data(3,0);
            
            HAL_UART_Transmit(&huart3,(uint8_t *)"1",1,50);
        }
    }
}

void Mode_3_Work(void)
{
    if(Mode == 3)
    {
        if( Matrix_Key_GetDownNum() == 16 )
        {
            HAL_UART_Transmit(&huart3,(uint8_t *)"0",1,50);
        }
        
        if( Matrix_Key_GetDownNum() == 5 && Mode3_Save_Data_flag == 0)
        {
            HAL_UART_Transmit(&huart3,(uint8_t *)"2",1,50);
        }
        if( Matrix_Key_GetDownNum() == 6 && Mode3_Save_Data_flag == 1)
        {
            HAL_UART_Transmit(&huart3,(uint8_t *)"3",1,50);
        }
    }
}

void All_Work(void)
{
    Mode_Change_Work();
    
    Mode_1_Work();
    Mode_2_Work();
    Mode_3_Work();
    OLED_Work();
    
}
