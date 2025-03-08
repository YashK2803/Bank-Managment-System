#include "global.h"

string addMonthsToDate(const string &date, int monthsToAdd)
{
  int year, month, day;
  struct tm date_tm = {0};

  // Parse the input date
  stringstream ss(date);
  ss >> get_time(&date_tm, "%Y-%m-%d");
  year = date_tm.tm_year + 1900;
  month = date_tm.tm_mon + 1;
  day = date_tm.tm_mday;

  // Calculate the new year and month
  month += monthsToAdd;
  while (month > 12)
  {
    month -= 12;
    year += 1;
  }
  while (month < 1)
  {
    month += 12;
    year -= 1;
  }

  // Adjust the day for months with fewer days
  int daysInMonth;
  switch (month)
  {
  case 4:
  case 6:
  case 9:
  case 11:
    daysInMonth = 30;
    break;
  case 2:
    // Check for leap year
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
    {
      daysInMonth = 29;
    }
    else
    {
      daysInMonth = 28;
    }
    break;
  default:
    daysInMonth = 31;
  }
  if (day > daysInMonth)
  {
    day = daysInMonth;
  }

  // Format the new date
  date_tm.tm_year = year - 1900;
  date_tm.tm_mon = month - 1;
  date_tm.tm_mday = day;

  char buffer[11];
  strftime(buffer, sizeof(buffer), "%Y-%m-%d", &date_tm);

  return string(buffer);
}

int compareDates(const string &date1, const string &date2)
{
  tm tm1 = {}, tm2 = {};

  // Parse date1 into tm1
  strptime(date1.c_str(), "%Y-%m-%d", &tm1);
  // Parse date2 into tm2
  strptime(date2.c_str(), "%Y-%m-%d", &tm2);

  // Convert tm structures to time_t for comparison
  time_t time1 = mktime(&tm1);
  time_t time2 = mktime(&tm2);

  // If date1 is earlier than date2, return -1
  if (difftime(time1, time2) < 0)
  {
    return -1;
  }
  else if (difftime(time1, time2) == 0)
  {
    return 0;
  }
  return 1;
}

int check_dates(const string &date1, const string &date2)
{
  tm tm1 = {}, tm2 = {};

  // Parse date1 into tm1
  strptime(date1.c_str(), "%Y-%m-%d", &tm1);
  // Parse date2 into tm2
  strptime(date2.c_str(), "%Y-%m-%d", &tm2);

  // Convert tm structures to time_t for comparison
  time_t time1 = mktime(&tm1);
  time_t time2 = mktime(&tm2);

  // If date1 is earlier than date2, return -1
  if (difftime(time1, time2) < 0)
  {
    return -1;
  }

  int yearDiff = tm1.tm_year - tm2.tm_year;
  int monthDiff = tm1.tm_mon - tm2.tm_mon;

  // Calculate total month difference
  int totalMonthDiff = yearDiff * 12 + monthDiff;

  // Adjust for partial month
  if (tm1.tm_mday < tm2.tm_mday)
  {
    totalMonthDiff--;
  }

  return totalMonthDiff;
}

string addDaysToDate(const string &date, int daysToAdd)
{
  struct tm date_tm = {0};
  stringstream ss(date);
  ss >> get_time(&date_tm, "%Y-%m-%d");

  time_t time = mktime(&date_tm);
  time += daysToAdd * 24 * 60 * 60; // Add the days in seconds

  date_tm = *localtime(&time);

  char buffer[11];
  strftime(buffer, sizeof(buffer), "%Y-%m-%d", &date_tm);

  return string(buffer);
}

bool validateDate(const string &date)
{
  struct tm tm = {0};
  istringstream ss(date);
  ss >> get_time(&tm, "%Y-%m-%d");

  if (ss.fail())
  {
    return false;
  }

  // Additional checks for valid day, month, and year
  if (tm.tm_year + 1900 < 1900 || tm.tm_mon < 0 || tm.tm_mon > 11 || tm.tm_mday < 1 || tm.tm_mday > 31)
  {
    return false;
  }

  // Check if the day is valid for the given month
  int daysInMonth;
  switch (tm.tm_mon + 1)
  {
  case 4:
  case 6:
  case 9:
  case 11:
    daysInMonth = 30;
    break;
  case 2:
    // Check for leap year
    if ((tm.tm_year + 1900) % 4 == 0 && ((tm.tm_year + 1900) % 100 != 0 || (tm.tm_year + 1900) % 400 == 0))
    {
      daysInMonth = 29;
    }
    else
    {
      daysInMonth = 28;
    }
    break;
  default:
    daysInMonth = 31;
  }

  if (tm.tm_mday > daysInMonth)
  {
    return false;
  }

  return true;
}
