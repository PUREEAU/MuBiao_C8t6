#include "Key.h"
#include "sys_tick.h"

/*
 * Matrix_Key_SetCol:���õ�ǰɨ�����,PIN0��Ӧ�����һ��
 * param i:��ǰɨ�������
 * @ret  -1--err  0--success
*/
int Matrix_Key_SetCol(int i){
    
    //1.������Ч���ж�
    //����ֻ��0~4��
    if(i < 0 || i > 3){
//        printf("col err\r\n");
        return -1;
    }
    //2.������Ӧ��ɨ��
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
 * Matrix_Key_isDown:�ж�ָ���İ���IO�Ƿ���
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
 * Matrix_Key_RowScan:��ȡָ����״̬,PIN4��Ӧ�����һ��
 * param i:��ǰɨ�������
 * @ret  0--û�а�������  other--�����е����
*/
int Matrix_Key_RowScan(void){
	
	//�����ж�ÿһ���Ƿ��а���
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
 * Matrix_Key_Scan:���󰴼��ӿ�
 * @ret  0--û�а���  other--���������
*/
int Matrix_Key_Scan(void){
 
	int col;
	int row;
	
	//1.ɨ������һ����һ�в����İ���
	for(col=0;col<4;col++){
		Matrix_Key_SetCol(col);
		row = Matrix_Key_RowScan();
		if(row != 0){
			break;
		}
	}
	if(col == 4){//ûɨ�赽����
		return 0;
	}
	//2.�������ĸ�����ֵ
	return ((row-1)*4+(col+1));
	
}
//int key_down_num = 0;
/*
 * Matrix_Key_GetDownNum:���󰴼��ӿ�
 * @ret  0--û�а���  other--��������� 
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
