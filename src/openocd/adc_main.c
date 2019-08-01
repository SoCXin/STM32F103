/* adc_serial.c
 *
 * (c) Tom Trebisky  11-16-2017
 *
 * Derived from my usb1.c
 * which was derived from inter.c
 *
 * My goal here is to write a driver for the STM32 adc
 * I don't have working USB code yet, so I will need
 * to communicate via serial.
 *
 * This uses a serial console, which I connect as follows:
 *  I use a CP2102 usb to serial gadget, which has 5 pins and
 *  works with 3.3 volt levels.  I make 3 connections:
 *  -- ground
 *  -- Tx on the CP2102 goes to A10 on the STM32
 *  -- Rx on the CP2102 goes to A9 on the STM32
 *
 * Note that the board does not need to be powered via USB.
 * It is sufficient to power it from the ST-LINK.
 * So I have two USB cables during development.
 *  -- one to the ST-Link
 *  -- one to the CP2102 serial/usb gadget
 *
 * Notable things added to this code:
 *  -- add character read features to serial.c
 *
 * Note that uart1 is on pins A9 and A10
 *  (it could alternately be on pins B6 and B7)
 *
 *  Uart 2 could be on pins A2 and A3
 *  Uart 3 could be on pins B10 and B11
 */

void rcc_init ( void );
void adc_init ( void );
void led_init ( int );

void led_on ( void );
void led_off ( void );

/* These are just stubs because I am too lazy to comment out
 * the calls in locore.s
 */
void usb_hp_handler ( void ) {}
void usb_lp_handler ( void ) {}
void usb_wk_handler ( void ) {}

/* We have an LED on this pin */
#define PC13	13

/* We use A2 to enable the test */
#define ENABLE	2

/* Sorta close to 1 ms with 72 Mhz processor */
static void
delay_one_ms ( void )
{
	volatile int count = 7273;

	while ( count-- )
	    ;
}

void
delay_ms ( int ms )
{
	while ( ms-- )
	    delay_one_ms ();
}

#ifdef LED_DEMO
/* By itself, with an 8 Mhz clock this gives a blink rate of about 2.7 Hz
 *  so the delay is about 185 ms
 * With a 72 Mhz clock this yields a 27.75 ms delay
 */
void
little_delay ( void )
{
	volatile int count = 1000 * 200;

	while ( count-- )
	    ;
}

/* We scale the above to try to get a 500 ms delay */
void
big_delay ( void )
{
	volatile int count = 1000 * 540;

	while ( count-- )
	    ;
}

#define NBLINKS		2

/* Turn the LED on for a pulse */
static void
led_show ( void )
{
	led_on ();
	little_delay ();
	led_off ();
}

static void
led_demo ( void )
{
	int i;


	for ( ;; ) {
	    for ( i=0; i<NBLINKS; i++ ) {
		led_on ();
		little_delay ();
		led_off ();
		little_delay ();
	    }

	    big_delay ();
	}
}
#endif /* LED_DEMO */

extern volatile unsigned long systick_count;

#define CHAN_TEMP	16
#define CHAN_VREF	17

void
adc_test ( void )
{
	int i;

	/* Make A0 an analog input */
	gpio_a_analog ( 0 );
	adc_set_chan ( 0 );

	printf ( " ADC channel 0\n" );
	for ( i=0; i<10; i++ ) {
	    adc_start ();
	    delay_ms (1000);
	}

	printf ( " temp\n" );
	adc_set_chan ( CHAN_TEMP );
	for ( i=0; i<10; i++ ) {
	    adc_start ();
	    delay_ms (1000);
	}

	printf ( " vref\n" );
	adc_set_chan ( CHAN_VREF );
	for ( i=0; i<10; i++ ) {
	    adc_start ();
	    delay_ms (1000);
	}
}

static int
my_strlen ( char *s )
{
	int rv = 0;

	while ( *s++ )
	    rv++;
	return rv;
}

void
startup ( void )
{
	int t;
	unsigned long systick_next;
	int x;
	char buf[80];
	char *p;

	rcc_init ();

	serial_init ();
	printf ( " -- Booting ------------------------------\n" );

	adc_init ();

	led_init ( PC13 );
	led_off ();

	gpio_a_output ( ENABLE );

	/* This gives us a 1 us interrupt rate !  */
	// systick_init_int ( 72 );

	/* This gives a 1 ms rate */
	// systick_init_int ( 72 * 1000 );

	systick_init_int ( 72 * 1000 * 2000 );

	/* Pull low to turn on NPN bipolar transistor */
	// gpio_a_set ( ENABLE, 1 );
	// for ( ;; ) ;

	x = 0;
	for ( ;; ) {
	    delay_ms ( 1000 );
	    if ( x ) {
		gpio_a_set ( ENABLE, 1 );
		x = 0;
	    } else {
		gpio_a_set ( ENABLE, 0 );
		x = 1;
	    }
	}

	// adc_test ();

	/* Wait for a command */
	serial_getl ( buf );

	// x = 1;
	x = my_strlen ( buf );
	for ( ;; ) {
	    // printf ( "cmd: %d %s\n", x, buf );
	    printf ( "---\n" );
	    for ( p=buf; *p; p++ ) 
		printf ( " %02x\n", *p );
	    delay_ms ( 2000 );
	    // x++;
	}

#ifdef notdef
	printf ( "Hello sailor ...\n" );

	for ( ;; ) {
	    // x = serial_getc ();
	    // printf ( "Read: %02x\n", x );
	    serial_getl ( buf );
	    printf ( "GOT: %s", buf );
	    // serial_puts ( buf );
	}
	x = 0;
	for ( ;; ) {
	    if ( serial_check() ) {
		// printf ( "Wait %d\n", x );
		serial_getl ( buf );
		serial_puts ( buf );
		x = 0;
	    }
	    x++;
	}
#endif

	// serial_puts ( "Hello World\n" );
	/*
	printf ( "Hello sailor ...\n" );
	t = 0xdeadbeef;
	printf ( "Data: %08x\n", t );
	printf ( "Data: %08x\n", 0x1234abcd );
	*/

/* With this timer running, I see a waveform on A1, A2, A3 that is
 * 2 us high, 2 us low (250 kHz).
 * The timer I set up is indeed running at 250 kHz and is tim2
 * These pins are:
 *  A1 = PWM2/2
 *  A2 = PWM2/3
 *  A3 = PWM2/4
 */
	// timer_init ();

	// led_demo ();

	serial_puts ( "Main is spinning\n" );

#ifdef notdef
	// printf ( "systick count %d\n", systick_count );
	systick_next = systick_count + 1000;

	for ( ;; ) {
	    // printf ( "Tock: %d %d\n", systick_count, systick_next );
	    if ( systick_count > systick_next ) {
		printf ( "Tick: %d\n", systick_count );
		systick_next += 1000;
	    }
	}
#endif

	for ( ;; )
	    ;
}

/* THE END */
