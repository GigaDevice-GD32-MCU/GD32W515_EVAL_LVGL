/* GD32W515P evaluation board definitions. */
#ifndef GD32W515P_EVAL_H
#define GD32W515P_EVAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "gd32w51x.h"

typedef enum {
    LED1 = 0U,
    LED2 = 1U,
    LED3 = 2U
} led_typedef_enum;

typedef enum {
    KEY_TAMPER_WAKEUP = 0U
} key_typedef_enum;

typedef enum {
    KEY_MODE_GPIO = 0,
    KEY_MODE_EXTI = 1
} keymode_typedef_enum;

#define LEDn                             3U
#define LED1_PIN                         GPIO_PIN_6
#define LED1_GPIO_PORT                   GPIOB
#define LED1_GPIO_CLK                    RCU_GPIOB
#define LED2_PIN                         GPIO_PIN_15
#define LED2_GPIO_PORT                   GPIOA
#define LED2_GPIO_CLK                    RCU_GPIOA
#define LED3_PIN                         GPIO_PIN_6
#define LED3_GPIO_PORT                   GPIOA
#define LED3_GPIO_CLK                    RCU_GPIOA

#define COMn                             1U
#define EVAL_COM0                        USART2
#define EVAL_COM0_CLK                    RCU_USART2
#define EVAL_COM0_TX_PIN                 GPIO_PIN_10
#define EVAL_COM0_RX_PIN                 GPIO_PIN_11
#define EVAL_COM0_GPIO_PORT              GPIOB
#define EVAL_COM0_GPIO_CLK               RCU_GPIOB
#define EVAL_COM0_TX_AF                  GPIO_AF_7
#define EVAL_COM0_RX_AF                  GPIO_AF_7

#define KEYn                             1U
#define TAMPER_WAKEUP_KEY_PIN            GPIO_PIN_2
#define TAMPER_WAKEUP_KEY_GPIO_PORT      GPIOA
#define TAMPER_WAKEUP_KEY_GPIO_CLK       RCU_GPIOA
#define TAMPER_WAKEUP_KEY_EXTI_LINE      EXTI_2
#define TAMPER_WAKEUP_KEY_EXTI_PORT_SOURCE EXTI_SOURCE_GPIOA
#define TAMPER_WAKEUP_KEY_EXTI_PIN_SOURCE  EXTI_SOURCE_PIN2
#define TAMPER_WAKEUP_KEY_EXTI_IRQn      EXTI2_IRQn

void gd_eval_led_init(led_typedef_enum lednum);
void gd_eval_led_on(led_typedef_enum lednum);
void gd_eval_led_off(led_typedef_enum lednum);
void gd_eval_led_toggle(led_typedef_enum lednum);
void gd_eval_key_init(key_typedef_enum key_num, keymode_typedef_enum key_mode);
uint8_t gd_eval_key_state_get(key_typedef_enum button);
void gd_eval_com_init(uint32_t com);

#ifdef __cplusplus
}
#endif

#endif
