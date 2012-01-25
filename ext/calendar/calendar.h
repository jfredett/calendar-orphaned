/*
  calendar.h: Written by Tadayoshi Funaba 1997-2000

  This code is in the public domain, but any use of it
  should publically acknowledge its source.

  $Id: calendar.h,v 1.5 2001-02-06 23:34:21+09 tadf Exp $
*/

#if !defined(__GNUC__) \
 || !( __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 6))
#undef __attribute__
#define __attribute__(x)
#endif

extern int gregorian_leap_year (int year) __attribute__ ((const));
#define gregorian_last_day_of_month last_day_of_gregorian_month
extern int last_day_of_gregorian_month (int month, int year) __attribute__ ((const));
extern int absolute_from_gregorian (int month, int day, int year) __attribute__ ((const));
extern void gregorian_from_absolute
(int date, int *rmonth, int *rday, int *ryear);
extern int Kday_on_or_before (int date, int k) __attribute__ ((const));
extern int absolute_from_iso (int week, int day, int year) __attribute__ ((const));
#define absolute_from_calendar_week absolute_from_iso
extern void iso_from_absolute (int date, int *rweek, int *rday, int *ryear);
#define calendar_week_from_absolute iso_from_absolute
extern int julian_leap_year (int year) __attribute__ ((const));
#define julian_last_day_of_month last_day_of_julian_month
extern int last_day_of_julian_month (int month, int year) __attribute__ ((const));
extern int absolute_from_julian (int month, int day, int year) __attribute__ ((const));
extern void julian_from_absolute
(int date, int *rmonth, int *rday, int *ryear);
extern int islamic_leap_year (int year) __attribute__ ((const));
#define islamic_last_day_of_month last_day_of_islamic_month
extern int last_day_of_islamic_month (int month, int year) __attribute__ ((const));
extern int absolute_from_islamic (int month, int day, int year) __attribute__ ((const));
extern void islamic_from_absolute
(int date, int *rmonth, int *rday, int *ryear);
extern int hebrew_leap_year (int year) __attribute__ ((const));
extern int last_month_of_hebrew_year (int year) __attribute__ ((const));
#define hebrew_last_day_of_month last_day_of_hebrew_month
extern int last_day_of_hebrew_month (int month, int year) __attribute__ ((const));
extern int hebrew_calendar_elapsed_days (int year) __attribute__ ((const));
extern int days_in_hebrew_year (int year) __attribute__ ((const));
extern int long_heshvan (int year) __attribute__ ((const));
extern int short_kislev (int year) __attribute__ ((const));
extern int absolute_from_hebrew (int month, int day, int year) __attribute__ ((const));
extern void hebrew_from_absolute
(int date, int *rmonth, int *rday, int *ryear);
#if !defined(PLUTO) && !defined(MODULE)
extern int independence_day (int year) __attribute__ ((const));
#endif
#ifndef PLUTO
extern int Nth_Kday (int n, int k, int month, int year) __attribute__ ((const));
#endif
#if !defined(PLUTO) && !defined(MODULE)
extern int labor_day (int year) __attribute__ ((const));
extern int memorial_day (int year) __attribute__ ((const));
extern int daylight_savings_start (int year) __attribute__ ((const));
extern int daylight_savings_end (int year) __attribute__ ((const));
extern int christmas (int year) __attribute__ ((const));
extern int advent (int year) __attribute__ ((const));
extern int epiphany (int year) __attribute__ ((const));
extern int eastern_orthodox_christmas (int year) __attribute__ ((const));
#endif /* not PLUTO and not MODULE */
extern int nicaean_rule_easter (int year) __attribute__ ((const));
extern int easter (int year) __attribute__ ((const));
#if !defined(PLUTO) && !defined(MODULE)
extern int pentecost (int year) __attribute__ ((const));
extern void islamic_date (int month, int day, int year, int date[3]);
extern void mulad_al_nabi (int year, int date[3]);
extern int yom_kippur (int year) __attribute__ ((const));
extern int passover (int year) __attribute__ ((const));
extern int purim (int year) __attribute__ ((const));
extern int ta_anit_esther (int year) __attribute__ ((const));
extern int tisha_b_av (int year) __attribute__ ((const));
extern int hebrew_birthday
(int birth_month, int birth_day, int birth_year, int year) __attribute__ ((const));
extern int yahrzeit (int death_month, int death_day, int death_year, int year) __attribute__ ((const));
#endif /* not PLUTO and not MODULE */
extern int absolute_from_mayan_long_count
(int baktun, int katun, int tun, int uinal, int kin) __attribute__ ((const));
extern void mayan_long_count_from_absolute
(int date, int *rbaktun, int *rkatun, int *rtun, int *ruinal, int *rkin);
extern void mayan_haab_from_absolute (int date, int *rday, int *rmonth);
extern int mayan_haab_difference (int day1, int month1, int day2, int month2) __attribute__ ((const));
extern int mayan_haab_on_or_before (int haab_day, int haab_month, int date) __attribute__ ((const));
extern void mayan_tzolkin_from_absolute (int date, int *rnumber, int *rname);
extern int mayan_tzolkin_difference
(int number1, int name1, int number2, int name2) __attribute__ ((const));
extern int mayan_tzolkin_on_or_before (int number, int name, int date) __attribute__ ((const));
extern int mayan_haab_tzolkin_on_or_before
(int day, int month, int number, int name, int date) __attribute__ ((const));
extern int french_last_day_of_month (int month, int year) __attribute__ ((const));
extern int french_leap_year (int year) __attribute__ ((const));
extern int absolute_from_french (int month, int day, int year) __attribute__ ((const));
extern void french_from_absolute
(int date, int *rmonth, int *rday, int *ryear);
extern double solar_longitude (double days) __attribute__ ((const));
extern double zodiac (double days) __attribute__ ((const));
extern void old_hindu_solar_from_absolute
(int date, int *rmonth, int *rday, int *ryear);
extern int absolute_from_old_hindu_solar (int month, int day, int year) __attribute__ ((const));
extern double lunar_longitude (double days) __attribute__ ((const));
extern double lunar_phase (double days) __attribute__ ((const));
extern double new_moon (double days) __attribute__ ((const));
extern void old_hindu_lunar_from_absolute
(int date, int *rmonth, int *rleapmonth, int *rday, int *ryear);
extern int old_hindu_lunar_precedes
(int month1, int leap1, int day1, int year1,
 int month2, int leap2, int day2, int year2) __attribute__ ((const));
extern int absolute_from_old_hindu_lunar
(int month, int leapmonth, int day, int year) __attribute__ ((const));


/*
Local Variables:
c-basic-offset: 2
End:
*/
