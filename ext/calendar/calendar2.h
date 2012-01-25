/*
  calendar2.h (beta): Written by Tadayoshi Funaba 1997,1999,2000,2002,2007

  This code is in the public domain, but any use of it
  should publically acknowledge its source.

  $Id: calendar2.h,v 1.6 2007-12-24 21:45:48+09 tadf Exp $
*/

#if !defined(__GNUC__) \
 || !( __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 6))
#undef __attribute__
#define __attribute__(x)
#endif

extern int world_leap_year (int year) __attribute__ ((const));
extern int world_last_day_of_month (int month, int year) __attribute__ ((const));
extern int absolute_from_world (int month, int day, int year) __attribute__ ((const));
extern void world_from_absolute
(int date, int *rmonth, int *rday, int *ryear);
extern int world_day_of_week_from_absolute (int date) __attribute__ ((const));
extern int coptic_leap_year (int year) __attribute__ ((const));
extern int coptic_last_day_of_month (int month, int year) __attribute__ ((const));
extern int absolute_from_coptic (int month, int day, int year) __attribute__ ((const));
extern void coptic_from_absolute
(int date, int *rmonth, int *rday, int *ryear);
extern int ethiopian_leap_year (int year) __attribute__ ((const));
extern int ethiopian_last_day_of_month (int month, int year) __attribute__ ((const));
extern int absolute_from_ethiopian (int month, int day, int year) __attribute__ ((const));
extern void ethiopian_from_absolute
(int date, int *rmonth, int *rday, int *ryear);
extern int jalaali_leap_year (int year) __attribute__ ((const));
extern int jalaali_last_day_of_month (int month, int year) __attribute__ ((const));
extern int absolute_from_jalaali (int month, int day, int year) __attribute__ ((const));
extern void jalaali_from_absolute
(int date, int *rmonth, int *rday, int *ryear);
#ifndef PLUTO
extern int absolute_from_ordinal_calendar (int day, int year) __attribute__ ((const));
extern void ordinal_calendar_from_absolute (int date, int *rday, int *ryear);
#endif
extern int absolute_from_julian_day_number (int date) __attribute__ ((const));
#define absolute_from_jdn absolute_from_julian_day_number
extern int julian_day_number_from_absolute (int date) __attribute__ ((const));
#define jdn_from_absolute julian_day_number_from_absolute
#ifndef PLUTO
extern int julian_day_number_from_mjd (int date) __attribute__ ((const));
#define jdn_from_mjd julian_day_number_from_mjd
extern int mjd_from_julian_day_number (int date) __attribute__ ((const));
#define mjd_from_jdn mjd_from_julian_day_number
extern int absolute_from_lilian_day_number (int date) __attribute__ ((const));
#define absolute_from_ldn absolute_from_lilian_day_number
extern int lilian_day_number_from_absolute (int date) __attribute__ ((const));
#define ldn_from_absolute lilian_day_number_from_absolute
extern int day_of_week_from_absolute (int date) __attribute__ ((const));
#endif /* not PLUTO */


/*
Local Variables:
c-basic-offset: 2
End:
*/
