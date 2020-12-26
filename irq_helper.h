#ifndef IRQ_HELPER_H
#define IRQ_HELPER_H

/*
* load next cycle to trigger irq2 from irq2in file
*/
int get_next_irq2_cycle();

/*
* update timer and trigger irq2 if neccessary
*/
void increase_timer();

#endif