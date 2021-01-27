#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* dayOfTheWeek(int day, int month, int year) 
{
	char* ret = (char*)malloc(10 * sizeof(char));
	int numyear = 1971, nummonth = 1;
	while (numyear < year)
    {
		if (numyear % 400 == 0 || (numyear % 4 == 0 && numyear % 100 != 0))
			day += 366;
		else
			day += 365;
		numyear++;
	}
	while (nummonth < month)
    {
		if (nummonth == 4 || nummonth == 6 || nummonth == 9 || nummonth == 11)
			day += 30;
		else if (nummonth == 2)
        {
			if (year % 400 == 0 || (numyear % 4 == 0 && numyear % 100 != 0))
				day += 29;
			else
				day += 28;
		}
		else
			day += 31;
		nummonth++;
	}
	day--;
	day %= 7;
	if (day == 0)
		strcpy(ret, "Friday");
	else if (day == 1)
		strcpy(ret, "Saturday");
	else if (day == 2)
		strcpy(ret, "Sunday");
	else if (day == 3)
		strcpy(ret, "Monday");
	else if (day == 4)
		strcpy(ret, "Tuesday");
	else if (day == 5)
		strcpy(ret, "Wednesday");
	else
		strcpy(ret, "Thursday");
	return ret;
}

int main()
{
    int day,month,year;
    printf("请输入年月日");
    scanf("%d%d%d",&year,&month,&day);
    char *p = dayOfTheWeek(day,month,year);
    printf("这是一周中的：%s",p);

    return 0;
}