/*
 * system.h - system hardware device configuration values 
 * Part of TinyG2 project
 *
 * Copyright (c) 2013 Alden S. Hart Jr.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY 
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
/*
 * INTERRUPT USAGE - TinyG uses a lot of them all over the place
 *
 *	HI	Stepper DDA pulse generation		(set in stepper.h)
 *	HI	Stepper load routine SW interrupt	(set in stepper.h)
 *	HI	Dwell timer counter 				(set in stepper.h)
 *  LO	Segment execution SW interrupt		(set in stepper.h) 
 *	MED	GPIO1 switch port					(set in gpio.h)
 *  MED	Serial RX for USB & RS-485			(set in xio_usart.h)
 *  LO	Serial TX for USB & RS-485			(set in xio_usart.h)
 *	LO	Real time clock interrupt			(set in xmega_rtc.h)
 */
#ifndef system_h
#define system_h

void sys_init(void);					// master hardware init
//void sys_port_bindings(double hw_version);
//void sys_get_id(char *id);

#define SYS_ID_LEN 12					// length of system ID string from sys_get_id()

/**** Global System Defines ****/
/* CPU clock */	

#undef F_CPU							// set for delays
#define F_CPU 84000000UL				// should always precede <avr/delay.h>

/**** Resource Assignment ****
 * This section describes what modules use resources such as timers, pins, etc.
 * Base addresses and channels are setup here, see detailed setup may be found 
 * in the referenced modules, e.g. (stepper.h)
 */
/* Timer Counter usage
 The SAM3x8e has 3 timer counter *blocks*, each with 3 *channels*. Usage is:
 
 - TC0 block, channels 0, 1, 2; Reserved for Arduino ADC triggers (adc.h)
 
 - TC1 block; (stepper.h)
 	- channel 0 = DDA interrupt timer	// aka 30 Timer Counter 3 (sam3x8e.h)
	- channel 1 = dwell timer			// aka 31 Timer Counter 4
	- channel 3 = reserved				// aka 32 Timer Counter 5

 - TC2 block; ()
 	- channel 0 = PWM channel, pin D8   // aka 33 Timer Counter 6 (sam3x8e.h)
 	- channel 1 = PWM channel, pin D9	// aka 34 Timer Counter 7
 	- channel 3 = PWM channel, pin D10	// aka 35 Timer Counter 8
 */

// DDA timer aliases
#define TC_BLOCK_DDA		TC1				// TC1 block base address (sam3x8e.h)
#define TC_CHANNEL_DDA		0				// DDA channel in DDA block
#define TC_ID_DDA			ID_TC3			// Device ID enumeration (sam3x8e.h)
#define TC_IRQn_DDA			TC3_IRQn		// Interrupt vector enumeration (same as device ID)
#define ISR_Handler_DDA		TC3_Handler		// alias for ISR handler function name
#define REG_CMR_DDA			REG_TC1_CMR0	// channel mode register
#define REG_CCR_DDA			REG_TC1_CCR0	// channel control register
#define REG_SR_DDA			REG_TC1_SR0		// status register
#define REG_RC_DDA			REG_TC1_RC0		// RC register (comparison register)
#define REG_IER_DDA			REG_TC1_IER0	// interrupt enable register
#define REG_IDR_DDA			REG_TC1_IDR0	// interrupt disable register
#define REG_IMR_DDA			REG_TC1_IMR0	// interrupt mask register

// Dwell timer aliases
#define TC_BLOCK_DWELL		TC1
#define TC_CHANNEL_DWELL	1
#define TC_ID_DWELL			ID_TC4

//#define TIMER_DWELL	 		TCD0		// Dwell timer	(see stepper.h)
//#define TIMER_LOAD			TCE0		// Loader timer	(see stepper.h)
//#define TIMER_EXEC			TCF0		// Exec timer	(see stepper.h)
//#define TIMER_5				TCC1		// unallocated timer
//#define TIMER_PWM1			TCD1		// PWM timer #1 (see pwm.c)
//#define TIMER_PWM2			TCE1		// PWM timer #2	(see pwm.c)


/*** Motor, output bit & switch port assignments ***
 *** These are not all the same, and must line up in multiple places in gpio.h ***
 * Sorry if this is confusing - it's a board routing issue
 */
#define PORT_MOTOR_1	PORTA			// motors mapped to ports
#define PORT_MOTOR_2 	PORTF
#define PORT_MOTOR_3	PORTE
#define PORT_MOTOR_6	PORTD
#define PORT_MOTOR_5	PORTD
#define PORT_MOTOR_6	PORTD

#define PORT_SWITCH_X 	PORTA			// Switch axes mapped to ports
#define PORT_SWITCH_Y 	PORTD
#define PORT_SWITCH_Z 	PORTE
#define PORT_SWITCH_A 	PORTF

#define PORT_OUT_V7_X	PORTA			// v7 mapping - Output bits mapped to ports
#define PORT_OUT_V7_Y 	PORTF
#define PORT_OUT_V7_Z	PORTD
#define PORT_OUT_V7_A	PORTE

#define PORT_OUT_V6_X	PORTA			// v6 and earlier mapping - Output bits mapped to ports
#define PORT_OUT_V6_Y 	PORTF
#define PORT_OUT_V6_Z	PORTE
#define PORT_OUT_V6_A	PORTD

// These next four must be changed when the PORT_MOTOR_* definitions change!
#define PORTCFG_VP0MAP_PORT_MOTOR_1_gc PORTCFG_VP0MAP_PORTA_gc
#define PORTCFG_VP1MAP_PORT_MOTOR_2_gc PORTCFG_VP1MAP_PORTF_gc
#define PORTCFG_VP2MAP_PORT_MOTOR_3_gc PORTCFG_VP2MAP_PORTE_gc
#define PORTCFG_VP3MAP_PORT_MOTOR_4_gc PORTCFG_VP3MAP_PORTD_gc

#define PORT_MOTOR_1_VPORT	VPORT0
#define PORT_MOTOR_2_VPORT	VPORT1
#define PORT_MOTOR_3_VPORT	VPORT2
#define PORT_MOTOR_4_VPORT	VPORT3

/*
 * Port setup - Stepper / Switch Ports:
 *	b0	(out) step			(SET is step,  CLR is rest)
 *	b1	(out) direction		(CLR = Clockwise)
 *	b2	(out) motor enable 	(CLR = Enabled)
 *	b3	(out) microstep 0 
 *	b4	(out) microstep 1
 *	b5	(out) output bit for GPIO port1
 *	b6	(in) min limit switch on GPIO 2 (note: motor controls and GPIO2 port mappings are not the same)
 *	b7	(in) max limit switch on GPIO 2 (note: motor controls and GPIO2 port mappings are not the same)
 */
#define MOTOR_PORT_DIR_gm 0x3F	// dir settings: lower 6 out, upper 2 in

enum cfgPortBits {			// motor control port bit positions
	STEP_BIT_bp = 0,		// bit 0
	DIRECTION_BIT_bp,		// bit 1
	MOTOR_ENABLE_BIT_bp,	// bit 2
	MICROSTEP_BIT_0_bp,		// bit 3
	MICROSTEP_BIT_1_bp,		// bit 4
	GPIO1_OUT_BIT_bp,		// bit 5 (4 gpio1 output bits; 1 from each axis)
	SW_MIN_BIT_bp,			// bit 6 (4 input bits for homing/limit switches)
	SW_MAX_BIT_bp			// bit 7 (4 input bits for homing/limit switches)
};

#define STEP_BIT_bm			(1<<STEP_BIT_bp)
#define DIRECTION_BIT_bm	(1<<DIRECTION_BIT_bp)
#define MOTOR_ENABLE_BIT_bm (1<<MOTOR_ENABLE_BIT_bp)
#define MICROSTEP_BIT_0_bm	(1<<MICROSTEP_BIT_0_bp)
#define MICROSTEP_BIT_1_bm	(1<<MICROSTEP_BIT_1_bp)
#define GPIO1_OUT_BIT_bm	(1<<GPIO1_OUT_BIT_bp)	// spindle and coolant output bits
#define SW_MIN_BIT_bm		(1<<SW_MIN_BIT_bp)		// minimum switch inputs
#define SW_MAX_BIT_bm		(1<<SW_MAX_BIT_bp)		// maximum switch inputs

/* Bit assignments for GPIO1_OUTs for spindle, PWM and coolant */

#define SPINDLE_BIT			0x08		// spindle on/off
#define SPINDLE_DIR			0x04		// spindle direction, 1=CW, 0=CCW
#define SPINDLE_PWM			0x02		// spindle PWMs output bit
#define MIST_COOLANT_BIT	0x01		// coolant on/off - these are the same due to limited ports
#define FLOOD_COOLANT_BIT	0x01		// coolant on/off

#define SPINDLE_LED			0
#define SPINDLE_DIR_LED		1
#define SPINDLE_PWM_LED		2
#define COOLANT_LED			3

#define INDICATOR_LED		SPINDLE_DIR_LED	// can use the spindle direction as an indicator LED

/**** Device singleton - global structure to allow iteration through similar devices ****/
/*
	Ports are shared between steppers and GPIO so we need a global struct.
	Each xmega port has 3 bindings; motors, switches and the output bit

	The initialization sequence is important. the order is:
		- sys_init()	binds all ports to the device struct
		- st_init() 	sets IO directions and sets stepper VPORTS and stepper specific functions
		- gpio_init()	sets up input and output functions and required interrupts	

	Care needs to be taken in routines that use ports not to write to bits that are 
	not assigned to the designated function - ur unpredicatable results will occur
*/
/*
typedef struct deviceSingleton {
//	PORT_t *st_port[MOTORS];	// bindings for stepper motor ports (stepper.c)
//	PORT_t *sw_port[MOTORS];	// bindings for switch ports (GPIO2)
//	PORT_t *out_port[MOTORS];	// bindings for output ports (GPIO1)
} deviceSingleton_t;
deviceSingleton_t device;
*/
#endif
