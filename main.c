/**
 * Asks for input of a date and shows some basic
 * info about that day.
 *
 * Author: Maximilian Fickers
 * Date:   18.12.2018
 **/

#include <stdio.h>
#include <stdlib.h>
#include "time.h"

/**
 * Start here.
 **/
int main()
{
    int day = 26, month = 2, year = 1994;

    input_date(&day, &month, &year);

    printf("\nTag des Jahres: %i\n", day_of_the_year(day, month, year));

    char *weekday;
    switch (day_of_the_week(day, month, year)) {
        case 1: weekday = "Montag"; break;
        case 2: weekday = "Dienstag"; break;
        case 3: weekday = "Mittwoch"; break;
        case 4: weekday = "Donnerstag"; break;
        case 5: weekday = "Freitag"; break;
        case 6: weekday = "Samstag"; break;
        case 7: weekday = "Sonntag"; break;
    }
    printf("Wochentag: %s\n", weekday);

    printf("Kalenderwoche: %i\n", week_number(day, month, year));

    return 0;
}
