#! /usr/local/bin/ruby

require 'calendar'
include Calendar

def cal(m, y)
  printf("   %s %d\n", MONTH_NAMES[m], y)
  printf(" S  M Tu  W Th  F  S\n")
  fst = absolute_from_gregorian(m, 1, y)
  print('   ' * day_of_week_from_absolute(fst))
  days = gregorian_last_day_of_month(m, y)
  for i in 1..days
    printf('%2d', i)
    if day_of_week_from_absolute(fst + i) != 0
      print(' ')
    else
      print("\n")
    end
  end
  if ((day_of_week_from_absolute(fst) + days) / 7) < 5
    print("\n")
  end
  print("\n")
end

def main()
  if $*.length > 2
    printf($stderr, "usage: cal [ month [year] ]\n")
    exit(1)
  end
  now = Time.now
  m = now.mon
  y = now.year
  m = $*[0].to_i if $*.length >= 1
  y = $*[1].to_i if $*.length >= 2
  cal(m, y)
end

main()
