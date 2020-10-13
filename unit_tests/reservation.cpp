#include "gtest/gtest.h"

extern "C" {
#include "../include/library.h"
}

#define SUCCESS 1
#define ERROR 0

TEST(reservation_functions, cmp_date_and_now) {
    time_t t = time(NULL);
    struct tm* now = localtime(&t);
    int _date[3];
    _date[0] = 1;
    _date[1] = 1;
    _date[2] = 2021;
    ASSERT_EQ(is_date_bigger_than_now(_date, now), SUCCESS);
    _date[2] = 2020;
    ASSERT_EQ(is_date_bigger_than_now(_date, now), ERROR);
}

TEST(reservation_functions, is_reserved) {
    time_t t = time(NULL);
    struct tm* now = localtime(&t);
    int date_begin[3];
    int date_end[3];
    date_begin[0] = 1;
    date_begin[1] = 1;
    date_begin[2] = 2017;
    date_end[0] = 1;
    date_end[1] = 1;
    date_end[2] = 2018;
    ASSERT_EQ(is_reserved(now, date_begin, date_end), ERROR);
    date_end[2] = 2022;
    ASSERT_EQ(is_reserved(now, date_begin, date_end), SUCCESS);
    date_begin[2] = 2021;
    ASSERT_EQ(is_reserved(now, date_begin, date_end), ERROR);
}