#include <avr/io.h>
#include <avr/interrupt.h>

int main(void)
{
    /* 配置 PB5 为输出（板载 LED，Arduino D13） */
    DDRB |= (1 << DDB5);

    /* 配置 PB0 为输入（ICP1，Arduino D8） */
    DDRB &= ~(1 << DDB0);

    /* Timer1 输入捕获配置：
       - ICES1 = 1：初始检测上升沿（从低到高 = 按钮按下）
       - CS11 + CS10 = CLK/64：启动 Timer1 时钟 */
    TCCR1B |= (1 << ICES1) | (1 << CS11) | (1 << CS10);

    /* 启用 Timer1 输入捕获中断（ICIE1） */
    TIMSK1 |= (1 << ICIE1);

    /* 全局中断使能 */
    sei();

    /* 主循环空闲，所有响应由中断服务例程处理 */
    while (1)
    {
        ;
    }

    return 0;
}

/* Timer1 输入捕获中断服务例程 */
ISR(TIMER1_CAPT_vect)
{
    /* 在 ICF1 位写 1 以清除 TIFR1 中的输入捕获标志 */
    TIFR1 |= (1 << ICF1);

    /* 读取 PINB 判断 PB0 当前电平 */
    if (PINB & (1 << PINB0))
    {
        /* PB0 为高电平：按钮按下（上升沿触发） */
        PORTB |= (1 << PORTB5);   /* 点亮 LED */

        /* 切换为下降沿检测，等待按钮松开 */
        TCCR1B &= ~(1 << ICES1);
    }
    else
    {
        /* PB0 为低电平：按钮松开（下降沿触发） */
        PORTB &= ~(1 << PORTB5);  /* 熄灭 LED */

        /* 切换为上升沿检测，等待按钮按下 */
        TCCR1B |= (1 << ICES1);
    }
}
