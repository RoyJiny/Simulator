#ifndef DEFINES_H
#define DEFINES_H

#define OPCODE_MASK 0xFF000
#define RD_MASK 0x00F00
#define RS_MASK 0x000F0
#define RT_MASK 0x0000F
#define PC_MASK 0x003FF
#define MEMORY_SIZE 4096
#define CODE_SIZE 1024
#define MEMORY_LINE_SIZE 8
#define CODE_LINE_SIZE 5

/*TODO: change paths*/
#define IMEMIN "./inputs/imemin.txt"
#define DMEMIN "./inputs/dmemin.txt"
#define IRQ2IN "./inputs/irq2in.txt"
#define DISKIN "./inputs/diskin.txt"

#define DMEMOUT "./outputs/dmemout.txt"
#define DISKOUT "./outputs/diskout.txt"

#define MONITORTXT "./outputs/monitor.txt"
#define MONITORYUV "./outputs/monitor.yuv"

#define REGOUT "./outputs/regout.txt"
#define TRACE "./outputs/trace.txt"
#define HWREGTRACE "./outputs/hwregtrace.txt"
#define CYCLES "./outputs/cycles.txt"
#define LEDS_FILE "./outputs/leds.txt"

typedef enum {
	ADD,
	SUB,
	AND,
	OR,
	XOR,
	MUL,
	SLL,
	SRA,
	SRL,
	BEQ,
	BNE,
	BLT,
	BGT,
	BLE,
	BGE,
	JAL,
	LW,
	SW,
	RETI,
	IN,
	OUT,
	HALT
} OPcode;

typedef enum {
	ZERO,
	IMM,
	V0,
	A0,
	A1,
	T0,
	T1,
	T2,
	T3,
	S0,
	S1,
	S2,
	GP,
	SP,
	FP,
	RA
} Register;

typedef enum {
	IRQ0ENABLE,
	IRQ1ENABLE,
	IRQ2ENABLE,
	IRQ0STATUS,
	IRQ1STATUS,
	IRQ2STATUS,
	IRQHANDLER,
	IRQRETURN,
	CLKS,
	LEDS,
	RESERVED,
	TIMERENABLE,
	TIMERCURRENT,
	TIMERMAX,
	DISKCMD,
	DISKSECTOR,
    DISKBUFFER,
    DISKSTATUS,
    MONITORCMD,
    MONITORX,
    MONITORY,
    MONITORDATA
} IO_Register;

#endif