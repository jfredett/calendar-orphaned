/*
  calendar2.c (beta): Written by Tadayoshi Funaba 1997,1999-2002

  This code is in the public domain, but any use of it
  should publically acknowledge its source.

  $Id: calendar2.c,v 1.8 2002-06-08 01:13:16+09 tadf Exp $
*/

#define MODULE

#ifndef NULL
#define NULL 0
#endif

#include <math.h>
#include "calendar2.h"

#undef quotient
#define quotient(m, n) i_quotient(m, n)
static int
i_quotient(int m, int n)
{
  return (int)floor((double)m / (double)n);
}
#undef mod
#define mod(m, n) i_mod(m, n)
static int
i_mod(int m, int n)
{
  int q, r;

  q = quotient(m, n);
  r = m - q * n;
  return r;
}
#undef oddp
#define oddp(n) ((n) % 2)

int
world_leap_year (int year)
{
  return
    mod (year, 4) == 0 &&
    !(mod (year, 400) == 100 ||
      mod (year, 400) == 200 ||
      mod (year, 400) == 300);
}

int
world_last_day_of_month (int month, int year)
{
  if (month == 6 && world_leap_year (year))
    return 31;
  switch (month) {
  case  1: return 31;
  case  2: return 30;
  case  3: return 30;
  case  4: return 31;
  case  5: return 30;
  case  6: return 30;
  case  7: return 31;
  case  8: return 30;
  case  9: return 30;
  case 10: return 31;
  case 11: return 30;
  case 12: return 31;
  default: return 0;
  }
}

int
absolute_from_world (int month, int day, int year)
{
  int sumres;

  {
    int temp, m;
    for (temp = 0, m = 1;
	 (m < month);
	 temp = temp + world_last_day_of_month (m, year), m++)
      ;
    sumres = temp;
  }
  return day
    + sumres
    + (365 * (year - 1))
    + quotient (year - 1, 4)
    - quotient (year - 1, 100)
    + quotient (year - 1, 400);
}

void
world_from_absolute (int date, int *rmonth, int *rday, int *ryear)
{
  int approx, month, day, year;
  int sumres1, sumres2;

  approx = quotient (date, 366);
  {
    int temp, y;
    for (temp = 0, y = approx;
	 (date >= absolute_from_world (1, 1, 1 + y));
	 temp = temp + 1, y++)
      ;
    sumres1 = temp;
  }
  year = approx
    + sumres1;
  {
    int temp, m;
    for (temp = 0, m = 1;
	 (date > absolute_from_world
	  (m, world_last_day_of_month (m, year), year));
	 temp = temp + 1, m++)
      ;
    sumres2 = temp;
  }
  month = 1
    + sumres2;
  day = date
    - (absolute_from_world (month, 1, year) - 1);
  if (rmonth)
    *rmonth = month;
  if (rday)
    *rday = day;
  if (ryear)
    *ryear = year;
}

int
world_day_of_week_from_absolute (int date)
{
  int month, day, year;

  world_from_absolute(date, &month, &day, &year);
  if (month == 12 && day == 31)
    return 7;
  else if (month == 6 && day == 31)
    return 8;
  else
    switch ((month - 1) % 3) {
    case 0: return (day - 1) % 7;
    case 1: return (day + 2) % 7;
    case 2: return (day + 4) % 7;
    }
  /*NOTREACHED*/
  return -1;
}

int
coptic_leap_year (int year)
{
  return mod (year, 4) == 3;
}

int
coptic_last_day_of_month (int month, int year)
{
  if (month == 13 && coptic_leap_year (year))
    return 6;
  switch (month) {
  case  1: return 30;
  case  2: return 30;
  case  3: return 30;
  case  4: return 30;
  case  5: return 30;
  case  6: return 30;
  case  7: return 30;
  case  8: return 30;
  case  9: return 30;
  case 10: return 30;
  case 11: return 30;
  case 12: return 30;
  case 13: return 5;
  default: return 0;
  }
}

#define EPOCH_COPTIC 103605
#define BEFORE_COPTIC (103605 - 1)

int
absolute_from_coptic (int month, int day, int year)
{
  int sumres;

  {
    int temp, m;
    for (temp = 0, m = 1;
	 (m < month);
	 temp = temp + coptic_last_day_of_month (m, year), m++)
      ;
    sumres = temp;
  }
  return day
    + sumres
    + (365 * (year - 1))
    + quotient (year, 4)
    + BEFORE_COPTIC;
}

void
coptic_from_absolute (int date, int *rmonth, int *rday, int *ryear)
{
  int approx, month, day, year;
  int sumres1, sumres2;

  if (date <= BEFORE_COPTIC) {
    if (rmonth)
      *rmonth = 0;
    if (rday)
      *rday = 0;
    if (ryear)
      *ryear = 0;
    return;
  }
  approx = quotient (date - BEFORE_COPTIC, 366);
  {
    int temp, y;
    for (temp = 0, y = approx;
	 (date >= absolute_from_coptic (1, 1, 1 + y));
	 temp = temp + 1, y++)
      ;
    sumres1 = temp;
  }
  year = approx
    + sumres1;
  {
    int temp, m;
    for (temp = 0, m = 1;
	 (date > absolute_from_coptic
	  (m, coptic_last_day_of_month (m, year), year));
	 temp = temp + 1, m++)
      ;
    sumres2 = temp;
  }
  month = 1
    + sumres2;
  day = date - (absolute_from_coptic (month, 1, year) - 1);
  if (rmonth)
    *rmonth = month;
  if (rday)
    *rday = day;
  if (ryear)
    *ryear = year;
}

int
ethiopian_leap_year (int year)
{
  return coptic_leap_year (year);
}

int
ethiopian_last_day_of_month (int month, int year)
{
  return coptic_last_day_of_month (month, year);
}

#define EPOCH_ETHIOPIAN 2796
#define BEFORE_ETHIOPIAN (2796 - 1)

int
absolute_from_ethiopian (int month, int day, int year)
{
  return
    absolute_from_coptic (month, day, year) - (EPOCH_COPTIC - EPOCH_ETHIOPIAN);
}

void
ethiopian_from_absolute (int date, int *rmonth, int *rday, int *ryear)
{
  coptic_from_absolute
    (date + (EPOCH_COPTIC - EPOCH_ETHIOPIAN),
     rmonth, rday, ryear);
}

int
jalaali_leap_year (int year)
{
    return mod (29 + (8 * year), 33) < 8;
}

int
jalaali_last_day_of_month (int month, int year)
{
  if (month == 12 && jalaali_leap_year (year))
      return 30;
  switch (month) {
  case  1: return 31;
  case  2: return 31;
  case  3: return 31;
  case  4: return 31;
  case  5: return 31;
  case  6: return 31;
  case  7: return 30;
  case  8: return 30;
  case  9: return 30;
  case 10: return 30;
  case 11: return 30;
  case 12: return 29;
  default: return 0;
  }
}

#define BEFORE_JALAALI (226895 - 1)

int
absolute_from_jalaali (int month, int day, int year)
{
  int sumres;

  {
    int temp, m;
    for (temp = 0, m = 1;
	 (m < month);
	 temp = temp + jalaali_last_day_of_month (m, year), m++)
      ;
    sumres = temp;
  }
  return day
    + sumres
    + (365 * (year - 1))
    + quotient (21 + (8 * year), 33)
    + BEFORE_JALAALI;
}

void
jalaali_from_absolute (int date, int *rmonth, int *rday, int *ryear)
{
  int approx, month, day, year;
  int sumres1, sumres2;

  if (date <= BEFORE_JALAALI) {
    if (rmonth)
      *rmonth = 0;
    if (rday)
      *rday = 0;
    if (ryear)
      *ryear = 0;
    return;
  }
  approx = quotient (date - BEFORE_JALAALI, 366);
  {
    int temp, y;
    for (temp = 0, y = approx;
	 (date >= absolute_from_jalaali (1, 1, 1 + y));
	 temp = temp + 1, y++)
      ;
    sumres1 = temp;
  }
  year = approx
    + sumres1;
  {
    int temp, m;
    for (temp = 0, m = 1;
	 (date > absolute_from_jalaali
	  (m, jalaali_last_day_of_month (m, year), year));
	 temp = temp + 1, m++)
      ;
    sumres2 = temp;
  }
  month = 1
    + sumres2;
  day = date - (absolute_from_jalaali (month, 1, year) - 1);
  if (rmonth)
    *rmonth = month;
  if (rday)
    *rday = day;
  if (ryear)
    *ryear = year;
}

#include "calendar.h"

#ifndef PLUTO
int
absolute_from_ordinal_calendar (int day, int year)
{
  return absolute_from_gregorian (1, 1, year) + day - 1;
}

void
ordinal_calendar_from_absolute (int date, int *rday, int *ryear)
{
  int month, day, year, date2;

  gregorian_from_absolute (date, &month, &day, &year);
  date2 = absolute_from_gregorian (1, 1, year);
  if (rday)
    *rday = date - date2 + 1;
  if (ryear)
    *ryear = year;
}
#endif

int
absolute_from_julian_day_number (int date) /* chronological */
{
  return date - 1721425;
}

int
julian_day_number_from_absolute (int date) /* chronological */
{
  return date + 1721425;
}

#ifndef PLUTO
int
julian_day_number_from_mjd (int date) /* chronological */
{
  return date + 2400001;
}

int
mjd_from_julian_day_number (int date) /* chronological */
{
  return date - 2400001;
}

int
absolute_from_lilian_day_number (int date)
{
  return date + 577735;
}

int
lilian_day_number_from_absolute (int date)
{
  return date - 577735;
}

int
day_of_week_from_absolute (int date)
{
  return date % 7;
}
#endif /* not PLUTO */


/*
Local Variables:
c-basic-offset: 2
End:
*/
