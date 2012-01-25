/*
  calendar.c: Translated by Tadayoshi Funaba 1996-2001

  The original Common Lisp code is from ``Calendrical
  Calculations'' by Nachum Dershowitz and Edward
  M. Reingold, Software---Practice & Experience,
  vol. 20, no. 9 (September, 1990), pp. 899--928 and
  from ``Calendrical Calculations, II: Three Historical
  Calendars'' by Edward M. Reingold, Nachum Dershowitz,
  and Stewart M. Clamen, Software---Practice & Experience,
  vol. 23, no. 4 (April, 1993), pp. 383--404.

  This code is in the public domain, but any use of it
  should publically acknowledge its source.

  $Id: calendar.c,v 1.8 2001-05-13 00:58:56+09 tadf Exp $
*/

#define MODULE

#ifndef NULL
#define NULL 0
#endif

#include <math.h>
#include "calendar.h"

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
gregorian_leap_year (int year)
{
  return
    mod (year, 4) == 0 &&
    !(mod (year, 400) == 100 ||
      mod (year, 400) == 200 ||
      mod (year, 400) == 300);
}

int
last_day_of_gregorian_month (int month, int year)
{
  if (month == 2 && gregorian_leap_year (year))
    return 29;
  switch (month) {
  case  1: return 31;
  case  2: return 28;
  case  3: return 31;
  case  4: return 30;
  case  5: return 31;
  case  6: return 30;
  case  7: return 31;
  case  8: return 31;
  case  9: return 30;
  case 10: return 31;
  case 11: return 30;
  case 12: return 31;
  default: return 0;
  }
}

int
absolute_from_gregorian (int month, int day, int year)
{
  int sumres;

  {
    int temp, m;
    for (temp = 0, m = 1;
	 (m < month);
	 temp = temp + last_day_of_gregorian_month (m, year), m++)
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
gregorian_from_absolute (int date, int *rmonth, int *rday, int *ryear)
{
  int approx, month, day, year;
  int sumres1, sumres2;

  approx = quotient (date, 366);
  {
    int temp, y;
    for (temp = 0, y = approx;
	 (date >= absolute_from_gregorian (1, 1, 1 + y));
	 temp = temp + 1, y++)
      ;
    sumres1 = temp;
  }
  year = approx
    + sumres1;
  {
    int temp, m;
    for (temp = 0, m = 1;
	 (date > absolute_from_gregorian
	  (m, last_day_of_gregorian_month (m, year), year));
	 temp = temp + 1, m++)
      ;
    sumres2 = temp;
  }
  month = 1
    + sumres2;
  day = date
    - (absolute_from_gregorian (month, 1, year) - 1);
  if (rmonth)
    *rmonth = month;
  if (rday)
    *rday = day;
  if (ryear)
    *ryear = year;
}

int
Kday_on_or_before (int date, int k)
{
  return date - mod (date - k, 7);
}

int
absolute_from_iso (int week, int day, int year)
{
  return Kday_on_or_before (absolute_from_gregorian (1, 4, year), 1)
    + (7 * (week - 1))
    + (day - 1);
}

void
iso_from_absolute (int date, int *rweek, int *rday, int *ryear)
{
  int approx, week, day, year;

  gregorian_from_absolute (date - 3, NULL, NULL, &approx);
  year = (date >= absolute_from_iso (1, 1, 1 + approx)) ?
    1 + approx : approx;
  week = 1 + quotient (date - absolute_from_iso (1, 1, year), 7);
  day = (mod (date, 7) == 0) ?
    7 : mod (date, 7);
  if (rweek)
    *rweek = week;
  if (rday)
    *rday = day;
  if (ryear)
    *ryear = year;
}

int
julian_leap_year (int year)
{
#ifndef PAPER
  return year != 4 && mod (year, 4) == 0;
#else
  return mod (year, 4) == 0;
#endif
}

int
last_day_of_julian_month (int month, int year)
{
  if (month == 2 && julian_leap_year (year))
    return 29;
  switch (month) {
  case  1: return 31;
  case  2: return 28;
  case  3: return 31;
  case  4: return 30;
  case  5: return 31;
  case  6: return 30;
  case  7: return 31;
  case  8: return 31;
  case  9: return 30;
  case 10: return 31;
  case 11: return 30;
  case 12: return 31;
  default: return 0;
  }
}

int
absolute_from_julian (int month, int day, int year)
{
  int sumres;

  {
    int temp, m;
    for (temp = 0, m = 1;
	 (m < month);
	 temp = temp + last_day_of_julian_month (m, year), m++)
      ;
    sumres = temp;
  }
  return day
    + sumres
    + (365 * (year - 1))
    + quotient (year - 1, 4)
#ifndef PAPER
    - ((year > 4) ? 2 : 1);
#else
    - 2;
#endif
}

void
julian_from_absolute (int date, int *rmonth, int *rday, int *ryear)
{
  int approx, month, day, year;
  int sumres1, sumres2;

#ifndef PAPER
  approx = quotient (date + 1, 366);
#else
  approx = quotient (date + 2, 366);
#endif
  {
    int temp, y;
    for (temp = 0, y = approx;
	 (date >= absolute_from_julian (1, 1, 1 + y));
	 temp = temp + 1, y++)
      ;
    sumres1 = temp;
  }
  year = approx
    + sumres1;
  {
    int temp, m;
    for (temp = 0, m = 1;
	 (date > absolute_from_julian
	  (m, last_day_of_julian_month (m, year), year));
	 temp = temp + 1, m++)
      ;
    sumres2 = temp;
  }
  month = 1
    + sumres2;
  day = date - (absolute_from_julian (month, 1, year) - 1);
  if (rmonth)
    *rmonth = month;
  if (rday)
    *rday = day;
  if (ryear)
    *ryear = year;
}

int
islamic_leap_year (int year)
{
  return mod (14 + (11 * year), 30) < 11;
}

int
last_day_of_islamic_month (int month, int year)
{
  return (oddp (month) ||
	  ((month == 12) && islamic_leap_year (year))) ?
    30 : 29;
}

int
absolute_from_islamic (int month, int day, int year)
{
  return day
    + (29 * (month - 1))
    + quotient (month, 2)
    + ((year - 1) * 354)
    + quotient (3 + (11 * year), 30)
    + 227014;
}

void
islamic_from_absolute (int date, int *rmonth, int *rday, int *ryear)
{
  int approx, month, day, year;
  int sumres1, sumres2;

  if (date <= 227014) {
    if (rmonth)
      *rmonth = 0;
    if (rday)
      *rday = 0;
    if (ryear)
      *ryear = 0;
    return;
  }
  approx = quotient (date - 227014, 355);
  {
    int temp, y;
    for (temp = 0, y = approx;
	 (date >= absolute_from_islamic (1, 1, 1 + y));
	 temp = temp + 1, y++)
      ;
    sumres1 = temp;
  }
  year = approx
    + sumres1;
  {
    int temp, m;
    for (temp = 0, m = 1;
	 (date > absolute_from_islamic
	  (m, last_day_of_islamic_month (m, year), year));
	 temp = temp + 1, m++)
      ;
    sumres2 = temp;
  }
  month = 1
    + sumres2;
  day = date
    - (absolute_from_islamic (month, 1, year) - 1);
  if (rmonth)
    *rmonth = month;
  if (rday)
    *rday = day;
  if (ryear)
    *ryear = year;
}

int
hebrew_leap_year (int year)
{
  return mod (1 + (7 * year), 19) < 7;
}

int
last_month_of_hebrew_year (int year)
{
  return (hebrew_leap_year (year)) ?
    13 : 12;
}

int
last_day_of_hebrew_month (int month, int year)
{
  return
    ((month == 2 ||
      month == 4 ||
      month == 6 ||
      month == 10 ||
      month == 13) ||
     (month == 12 && !hebrew_leap_year (year)) ||
     (month == 8 && !long_heshvan (year)) ||
     (month == 9 && short_kislev (year))) ?
    29 : 30;
}

int
hebrew_calendar_elapsed_days (int year)
{
  int months_elapsed, parts_elapsed, hours_elapsed,
    day, parts, alternative_day;

  months_elapsed = (235 * quotient (year - 1, 19))
    + (12 * mod (year - 1, 19))
    + quotient (1 + (7 * mod (year - 1, 19)), 19);
  parts_elapsed = 204
    + (793 * mod (months_elapsed, 1080));
  hours_elapsed = 5
    + (12 * months_elapsed)
    + (793 * quotient (months_elapsed, 1080))
    + quotient (parts_elapsed, 1080);
  day = 1
    + (29 * months_elapsed)
    + quotient (hours_elapsed, 24);
  parts = (1080 * mod (hours_elapsed, 24))
    + mod (parts_elapsed, 1080);
  alternative_day =
    ((parts >= 19440) ||
     ((mod (day, 7) == 2) && (parts >= 9924)
      && !hebrew_leap_year (year)) ||
     ((mod (day, 7) == 1) && (parts >= 16789)
      && hebrew_leap_year (year - 1))) ?
    1 + day : day;
  return ((mod (alternative_day, 7) == 0) ||
	  (mod (alternative_day, 7) == 3) ||
	  (mod (alternative_day, 7) == 5)) ?
    1 + alternative_day : alternative_day;
}

int
days_in_hebrew_year (int year)
{
  return
    hebrew_calendar_elapsed_days (1 + year)
    - hebrew_calendar_elapsed_days (year);
}

int
long_heshvan (int year)
{
  return mod (days_in_hebrew_year (year), 10) == 5;
}

int
short_kislev (int year)
{
  return mod (days_in_hebrew_year (year), 10) == 3;
}

int
absolute_from_hebrew (int month, int day, int year)
{
  if (month < 7) {
    int sumres1, sumres2;

    {
      int temp, m;
      for (temp = 0, m = 7;
	   (m <= last_month_of_hebrew_year (year));
	   temp = temp + last_day_of_hebrew_month (m, year), m++)
	;
      sumres1 = temp;
    }
    {
      int temp, m;
      for (temp = 0, m = 1;
	   (m < month);
	   temp = temp + last_day_of_hebrew_month (m, year), m++)
	;
      sumres2 = temp;
    }
    return day
      + sumres1
      + sumres2
      + hebrew_calendar_elapsed_days (year)
      - 1373429;
  } else {
    int sumres;

    {
      int temp, m;
      for (temp = 0, m = 7;
	   (m < month);
	   temp = temp + last_day_of_hebrew_month (m, year), m++)
	;
      sumres = temp;
    }
    return day
      + sumres
      + hebrew_calendar_elapsed_days (year)
      - 1373429;
  }
}

void
hebrew_from_absolute (int date, int *rmonth, int *rday, int *ryear)
{
  int approx, year, start, month, day;
  int sumres1, sumres2;

  approx = quotient (date + 1373429, 366);
  {
    int temp, y;
    for (temp = 0, y = approx;
	 (date >= absolute_from_hebrew (7, 1, 1 + y));
	 temp = temp + 1, y++)
      ;
    sumres1 = temp;
  }
  year = approx
    + sumres1;
  start = (date < absolute_from_hebrew (1, 1, year)) ?
    7 : 1;
  {
    int temp, m;
    for (temp = 0, m = start;
	 (date > absolute_from_hebrew
	  (m, last_day_of_hebrew_month (m, year), year));
	 temp = temp + 1, m++)
      ;
    sumres2 = temp;
  }
  month = start
    + sumres2;
  day = date
    - (absolute_from_hebrew (month, 1, year) - 1);
  if (rmonth)
    *rmonth = month;
  if (rday)
    *rday = day;
  if (ryear)
    *ryear = year;
}

#if !defined(PLUTO) && !defined(MODULE)
int
independence_day (int year)
{
  return absolute_from_gregorian (7, 4, year);
}
#endif

#ifndef PLUTO
int
Nth_Kday (int n, int k, int month, int year)
{
  if (n > 0)
    return
      Kday_on_or_before
      (absolute_from_gregorian (month, 7, year), k)
      + (7 * (n - 1));
  return
    Kday_on_or_before
    (absolute_from_gregorian
     (month,
      last_day_of_gregorian_month (month, year),
      year),
     k)
    + (7 * (1 + n));
}
#endif

#if !defined(PLUTO) && !defined(MODULE)
int
labor_day (int year)
{
  return Nth_Kday (1, 1, 9, year);
}

int
memorial_day (int year)
{
  return Nth_Kday (-1, 1, 5, year);
}

int
daylight_savings_start (int year)
{
  return Nth_Kday (1, 0, 4, year);
}

int
daylight_savings_end (int year)
{
  return Nth_Kday (-1, 0, 10, year);
}

int
christmas (int year)
{
  return absolute_from_gregorian (12, 25, year);
}

int
advent (int year)
{
  return Kday_on_or_before (absolute_from_gregorian (12, 3, year), 0);
}

int
epiphany (int year)
{
  return 12 + christmas (year);
}

int
eastern_orthodox_christmas (int year)
{
  int jan1, dec31, y, c1, c2;

  jan1 = absolute_from_gregorian (1, 1, year);
  dec31 = absolute_from_gregorian (12, 31, year);
  julian_from_absolute (jan1, NULL, NULL, &y);
  c1 = absolute_from_julian (12, 25, y);
  c2 = absolute_from_julian (12, 25, (1 + y));
  if (jan1 <= c1 && c1 <= dec31)
    return c1;
  else if (jan1 <= c2 && c2 <= dec31)
    return c2;
  return 0;
}
#endif /* not PLUTO and not MODULE */

int
nicaean_rule_easter (int year)
{
  int shifted_epact, paschal_moon;

  shifted_epact = mod (14 + (11 * mod (year, 19)), 30);
  paschal_moon = absolute_from_julian (4, 19, year)
    - shifted_epact;
  return Kday_on_or_before (paschal_moon + 7, 0);
}

int
easter (int year)
{
  int century, shifted_epact, adjusted_epact, paschal_moon;

  century = 1 + quotient (year, 100);
  shifted_epact = mod (14 + (11 * mod (year, 19))
		       - quotient (3 * century, 4)
		       + quotient (5 + (8 * century), 25)
		       + (30 * century),
		       30);
  adjusted_epact = ((shifted_epact == 0)
		    || ((shifted_epact == 1) && (10 < mod (year, 19)))) ?
    1 + shifted_epact : shifted_epact;
  paschal_moon = absolute_from_gregorian (4, 19, year)
    - adjusted_epact;
  return Kday_on_or_before (paschal_moon + 7, 0);
}

#if !defined(PLUTO) && !defined(MODULE)
int
pentecost (int year)
{
  return 49 + easter (year);
}

void
islamic_date (int month, int day, int year, int date[3])
{
  int jan1, dec31, y, date1, date2, date3;

  jan1 = absolute_from_gregorian (1, 1, year);
  dec31 = absolute_from_gregorian (12, 31, year);
  islamic_from_absolute (jan1, NULL, NULL, &y);
  date1 = absolute_from_islamic (month, day, y);
  date2 = absolute_from_islamic (month, day, 1 + y);
  date3 = absolute_from_islamic (month, day, 2 + y);
  date[0] = (jan1 <= date1 && date1 <= dec31) ?
    date1 : 0;
  date[1] = (jan1 <= date2 && date2 <= dec31) ?
    date2 : 0;
  date[2] = (jan1 <= date3 && date3 <= dec31) ?
    date3 : 0;
}

void
mulad_al_nabi (int year, int date[3])
{
  islamic_date (3, 12, year, date);
}

int
yom_kippur (int year)
{
  return absolute_from_hebrew (7, 10, year + 3761);
}

int
passover (int year)
{
  return absolute_from_hebrew (1, 15, year + 3760);
}

int
purim (int year)
{
  return
    absolute_from_hebrew
    (last_month_of_hebrew_year (year + 3760),
     14,
     year + 3760);
}

int
ta_anit_esther (int year)
{
  int purim_date;

  purim_date = purim (year);
  return (mod (purim_date, 7) == 0) ?
    purim_date - 3 : purim_date - 1;
}

int
tisha_b_av (int year)
{
  int ninth_of_av;

  ninth_of_av = absolute_from_hebrew (5, 9, year + 3760);
  return (mod (ninth_of_av, 7) == 6) ?
    1 + ninth_of_av : ninth_of_av;
}

int
hebrew_birthday (int birth_month, int birth_day, int birth_year, int year)
{
  return
    (birth_month == last_month_of_hebrew_year (birth_year)) ?
    absolute_from_hebrew
    (last_month_of_hebrew_year (year), birth_day, year) :
    absolute_from_hebrew
    (birth_month, birth_day, year);
}

int
yahrzeit (int death_month, int death_day, int death_year, int year)
{
  if (death_month == 8 &&
      death_day == 30 &&
      !long_heshvan (1 + death_year))
    return absolute_from_hebrew (9, 1, year) - 1;
  if (death_month == 9 &&
      death_day == 30 &&
      !short_kislev (1 + death_year))
    return absolute_from_hebrew (10, 1, year) - 1;
  if (death_month == 13)
    return absolute_from_hebrew
      (last_month_of_hebrew_year (year), death_day, year);
  if (death_day == 30 &&
      death_month == 12 &&
      !hebrew_leap_year (year))
    return absolute_from_hebrew (11, 30, year);
  return absolute_from_hebrew (death_month, death_day, year);
}
#endif /* not PLUTO and not MODULE */

#ifndef SPINDEN_CORRELATION
#ifndef PAPER
/* GMT - Sept 6, 3114 BCE (Julian) */
#define mayan_days_before_absolute_zero 1137142
#else
/* GMT - Sept 8, 3114 BCE (Julian) */
#define mayan_days_before_absolute_zero 1137140
#endif
#else
/* SPINDEN - Nov 11, 3374 BCE (Julian) */
#define mayan_days_before_absolute_zero 1232041
#endif

int
absolute_from_mayan_long_count
(int baktun, int katun, int tun, int uinal, int kin)
{
  return baktun * 144000
    + katun * 7200
    + tun * 360
    + uinal * 20
    + kin
    - mayan_days_before_absolute_zero;
}

void
mayan_long_count_from_absolute
(int date, int *rbaktun, int *rkatun, int *rtun, int *ruinal, int *rkin)
{
  int long_count,
    baktun, day_of_baktun,
    katun, day_of_katun,
    tun, day_of_tun,
    uinal, kin;

  long_count = date + mayan_days_before_absolute_zero;
  baktun = quotient (long_count, 144000);
  day_of_baktun = mod (long_count, 144000);
  katun = quotient (day_of_baktun, 7200);
  day_of_katun = mod (day_of_baktun, 7200);
  tun = quotient (day_of_katun, 360);
  day_of_tun = mod (day_of_katun, 360);
  uinal = quotient (day_of_tun, 20);
  kin = mod (day_of_tun, 20);
  if (rbaktun)
    *rbaktun = baktun;
  if (rkatun)
    *rkatun = katun;
  if (rtun)
    *rtun = tun;
  if (ruinal)
    *ruinal = uinal;
  if (rkin)
    *rkin = kin;
}

#define mayan_haab_at_epoch_day 8
#define mayan_haab_at_epoch_month 18

void
mayan_haab_from_absolute (int date, int *rday, int *rmonth)
{
  int long_count, day_of_haab, day, month;

  long_count = date + mayan_days_before_absolute_zero;
  day_of_haab = mod
    (long_count
     + mayan_haab_at_epoch_day
     + (20 * (mayan_haab_at_epoch_month - 1)),
     365);
  day = mod (day_of_haab, 20);
  month = 1 + quotient (day_of_haab, 20);
  if (rday)
    *rday = day;
  if (rmonth)
    *rmonth = month;
}

int
mayan_haab_difference (int day1, int month1, int day2, int month2)
{
  return mod ((20 * (month2 - month1))
	      + (day2 - day1),
	      365);
}

int
mayan_haab_on_or_before (int haab_day, int haab_month, int date)
{
  int zero_day, zero_month;

  mayan_haab_from_absolute (0, &zero_day, &zero_month);
  return date
    - mod (date
	   - mayan_haab_difference
	   (zero_day, zero_month, haab_day, haab_month),
	   365);
}

static int
adjusted_mod (int m, int n)
{
  return 1 + mod (m - 1, n);
}

#define mayan_tzolkin_at_epoch_number 4
#define mayan_tzolkin_at_epoch_name 20

void
mayan_tzolkin_from_absolute (int date, int *rnumber, int *rname)
{
  int long_count, number, name;

  long_count = date + mayan_days_before_absolute_zero;
  number = adjusted_mod (long_count + mayan_tzolkin_at_epoch_number,
			 13);
  name = adjusted_mod (long_count + mayan_tzolkin_at_epoch_name,
		       20);
  if (rnumber)
    *rnumber = number;
  if (rname)
    *rname = name;
}

int
mayan_tzolkin_difference (int number1, int name1, int number2, int name2)
{
  int number_difference, name_difference;

  number_difference = number2 - number1;
  name_difference = name2 - name1;
  return mod (number_difference
	      + (13 * mod (3 * (number_difference - name_difference),
			   20)),
	      260);
}

int
mayan_tzolkin_on_or_before (int number, int name, int date)
{
  int zero_number, zero_name;

  mayan_tzolkin_from_absolute (0, &zero_number, &zero_name);
  return date
    - mod (date
	   - mayan_tzolkin_difference
	   (zero_number, zero_name,
	    number, name),
	   260);
}

int
mayan_haab_tzolkin_on_or_before
(int day, int month, int number, int name, int date)
{
  int zero_day, zero_month, zero_number, zero_name,
    haab_difference, tzolkin_difference, difference;

  mayan_haab_from_absolute (0, &zero_day, &zero_month);
  haab_difference = mayan_haab_difference (zero_day, zero_month,
					   day, month);
  mayan_tzolkin_from_absolute (0, &zero_number, &zero_name);
  tzolkin_difference = mayan_tzolkin_difference (zero_number, zero_name,
						 number, name);
  difference = tzolkin_difference - haab_difference;
  if (mod (difference, 5) == 0)
    return date
      - mod (date
	     - (haab_difference + (365 * difference)),
	     18980);
  return 0;
}

int
french_last_day_of_month (int month, int year)
{
  if (month < 13)
    return 30;
  else if (french_leap_year (year))
    return 6;
  return 5;
}

int
french_leap_year (int year)
{
  return
    (year == 3 ||
     year == 7 ||
     year == 11) ||
    (year == 15 ||
     year == 20) ||
    (year > 20 &&
     (0 == mod (year, 4)) &&
     !((mod (year, 400) == 100) ||
       (mod (year, 400) == 200) ||
       (mod (year, 400) == 300)) &&
     !(0 == mod (year, 4000)));
}

int
absolute_from_french (int month, int day, int year)
{
  return 654414
    + (365 * (year - 1))
    + ((year < 20) ?
       (quotient (year, 4)) :
       (quotient (year - 1, 4)
	- quotient (year - 1, 100)
	+ quotient (year - 1, 400)
	- quotient (year - 1, 4000)))
    + (30 * (month - 1))
    + day;
}

void
french_from_absolute (int date, int *rmonth, int *rday, int *ryear)
{
  int approx, year, month, day;
  int sumres1, sumres2;

  if (date < 654415) {
    if (rmonth)
      *rmonth = 0;
    if (rday)
      *rday = 0;
    if (ryear)
      *ryear = 0;
    return;
  }
  approx = quotient (date - 654414, 366);
  {
    int temp, y;
    for (temp = 0, y = approx;
	 (date >= absolute_from_french (1, 1, (1 + y)));
	 temp = temp + 1, y++)
      ;
    sumres1 = temp;
  }
  year = approx
    + sumres1;
  {
    int temp, m;
    for (temp = 0, m = 1;
	 (date > absolute_from_french
	  (m, french_last_day_of_month (m, year), year));
	 temp = temp + 1, m++)
      ;
    sumres2 = temp;
  }
  month = 1
    + sumres2;
  day = date - (absolute_from_french (month, 1, year) - 1);
  if (rmonth)
    *rmonth = month;
  if (rday)
    *rday = day;
  if (ryear)
    *ryear = year;
}

#include <math.h>
#undef quotient
#define quotient(m, n) (floor(((double)(m)) / ((double)(n))))
#undef mod
#define mod(m, n) f_mod(m, n)
static double
f_mod(double m, double n)
{
  double x;

  x = fmod(m, n);
  if ((n < 0) ? (x > 0) : (x < 0))
    x += n;
  return x;
}
#undef oddp
#define oddp(n) (((int)(n)) % 2)

#define solar_sidereal_year (365 + 279457.0 / 1080000)
#define solar_month (solar_sidereal_year / 12)
#define lunar_sidereal_month (27 + 4644439.0 / 14438334)
#define lunar_synodic_month (29 + 7087771.0 / 13358334)

double
solar_longitude (double days)
{
  return mod (days / solar_sidereal_year, 1) * 360;
}

double
zodiac (double days)
{
  return 1 + quotient (solar_longitude (days), 30);
}

void
old_hindu_solar_from_absolute (int date, int *rmonth, int *rday, int *ryear)
{
  double hdate;
  int year, month, day;

  hdate = date + 1132959 + 1.0 / 4;
  year = quotient (hdate, solar_sidereal_year);
  month = zodiac (hdate);
  day = 1 + floor (mod (hdate, solar_month));
  if (rmonth)
    *rmonth = month;
  if (rday)
    *rday = day;
  if (ryear)
    *ryear = year;
}

int
absolute_from_old_hindu_solar (int month, int day, int year)
{
  return floor ((year * solar_sidereal_year)
		+ ((month - 1) * solar_month)
		+ day - 1.0 / 4
		- 1132959);
}

double
lunar_longitude (double days)
{
  return mod (days / lunar_sidereal_month, 1) * 360;
}

double
lunar_phase (double days)
{
  return 1
    + quotient
    (mod (lunar_longitude (days) - solar_longitude (days),
	  360),
     12);
}

double
new_moon (double days)
{
  return days - mod (days, lunar_synodic_month);
}

void
old_hindu_lunar_from_absolute
(int date, int *rmonth, int *rleapmonth, int *rday, int *ryear)
{
  double hdate, sunrise, last_new_moon, next_new_moon, next_month;
  int day, month, leapmonth, year;

  hdate = date + 1132959;
  sunrise = hdate + 1.0 / 4;
  last_new_moon = new_moon (sunrise);
  next_new_moon = last_new_moon + lunar_synodic_month;
  day = lunar_phase (sunrise);
  month = adjusted_mod (1 + zodiac (last_new_moon), 12);
  leapmonth = zodiac (last_new_moon) == zodiac (next_new_moon);
  next_month = next_new_moon + (leapmonth ? lunar_synodic_month : 0);
  year = quotient (next_month, solar_sidereal_year);
  if (rmonth)
    *rmonth = month;
  if (rleapmonth)
    *rleapmonth = leapmonth;
  if (rday)
    *rday = day;
  if (ryear)
    *ryear = year;
}

int
old_hindu_lunar_precedes
(int month1, int leap1, int day1, int year1,
 int month2, int leap2, int day2, int year2)
{
  return ((year1 < year2) ||
	  ((year1 == year2) &&
	   ((month1 < month2) ||
	    ((month1 == month2) &&
	     ((leap1 && !leap2) ||
	      ((leap1 == leap2) &&
	       (day1 < day2)))))));
}

int
absolute_from_old_hindu_lunar (int month, int leapmonth, int day, int year)
{
  int years, months, approx, try,
    month1, leapmonth1, day1, year1;
  int sumres;

  years = year;
  months = month - 2;
  approx = floor (years * solar_sidereal_year)
    + floor (months * lunar_synodic_month)
    - 1132959;
  {
    int temp, i;
    for (temp = 0, i = approx;
	 (old_hindu_lunar_from_absolute
	  (i, &month1, &leapmonth1, &day1, &year1),
	  old_hindu_lunar_precedes
	  (month1, leapmonth1, day1, year1,
	   month, leapmonth, day, year));
	 temp = temp + 1, i++)
      ;
    sumres = temp;
  }
  try = approx
    + sumres;
  old_hindu_lunar_from_absolute (try, &month1, &leapmonth1, &day1, &year);
  if (month1 == month &&
      leapmonth1 == leapmonth &&
      day1 == day &&
      year1 == year)
    return try;
  return 0;
}


/*
Local Variables:
c-basic-offset: 2
End:
*/
