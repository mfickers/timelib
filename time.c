/**
 * Library of time-related functions
 *
 * Author: Maximilian Fickers
 * Date:   15.01.2019
 **/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "time.h"

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
int _is_valid_year(int year)
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
    if (_is_valid_year(year) == -1) {
        return -1;
    }

    // Returns the number of days for each months, adding a
    // day to February for leap years and returning -1 for
    // invalid months.
    switch (month) {
        case 1: return LONG_MONTH;                   // January
        case 2: return FEBRUARY + is_leapyear(year); // February
        case 3: return LONG_MONTH;                   // March
        case 4: return SHORT_MONTH;                  // April
        case 5: return LONG_MONTH;                   // May
        case 6: return SHORT_MONTH;                  // June
        case 7: return LONG_MONTH;                   // July
        case 8: return LONG_MONTH;                   // August
        case 9: return SHORT_MONTH;                  // September
        case 10: return LONG_MONTH;                  // October
        case 11: return SHORT_MONTH;                 // November
        case 12: return LONG_MONTH;                  // December
        default: return -1;                          // Invalid month
    }
}

/**
 * For a given date (day, month, year), returns
 * the total number of days passed for that year
 * including the given day.
 **/
int day_of_the_year(struct date date)
{
    int month = date.month, day = date.day;

    // Ignore current month in loop
    month--;
    // Add the days of past months to the current day.
    while (month > 0) {
        day += get_days_for_month(month--, date.year);
    }

    return day;
}

/**
 * Calculates the weekday of January 1st for a given
 * year, using  a modified version of Gauss's algorithm.
 *
 * Returns:
 * 1  for Monday
 * ...
 * 7  for Sunday.
 **/
int new_years_day(int year)
{
    return ((1 + 5 * ((year - 1) % 4) + 4 * ((year - 1) % 100) + 6 * ((year - 1) % 400)) % 7 + 6) % 7 + 1;

}

/**
 * Calculates the weekday of a given date.
 *
 * Returns:
 * 1  for Monday
 * ...
 * 7  for Sunday.
 **/
int day_of_the_week(struct date date)
{
    int nyd = new_years_day(date.year); // Weekday of new year's day.
    int past_days = day_of_the_year(date) - 1; // Past days of this year.

    // Calculate weekday in the correct format.
    return (nyd + past_days - 1) % 7 + 1;
}

/**
 * Returns the number of the week in the corresponding year based on ISO 8601.
 **/
int week_number(struct date date)
{
    // Start of first week can go back up to 29th of December.
    // This calculation will work up to the upper limit of years.
    if (date.month == 12 && date.day >= 29) {
        int days_to_new_year = 31 - date.day;

        // Current week is first of next year when Thursday is after 31st.
        if (day_of_the_week(date) <= (3 - days_to_new_year)) {
            return 1;
        }
    }

    int offset_nyd = 0; // Offset between Monday of week 1 and new year's day.

    // First week is the one with the first Thursday of the year.
    if (new_years_day(date.year) <= 4) {
        // First week starts in current year => negative or no offset.
        offset_nyd = 1 - new_years_day(date.year);
    } else {
        // First week starts past New Year's Day => positive offset.
        offset_nyd = 8 - new_years_day(date.year);
    }

    // Calculate offset to Monday of first week of current year.
    int offset_first_week = day_of_the_year(date) - offset_nyd - 1;

    if (offset_first_week >= 0) {
        // Monday of first week is before or on the current day.
        return (offset_first_week / 7) + 1;
    } else {
        // Monday of first week is after the current day.
        return 53;
    }
}

/**
 * Validates a given date (day, month, year)
 *
 * Returns:
 * 1  for valid dates.
 * 0  for invalid dates.
 **/
int exists_date(struct date date)
{
    // Validate year
    if (_is_valid_year(date.year) == -1) {
        return 0;
    }

    // Validate month
    if (date.month < 1 || date.month > 12) {
        return 0;
    }

    // Validate day
    if (date.day < 1 || date.day > get_days_for_month(date.month, date.year)) {
        return 0;
    }

    return -1;
}

/**
 * Gets a valid date from console input.
 **/
void input_date (struct date *date)
{
    const int DATE_LENGTH = 11;

    char input[DATE_LENGTH];
    struct date tmp;

    // Repeat prompt until input is a valid date.
    do {
        tmp.day = 0, tmp.month = 0, tmp.year = 0;

        printf("Bitte geben Sie ein Datum ein (dd.mm.yyyy):");
        scanf("%s", input);

        // Cut string at delimiter.
        for (int i = 0; i < DATE_LENGTH; i++) {
            if (isdigit(input[i])) {
                continue;
            } else if (input[i] == '.') {
                // Delimiter found, replace with NULL-Terminator
                input[i] = '\0';
            } else {
                // Invalid character.
                break;
            }
        }

        int offset = 0;
        tmp.day = atoi(&input[offset]);
        offset += strlen(&input[offset]) + 1;
        tmp.month = atoi(&input[offset]);
        offset += strlen(&input[offset]) + 1;
        tmp.year = atoi(&input[offset]);

    } while (!exists_date(tmp));

    // Save the valid date to the given struct pointer.
    date->day = tmp.day;
    date->month = tmp.month;
    date->year = tmp.year;

    return;
}
