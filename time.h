#ifndef TIME_H_INCLUDED
#define TIME_H_INCLUDED

struct date {
    int day;
    int month;
    int year;
};

int day_of_the_year(struct date date);
void input_date(struct date *date);
int is_leapyear(int year);
int get_days_for_month(int month, int year);
int exists_date(struct date date);
int day_of_the_week(struct date date);
int week_number(struct date date);

#endif // TIME_H_INCLUDED
