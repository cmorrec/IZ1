#ifndef IZ1_LIBRARY_H
# define IZ1_LIBRARY_H

#include <time.h>

typedef struct s_reader {
    char name[25];
    int date_begin[3];
    int date_end[3];
} reader;

typedef struct s_book {
    char isbn[17];
    int publish_year;
    int count;
    int num_readers;
    char title[25];
    reader *readers;
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

#endif //IZ1_LIBRARY_H
