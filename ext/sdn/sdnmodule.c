/*
  sdnmodule.c: Copyright (C) 1997, 1999 Tadayoshi Funaba All rights reserved
  $Id: sdnmodule.c,v 1.6 1999/08/05 22:50:16 tadf Exp $
*/

#include "ruby.h"
#include "sdncal.h"

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
sdn_SdnToGregorian(VALUE obj, VALUE sdn)
{
    int y, m, md;
    long isdn;

    isdn = NUM2LONG(sdn); CHECK_DOMAIN(isdn, 1);
    SdnToGregorian(isdn, &y, &m, &md);
    return rb_ary_new3(3, INT2NUM(y), INT2NUM(m), INT2NUM(md));
}

static VALUE
sdn_GregorianToSdn(VALUE obj, VALUE y, VALUE m, VALUE md)
{
    int iy, im, imd, iy2, im2, imd2;
    long sdn;

    iy = NUM2INT(y); CHECK_DOMAIN(iy, -4714);
    im = NUM2INT(m); CHECK_DOMAIN2(im, 1, 12);
    imd = NUM2INT(md); CHECK_DOMAIN2(imd, 1, 31);
    sdn = GregorianToSdn(iy, im, imd);
    CHECK_RANGE(sdn, 1);
    SdnToGregorian(sdn, &iy2, &im2, &imd2);
    if (iy != iy2 || im != im2 || imd != imd2)
	rb_raise(rb_eArgError, "invalid date");
    return INT2NUM(sdn);
}

static VALUE
sdn_SdnToJulian(VALUE obj, VALUE sdn)
{
    int y, m, md;
    long isdn;

    isdn = NUM2LONG(sdn); CHECK_DOMAIN(isdn, 1);
    SdnToJulian(isdn, &y, &m, &md);
    return rb_ary_new3(3, INT2NUM(y), INT2NUM(m), INT2NUM(md));
}

static VALUE
sdn_JulianToSdn(VALUE obj, VALUE y, VALUE m, VALUE md)
{
    int iy, im, imd, iy2, im2, imd2;
    long sdn;

    iy = NUM2INT(y); CHECK_DOMAIN(iy, -4713);
    im = NUM2INT(m); CHECK_DOMAIN2(im, 1, 12);
    imd = NUM2INT(md); CHECK_DOMAIN2(imd, 1, 31);
    sdn = JulianToSdn(iy, im, imd);
    CHECK_RANGE(sdn, 1);
    SdnToJulian(sdn, &iy2, &im2, &imd2);
    if (iy != iy2 || im != im2 || imd != imd2)
	rb_raise(rb_eArgError, "invalid date");
    return INT2NUM(sdn);
}

static VALUE
sdn_SdnToJewish(VALUE obj, VALUE sdn)
{
    int y, m, md;
    long isdn;

    isdn = NUM2LONG(sdn); CHECK_DOMAIN(isdn, 347998);
    SdnToJewish(NUM2INT(sdn), &y, &m, &md);
    return rb_ary_new3(3, INT2NUM(y), INT2NUM(m), INT2NUM(md));
}

static VALUE
sdn_JewishToSdn(VALUE obj, VALUE y, VALUE m, VALUE md)
{
    int iy, im, imd, iy2, im2, imd2;
    long sdn;

    iy = NUM2INT(y); CHECK_DOMAIN(iy, 1);
    im = NUM2INT(m); CHECK_DOMAIN2(im, 1, 13);
    imd = NUM2INT(md); CHECK_DOMAIN2(imd, 1, 30);
    sdn = JewishToSdn(iy, im, imd);
    CHECK_RANGE(sdn, 1); /* 347998 */
    SdnToJewish(sdn, &iy2, &im2, &imd2);
    if (iy != iy2 || im != im2 || imd != imd2)
	rb_raise(rb_eArgError, "invalid date");
    return INT2NUM(sdn);
}

static VALUE
sdn_SdnToFrench(VALUE obj, VALUE sdn)
{
    int y, m, md;
    long isdn;

    isdn = NUM2LONG(sdn); CHECK_DOMAIN2(isdn, 2375840, 2380952);
    SdnToFrench(isdn, &y, &m, &md);
    return rb_ary_new3(3, INT2NUM(y), INT2NUM(m), INT2NUM(md));
}

static VALUE
sdn_FrenchToSdn(VALUE obj, VALUE y, VALUE m, VALUE md)
{
    int iy, im, imd, iy2, im2, imd2;
    long sdn;

    iy = NUM2INT(y); CHECK_DOMAIN(iy, 1);
    im = NUM2INT(m); CHECK_DOMAIN2(im, 1, 13);
    imd = NUM2INT(md); CHECK_DOMAIN2(imd, 1, 30);
    sdn = FrenchToSdn(iy, im, imd);
    CHECK_RANGE(sdn, 1);
    SdnToFrench(sdn, &iy2, &im2, &imd2);
    if (iy != iy2 || im != im2 || imd != imd2)
	rb_raise(rb_eArgError, "invalid date");
    return INT2NUM(sdn);
}

static VALUE
sdn_DayOfWeek(VALUE obj, VALUE sdn)
{
    long isdn;
    int wd;

    isdn = NUM2LONG(sdn);
    wd = DayOfWeek(isdn);
    return INT2NUM(wd);
}

void
Init_sdn(void)
{
    VALUE mod;

    mod = rb_define_module("Sdn");
    rb_define_module_function(mod, "SdnToGregorian", sdn_SdnToGregorian, 1);
    rb_define_module_function(mod, "GregorianToSdn", sdn_GregorianToSdn, 3);
    rb_define_module_function(mod, "SdnToJulian", sdn_SdnToJulian, 1);
    rb_define_module_function(mod, "JulianToSdn", sdn_JulianToSdn, 3);
    rb_define_module_function(mod, "SdnToJewish", sdn_SdnToJewish, 1);
    rb_define_module_function(mod, "JewishToSdn", sdn_JewishToSdn, 3);
    rb_define_module_function(mod, "SdnToFrench", sdn_SdnToFrench, 1);
    rb_define_module_function(mod, "FrenchToSdn", sdn_FrenchToSdn, 3);
    rb_define_module_function(mod, "DayOfWeek", sdn_DayOfWeek, 1);
    rb_define_const
	(mod, "VERSION",
	 rb_str_new2("$Revision: 1.6 $"));
    rb_define_const
	(mod, "MonthNameShort",
	 rb_ary_new3
	 (13,
	  rb_str_new2(MonthNameShort[0]), rb_str_new2(MonthNameShort[1]),
	  rb_str_new2(MonthNameShort[2]), rb_str_new2(MonthNameShort[3]),
	  rb_str_new2(MonthNameShort[4]), rb_str_new2(MonthNameShort[5]),
	  rb_str_new2(MonthNameShort[6]), rb_str_new2(MonthNameShort[7]),
	  rb_str_new2(MonthNameShort[8]), rb_str_new2(MonthNameShort[9]),
	  rb_str_new2(MonthNameShort[10]), rb_str_new2(MonthNameShort[11]),
	  rb_str_new2(MonthNameShort[12])));
    rb_define_const
	(mod, "MonthNameLong",
	 rb_ary_new3
	 (13,
	  rb_str_new2(MonthNameLong[0]), rb_str_new2(MonthNameLong[1]),
	  rb_str_new2(MonthNameLong[2]), rb_str_new2(MonthNameLong[3]),
	  rb_str_new2(MonthNameLong[4]), rb_str_new2(MonthNameLong[5]),
	  rb_str_new2(MonthNameLong[6]), rb_str_new2(MonthNameLong[7]),
	  rb_str_new2(MonthNameLong[8]), rb_str_new2(MonthNameLong[9]),
	  rb_str_new2(MonthNameLong[10]), rb_str_new2(MonthNameLong[11]),
	  rb_str_new2(MonthNameLong[12])));
    rb_define_const
	(mod, "JewishMonthName",
	 rb_ary_new3
	 (14,
	  rb_str_new2(JewishMonthName[0]), rb_str_new2(JewishMonthName[1]),
	  rb_str_new2(JewishMonthName[2]), rb_str_new2(JewishMonthName[3]),
	  rb_str_new2(JewishMonthName[4]), rb_str_new2(JewishMonthName[5]),
	  rb_str_new2(JewishMonthName[6]), rb_str_new2(JewishMonthName[7]),
	  rb_str_new2(JewishMonthName[8]), rb_str_new2(JewishMonthName[9]),
	  rb_str_new2(JewishMonthName[10]), rb_str_new2(JewishMonthName[11]),
	  rb_str_new2(JewishMonthName[12]), rb_str_new2(JewishMonthName[13])));
    rb_define_const
	(mod, "FrenchMonthName",
	 rb_ary_new3
	 (14,
	  rb_str_new2(FrenchMonthName[0]), rb_str_new2(FrenchMonthName[1]),
	  rb_str_new2(FrenchMonthName[2]), rb_str_new2(FrenchMonthName[3]),
	  rb_str_new2(FrenchMonthName[4]), rb_str_new2(FrenchMonthName[5]),
	  rb_str_new2(FrenchMonthName[6]), rb_str_new2(FrenchMonthName[7]),
	  rb_str_new2(FrenchMonthName[8]), rb_str_new2(FrenchMonthName[9]),
	  rb_str_new2(FrenchMonthName[10]), rb_str_new2(FrenchMonthName[11]),
	  rb_str_new2(FrenchMonthName[12]), rb_str_new2(FrenchMonthName[13])));
    rb_define_const
	(mod, "DayNameShort",
	 rb_ary_new3
	 (7,
	  rb_str_new2(DayNameShort[0]), rb_str_new2(DayNameShort[1]),
	  rb_str_new2(DayNameShort[2]), rb_str_new2(DayNameShort[3]),
	  rb_str_new2(DayNameShort[4]), rb_str_new2(DayNameShort[5]),
	  rb_str_new2(DayNameShort[6])));
    rb_define_const
	(mod, "DayNameLong",
	 rb_ary_new3
	 (7,
	  rb_str_new2(DayNameLong[0]), rb_str_new2(DayNameLong[1]),
	  rb_str_new2(DayNameLong[2]), rb_str_new2(DayNameLong[3]),
	  rb_str_new2(DayNameLong[4]), rb_str_new2(DayNameLong[5]),
	  rb_str_new2(DayNameLong[6])));
}
