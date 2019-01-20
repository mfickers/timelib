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
int is_valid_year(int year)
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
int day_of_the_year(int day, int month, int year)
{
    // Ignore current month in loop
    month--;
    // Add the days of past months to the current day.
    while (month > 0) {
        day += get_days_for_month(month--, year);
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
int day_of_the_week(int day, int month, int year)
{
    int nyd = new_years_day(year); // Weekday of new year's day.
    int past_days = day_of_the_year(day, month, year) - 1; // Past days of this year.

    // Calculate weekday in the correct format.
    return (nyd + past_days - 1) % 7 + 1;
}

/**
 * Returns the number of the week in the corresponding year based on ISO 8601.
 **/
int week_number(int day, int month, int year)
{
    // Start of first week can go back up to 29th of December.
    // This calculation will work up to the upper limit of years.
    if (month == 12 && day >= 29) {
        int days_to_new_year = 31 - day;

        // Current week is first of next year when Thursday is after 31st.
        if (day_of_the_week(day, month, year) <= (3 - days_to_new_year)) {
            return 1;
        }
    }

    int offset_nyd = 0; // Offset between Monday of week 1 and new year's day.

    // First week is the one with the first Thursday of the year.
    if (new_years_day(year) <= 4) {
        // First week starts in current year => negative or no offset.
        offset_nyd = 1 - new_years_day(year);
    } else {
        // First week starts past New Year's Day => positive offset.
        offset_nyd = 8 - new_years_day(year);
    }

    // Calculate offset to Monday of first week of current year.
    int offset_first_week = day_of_the_year(day, month, year) - offset_nyd - 1;

    printf("Offset to first week %i\n", offset_first_week);

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

    return -1;
}

/**
 * Gets a valid date from console input.
 *
 * TODO: Input validation.
 **/
void input_date (int *day, int *month, int *year)
{
    char date[11];
    int tmp_day = 0, tmp_month = 0, tmp_year = 0;

    // Repeat prompt until input is a valid date.
    do {
        printf("Bitte geben Sie ein Datum ein (dd.mm.yyyy):");
        scanf("%s", date);

        // Cut input string by adding null characters after each segment.
        date[2] = '\0';
        tmp_day = atoi(&date[0]);
        date[5] = '\0';
        tmp_month = atoi(&date[3]);
        date[10] = '\0';
        tmp_year = atoi(&date[6]);
    } while (!exists_date(tmp_day, tmp_month, tmp_year));

    // Save the valid date to the given pointers.
    *day = tmp_day;
    *month = tmp_month;
    *year = tmp_year;

    return;
}

/**
 * Start here.
 **/
int main()
{
    int day = 26, month = 2, year = 1994;

    input_date(&day, &month, &year);

    printf("Tag des Jahres: %i\n", day_of_the_year(day, month, year));

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
