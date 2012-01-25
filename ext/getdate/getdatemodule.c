/*
  getdatemodule.c: Copyright (C) 1999,2001,2007 Tadayoshi Funaba All rights reserved
  $Id: getdatemodule.c,v 1.5 2007-12-24 21:47:45+09 tadf Exp $
*/

#include "ruby.h"
#include "getdate.h"

extern VALUE rb_cTime;

static VALUE
getdate_getdate(int argc, VALUE *argv, VALUE obj)
{
    VALUE str, time;
    long len;
    char *cstr;
    time_t ctime, when;

    if (rb_scan_args(argc, argv, "11", &str, &time) == 2)
	ctime = NUM2INT(rb_funcall(time, rb_intern("to_i"), 0, 0));
    cstr = rb_str2cstr(str, &len);
    if (strlen(cstr) < len) {
	char *d, *s, *e;

	d = alloca(len + 1);
	memcpy(d, cstr, len + 1);
	s = d;
	e = d + len;
	while (s < e) {
	    if (*s == '\0')
		*s = ' ';
	    s++;
	}
	cstr = d;
    }
    when = get_date(cstr, argc == 2 ? &ctime : NULL);
    if (when == -1)
	rb_raise(rb_eArgError, "invalid date");
    return rb_time_new(when, 0);
}

void
Init_getdate(void)
{
    VALUE mod;

    mod = rb_define_module("GetDate");
    rb_define_module_function(mod, "getdate", getdate_getdate, -1);
    rb_define_const
	(mod, "VERSION",
	 rb_str_new2("$Revision: 1.5 $"));
}
