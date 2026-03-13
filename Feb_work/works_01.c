#include <stdio.h>

int is_leapyear(int year) {
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
        return 1;
    return 0;
}

int get_month_days(int year, int month) {
    int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && is_leapyear(year))
        return 29;
    return days[month - 1];
}

int get_year_days(int year, int month, int day) {
    int sum = 0;
    for (int i = 1; i < month; i++) {
        sum += get_month_days(year, i);
    }
    sum += day;
    return sum;
}

int get_week(int year, int month, int day) {
    int y = year;
    int d = get_year_days(year, month, day);
    int sum = (y - 1) + (y - 1) / 4 - (y - 1) / 100 + (y - 1) / 400 + d;
    return sum % 7;
}

void print_month_calendar(int year, int month) {
    int days = get_month_days(year, month);
    int first_week = get_week(year, month, 1);
    
    printf("SUN MON TUE WED THU FRI SAT\n");
    
    for (int i = 0; i < first_week; i++) {
        printf("    ");
    }
    
    for (int d = 1; d <= days; d++) {
        printf("%3d ", d);
        if ((first_week + d) % 7 == 0)
            printf("\n");
    }
    printf("\n");
}

int main() {
    int year, month;
    printf("Enter year and month: \n");
    scanf("%d %d", &year, &month);
    print_month_calendar(year, month);
    return 0;
}