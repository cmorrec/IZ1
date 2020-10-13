#include "gtest/gtest.h"

extern "C" {
#include "../include/library.h"
}

#define SUCCESS 1
#define ERROR 0

TEST(validate_functions, validate_dates) {
    int date_begin[3];
    int date_end[3];
    date_begin[0] = 1;
    date_begin[1] = 1;
    date_begin[2] = 2017;
    date_end[0] = 3;
    date_end[1] = 8;
    date_end[2] = 2016;
    ASSERT_EQ(validate_date(date_begin, date_end), ERROR);
    date_end[2] = 2017;
    ASSERT_EQ(validate_date(date_begin, date_end), SUCCESS);
    date_end[0] = 1;
    date_end[1] = 1;
    ASSERT_EQ(validate_date(date_begin, date_end), ERROR);
}

TEST(validate_functions, validate_isbn) {
    char isbn_correct[] = "000-0-00-000000-0";
    ASSERT_EQ(validate_isbn(isbn_correct), SUCCESS);
    char isbn_too_long[] = "000-0-00-000000-0lhkkj";
    ASSERT_EQ(validate_isbn(isbn_too_long), SUCCESS);
    char isbn_too_short[] = "000-0-00-00";
    ASSERT_EQ(validate_isbn(isbn_too_short), ERROR);
    char isbn_not_correct1[] = "0000--00-000000-0";
    ASSERT_EQ(validate_isbn(isbn_not_correct1), ERROR);
    char isbn_not_correct2[] = "000-0-0i-000000-0";
    ASSERT_EQ(validate_isbn(isbn_not_correct2), ERROR);
}

TEST(validate_functions, validate_publish_year) {
    int correct_year = 1956;
    validate_publish_year(&correct_year);
    ASSERT_EQ(correct_year, 1956);
    int not_correct_year = 2041;
    validate_publish_year(&not_correct_year);
    ASSERT_EQ(not_correct_year, 2020);
}

TEST(validate_functions, validate_book) {
    book correct_book;
    strcpy(correct_book.title, "Title");
    strcpy(correct_book.isbn, "000-0-00-000000-0");
    correct_book.publish_year = -1945;
    correct_book.count = -56;
    correct_book.num_readers = -78;
    ASSERT_EQ(validate_book(&correct_book), SUCCESS);
    book not_correct_book;
    strcpy(not_correct_book.title, "Title");
    strcpy(not_correct_book.isbn, "000");
    not_correct_book.publish_year = -1945;
    not_correct_book.count = -56;
    not_correct_book.num_readers = -78;
    ASSERT_EQ(validate_book(&not_correct_book), ERROR);
}