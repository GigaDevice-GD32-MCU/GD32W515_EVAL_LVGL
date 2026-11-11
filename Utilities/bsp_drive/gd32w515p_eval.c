#include "gd32w515p_eval.h"
#include <stdio.h>

static uint32_t GPIO_PORT[LEDn] = {LED1_GPIO_PORT, LED2_GPIO_PORT, LED3_GPIO_PORT};
static uint32_t GPIO_PIN[LEDn] = {LED1_PIN, LED2_PIN, LED3_PIN};
static rcu_periph_enum COM_CLK[COMn] = {EVAL_COM0_CLK};
static uint32_t COM_TX_PIN[COMn] = {EVAL_COM0_TX_PIN};
static uint32_t COM_RX_PIN[COMn] = {EVAL_COM0_RX_PIN};
static rcu_periph_enum GPIO_CLK[LEDn] = {LED1_GPIO_CLK, LED2_GPIO_CLK, LED3_GPIO_CLK};
static uint32_t KEY_PORT[KEYn] = {TAMPER_WAKEUP_KEY_GPIO_PORT};
static uint32_t KEY_PIN[KEYn] = {TAMPER_WAKEUP_KEY_PIN};
static rcu_periph_enum KEY_CLK[KEYn] = {TAMPER_WAKEUP_KEY_GPIO_CLK};
static exti_line_enum KEY_EXTI_LINE[KEYn] = {TAMPER_WAKEUP_KEY_EXTI_LINE};
static uint8_t KEY_PORT_SOURCE[KEYn] = {TAMPER_WAKEUP_KEY_EXTI_PORT_SOURCE};
static uint8_t KEY_PIN_SOURCE[KEYn] = {TAMPER_WAKEUP_KEY_EXTI_PIN_SOURCE};
static IRQn_Type KEY_IRQn[KEYn] = {TAMPER_WAKEUP_KEY_EXTI_IRQn};

void gd_eval_led_init(led_typedef_enum lednum)
{
    rcu_periph_clock_enable(GPIO_CLK[lednum]);
    gpio_mode_set(GPIO_PORT[lednum], GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN[lednum]);
    gpio_output_options_set(GPIO_PORT[lednum], GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, GPIO_PIN[lednum]);
    GPIO_BC(GPIO_PORT[lednum]) = GPIO_PIN[lednum];
}

void gd_eval_led_on(led_typedef_enum lednum)
{
    GPIO_BOP(GPIO_PORT[lednum]) = GPIO_PIN[lednum];
}

void gd_eval_led_off(led_typedef_enum lednum)
{
    GPIO_BC(GPIO_PORT[lednum]) = GPIO_PIN[lednum];
}

void gd_eval_led_toggle(led_typedef_enum lednum)
{
    GPIO_TG(GPIO_PORT[lednum]) = GPIO_PIN[lednum];
}

void gd_eval_key_init(key_typedef_enum key_num, keymode_typedef_enum key_mode)
{
    rcu_periph_clock_enable(KEY_CLK[key_num]);
    rcu_periph_clock_enable(RCU_SYSCFG);
    gpio_mode_set(KEY_PORT[key_num], GPIO_MODE_INPUT, GPIO_PUPD_NONE, KEY_PIN[key_num]);
    if(key_mode == KEY_MODE_EXTI) {
        nvic_irq_enable(KEY_IRQn[key_num], 2U, 0U);
        syscfg_exti_line_config(KEY_PORT_SOURCE[key_num], KEY_PIN_SOURCE[key_num]);
        exti_init(KEY_EXTI_LINE[key_num], EXTI_INTERRUPT, EXTI_TRIG_FALLING);
        exti_interrupt_flag_clear(KEY_EXTI_LINE[key_num]);
    }
}

uint8_t gd_eval_key_state_get(key_typedef_enum button)
{
    return gpio_input_bit_get(KEY_PORT[button], KEY_PIN[button]);
}

void gd_eval_com_init(uint32_t com)
{
    uint32_t com_id = 0U;
    if(EVAL_COM0 == com) {
        com_id = 0U;
    }
    rcu_periph_clock_enable(EVAL_COM0_GPIO_CLK);
    rcu_periph_clock_enable(COM_CLK[com_id]);
    gpio_af_set(EVAL_COM0_GPIO_PORT, EVAL_COM0_TX_AF, COM_TX_PIN[com_id]);
    gpio_af_set(EVAL_COM0_GPIO_PORT, EVAL_COM0_RX_AF, COM_RX_PIN[com_id]);
    gpio_mode_set(EVAL_COM0_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, COM_TX_PIN[com_id]);
    gpio_output_options_set(EVAL_COM0_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, COM_TX_PIN[com_id]);
    gpio_mode_set(EVAL_COM0_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, COM_RX_PIN[com_id]);
    gpio_output_options_set(EVAL_COM0_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, COM_RX_PIN[com_id]);
    usart_deinit(com);
    usart_baudrate_set(com, 115200U);
    usart_receive_config(com, USART_RECEIVE_ENABLE);
    usart_transmit_config(com, USART_TRANSMIT_ENABLE);
    usart_enable(com);
}

#if defined(__GNUC__) && !defined(__clang__)
int __io_putchar(int ch)
{
    usart_data_transmit(EVAL_COM0, (uint8_t) ch);
    while(RESET == usart_flag_get(EVAL_COM0, USART_FLAG_TBE)) {
    }
    return ch;
}
#else
int fputc(int ch, FILE *f)
{
    usart_data_transmit(EVAL_COM0, (uint8_t)ch);
    while(RESET == usart_flag_get(EVAL_COM0, USART_FLAG_TBE)) {
    }
    return ch;
}
#endif
