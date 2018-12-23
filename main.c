/**
 * Author: Maximilian Fickers
 * Date:   18.12.2018
 *
 * Library of time-related functions.
 **/

#include <stdio.h>
#include <stdlib.h>

#define GREGORIAN_CALENDAR 1582
#define YEAR_UPPER_BOUND 2400
#define LONG_MONTH 31
#define SHORT_MONTH 30
#define FEBRUARY 28

/**
 * Checks if year is valid
 * valid years have to be between 1582 and 2400
 *
 * Returns:
 * 0 for valid years
 * -1 for invalid years
 **/
int is_valid_year (int year)
{
    if (year < GREGORIAN_CALENDAR || year > YEAR_UPPER_BOUND) {
        return -1;
    }

    return 0;
}

/**
 * Checks if a given year is a leap year or not.
 *
 * Returns:
 * 1  for leap years.
 * 0  for regular years.
 * -1 for years before the implementation of the
 *    Gregorian Calendar
 **/
int is_leapyear(int year)
{
    // This leap year calculation was implemented with
    // the Gregorian Calendar
    if (year < GREGORIAN_CALENDAR) {
        return -1;
    }

    // A leap year has to be dividible without remainder by 4
    // but not 100, except when also dividible by 400.
    if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) {
        return 1;
    }

    return 0;
}

/**
 * Returns the number of days for a month
 * of a year
 **/
int get_days_for_month(int month, int year)
{
    // Validate year
    if (is_valid_year(year) == -1) {
        return -1;
    }

    // Returns the number of days for each months, adding a
    // day to February for leap years and returning -1 for
    // invalid months.
    switch (month) {
        // January
        case 1: return LONG_MONTH;
        // February
        case 2: return FEBRUARY + is_leapyear(year);
        // March
        case 3: return LONG_MONTH;
        // April
        case 4: return SHORT_MONTH;
        // May
        case 5: return LONG_MONTH;
        // June
        case 6: return SHORT_MONTH;
        // July
        case 7: return LONG_MONTH;
        // August
        case 8: return LONG_MONTH;
        // September
        case 9: return SHORT_MONTH;
        // October
        case 10: return LONG_MONTH;
        // November
        case 11: return SHORT_MONTH;
        // December
        case 12: return LONG_MONTH;
        // Invalid month
        default: return -1;
    }
}

/**
 * For a given date (day, month, year), returns
 * the total number of days passed for that year
 * including the given day.
 **/
int day_of_the_year(int day, int month, int year)
{
    // Add the days of past months to the current day.
    while (month > 1) {
        day += get_days_for_month(month--, year);
    }

    return day;
}

/**
 * Validates a given date (day, month, year)
 *
 * Returns:
 * 1  for valid dates.
 * 0  for invalid dates.
 **/
int exists_date(int day, int month, int year)
{
    // Validate year
    if (is_valid_year(year) == -1) {
        return 0;
    }

    // Validate month
    if (month < 1 || month > 12) {
        return 0;
    }

    // Validate day
    if (day < 1 || day > get_days_for_month(month, year)) {
        return 0;
    }

    return 1;
}

/**
 * Gets a valid date from console input.
 *
 * TODO: VALIDATION AND DOCUMENTATION
 **/
void input_date (int *day, int *month, int *year)
{
    char date[10];
    int tmp_day = 0, tmp_month = 0, tmp_year = 0;

    do {
        printf("Bitte geben Sie ein Datum ein (dd.mm.yyyy):");
        scanf("%s", date);

        date[2] = '\0';
        tmp_day = atoi(&date[0]);
        date[5] = '\0';
        tmp_month = atoi(&date[3]);
        date[10] = '\0';
        tmp_month = atoi(&date[6]);
    } while (exists_date(tmp_day, tmp_month, tmp_year));

    // Save the valid date to the given pointers.
    day = tmp_day;
    month = tmp_month;
    year = tmp_year;

    return;
}

/**
 * Start
 **/
int main()
{
    int day = 0, month = 0, year = 0;

    input_date(&day, &month, &year);

    printf("Tag des Jahres: %i\n", day_of_the_year(day, month, year));
    printf("Validierung: %i\n", exists_date(day, month, year));
    return 0;
}
