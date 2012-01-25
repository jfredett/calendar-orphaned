/*
  qref.h: Copyright (C) 1997,1998 Tadayoshi Funaba All rights reserved
  $Id: qref.h,v 1.3 1998/04/11 10:24:06 tadf Exp $
*/

typedef struct {
    int j;
    int y, yd, m, md, wd;
    int leap;
} QDATE;

extern void qref(int j, QDATE *q);
extern int rqref(QDATE *q);
