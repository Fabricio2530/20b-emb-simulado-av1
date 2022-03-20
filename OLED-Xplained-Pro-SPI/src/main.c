#include <asf.h>

#include "PIO_OLED.h"
#include "PIO_FUNCTIONS.h"
#include "TC-RTT-RTC.h"

#include "gfx_mono_ug_2832hsweg04.h"
#include "gfx_mono_text.h"
#include "sysfont.h"


//Variaveis globais

volatile char led1_ligado = 1;
volatile char led2_ligado = 1;
volatile char led3_ligado = 1;

volatile char tc_flag1;
volatile char tc_flag6;
volatile char tc_flag3;


void checa_tc_1(int flag){
	delay_ms(300);
	if (flag) {
		tc_start(TC0, 1);
	} else {
		tc_stop(TC0, 1);
	}
	
}

void checa_tc_6(int flag){
	delay_ms(300);
	if (flag) {
		tc_start(TC2, 0);
		} else {
		tc_stop(TC2, 0);
	}
	
}

void checa_tc_3(int flag){
	delay_ms(300);
	if (flag) {
		tc_start(TC1, 0);
		} else {
		tc_stop(TC1, 0);
	}
	
}

void init(void) {
	//Initialize the board clock
	sysclk_init();
	oled_init();
	gfx_mono_ssd1306_init();
	
	// Desativa WatchDog Timer
	WDT->WDT_MR = WDT_MR_WDDIS;
}

void TC1_Handler(void) {
	/**
	* Devemos indicar ao TC que a interrupção foi satisfeita.
	* Isso é realizado pela leitura do status do periférico
	**/
	volatile uint32_t status = tc_get_status(TC0, 1);

	/** Muda o estado do LED (pisca) **/
	tc_flag1 = 1;
}

void TC3_Handler(void) {
	/**
	* Devemos indicar ao TC que a interrupção foi satisfeita.
	* Isso é realizado pela leitura do status do periférico
	**/
	volatile uint32_t status = tc_get_status(TC1, 0);

	/** Muda o estado do LED (pisca) **/
	
	tc_flag3 = 1;
	
}

void TC6_Handler(void) {
	/**
	* Devemos indicar ao TC que a interrupção foi satisfeita.
	* Isso é realizado pela leitura do status do periférico
	**/
	volatile uint32_t status = tc_get_status(TC2, 0);

	/** Muda o estado do LED (pisca) **/
	tc_flag6 = 1;
	
}

int main (void)
{
	board_init();
	sysclk_init();
	init();
	delay_init();
	
	TC_init(TC2, ID_TC6, 0, 1);
	tc_start(TC2, 0);
	
	TC_init(TC1, ID_TC3, 0, 10);
	tc_start(TC1, 0);
	
	TC_init(TC0, ID_TC1, 1, 5);
	tc_start(TC0, 1);

	gfx_mono_draw_string("Aguardando", 0,16, &sysfont);
  /* Insert application code here, after the board has been initialized. */
	while(1) {
			
			if (flag_but1) {
				gfx_mono_draw_string("Botao 1   ", 0,16, &sysfont);
				led1_ligado = !led1_ligado;
				checa_tc_1(led1_ligado);
				flag_but1 = 0;
			}
				
			if (flag_but2) {
				gfx_mono_draw_string("Botao 2   ", 0,16, &sysfont);
				led2_ligado = !led2_ligado;
				checa_tc_3(led2_ligado);
				flag_but2 = 0;
			}
			
			if (flag_but3) {
				gfx_mono_draw_string("Botao 3    ", 0,16, &sysfont);
				led3_ligado = !led3_ligado;
				checa_tc_6(led3_ligado);
				flag_but3 = 0;
			}
			
			
			if (tc_flag1) {
				pin_toggle(LED_PI1, LED_PI1_IDX_MASK);
				tc_flag1 = 0;
			}
			
			if (tc_flag3) {
				pin_toggle(LED_PI2, LED_PI2_IDX_MASK);
				tc_flag3 = 0;
			}
			
			if (tc_flag6) {
				
				pin_toggle(LED_PI3, LED_PI3_IDX_MASK);
				tc_flag6 = 0;
			}
			
			pmc_sleep(SAM_PM_SMODE_SLEEP_WFI);
			
	}
}
