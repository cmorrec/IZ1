#ifndef IZ1_LIBRARY_H
# define IZ1_LIBRARY_H

#include <time.h>
#define NAME_LENGTH 25
#define TITLE_LENGTH 25
#define ISBN_LENGTH 17
#define DATE_SIZE 3

typedef struct s_reader {
    char name[NAME_LENGTH];
    int date_begin[DATE_SIZE];
    int date_end[DATE_SIZE];
} reader;

typedef struct s_book {
    char isbn[ISBN_LENGTH];
    int publish_year;
    int count;
    int num_readers;
    char title[TITLE_LENGTH];
    reader* readers;
    struct s_book* next;
} book;

int scan_books(book** books);
void print_error(int code);
void print_reserved_books(book* books);
void free_books(book* books);

// Validation
int validate_date(int* date_begin, int* date_end);
int validate_isbn(const char* isbn);
void validate_publish_year(int* publish_year);
int validate_book(book* _book);

// Reservation
int is_date_bigger_than_now(const int* date, struct tm* now);
int is_reserved(struct tm* now, int* date_begin, int* date_end);
int count_reserved_books(book* book);
int push_back(book** begin_list, book data);

#endif //IZ1_LIBRARY_H
