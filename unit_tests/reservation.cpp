#include "gtest/gtest.h"

extern "C" {
#include "library.h"
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

void set_readers(book* book) {
    book->readers = static_cast<reader*>(calloc(book->num_readers, sizeof(reader)));
    for (int i = 0; i < book->num_readers; i++) {
        strcpy(book->readers[i].name, "name");
        book->readers[i].date_begin[0] = book->readers[i].date_end[0] = 1;
        book->readers[i].date_begin[1] = book->readers[i].date_end[1] = 1;
        book->readers[i].date_begin[2] = 2011;
        book->readers[i].date_end[2] = 2018 + i;
    }
}

TEST(reservation_functions, count_reserved_books) {
    book* books = nullptr;
    book temp;
    temp.count = 7;
    temp.num_readers = 6;
    temp.publish_year = 19;
    strcpy(temp.isbn, "000-0-00-000000-0");
    strcpy(temp.title, "title1");
    set_readers(&temp);
    int count_of_reserved = temp.num_readers - 3;
    ASSERT_EQ(count_reserved_books(&temp), count_of_reserved);
    push_back(&books, temp);
    set_readers(&temp);
    push_back(&books, temp);
    set_readers(&temp);
    push_back(&books, temp);
    book* iterator;
    iterator = books;
    int i = 0;
    while (iterator != nullptr) {
        ASSERT_EQ(count_reserved_books(iterator), count_of_reserved);
        iterator = iterator->next;
        i++;
    }
    free_books(books);
}