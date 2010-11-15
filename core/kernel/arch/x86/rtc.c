/* 
 * rtc.c - The architecture level RTC chip driver
 * 
 * 
 * This file is part of the Zuva Operating System. 
 * 
 * (C) Copyright Chris Dyer & Zuvium 2009 - 2010. Do Not Re-distribute. 
 * For internal viewing only.
 * 
 */

#ifdef ARCH_X86
    #include <arch/x86.h>
#endif

#include <time.h>
#include <mm.h>

#define BCDTOBIN(val) ( ( ( val ) & 0x0f ) + ( ( val ) >> 4 ) * 10 ) /* <--- Thanks to the creator of yaosp for this! :) */
#define BINTOBCD(val) ( ( ( ( val ) / 10 ) << 4 ) + ( val ) % 10 )   /* <--- Thanks to the creator of yaosp for this! :) */

time_detailed_t* arch_rtc_get_time()
{
    time_detailed_t *time;// = kmalloc(sizeof(time_detailed_t));
    int8_t port = 0x80; /* The default busy value */
    uint8_t settings;
	
    do 
    {
	outb(RTC_ADDRESS, 0x0A);
        port = inb(RTC_DATA);
    } 
    while (port == 0x80);
	
	/* Get device settings */
    outb(RTC_ADDRESS, 0x0B);
    settings = inb(RTC_DATA);
    
    /* Year */
    outb(RTC_ADDRESS, 0x09);
    time->year = inb(RTC_DATA);

    /* Month */
    outb(RTC_ADDRESS, 0x08);
    time->month = inb(RTC_DATA) - 1;

    /* Day of month */
    outb(RTC_ADDRESS, 0x07);
    time->day = inb(RTC_DATA);

    /* Day of week */
    /*outb(RTC_ADDRESS, 0x06);*/

    /* Hours */
    outb(RTC_ADDRESS, 0x04);
    time->hour = inb(RTC_DATA);

    /* Minutes */
    outb(RTC_ADDRESS, 0x02);
    time->min = inb(RTC_DATA);

    /* Seconds */
    outb(RTC_ADDRESS, 0x00);
    time->sec = inb(RTC_DATA);
    
    if ((settings & 0x04) == 0)
    {
	time->year = BCDTOBIN(time->year);
	time->month = BCDTOBIN(time->month);
	time->day = BCDTOBIN(time->day);
	time->hour = BCDTOBIN(time->hour);
	time->min = BCDTOBIN(time->min);
	time->sec = BCDTOBIN(time->sec);
    }
    
    return time;
}
