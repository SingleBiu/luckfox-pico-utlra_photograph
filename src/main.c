/*
 * @Author: SingleBiu
 * @Date: 2021-07-22 14:48:40
 * @LastEditors: SingleBiu
 * @LastEditTime: 2024-10-15 13:04:25
 * @Description: 一个简单的能展示bmp图片和jpg图片的电子相册
 */
#include"lcd.h"
#include"bmp.h"
#include"jpg.h"
#include"ts.h"
#include"node.h"
#include<pthread.h>

// 按钮
int btn_l = FALSE;
int btn_r = FALSE;

// 定义存放图片的链表
Head_Node *H = NULL;
// 触摸屏输入的值
int TOUCH_EVENT = MOVE_EMPTY;
// 触摸屏文件描述符
int ts_fd;

void *thread_touch()
{
    while(1)
	{
        get_user_input(ts_fd);
	}
}

void *thread_touch_click()
{
    while (1)
    {
        get_user_input_click(ts_fd);
    }
    
}

int main(int argc, char *argv[])
{
    
    // 初始化LCD屏幕
    lcd_init();
    
    // // 检查LCD屏幕有无坏点
    // lcd_color_check();
    
    //清屏 白色
    lcd_draw_rect(0,0,WIDTH,HEIGHT,WHITE);
    
    // 初始化图片链表
    H = init_Node();

    DIR *dir = opendir(argv[1]);
    // 读取目录
    readdirent(dir,argv[1]);

    // 定义一个指针指向图片链表
    Node *ptr = H->pic_list;
    
    // 开始的图片
    bmp_display("startup.bmp",0,0);

    jpg_displasy("left.jpg",0,600);
    jpg_displasy("right.jpg",560,600);

    ts_fd = touch_init();


    pthread_t ts;
    int res = pthread_create(&ts,NULL,thread_touch_click,NULL);
    if (res != 0)
    {
        perror("Failed to create pthread");
        exit(res);
    }

    while (1)
    {
        if (TRUE == btn_l)
        {
            lcd_draw_rect(0,0,WIDTH,HEIGHT,WHITE);
            // 上一张
            ptr = ptr->prev;
            display(ptr->pathname,0,0);
            btn_l = FALSE;
            jpg_displasy("left.jpg",0,600);
            jpg_displasy("right.jpg",560,600);
        }
        
        if (TRUE == btn_r)
        {
            lcd_draw_rect(0,0,WIDTH,HEIGHT,WHITE);
            // 下一张
            ptr = ptr->next;
            display(ptr->pathname,0,0);
            btn_r = FALSE;
            jpg_displasy("left.jpg",0,600);
            jpg_displasy("right.jpg",560,600);
        }
        
        
    }
    
    // while (1)
    // {
    //     // // 获得触摸屏幕输入
    //     // int mv = get_user_input(ts_fd);
    //     // if (mv == MOVE_RIGHT)
    //     if (TOUCH_EVENT == MOVE_RIGHT)
    //     {
    //         lcd_draw_rect(0,0,WIDTH,HEIGHT,WHITE);
    //         // 上一张
    //         ptr = ptr->prev;
    //         display(ptr->pathname,0,0);
    //         TOUCH_EVENT = MOVE_EMPTY;
    //     }
    //     // else if (mv == MOVE_LEFT)
    //     else if (TOUCH_EVENT == MOVE_LEFT)
    //     {
    //         lcd_draw_rect(0,0,WIDTH,HEIGHT,WHITE);
    //         // 下一张
    //         ptr = ptr->next;
    //         display(ptr->pathname,0,0);
    //         TOUCH_EVENT = MOVE_EMPTY;
    //     }
    // }
    

    while(1){}

    // 关闭LCD屏幕
    close(lcd_fd);

    return 0;
}
