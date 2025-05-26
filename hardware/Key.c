#include "Key.h"
#include "sys_tick.h"

/*
 * Matrix_Key_SetCol:设置当前扫描的列,PIN0对应右起第一列
 * param i:当前扫描的列数
 * @ret  -1--err  0--success
*/
int Matrix_Key_SetCol(int i){
    
    //1.参数有效性判断
    //按键只有0~4列
    if(i < 0 || i > 3){
//        printf("col err\r\n");
        return -1;
    }
    //2.设置相应列扫描
    switch(i){
        case 3:
            GPIOB->BRR |= MATRIX_KEY_PIN_0;
            GPIOB->BSRR |= (MATRIX_KEY_PIN_1|MATRIX_KEY_PIN_2|MATRIX_KEY_PIN_3);
            break;
        case 2:
            GPIOB->BRR |= MATRIX_KEY_PIN_1;
            GPIOB->BSRR |= (MATRIX_KEY_PIN_0|MATRIX_KEY_PIN_2|MATRIX_KEY_PIN_3);
            break;
        case 1:
            GPIOB->BRR |= MATRIX_KEY_PIN_2;
            GPIOB->BSRR |= (MATRIX_KEY_PIN_0|MATRIX_KEY_PIN_1|MATRIX_KEY_PIN_3);
            break;
        case 0:
            GPIOB->BRR |= MATRIX_KEY_PIN_3;
            GPIOB->BSRR |= (MATRIX_KEY_PIN_0|MATRIX_KEY_PIN_1|MATRIX_KEY_PIN_2);
            
            break;
    }
    return 0;
}

/*
 * Matrix_Key_isDown:判断指定的按键IO是否按下
 * @ret  0--UP  1--DOWN
*/
int Matrix_Key_isDown(GPIO_TypeDef* KEY_Port,uint32_t KEY_Pin){
    
    if(HAL_GPIO_ReadPin(KEY_Port,KEY_Pin) == GPIO_PIN_RESET){
        SysTick_DelayMs(40);
        if(HAL_GPIO_ReadPin(KEY_Port,KEY_Pin) == GPIO_PIN_RESET){
            return 1;
        }
    }
    return 0;
}

/*
 * Matrix_Key_RowScan:获取指定行状态,PIN4对应上面第一行
 * param i:当前扫描的行数
 * @ret  0--没有按键按下  other--按下行的序号
*/
int Matrix_Key_RowScan(void){
	
	//依次判断每一行是否有按下
	if(Matrix_Key_isDown(MATRIX_KEY_PORT_4,MATRIX_KEY_PIN_4)){
		return 1;
	}
	if(Matrix_Key_isDown(MATRIX_KEY_PORT_5,MATRIX_KEY_PIN_5)){
		return 2;
	}
	if(Matrix_Key_isDown(MATRIX_KEY_PORT_6,MATRIX_KEY_PIN_6)){
		return 3;
	}
	if(Matrix_Key_isDown(MATRIX_KEY_PORT_7,MATRIX_KEY_PIN_7)){
		return 4;
	}
	return 0;
}

/*
 * Matrix_Key_Scan:矩阵按键接口
 * @ret  0--没有按下  other--按键的序号
*/
int Matrix_Key_Scan(void){
 
	int col;
	int row;
	
	//1.扫描是哪一行哪一列产生的按键
	for(col=0;col<4;col++){
		Matrix_Key_SetCol(col);
		row = Matrix_Key_RowScan();
		if(row != 0){
			break;
		}
	}
	if(col == 4){//没扫描到按键
		return 0;
	}
	//2.计算是哪个按键值
	return ((row-1)*4+(col+1));
	
}
//int key_down_num = 0;
/*
 * Matrix_Key_GetDownNum:矩阵按键接口
 * @ret  0--没有按下  other--按键的序号 
*/
int Matrix_Key_GetDownNum(void){
    
    int key_down_num = 0;
    int res = 0;
     int key_up = 1;
    
    key_down_num = Matrix_Key_Scan();
    if(key_down_num != 0){
        if( key_up == 1){
            key_up = 0;
            res = key_down_num;
            //printf("key_down_num = %d\r\n",key_down_num);
        }
    }else{
        key_up = 1;
    }
    return res;
}
