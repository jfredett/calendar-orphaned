/*
  calendarmodule.c: Copyright (C) 1997-2002,2007 Tadayoshi Funaba All rights reserved
  $Id: calendarmodule.c,v 1.13 2007-12-24 21:46:35+09 tadf Exp $
*/

#include "ruby.h"
#include "calendar.h"
#include "calendar2.h"
#include "qref.h"

#define INT2BOOL(x) ((x) ? Qtrue : Qfalse)
#define BOOL2INT(x) (RTEST(x) ? 1 : 0)

#define CHECK_RANGE(v, m)			\
{						\
    if ((v) < (m))				\
	rb_raise(rb_eArgError, "out of range");	\
}

#define CHECK_DOMAIN(v, m)			\
{						\
    if ((v) < (m))				\
	rb_raise(rb_eArgError, "out of domain");\
}

#define CHECK_DOMAIN2(v, m, n)			\
{						\
    if ((v) < (m) || (v) > (n))			\
	rb_raise(rb_eArgError, "out of domain");\
}

static VALUE
calendar_gregorian_leap_year(VALUE obj, VALUE y)
{
    int iy, l;

    iy = NUM2INT(y); CHECK_DOMAIN(iy, 1);
    l = gregorian_leap_year(iy);
    return INT2BOOL(l);
}

static VALUE
calendar_gregorian_last_day_of_month(VALUE obj, VALUE m, VALUE y)
{
    int im, iy, md;

    im = NUM2INT(m); CHECK_DOMAIN2(im, 1, 12);
    iy = NUM2INT(y); CHECK_DOMAIN(iy, 1);
    md = gregorian_last_day_of_month(im, iy);
    return INT2NUM(md);
}

static VALUE
calendar_absolute_from_gregorian(VALUE obj, VALUE m, VALUE md, VALUE y)
{
    int im, imd, iy, a, im2, imd2, iy2;

    im = NUM2INT(m); CHECK_DOMAIN2(im, 1, 12);
    imd = NUM2INT(md); CHECK_DOMAIN2(imd, 1, 31);
    iy = NUM2INT(y); CHECK_DOMAIN(iy, 1);
    a = absolute_from_gregorian(im, imd, iy);
    CHECK_RANGE(a, 1);
    gregorian_from_absolute(a, &im2, &imd2, &iy2);
    if (im != im2 || imd != imd2 || iy != iy2)
	rb_raise(rb_eArgError, "invalid date");
    return INT2NUM(a);
}

static VALUE
calendar_gregorian_from_absolute(VALUE obj, VALUE a)
{
    int ia, m, md, y;

    ia = NUM2INT(a); CHECK_DOMAIN(ia, 1);
    gregorian_from_absolute(ia, &m, &md, &y);
    return rb_ary_new3(3, INT2NUM(m), INT2NUM(md), INT2NUM(y));
}

static VALUE
calendar_Kday_on_or_before(VALUE obj, VALUE a, VALUE k)
{
    int ia, ik, b;

    ia = NUM2INT(a); CHECK_DOMAIN(ia, 1);
    ik = NUM2INT(k); CHECK_DOMAIN2(ik, 0, 6);
    b = Kday_on_or_before(ia, NUM2INT(k));
    CHECK_RANGE(b, 1);
    return INT2NUM(b);
}

static VALUE
calendar_absolute_from_iso(VALUE obj, VALUE w, VALUE wd, VALUE y)
{
    int iw, iwd, iy, a, iw2, iwd2, iy2;

    iw = NUM2INT(w); CHECK_DOMAIN2(iw, 1, 53);
    iwd = NUM2INT(wd); CHECK_DOMAIN2(iwd, 1, 7);
    iy = NUM2INT(y); CHECK_DOMAIN(iy, 1);
    a = absolute_from_iso(iw, iwd, iy);
    CHECK_RANGE(a, 1);
    iso_from_absolute(a, &iw2, &iwd2, &iy2);
    if (iw != iw2 || iwd != iwd2 || iy != iy2)
	rb_raise(rb_eArgError, "invalid date");
    return INT2NUM(a);
}

static VALUE
calendar_iso_from_absolute(VALUE obj, VALUE a)
{
    int ia, w, wd, y;

    ia = NUM2INT(a); CHECK_DOMAIN(ia, 1);
    iso_from_absolute(ia, &w, &wd, &y);
    return rb_ary_new3(3, INT2NUM(w), INT2NUM(wd), INT2NUM(y));
}

static VALUE
calendar_julian_leap_year(VALUE obj, VALUE y)
{
    int iy, l;

    iy = NUM2INT(y); CHECK_DOMAIN(iy, 1);
    l = julian_leap_year(iy);
    return INT2BOOL(l);
}

static VALUE
calendar_julian_last_day_of_month(VALUE obj, VALUE m, VALUE y)
{
    int im, iy, md;

    im = NUM2INT(m); CHECK_DOMAIN2(im, 1, 12);
    iy = NUM2INT(y); CHECK_DOMAIN(iy, 1);
    md = julian_last_day_of_month(im, iy);
    return INT2NUM(md);
}

static VALUE
calendar_absolute_from_julian(VALUE obj, VALUE m, VALUE md, VALUE y)
{
    int im, imd, iy, a, im2, imd2, iy2;

    im = NUM2INT(m); CHECK_DOMAIN2(im, 1, 12);
    imd = NUM2INT(md); CHECK_DOMAIN2(imd, 1, 31);
    iy = NUM2INT(y); CHECK_DOMAIN(iy, 1);
    a = absolute_from_julian(im, imd, iy);
    CHECK_RANGE(a, 1);
    julian_from_absolute(a, &im2, &imd2, &iy2);
    if (im != im2 || imd != imd2 || iy != iy2)
	rb_raise(rb_eArgError, "invalid date");
    return INT2NUM(a);
}

static VALUE
calendar_julian_from_absolute(VALUE obj, VALUE a)
{
    int ia, m, md, y;

    ia = NUM2INT(a); CHECK_DOMAIN(ia, 1);
    julian_from_absolute(ia, &m, &md, &y);
    return rb_ary_new3(3, INT2NUM(m), INT2NUM(md), INT2NUM(y));
}

static VALUE
calendar_islamic_leap_year(VALUE obj, VALUE y)
{
    int iy, l;

    iy = NUM2INT(y); CHECK_DOMAIN(iy, 1);
    l = islamic_leap_year(iy);
    return INT2BOOL(l);
}

static VALUE
calendar_islamic_last_day_of_month(VALUE obj, VALUE m, VALUE y)
{
    int im, iy, md;

    im = NUM2INT(m); CHECK_DOMAIN2(im, 1, 12);
    iy = NUM2INT(y); CHECK_DOMAIN(iy, 1);
    md = islamic_last_day_of_month(im, iy);
    return INT2NUM(md);
}

static VALUE
calendar_absolute_from_islamic(VALUE obj, VALUE m, VALUE md, VALUE y)
{
    int im, imd, iy, a, im2, imd2, iy2;

    im = NUM2INT(m); CHECK_DOMAIN2(im, 1, 12);
    imd = NUM2INT(md); CHECK_DOMAIN2(imd, 1, 30);
    iy = NUM2INT(y); CHECK_DOMAIN(iy, 1);
    a = absolute_from_islamic(im, imd, iy);
    CHECK_RANGE(a, 1); /* 227015 */
    islamic_from_absolute(a, &im2, &imd2, &iy2);
    if (im != im2 || imd != imd2 || iy != iy2)
	rb_raise(rb_eArgError, "invalid date");
    return INT2NUM(a);
}

static VALUE
calendar_islamic_from_absolute(VALUE obj, VALUE a)
{
    int ia, m, md, y;

    ia = NUM2INT(a); CHECK_DOMAIN(ia, 227015);
    islamic_from_absolute(ia, &m, &md, &y);
    return rb_ary_new3(3, INT2NUM(m), INT2NUM(md), INT2NUM(y));
}

static VALUE
calendar_hebrew_leap_year(VALUE obj, VALUE y)
{
    int iy, l;

    iy = NUM2INT(y); CHECK_DOMAIN(iy, 3761);
    l = hebrew_leap_year(iy);
    return INT2BOOL(l);
}

static VALUE
calendar_last_month_of_hebrew_year(VALUE obj, VALUE y)
{
    int iy, m;

    iy = NUM2INT(y); CHECK_DOMAIN(iy, 1);
    m = last_month_of_hebrew_year(iy);
    return INT2NUM(m);
}

static VALUE
calendar_hebrew_last_day_of_month(VALUE obj, VALUE m, VALUE y)
{
    int im, iy, md;

    im = NUM2INT(m); CHECK_DOMAIN2(im, 1, 13);
    iy = NUM2INT(y); CHECK_DOMAIN(iy, 1);
    md = hebrew_last_day_of_month(im, iy);
    return INT2NUM(md);
}

static VALUE
calendar_hebrew_calendar_elapsed_days(VALUE obj, VALUE y)
{
    int iy, d;

    iy = NUM2INT(y); CHECK_DOMAIN(iy, 1);
    d = hebrew_calendar_elapsed_days(iy);
    return INT2NUM(d);
}

static VALUE
calendar_days_in_hebrew_year(VALUE obj, VALUE y)
{
    int iy, d;

    iy = NUM2INT(y); CHECK_DOMAIN(iy, 1);
    d = days_in_hebrew_year(iy);
    return INT2NUM(d);
}

static VALUE
calendar_long_heshvan(VALUE obj, VALUE y)
{
    int iy, l;

    iy = NUM2INT(y); CHECK_DOMAIN(iy, 1);
    l = long_heshvan(iy);
    return INT2BOOL(l);
}

static VALUE
calendar_short_kislev(VALUE obj, VALUE y)
{
    int iy, s;

    iy = NUM2INT(y); CHECK_DOMAIN(iy, 1);
    s = short_kislev(iy);
    return INT2BOOL(s);
}

static VALUE
calendar_absolute_from_hebrew(VALUE obj, VALUE m, VALUE md, VALUE y)
{
    int im, imd, iy, a, im2, imd2, iy2;

    im = NUM2INT(m); CHECK_DOMAIN2(im, 1, 13);
    imd = NUM2INT(md); CHECK_DOMAIN2(imd, 1, 30);
    iy = NUM2INT(y); CHECK_DOMAIN(iy, 1);
    a = absolute_from_hebrew(im, imd, iy);
    CHECK_RANGE(a, 1);
    hebrew_from_absolute(a, &im2, &imd2, &iy2);
    if (im != im2 || imd != imd2 || iy != iy2)
	rb_raise(rb_eArgError, "invalid date");
    return INT2NUM(a);
}

static VALUE
calendar_hebrew_from_absolute(VALUE obj, VALUE a)
{
    int ia, m, md, y;

    ia = NUM2INT(a); CHECK_DOMAIN(ia, 1);
    hebrew_from_absolute(ia, &m, &md, &y);
    return rb_ary_new3(3, INT2NUM(m), INT2NUM(md), INT2NUM(y));
}

static VALUE
calendar_Nth_Kday(VALUE obj, VALUE n, VALUE k, VALUE m, VALUE y)
{
    int in, ik, im, iy, b;

    in = NUM2INT(n);
    if (in < -5 || in == 0 || in > 5)
	rb_raise(rb_eArgError, "out of range");
    ik = NUM2INT(k); CHECK_DOMAIN2(ik, 0, 6);
    im = NUM2INT(m); CHECK_DOMAIN2(im, 1, 12);
    iy = NUM2INT(y); CHECK_DOMAIN(iy, 1);
    b = Nth_Kday(in, ik, im, iy);
    CHECK_RANGE(b, 1);
    return INT2NUM(b);
}

static VALUE
calendar_nicaean_rule_easter(VALUE obj, VALUE y)
{
    int iy, a;

    iy = NUM2INT(y); CHECK_DOMAIN(iy, 1);
    a = nicaean_rule_easter(iy);
    CHECK_RANGE(a, 1);
    return INT2NUM(a);
}

static VALUE
calendar_easter(VALUE obj, VALUE y)
{
    int iy, a;

    iy = NUM2INT(y); CHECK_DOMAIN(iy, 1);
    a = easter(iy);
    CHECK_RANGE(a, 1);
    return INT2NUM(a);
}

static VALUE
calendar_absolute_from_mayan_long_count
(VALUE obj, VALUE bt, VALUE kt, VALUE t, VALUE u, VALUE k)
{
    int ibt, ikt, it, iu, ik, a, ibt2, ikt2, it2, iu2, ik2;

    ibt = NUM2INT(bt); CHECK_DOMAIN2(ibt, 0, 19);
    ikt = NUM2INT(kt); CHECK_DOMAIN2(ikt, 0, 19);
    it = NUM2INT(t); CHECK_DOMAIN2(it, 0, 19);
    iu = NUM2INT(u); CHECK_DOMAIN2(iu, 0, 17);
    ik = NUM2INT(k); CHECK_DOMAIN2(ik, 0, 19);
    a = absolute_from_mayan_long_count(ibt, ikt, it, iu, ik);
    CHECK_RANGE(a, 1);
    mayan_long_count_from_absolute(a, &ibt2, &ikt2, &it2, &iu2, &ik2);
    if (ibt != ibt2 || ikt != ikt2 || it != it2 || iu != iu2 || ik != ik2)
	rb_raise(rb_eArgError, "invalid date");
    return INT2NUM(a);
}

static VALUE
calendar_mayan_long_count_from_absolute(VALUE obj, VALUE a)
{
    int ia, bt, kt, t, u, k;

    ia = NUM2INT(a); CHECK_DOMAIN(ia, 1);
    mayan_long_count_from_absolute(ia, &bt, &kt, &t, &u, &k);
    return rb_ary_new3
	(5, INT2NUM(bt), INT2NUM(kt), INT2NUM(t), INT2NUM(u), INT2NUM(k));
}

static VALUE
calendar_mayan_haab_from_absolute(VALUE obj, VALUE a)
{
    int ia, md, m;

    ia = NUM2INT(a); CHECK_DOMAIN(ia, 1);
    mayan_haab_from_absolute(ia, &md, &m);
    return rb_ary_new3(2, INT2NUM(md), INT2NUM(m));
}

static VALUE
calendar_mayan_haab_difference
(VALUE obj, VALUE md1, VALUE m1, VALUE md2, VALUE m2)
{
    int imd1, im1, imd2, im2, d;

    imd1 = NUM2INT(md1); CHECK_DOMAIN2(imd1, 0, 19);
    im1 = NUM2INT(m1); CHECK_DOMAIN2(im1, 1, 19);
    imd2 = NUM2INT(md2); CHECK_DOMAIN2(imd2, 0, 19);
    im2 = NUM2INT(m2); CHECK_DOMAIN2(im2, 1, 19);
    d = mayan_haab_difference(imd1, im1, imd2, im2);
    return INT2NUM(d);
}

static VALUE
calendar_mayan_haab_on_or_before(VALUE obj, VALUE md, VALUE m, VALUE a)
{
    int imd, im, ia, d;

    imd = NUM2INT(md); CHECK_DOMAIN2(imd, 0, 19);
    im = NUM2INT(m); CHECK_DOMAIN2(im, 1, 19);
    ia = NUM2INT(a); CHECK_DOMAIN(ia, 1);
    d = mayan_haab_on_or_before(imd, im, ia);
    CHECK_RANGE(d, 1);
    return INT2NUM(d);
}

static VALUE
calendar_mayan_tzolkin_from_absolute(VALUE obj, VALUE a)
{
    int ia, nu, na;

    ia = NUM2INT(a); CHECK_DOMAIN(ia, 1);
    mayan_tzolkin_from_absolute(ia, &nu, &na);
    return rb_ary_new3(2, INT2NUM(nu), INT2NUM(na));
}

static VALUE
calendar_mayan_tzolkin_difference
(VALUE obj, VALUE nu1, VALUE na1, VALUE nu2, VALUE na2)
{
    int inu1, ina1, inu2, ina2, d;

    inu1 = NUM2INT(nu1); CHECK_DOMAIN2(inu1, 1, 13);
    ina1 = NUM2INT(na1); CHECK_DOMAIN2(ina1, 1, 20);
    inu2 = NUM2INT(nu1); CHECK_DOMAIN2(inu2, 1, 13);
    ina2 = NUM2INT(na2); CHECK_DOMAIN2(ina2, 1, 20);
    d = mayan_tzolkin_difference(inu1, ina1, inu2, ina2);
    return INT2NUM(d);
}

static VALUE
calendar_mayan_tzolkin_on_or_before(VALUE obj, VALUE nu, VALUE na, VALUE a)
{
    int inu, ina, ia, d;

    inu = NUM2INT(nu); CHECK_DOMAIN2(inu, 1, 13);
    ina = NUM2INT(na); CHECK_DOMAIN2(ina, 1, 20);
    ia = NUM2INT(a); CHECK_DOMAIN(ia, 1);
    d = mayan_tzolkin_on_or_before(inu, ina, ia);
    CHECK_RANGE(d, 1);
    return INT2NUM(d);
}

static VALUE
calendar_mayan_haab_tzolkin_on_or_before
(VALUE obj, VALUE md, VALUE m, VALUE nu, VALUE na, VALUE a)
{
    int imd, im, inu, ina, ia, d;

    imd = NUM2INT(md); CHECK_DOMAIN2(imd, 0, 19);
    im = NUM2INT(m); CHECK_DOMAIN2(im, 1, 19);
    inu = NUM2INT(nu); CHECK_DOMAIN2(inu, 1, 13);
    ina = NUM2INT(na); CHECK_DOMAIN2(ina, 1, 20);
    ia = NUM2INT(a); CHECK_DOMAIN(ia, 1);
    d = mayan_haab_tzolkin_on_or_before(imd, im, inu, ina, ia);
    CHECK_RANGE(d, 1);
    return INT2NUM(d);
}

static VALUE
calendar_french_last_day_of_month(VALUE obj, VALUE m, VALUE y)
{
    int im, iy, md;

    im = NUM2INT(m); CHECK_DOMAIN2(im, 1, 13);
    iy = NUM2INT(y); CHECK_DOMAIN(iy, 1);
    md = french_last_day_of_month(im, iy);
    return INT2NUM(md);
}

static VALUE
calendar_french_leap_year(VALUE obj, VALUE y)
{
    int iy, l;

    iy = NUM2INT(y); CHECK_DOMAIN(iy, 1);
    l = french_leap_year(iy);
    return INT2BOOL(l);
}

static VALUE
calendar_absolute_from_french(VALUE obj, VALUE m, VALUE md, VALUE y)
{
    int im, imd, iy, a, im2, imd2, iy2;

    im = NUM2INT(m); CHECK_DOMAIN2(im, 1, 13);
    imd = NUM2INT(md); CHECK_DOMAIN2(imd, 1, 30);
    iy = NUM2INT(y); CHECK_DOMAIN(iy, 1);
    a = absolute_from_french(im, imd, iy);
    CHECK_RANGE(a, 1); /* 654415 */
    french_from_absolute(a, &im2, &imd2, &iy2);
    if (im != im2 || imd != imd2 || iy != iy2)
	rb_raise(rb_eArgError, "invalid date");
    return INT2NUM(a);
}

static VALUE
calendar_french_from_absolute(VALUE obj, VALUE a)
{
    int ia, m, md, y;

    ia = NUM2INT(a); CHECK_DOMAIN(ia, 654415);
    french_from_absolute(ia, &m, &md, &y);
    return rb_ary_new3(3, INT2NUM(m), INT2NUM(md), INT2NUM(y));
}

static double
getfloat(VALUE x)
{
    if (TYPE(x) == T_FLOAT)
#ifndef RFLOAT_VALUE
	return RFLOAT(x)->value;
#else
	return RFLOAT_VALUE(x);
#endif
    return (double)NUM2INT(x);
}

static VALUE
calendar_solar_longitude(VALUE obj, VALUE d)
{
    double l;

    l = solar_longitude(getfloat(d));
    return rb_float_new(l);
}

static VALUE
calendar_zodiac(VALUE obj, VALUE d)
{
    double l;

    l = zodiac(getfloat(d));
    return rb_float_new(l);
}

static VALUE
calendar_old_hindu_solar_from_absolute(VALUE obj, VALUE a)
{
    int ia, m, md, y;

    ia = NUM2INT(a); CHECK_DOMAIN(ia, 1);
    old_hindu_solar_from_absolute(ia, &m, &md, &y);
    return rb_ary_new3(3, INT2NUM(m), INT2NUM(md), INT2NUM(y));
}

static VALUE
calendar_absolute_from_old_hindu_solar(VALUE obj, VALUE m, VALUE md, VALUE y)
{
    int im, imd, iy, a, im2, imd2, iy2;

    im = NUM2INT(m); CHECK_DOMAIN2(im, 1, 12);
    imd = NUM2INT(md); CHECK_DOMAIN2(imd, 1, 31);
    iy = NUM2INT(y); CHECK_DOMAIN(iy, 1);
    a = absolute_from_old_hindu_solar(im, imd, iy);
    CHECK_RANGE(a, 1);
    old_hindu_solar_from_absolute(a, &im2, &imd2, &iy2);
    if (im != im2 || imd != imd2 || iy != iy2)
	rb_raise(rb_eArgError, "invalid date");
    return INT2NUM(a);
}

static VALUE
calendar_lunar_longitude(VALUE obj, VALUE d)
{
    double l;

    l = lunar_longitude(getfloat(d));
    return rb_float_new(l);
}

static VALUE
calendar_lunar_phase(VALUE obj, VALUE d)
{
    double p;

    p = lunar_phase(getfloat(d));
    return rb_float_new(p);
}

static VALUE
calendar_new_moon(VALUE obj, VALUE d)
{
    double n;

    n = new_moon(getfloat(d));
    return rb_float_new(n);
}

static VALUE
calendar_old_hindu_lunar_from_absolute(VALUE obj, VALUE a)
{
    int ia, m, l, md, y;

    ia = NUM2INT(a); CHECK_DOMAIN(ia, 1);
    old_hindu_lunar_from_absolute(ia, &m, &l, &md, &y);
    return rb_ary_new3(4, INT2NUM(m), INT2BOOL(l), INT2NUM(md), INT2NUM(y));
}

static VALUE
calendar_old_hindu_lunar_precedes
(VALUE obj, VALUE m1, VALUE l1, VALUE md1, VALUE y1,
 VALUE m2, VALUE l2, VALUE md2, VALUE y2)
{
    int im1, il1, imd1, iy1, im2, il2, imd2, iy2, d;

    im1 = NUM2INT(m1); CHECK_DOMAIN2(im1, 1, 12);
    il1 = BOOL2INT(l1);
    imd1 = NUM2INT(md1); CHECK_DOMAIN2(imd1, 1, 30);
    iy1 = NUM2INT(y1); CHECK_DOMAIN(iy1, 1);
    im2 = NUM2INT(m2); CHECK_DOMAIN2(im2, 1, 12);
    il2 = BOOL2INT(l2);
    imd2 = NUM2INT(md2); CHECK_DOMAIN2(imd2, 1, 30);
    iy2 = NUM2INT(y2); CHECK_DOMAIN(iy2, 1);
    d = old_hindu_lunar_precedes(im1, il1, imd1, iy1, im2, il2, imd2, iy2);
    return INT2NUM(d);
}

static VALUE
calendar_absolute_from_old_hindu_lunar
(VALUE obj, VALUE m, VALUE l, VALUE md, VALUE y)
{
    int im, il, imd, iy, a, im2, il2, imd2, iy2;

    im = NUM2INT(m); CHECK_DOMAIN2(im, 1, 12);
    il = BOOL2INT(l);
    imd = NUM2INT(md); CHECK_DOMAIN2(imd, 1, 30);
    iy = NUM2INT(y); CHECK_DOMAIN(iy, 1);
    a = absolute_from_old_hindu_lunar(im, il, imd, iy);
    CHECK_RANGE(a, 1);
    old_hindu_lunar_from_absolute(a, &im2, &il2, &imd2, &iy2);
    if (im != im2 || il != il2 || imd != imd2 || iy != iy2)
	rb_raise(rb_eArgError, "invalid date");
    return INT2NUM(a);
}

static VALUE
calendar_world_leap_year(VALUE obj, VALUE y)
{
    int iy, l;

    iy = NUM2INT(y); CHECK_DOMAIN(iy, 1);
    l = world_leap_year(iy);
    return INT2BOOL(l);
}

static VALUE
calendar_world_last_day_of_month(VALUE obj, VALUE m, VALUE y)
{
    int im, iy, md;

    im = NUM2INT(m); CHECK_DOMAIN2(im, 1, 12);
    iy = NUM2INT(y); CHECK_DOMAIN(iy, 1);
    md = world_last_day_of_month(im, iy);
    return INT2NUM(md);
}

static VALUE
calendar_absolute_from_world(VALUE obj, VALUE m, VALUE md, VALUE y)
{
    int im, imd, iy, a, im2, imd2, iy2;

    im = NUM2INT(m); CHECK_DOMAIN2(im, 1, 12);
    imd = NUM2INT(md); CHECK_DOMAIN2(imd, 1, 31);
    iy = NUM2INT(y); CHECK_DOMAIN(iy, 1);
    a = absolute_from_world(im, imd, iy);
    CHECK_RANGE(a, 1);
    world_from_absolute(a, &im2, &imd2, &iy2);
    if (im != im2 || imd != imd2 || iy != iy2)
	rb_raise(rb_eArgError, "invalid date");
    return INT2NUM(a);
}

static VALUE
calendar_world_from_absolute(VALUE obj, VALUE a)
{
    int ia, m, md, y;

    ia = NUM2INT(a); CHECK_DOMAIN(ia, 1);
    world_from_absolute(ia, &m, &md, &y);
    return rb_ary_new3(3, INT2NUM(m), INT2NUM(md), INT2NUM(y));
}

static VALUE
calendar_world_day_of_week_from_absolute(VALUE obj, VALUE a)
{
    int ia, w;

    ia = NUM2INT(a); CHECK_DOMAIN(ia, 1);
    w = world_day_of_week_from_absolute(ia);
    return INT2NUM(w);
}

static VALUE
calendar_coptic_leap_year(VALUE obj, VALUE y)
{
    int iy, l;

    iy = NUM2INT(y); CHECK_DOMAIN(iy, 1);
    l = coptic_leap_year(iy);
    return INT2BOOL(l);
}

static VALUE
calendar_coptic_last_day_of_month(VALUE obj, VALUE m, VALUE y)
{
    int im, iy, md;

    im = NUM2INT(m); CHECK_DOMAIN2(im, 1, 13);
    iy = NUM2INT(y); CHECK_DOMAIN(iy, 1);
    md = coptic_last_day_of_month(im, iy);
    return INT2NUM(md);
}

static VALUE
calendar_absolute_from_coptic(VALUE obj, VALUE m, VALUE md, VALUE y)
{
    int im, imd, iy, a, im2, imd2, iy2;

    im = NUM2INT(m); CHECK_DOMAIN2(im, 1, 13);
    imd = NUM2INT(md); CHECK_DOMAIN2(imd, 1, 30);
    iy = NUM2INT(y); CHECK_DOMAIN(iy, 1);
    a = absolute_from_coptic(im, imd, iy);
    CHECK_RANGE(a, 1); /* 103605 */
    coptic_from_absolute(a, &im2, &imd2, &iy2);
    if (im != im2 || imd != imd2 || iy != iy2)
	rb_raise(rb_eArgError, "invalid date");
    return INT2NUM(a);
}

static VALUE
calendar_coptic_from_absolute(VALUE obj, VALUE a)
{
    int ia, m, md, y;

    ia = NUM2INT(a); CHECK_DOMAIN(ia, 103605);
    coptic_from_absolute(ia, &m, &md, &y);
    return rb_ary_new3(3, INT2NUM(m), INT2NUM(md), INT2NUM(y));
}

static VALUE
calendar_ethiopian_leap_year(VALUE obj, VALUE y)
{
    int iy, l;

    iy = NUM2INT(y); CHECK_DOMAIN(iy, 1);
    l = ethiopian_leap_year(iy);
    return INT2BOOL(l);
}

static VALUE
calendar_ethiopian_last_day_of_month(VALUE obj, VALUE m, VALUE y)
{
    int im, iy, md;

    im = NUM2INT(m); CHECK_DOMAIN2(im, 1, 13);
    iy = NUM2INT(y); CHECK_DOMAIN(iy, 1);
    md = ethiopian_last_day_of_month(im, iy);
    return INT2NUM(md);
}

static VALUE
calendar_absolute_from_ethiopian(VALUE obj, VALUE m, VALUE md, VALUE y)
{
    int im, imd, iy, a, im2, imd2, iy2;

    im = NUM2INT(m); CHECK_DOMAIN2(im, 1, 13);
    imd = NUM2INT(md); CHECK_DOMAIN2(imd, 1, 30);
    iy = NUM2INT(y); CHECK_DOMAIN(iy, 1);
    a = absolute_from_ethiopian(im, imd, iy);
    CHECK_RANGE(a, 1); /* 2796 */
    ethiopian_from_absolute(a, &im2, &imd2, &iy2);
    if (im != im2 || imd != imd2 || iy != iy2)
	rb_raise(rb_eArgError, "invalid date");
    return INT2NUM(a);
}

static VALUE
calendar_ethiopian_from_absolute(VALUE obj, VALUE a)
{
    int ia, m, md, y;

    ia = NUM2INT(a); CHECK_DOMAIN(ia, 2796);
    ethiopian_from_absolute(ia, &m, &md, &y);
    return rb_ary_new3(3, INT2NUM(m), INT2NUM(md), INT2NUM(y));
}

static VALUE
calendar_jalaali_leap_year(VALUE obj, VALUE y)
{
    int iy, l;

    iy = NUM2INT(y); CHECK_DOMAIN(iy, 1);
    l = jalaali_leap_year(iy);
    return INT2BOOL(l);
}

static VALUE
calendar_jalaali_last_day_of_month(VALUE obj, VALUE m, VALUE y)
{
    int im, iy, md;

    im = NUM2INT(m); CHECK_DOMAIN2(im, 1, 12);
    iy = NUM2INT(y); CHECK_DOMAIN(iy, 1);
    md = jalaali_last_day_of_month(NUM2INT(m), NUM2INT(y));
    return INT2NUM(md);
}

static VALUE
calendar_absolute_from_jalaali(VALUE obj, VALUE m, VALUE md, VALUE y)
{
    int im, imd, iy, a, im2, imd2, iy2;

    im = NUM2INT(m); CHECK_DOMAIN2(im, 1, 12);
    imd = NUM2INT(md); CHECK_DOMAIN2(imd, 1, 31);
    iy = NUM2INT(y); CHECK_DOMAIN(iy, 1);
    a = absolute_from_jalaali(im, imd, iy);
    CHECK_RANGE(a, 1);  /* 632680 - 823335 */
    jalaali_from_absolute(a, &im2, &imd2, &iy2);
    if (im != im2 || imd != imd2 || iy != iy2)
	rb_raise(rb_eArgError, "invalid date");
    return INT2NUM(a);
}

static VALUE
calendar_jalaali_from_absolute(VALUE obj, VALUE a)
{
    int ia, m, md, y;

    ia = NUM2INT(a); CHECK_DOMAIN2(ia, 632680, 823335);
    jalaali_from_absolute(ia, &m, &md, &y);
    return rb_ary_new3(3, INT2NUM(m), INT2NUM(md), INT2NUM(y));
}

static VALUE
calendar_absolute_from_ordinal_calendar(VALUE obj, VALUE yd, VALUE y)
{
    int iyd, iy, a, iyd2, iy2;

    iyd = NUM2INT(yd); CHECK_DOMAIN2(iyd, 1, 366);
    iy = NUM2INT(y); CHECK_DOMAIN(iy, 1);
    a = absolute_from_ordinal_calendar(iyd, iy);
    CHECK_RANGE(a, 1);
    ordinal_calendar_from_absolute(a, &iyd2, &iy2);
    if (iyd != iyd2 || iy != iy2)
	rb_raise(rb_eArgError, "invalid date");
    return INT2NUM(a);
}

static VALUE
calendar_ordinal_calendar_from_absolute(VALUE obj, VALUE a)
{
    int ia, yd, y;

    ia = NUM2INT(a); CHECK_DOMAIN(ia, 1);
    ordinal_calendar_from_absolute(ia, &yd, &y);
    return rb_ary_new3(2, INT2NUM(yd), INT2NUM(y));
}

static VALUE
calendar_absolute_from_julian_day_number(VALUE obj, VALUE j)
{
    int ij, a;

    ij = NUM2INT(j); CHECK_DOMAIN(ij, 1721426);
    a = absolute_from_julian_day_number(ij);
    CHECK_RANGE(a, 1);
    return INT2NUM(a);
}

static VALUE
calendar_julian_day_number_from_absolute(VALUE obj, VALUE a)
{
    int ia, j;

    ia = NUM2INT(a); CHECK_DOMAIN(ia, 1);
    j = julian_day_number_from_absolute(ia);
    CHECK_RANGE(j, 1721426);
    return INT2NUM(j);
}

static VALUE
calendar_julian_day_number_from_mjd(VALUE obj, VALUE m)
{
    int im, j;

    im = NUM2INT(m); CHECK_DOMAIN(im, -678576);
    j = julian_day_number_from_mjd(im);
    CHECK_RANGE(j, 1721426);
    return INT2NUM(j);

}

static VALUE
calendar_mjd_from_julian_day_number(VALUE obj, VALUE j)
{
    int ij, m;

    ij = NUM2INT(j); CHECK_DOMAIN(ij, 1721426);
    m = mjd_from_julian_day_number(ij);
    CHECK_RANGE(m, -678576);
    return INT2NUM(m);
}

static VALUE
calendar_absolute_from_lilian_day_number(VALUE obj, VALUE l)
{
    int il, a;

    il = NUM2INT(l); CHECK_DOMAIN(il, -577734);
    a = absolute_from_lilian_day_number(il);
    CHECK_RANGE(a, 1);
    return INT2NUM(a);
}

static VALUE
calendar_lilian_day_number_from_absolute(VALUE obj, VALUE a)
{
    int ia, l;

    ia = NUM2INT(a); CHECK_DOMAIN(ia, 1);
    l = lilian_day_number_from_absolute(ia);
    CHECK_RANGE(l, -577734);
    return INT2NUM(l);
}

static VALUE
calendar_day_of_week_from_absolute(VALUE obj, VALUE a)
{
    int ia, w;

    ia = NUM2INT(a); CHECK_DOMAIN(ia, 1);
    w = day_of_week_from_absolute(ia);
    return INT2NUM(w);
}

static VALUE
calendar_absolute_from_kyureki(VALUE obj, VALUE m, VALUE l, VALUE md, VALUE y)
{
    int j, a;
    QDATE q, q2;

    q.j = 0;
    q.y = NUM2INT(y); CHECK_DOMAIN(q.y, 1);
    q.yd = 0;
    q.m = NUM2INT(m); CHECK_DOMAIN2(q.m, 1, 12);
    q.md = NUM2INT(md); CHECK_DOMAIN2(q.md, 1, 30);
    q.wd = 0;
    q.leap = BOOL2INT(l);
    j = rqref(&q);
    a = absolute_from_julian_day_number(j);
    CHECK_RANGE(a, 1); /* 162193 - 767009 */
    qref(j, &q2);
    if (q.y != q2.y || q.m != q2.m || q.md != q2.md || q.leap != q2.leap)
	rb_raise(rb_eArgError, "invalid date");
    return INT2NUM(a);
}

static VALUE
calendar_kyureki_from_absolute(VALUE obj, VALUE a)
{
    int ia, j;
    QDATE q;

    ia = NUM2INT(a); CHECK_DOMAIN2(ia, 162193, 767009);
    j = julian_day_number_from_absolute(ia);
    qref(j, &q);
    return rb_ary_new3
	(4, INT2NUM(q.m), INT2BOOL(q.leap), INT2NUM(q.md), INT2NUM(q.y));
}

static VALUE
calendar_kyureki_day_of_week_from_absolute(VALUE obj, VALUE a)
{
    int ia, j;
    QDATE q;

    ia = NUM2INT(a); CHECK_DOMAIN2(ia, 162193, 767009);
    j = julian_day_number_from_absolute(ia);
    qref(j, &q);
    return INT2NUM(q.wd);
}

void
Init_calendar(void)
{
    VALUE mod, current_civil_month_names;

    mod = rb_define_module("Calendar");
    rb_define_module_function
	(mod, "gregorian_leap_year", calendar_gregorian_leap_year, 1);
    rb_define_alias(mod, "gregorian_leap_year?", "gregorian_leap_year");
    rb_define_module_function
	(mod, "gregorian_last_day_of_month",
	 calendar_gregorian_last_day_of_month, 2);
    rb_define_module_function
	(mod, "absolute_from_gregorian", calendar_absolute_from_gregorian, 3);
    rb_define_module_function
	(mod, "gregorian_from_absolute", calendar_gregorian_from_absolute, 1);
    rb_define_module_function
	(mod, "Kday_on_or_before", calendar_Kday_on_or_before, 2);
    rb_define_module_function
	(mod, "absolute_from_iso", calendar_absolute_from_iso, 3);
    rb_define_module_function
	(mod, "iso_from_absolute", calendar_iso_from_absolute, 1);
    rb_define_module_function
	(mod, "absolute_from_calendar_week", calendar_absolute_from_iso, 3);
    rb_define_module_function
	(mod, "calendar_week_from_absolute",  calendar_iso_from_absolute, 1);
    rb_define_module_function
	(mod, "julian_leap_year", calendar_julian_leap_year, 1);
    rb_define_alias(mod, "julian_leap_year?", "julian_leap_year");
    rb_define_module_function
	(mod, "julian_last_day_of_month",
	 calendar_julian_last_day_of_month, 2);
    rb_define_module_function
	(mod, "absolute_from_julian", calendar_absolute_from_julian, 3);
    rb_define_module_function
	(mod, "julian_from_absolute", calendar_julian_from_absolute, 1);
    rb_define_module_function
	(mod, "islamic_leap_year", calendar_islamic_leap_year, 1);
    rb_define_alias(mod, "islamic_leap_year?", "islamic_leap_year");
    rb_define_module_function
	(mod, "islamic_last_day_of_month",
	 calendar_islamic_last_day_of_month, 2);
    rb_define_module_function
	(mod, "absolute_from_islamic", calendar_absolute_from_islamic, 3);
    rb_define_module_function
	(mod, "islamic_from_absolute", calendar_islamic_from_absolute, 1);
    rb_define_module_function
	(mod, "hebrew_leap_year", calendar_hebrew_leap_year, 1);
    rb_define_alias(mod, "hebrew_leap_year?", "hebrew_leap_year");
    rb_define_module_function
	(mod, "last_month_of_hebrew_year",
	 calendar_last_month_of_hebrew_year, 1);
    rb_define_module_function
	(mod, "hebrew_last_day_of_month",
	 calendar_hebrew_last_day_of_month, 2);
    rb_define_module_function
	(mod, "hebrew_calendar_elapsed_days",
	 calendar_hebrew_calendar_elapsed_days, 1);
    rb_define_module_function
	(mod, "days_in_hebrew_year", calendar_days_in_hebrew_year, 1);
    rb_define_module_function
	(mod, "long_heshvan", calendar_long_heshvan, 1);
    rb_define_alias(mod, "long_heshvan?", "long_heshvan");
    rb_define_module_function
	(mod, "short_kislev", calendar_short_kislev, 1);
    rb_define_alias(mod, "short_kislev?", "short_kislev");
    rb_define_module_function
	(mod, "absolute_from_hebrew", calendar_absolute_from_hebrew, 3);
    rb_define_module_function
	(mod, "hebrew_from_absolute", calendar_hebrew_from_absolute, 1);
    rb_define_module_function
	(mod, "Nth_Kday", calendar_Nth_Kday, 4);
    rb_define_module_function
	(mod, "nicaean_rule_easter", calendar_nicaean_rule_easter, 1);
    rb_define_module_function
	(mod, "easter", calendar_easter, 1);
    rb_define_module_function
	(mod, "absolute_from_mayan_long_count",
	 calendar_absolute_from_mayan_long_count, 5);
    rb_define_module_function
	(mod, "mayan_long_count_from_absolute",
	 calendar_mayan_long_count_from_absolute, 1);
    rb_define_module_function
	(mod, "mayan_haab_from_absolute",
	 calendar_mayan_haab_from_absolute, 1);
    rb_define_module_function
	(mod, "mayan_haab_difference", calendar_mayan_haab_difference, 4);
    rb_define_module_function
	(mod, "mayan_haab_on_or_before", calendar_mayan_haab_on_or_before, 3);
    rb_define_module_function
	(mod, "mayan_tzolkin_from_absolute",
	 calendar_mayan_tzolkin_from_absolute, 1);
    rb_define_module_function
	(mod, "mayan_tzolkin_difference",
	 calendar_mayan_tzolkin_difference, 4);
    rb_define_module_function
	(mod, "mayan_tzolkin_on_or_before",
	 calendar_mayan_tzolkin_on_or_before, 3);
    rb_define_module_function
	(mod, "mayan_haab_tzolkin_on_or_before",
	 calendar_mayan_haab_tzolkin_on_or_before, 5);
    rb_define_module_function
	(mod, "french_last_day_of_month",
	 calendar_french_last_day_of_month, 2);
    rb_define_module_function
	(mod, "french_leap_year", calendar_french_leap_year, 1);
    rb_define_alias(mod, "french_leap_year?", "french_leap_year");
    rb_define_module_function
	(mod, "absolute_from_french", calendar_absolute_from_french, 3);
    rb_define_module_function
	(mod, "french_from_absolute", calendar_french_from_absolute, 1);
    rb_define_module_function
	(mod, "solar_longitude", calendar_solar_longitude, 1);
    rb_define_module_function
	(mod, "zodiac", calendar_zodiac, 1);
    rb_define_module_function
	(mod, "old_hindu_solar_from_absolute",
	 calendar_old_hindu_solar_from_absolute, 1);
    rb_define_module_function
	(mod, "absolute_from_old_hindu_solar",
	 calendar_absolute_from_old_hindu_solar, 3);
    rb_define_module_function
	(mod, "lunar_longitude", calendar_lunar_longitude, 1);
    rb_define_module_function
	(mod, "lunar_phase", calendar_lunar_phase, 1);
    rb_define_module_function
	(mod, "new_moon", calendar_new_moon, 1);
    rb_define_module_function
	(mod, "old_hindu_lunar_from_absolute",
	 calendar_old_hindu_lunar_from_absolute, 1);
    rb_define_module_function
	(mod, "old_hindu_lunar_precedes",
	 calendar_old_hindu_lunar_precedes, 8);
    rb_define_module_function
	(mod, "absolute_from_old_hindu_lunar",
	 calendar_absolute_from_old_hindu_lunar, 4);
    rb_define_module_function
	(mod, "world_leap_year", calendar_world_leap_year, 1);
    rb_define_alias(mod, "world_leap_year?", "world_leap_year");
    rb_define_module_function
	(mod, "world_last_day_of_month", calendar_world_last_day_of_month, 2);
    rb_define_module_function
	(mod, "absolute_from_world", calendar_absolute_from_world, 3);
    rb_define_module_function
	(mod, "world_from_absolute", calendar_world_from_absolute, 1);
    rb_define_module_function
	(mod, "world_day_of_week_from_absolute",
	 calendar_world_day_of_week_from_absolute, 1);
    rb_define_module_function
	(mod, "coptic_leap_year", calendar_coptic_leap_year, 1);
    rb_define_alias(mod, "coptic_leap_year?", "coptic_leap_year");
    rb_define_module_function
	(mod, "coptic_last_day_of_month",
	 calendar_coptic_last_day_of_month, 2);
    rb_define_module_function
	(mod, "absolute_from_coptic", calendar_absolute_from_coptic, 3);
    rb_define_module_function
	(mod, "coptic_from_absolute", calendar_coptic_from_absolute, 1);
    rb_define_module_function
	(mod, "ethiopian_leap_year", calendar_ethiopian_leap_year, 1);
    rb_define_alias(mod, "ethiopian_leap_year?", "ethiopian_leap_year");
    rb_define_module_function
	(mod, "ethiopian_last_day_of_month",
	 calendar_ethiopian_last_day_of_month, 2);
    rb_define_module_function
	(mod, "absolute_from_ethiopian", calendar_absolute_from_ethiopian, 3);
    rb_define_module_function
	(mod, "ethiopian_from_absolute", calendar_ethiopian_from_absolute, 1);
    rb_define_module_function
	(mod, "jalaali_leap_year", calendar_jalaali_leap_year, 1);
    rb_define_alias(mod, "jalaali_leap_year?", "jalaali_leap_year");
    rb_define_module_function
	(mod, "jalaali_last_day_of_month",
	 calendar_jalaali_last_day_of_month, 2);
    rb_define_module_function
	(mod, "absolute_from_jalaali", calendar_absolute_from_jalaali, 3);
    rb_define_module_function
	(mod, "jalaali_from_absolute", calendar_jalaali_from_absolute, 1);
    rb_define_module_function
	(mod, "absolute_from_ordinal_calendar",
	 calendar_absolute_from_ordinal_calendar, 2);
    rb_define_module_function
	(mod, "ordinal_calendar_from_absolute",
	 calendar_ordinal_calendar_from_absolute, 1);
    rb_define_module_function
	(mod, "absolute_from_julian_day_number",
	 calendar_absolute_from_julian_day_number, 1);
    rb_define_module_function
	(mod, "julian_day_number_from_absolute",
	 calendar_julian_day_number_from_absolute, 1);
    rb_define_module_function
	(mod, "absolute_from_jdn",
	 calendar_absolute_from_julian_day_number, 1);
    rb_define_module_function
	(mod, "jdn_from_absolute",
	 calendar_julian_day_number_from_absolute, 1);
    rb_define_module_function
	(mod, "julian_day_number_from_mjd",
	 calendar_julian_day_number_from_mjd, 1);
    rb_define_module_function
	(mod, "mjd_from_julian_day_number",
	 calendar_mjd_from_julian_day_number, 1);
    rb_define_module_function
	(mod, "jdn_from_mjd",
	 calendar_julian_day_number_from_mjd, 1);
    rb_define_module_function
	(mod, "mjd_from_jdn",
	 calendar_mjd_from_julian_day_number, 1);
    rb_define_module_function
	(mod, "absolute_from_lilian_day_number",
	 calendar_absolute_from_lilian_day_number, 1);
    rb_define_module_function
	(mod, "lilian_day_number_from_absolute",
	 calendar_lilian_day_number_from_absolute, 1);
    rb_define_module_function
	(mod, "absolute_from_ldn",
	 calendar_absolute_from_lilian_day_number, 1);
    rb_define_module_function
	(mod, "ldn_from_absolute",
	 calendar_lilian_day_number_from_absolute, 1);
    rb_define_module_function
	(mod, "day_of_week_from_absolute",
	 calendar_day_of_week_from_absolute, 1);
    rb_define_module_function
	(mod, "absolute_from_kyureki", calendar_absolute_from_kyureki, 4);
    rb_define_module_function
	(mod, "kyureki_from_absolute", calendar_kyureki_from_absolute, 1);
    rb_define_module_function
	(mod, "kyureki_day_of_week_from_absolute",
	 calendar_kyureki_day_of_week_from_absolute, 1);
    rb_define_const
	(mod, "VERSION",
	 rb_str_new2("$Revision: 1.13 $"));
    rb_define_const
	(mod, "DAY_NAMES",
	 rb_ary_new3
	 (7,
	  rb_str_new2("Sunday"), rb_str_new2("Monday"), rb_str_new2("Tuesday"),
	  rb_str_new2("Wednesday"), rb_str_new2("Thursday"),
	  rb_str_new2("Friday"), rb_str_new2("Saturday")));
    rb_define_const
	(mod, "MONTH_NAMES",
	 current_civil_month_names = rb_ary_new3
	 (13,
	  rb_str_new2("*"), rb_str_new2("January"), rb_str_new2("February"),
	  rb_str_new2("March"), rb_str_new2("April"), rb_str_new2("May"),
	  rb_str_new2("June"), rb_str_new2("July"), rb_str_new2("August"),
	  rb_str_new2("September"), rb_str_new2("October"),
	  rb_str_new2("November"), rb_str_new2("December")));
    rb_define_const(mod, "GREGORIAN_MONTH_NAMES", current_civil_month_names);
    rb_define_const(mod, "JULIAN_MONTH_NAMES", current_civil_month_names);
#ifndef PAPER
    rb_define_const
	(mod, "ISLAMIC_MONTH_NAMES",
	 rb_ary_new3
	 (13,
	  rb_str_new2("*"), rb_str_new2("Muharram"), rb_str_new2("Safar"),
	  rb_str_new2("Rabi` I"), rb_str_new2("Rabi` II"),
	  rb_str_new2("Jumada I"), rb_str_new2("Jumada II"),
	  rb_str_new2("Rajab"), rb_str_new2("Sha`ban"),
	  rb_str_new2("Ramadan"), rb_str_new2("Shawwal"),
	  rb_str_new2("Du al-Qa`da"), rb_str_new2("Du al-Hijja")));
#else
    rb_define_const
	(mod, "ISLAMIC_MONTH_NAMES",
	 rb_ary_new3
	 (13,
	  rb_str_new2("*"), rb_str_new2("Muharram"), rb_str_new2("Safar"),
	  rb_str_new2("Rabi I"), rb_str_new2("Rabi II"),
	  rb_str_new2("Jumada I"), rb_str_new2("Jumada II"),
	  rb_str_new2("Rajab"), rb_str_new2("Sha'ban"),
	  rb_str_new2("Ramadan"), rb_str_new2("Shawwal"),
	  rb_str_new2("Dhu al-Qada"), rb_str_new2("Dhu al-Hijjah")));
#endif
    rb_define_const
	(mod, "HEBREW_MONTH_NAMES",
	 rb_ary_new3
	 (2,
	  rb_ary_new3
	  (13,
	   rb_str_new2("*"), rb_str_new2("Nisan"), rb_str_new2("Iyyar"),
	   rb_str_new2("Sivan"), rb_str_new2("Tammuz"), rb_str_new2("Av"),
	   rb_str_new2("Elul"), rb_str_new2("Tishri"), rb_str_new2("Heshvan"),
	   rb_str_new2("Kislev"), rb_str_new2("Teveth"), rb_str_new2("Shevat"),
	   rb_str_new2("Adar")),
	  rb_ary_new3
	  (14,
	   rb_str_new2("*"), rb_str_new2("Nisan"), rb_str_new2("Iyyar"),
	   rb_str_new2("Sivan"), rb_str_new2("Tammuz"), rb_str_new2("Av"),
	   rb_str_new2("Elul"), rb_str_new2("Tishri"), rb_str_new2("Heshvan"),
	   rb_str_new2("Kislev"), rb_str_new2("Teveth"), rb_str_new2("Shevat"),
	   rb_str_new2("Adar I"), rb_str_new2("Adar II"))));
    rb_define_const
	(mod, "MAYAN_HAAB_MONTH_NAMES",
	 rb_ary_new3
	 (20,
	  rb_str_new2("*"), rb_str_new2("Pop"), rb_str_new2("Uo"),
	  rb_str_new2("Zip"), rb_str_new2("Zotz"), rb_str_new2("Tzec"),
	  rb_str_new2("Xul"), rb_str_new2("Yaxkin"), rb_str_new2("Mol"),
	  rb_str_new2("Chen"), rb_str_new2("Yax"), rb_str_new2("Zac"),
	  rb_str_new2("Ceh"), rb_str_new2("Mac"), rb_str_new2("Kankin"),
	  rb_str_new2("Muan"), rb_str_new2("Pax"), rb_str_new2("Kayab"),
	  rb_str_new2("Cumku"), rb_str_new2("Uayeb")));
    rb_define_const
	(mod, "MAYAN_TZOLKIN_MONTH_NAMES",
	 rb_ary_new3
	 (21,
	  rb_str_new2("*"), rb_str_new2("Imix"), rb_str_new2("Ik"),
	  rb_str_new2("Akbal"), rb_str_new2("Kan"), rb_str_new2("Chicchan"),
	  rb_str_new2("Cimi"), rb_str_new2("Manik"), rb_str_new2("Lamat"),
	  rb_str_new2("Muluc"), rb_str_new2("Oc"), rb_str_new2("Chuen"),
	  rb_str_new2("Eb"), rb_str_new2("Ben"), rb_str_new2("Ix"),
	  rb_str_new2("Men"), rb_str_new2("Cib"), rb_str_new2("Caban"),
	  rb_str_new2("Etznab"), rb_str_new2("Cauac"), rb_str_new2("Ahau")));
#ifndef CS_LATIN1
    rb_define_const
	(mod, "FRENCH_MONTH_NAMES",
	 rb_ary_new3
	 (14,
	  rb_str_new2("*"), rb_str_new2("Vende'miaire"),
	  rb_str_new2("Brumaire"), rb_str_new2("Frimaire"),
	  rb_str_new2("Nivo^se"), rb_str_new2("Pluvio^se"),
	  rb_str_new2("Vento^se"), rb_str_new2("Germinal"),
	  rb_str_new2("Flore'al"), rb_str_new2("Prairial"),
	  rb_str_new2("Messidor"), rb_str_new2("Thermidor"),
	  rb_str_new2("Fructidor"), rb_str_new2("*")));
#else
    rb_define_const
	(mod, "FRENCH_MONTH_NAMES",
	 rb_ary_new3
	 (14,
	  rb_str_new2("*"), rb_str_new2("Vend\351miaire"),
	  rb_str_new2("Brumaire"), rb_str_new2("Frimaire"),
	  rb_str_new2("Niv\364se"), rb_str_new2("Pluvi\364se"),
	  rb_str_new2("Vent\364se"), rb_str_new2("Germinal"),
	  rb_str_new2("Flor\351al"), rb_str_new2("Prairial"),
	  rb_str_new2("Messidor"), rb_str_new2("Thermidor"),
	  rb_str_new2("Fructidor"), rb_str_new2("*")));
#endif
#ifndef PAPER
    rb_define_const
	(mod, "FRENCH_DAY_NAMES",
	 rb_ary_new3
	 (11,
	  rb_str_new2("*"), rb_str_new2("Primidi"), rb_str_new2("Doudi"),
	  rb_str_new2("Tridi"), rb_str_new2("Quartidi"),
	  rb_str_new2("Quintidi"), rb_str_new2("Sextidi"),
	  rb_str_new2("Septidi"), rb_str_new2("Octidi"),
	  rb_str_new2("Nonidi"), rb_str_new2("Decadi")));
#else
    rb_define_const
	(mod, "FRENCH_DAY_NAMES",
	 rb_ary_new3
	 (11,
	  rb_str_new2("*"), rb_str_new2("Primidi"), rb_str_new2("Doudi"),
	  rb_str_new2("Tridi"), rb_str_new2("Quartidi"),
	  rb_str_new2("Quintidi"), rb_str_new2("Sextidi"),
	  rb_str_new2("Septidi"), rb_str_new2("Oxtidi"),
	  rb_str_new2("Nonidi"), rb_str_new2("Decadi")));
#endif
#ifndef CS_LATIN1
    rb_define_const
	(mod, "FRENCH_SANSCULOTTIDES_NAMES",
	 rb_ary_new3
	 (11,
	  rb_str_new2("*"), rb_str_new2("Vertu"), rb_str_new2("Genie"),
	  rb_str_new2("Labour"), rb_str_new2("Raison"),
	  rb_str_new2("Recompense"), rb_str_new2("Revolution"),
	  rb_str_new2("*"), rb_str_new2("*"), rb_str_new2("*"),
	  rb_str_new2("*")));
#else
    rb_define_const
	(mod, "FRENCH_SANSCULOTTIDES_NAMES",
	 rb_ary_new3
	 (11,
	  rb_str_new2("*"), rb_str_new2("Vertu"), rb_str_new2("G\351nie"),
	  rb_str_new2("Labour"), rb_str_new2("Raison"),
	  rb_str_new2("R\351compense"), rb_str_new2("R\351volution"),
	  rb_str_new2("*"), rb_str_new2("*"), rb_str_new2("*"),
	  rb_str_new2("*")));
#endif
#ifndef PAPER
    rb_define_const
	(mod, "OLD_HINDU_SOLAR_MONTH_NAMES",
	 rb_ary_new3
	 (13,
	  rb_str_new2("*"), rb_str_new2("Mesha"), rb_str_new2("Vrishabha"),
	  rb_str_new2("Mithuna"), rb_str_new2("Karka"), rb_str_new2("Simha"),
	  rb_str_new2("Kanya"), rb_str_new2("Tula"), rb_str_new2("Vris'chika"),
	  rb_str_new2("Dhanu"), rb_str_new2("Makara"), rb_str_new2("Kumbha"),
	  rb_str_new2("Mina")));
#else
    rb_define_const
	(mod, "OLD_HINDU_SOLAR_MONTH_NAMES",
	 rb_ary_new3
	 (13,
	  rb_str_new2("*"), rb_str_new2("Mesha"), rb_str_new2("Vrshabha"),
	  rb_str_new2("Mithuna"), rb_str_new2("Karka"), rb_str_new2("Simha"),
	  rb_str_new2("Kanya"), rb_str_new2("Tula"), rb_str_new2("Vris'chika"),
	  rb_str_new2("Dhanus"), rb_str_new2("Makara"), rb_str_new2("Kumbha"),
	  rb_str_new2("Mina")));
#endif
    rb_define_const
	(mod, "OLD_HINDU_LUNAR_MONTH_NAMES",
	 rb_ary_new3
	 (13,
	  rb_str_new2("*"), rb_str_new2("Chaitra"), rb_str_new2("Vaisa'akha"),
	  rb_str_new2("Jyaishtha"), rb_str_new2("Ashadha"),
	  rb_str_new2("S'ravana"), rb_str_new2("Bhadrapada"),
	  rb_str_new2("As'vina"), rb_str_new2("Karttika"),
	  rb_str_new2("Margas'ira"), rb_str_new2("Pausha"),
	  rb_str_new2("Magha"), rb_str_new2("Phalguna")));
    rb_define_const
	(mod, "WORLD_DAY_NAMES",
	 rb_ary_new3
	 (9,
	  rb_str_new2("Sunday"), rb_str_new2("Monday"), rb_str_new2("Tuesday"),
	  rb_str_new2("Wednesday"), rb_str_new2("Thursday"),
	  rb_str_new2("Friday"), rb_str_new2("Saturday"),
	  rb_str_new2("World"), rb_str_new2("Leap")));
    rb_define_const(mod, "WORLD_MONTH_NAMES", current_civil_month_names);
    rb_define_const
	(mod, "COPTIC_MONTH_NAMES",
	 rb_ary_new3
	 (14,
	  rb_str_new2("*"), rb_str_new2("Tut"), rb_str_new2("Babah"),
	  rb_str_new2("Hatur"), rb_str_new2("Kiyahk"), rb_str_new2("Tubah"),
	  rb_str_new2("Amshir"), rb_str_new2("Baramhat"),
	  rb_str_new2("Baramundah"), rb_str_new2("Bashans"),
	  rb_str_new2("Ba'unah"), rb_str_new2("Abib"), rb_str_new2("Misra"),
	  rb_str_new2("al-Nasi")));
    rb_define_const
	(mod, "ETHIOPIAN_MONTH_NAMES",
	 rb_ary_new3
	 (14,
	  rb_str_new2("*"), rb_str_new2("Maskaram"), rb_str_new2("Teqemt"),
	  rb_str_new2("Khedar"), rb_str_new2("Takhs'as'"), rb_str_new2("Ter"),
	  rb_str_new2("Yakatit"), rb_str_new2("Magabit"),
	  rb_str_new2("Miyazya"), rb_str_new2("Genbot"), rb_str_new2("Sane"),
	  rb_str_new2("Hamle"), rb_str_new2("Nahase"),
	  rb_str_new2("Paguemen")));
    rb_define_const
	(mod, "JALAALI_MONTH_NAMES",
	 rb_ary_new3
	 (13,
	  rb_str_new2("*"), rb_str_new2("Farvardin"),
	  rb_str_new2("Ordibehest"), rb_str_new2("Xordad"),
	  rb_str_new2("Tir"), rb_str_new2("Mordad"), rb_str_new2("Sahrivar"),
	  rb_str_new2("Mehr"), rb_str_new2("Aban"), rb_str_new2("Azar"),
	  rb_str_new2("Dey"), rb_str_new2("Bahman"), rb_str_new2("Esfand")));
    rb_define_const
	(mod, "KYUREKI_DAY_NAMES",
	 rb_ary_new3
	 (6,
	  rb_str_new2("Senkachi"), rb_str_new2("Tomobiki"),
	  rb_str_new2("Semmake"), rb_str_new2("Butsumetsu"),
	  rb_str_new2("Taian"), rb_str_new2("Shakku")));
    rb_define_const
	(mod, "KYUREKI_MONTH_NAMES",
	 rb_ary_new3
	 (13,
	  rb_str_new2("*"), rb_str_new2("Mutsuki"), rb_str_new2("Kisaragi"),
	  rb_str_new2("Yayoi"), rb_str_new2("Uzuki"), rb_str_new2("Satsuki"),
	  rb_str_new2("Minazuki"), rb_str_new2("Fuzuki"),
	  rb_str_new2("Hazuki"), rb_str_new2("Nagatsuki"),
	  rb_str_new2("Kannazuki"), rb_str_new2("Shimotsuki"),
	  rb_str_new2("Shiwasu")));
}
