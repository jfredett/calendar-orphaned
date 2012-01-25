# calclass.rb (prototype): Written by Tadayoshi Funaba 1999
# $Id: calclass.rb,v 1.3 1999/07/17 03:49:32 tadf Exp $

require 'calendar'

class Absolute

  include Comparable
  include Calendar
  extend Calendar

  def initialize(abs)
    if abs < 1
      fail ArgumentError, 'out of domain'
    end
    @abs = abs
  end

  def Absolute.at(abs)
    Absolute.new(abs)
  end

  attr :abs

  def jd
    julian_day_number_from_absolute(@abs)
  end

  def mjd
    mjd_from_absolute(@abs)
  end

  def + (other)
    case other
    when Numeric;  return type.at(@abs + other)
    end
    fail TypeError, 'expected numeric'
  end

  def - (other)
    case other
    when Numeric;  return type.at(@abs - other)
    when Absolute; return @abs - other.abs
    end
    fail TypeError, 'expected numeric or date'
  end

  def <=> (other)
    case other
    when Numeric;  return @abs <=> other
    when Absolute; return @abs <=> other.abs
    end
    fail TypeError, 'expected numeric or date'
  end

  def downto(min)
    @abs.downto(min.abs) do |abs|
      yield type.at(abs)
    end
    self
  end

  def upto(max)
    @abs.upto(max.abs) do |abs|
      yield type.at(abs)
    end
    self
  end

  def step(max, step)
    @abs.step(max.abs, step) do |abs|
      yield type.at(abs)
    end
    self
  end

  def eql? (other)
    self == other
  end

  def hash
    @abs
  end

  def to_s
    format('%d', @abs)
  end

end

class Gregorian < Absolute

  include Comparable
  include Calendar
  extend Calendar

  def initialize(month, day, year)
    abs = absolute_from_gregorian(month, day, year)
    super(abs)
    @month = month
    @day = day
    @year = year
  end

  def Gregorian.at(abs)
    month, day, year = gregorian_from_absolute(abs)
    Gregorian.new(month, day, year)
  end

  attr :month
  attr :day
  attr :year

  def dow
    day_of_week_from_absolute(@abs)
  end

  def doy
    abs = absolute_from_gregorian(1, 1, @year)
    1 + @abs - abs
  end

  def leap?
    gregorian_leap_year(@year)
  end

  def to_s
    format('%s, %s %d, %d',
	   DAY_NAMES[dow], GREGORIAN_MONTH_NAMES[month], day, year)
  end

end

class ISO < Absolute

  include Comparable
  include Calendar
  extend Calendar

  def initialize(week, day, year)
    abs = absolute_from_iso(week, day, year)
    super(abs)
    @week = week
    @day = day
    @year = year
  end

  def ISO.at(abs)
    week, day, year = iso_from_absolute(abs)
    ISO.new(week, day, year)
  end

  attr :week
  attr :day
  attr :year

  def doy
    abs = absolute_from_iso(1, 1, @year)
    1 + @abs - abs
  end

  def leap?
    gregorian_leap_year(@year)
  end

  def to_s
    format('%04d-W%02d-%02d', year, week, day)
  end

end

class Julian < Absolute

  include Comparable
  include Calendar
  extend Calendar

  def initialize(month, day, year)
    abs = absolute_from_julian(month, day, year)
    super(abs)
    @month = month
    @day = day
    @year = year
  end

  def Julian.at(abs)
    month, day, year = julian_from_absolute(abs)
    Julian.new(month, day, year)
  end

  attr :month
  attr :day
  attr :year

  def dow
    day_of_week_from_absolute(@abs)
  end

  def doy
    abs = absolute_from_julian(1, 1, @year)
    1 + @abs - abs
  end

  def leap?
    julian_leap_year(@year)
  end

  def to_s
    format('%s, %s %d, %d',
	   DAY_NAMES[dow], JULIAN_MONTH_NAMES[month], day, year)
  end

end

class Islamic < Absolute

  include Comparable
  include Calendar
  extend Calendar

  def initialize(month, day, year)
    abs = absolute_from_islamic(month, day, year)
    super(abs)
    @month = month
    @day = day
    @year = year
  end

  def Islamic.at(abs)
    month, day, year = islamic_from_absolute(abs)
    Islamic.new(month, day, year)
  end

  attr :month
  attr :day
  attr :year

  def doy
    abs = absolute_from_islamic(1, 1, @year)
    1 + @abs - abs
  end

  def leap?
    islamic_leap_year(@year)
  end

  def to_s
    format('%s %d, %d', ISLAMIC_MONTH_NAMES[month], day, year)
  end

end

class Hebrew < Absolute

  include Comparable
  include Calendar
  extend Calendar

  def initialize(month, day, year)
    abs = absolute_from_hebrew(month, day, year)
    super(abs)
    @month = month
    @day = day
    @year = year
  end

  def Hebrew.at(abs)
    month, day, year = hebrew_from_absolute(abs)
    Hebrew.new(month, day, year)
  end

  attr :month
  attr :day
  attr :year

  def doy
    abs = absolute_from_hebrew(1, 1, @year)
    1 + @abs - abs
  end

  def leap?
    hebrew_leap_year(@year)
  end

  def to_s
    format('%s %d, %d',
	   HEBREW_MONTH_NAMES[if leap? then 1 else 0 end][month], day, year)
  end

end

class Kyureki < Absolute

  include Comparable
  include Calendar
  extend Calendar

  def initialize(month, leap, day, year)
    abs = absolute_from_kyureki(month, leap, day, year)
    super(abs)
    @month = month
    @leap = leap
    @day = day
    @year = year
  end

  def Kyureki.at(abs)
    month, leap, day, year = kyureki_from_absolute(abs)
    Kyureki.new(month, leap, day, year)
  end

  attr :month
  attr :leap
  attr :day
  attr :year

  def dow
    kyureki_day_of_week_from_absolute(@abs)
  end

  def doy
    abs = absolute_from_kyureki(1, false, 1, @year)
    1 + @abs - abs
  end

  def leap?
    leap
  end

  def to_s
    format('%s, %s%s, %d, %d',
	   KYUREKI_DAY_NAMES[dow],
	   (if leap? then 'Uru-' else '' end),
	   KYUREKI_MONTH_NAMES[month], day, year)
  end

end
