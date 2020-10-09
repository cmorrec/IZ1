#include "library.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SUCCESS 1
#define ERROR 0
#define ERROR_MEMORY -1

void print_greeting() {
    puts("Hi, thank you to using my program. Next you should fill in data about this library.");
}

void print_info() {
    printf("%s\n%s\t%s\t%s\t%s\t%s\n%s\n%s\n",
           "Enter info about book",
           "isbn in format: XXX-X-XX-XXXXXX-X",
           "title",
           "year",
           "count",
           "number of readers",
           "For example",
           "000-0-00-000000-0 Title 1954 5 2"
    );
}

void print_reader_info() {
    printf("%s\n%s\t%s\t%s\n%s\n%s\n",
           "Enter info about reader",
           "name",
           "date_begin",
           "date_end",
           "For example",
           "name1 1 1 2017 2 1 2021"
    );
}

int scan_agree() {
    char c = ' ';
    puts("Add else one book? y/n");
    while (c != 'y' && c != 'n')
        scanf("%c", &c);
    if (c == 'n')
        return ERROR;
    return SUCCESS;
}

int validate_date(int* date_begin, int* date_end) {
    if (date_end[0] < 1)
        date_end[0] = 1;
    if (date_end[1] < 1)
        date_end[1] = 1;
    if (date_end[2] < 1)
        date_end[2] = 1;
    if (date_begin[0] < 1)
        date_begin[0] = 1;
    if (date_begin[1] < 1)
        date_begin[1] = 1;
    if (date_begin[2] < 1)
        date_begin[2] = 1;

    if (date_end[2] > date_begin[2])
        return SUCCESS;
    if (date_end[2] == date_begin[2]) {
        if (date_end[1] > date_begin[1])
            return SUCCESS;
        if (date_end[1] == date_begin[1])
            if (date_end[0] > date_begin[0])
                return SUCCESS;
    }
    return ERROR;
}

int validate_isbn(const char* isbn) {
    int i = 0;
    for (; i < 17 && isbn[i] != '\0'; i++) {
        if (i == 3 || i == 5 || i == 8 || i == 15) {
            if (isbn[i] != '-')
                return ERROR;
            else
                continue;
        }
        if (isbn[i] < '0' || isbn[i] > '9')
            return ERROR;
    }
    if (i < 17)
        return ERROR;
    return SUCCESS;
}

void validate_publish_year(int* publish_year) {
    time_t t = time(NULL);
    struct tm* now = localtime(&t);
    if (now->tm_year + 1900 < *publish_year)
        *publish_year = now->tm_year + 1900;
}

int validate_book(book* _book) {
    if (validate_isbn(_book->isbn) == ERROR)
        return ERROR;
    validate_publish_year(&(_book->publish_year));
    if (_book->count < 0)
        _book->count = 0;
    if (_book->num_readers < 0)
        _book->num_readers = 0;
    return SUCCESS;
}

int scan_book(book* _book) {
    return scanf("%s%25s%d%d%d",
                 _book->isbn,
                 _book->title,
                 &(_book->publish_year),
                 &(_book->count),
                 &(_book->num_readers));
}

int scan_reader(reader* _reader) {
    return scanf("%25s%d%d%d%d%d%d",
                 _reader->name,
                 &(_reader->date_begin[0]),
                 &(_reader->date_begin[1]),
                 &(_reader->date_begin[2]),
                 &(_reader->date_end[0]),
                 &(_reader->date_end[1]),
                 &(_reader->date_end[2]));
}

book* create_elem(book data) {
    book* head = (book*) calloc(1, sizeof(book));
    if (!head)
        return NULL;
    strcpy(head->isbn, data.isbn);
    strcpy(head->title, data.title);
    head->readers = data.readers;
    head->publish_year = data.publish_year;
    head->count = data.count;
    head->num_readers = data.num_readers;
    head->next = NULL;
    return head;
}

int push_back(book** begin_list, book data) {
    book* temp;
    if (*begin_list) {
        temp = *begin_list;
        while (temp->next)
            temp = temp->next;
        temp->next = create_elem(data);
        if (!temp->next)
            return ERROR_MEMORY;
    } else
        *begin_list = create_elem(data);
    return SUCCESS;
}

int scan_books(book** books) {
    print_greeting();
    book temp;
    while (scan_agree()) {
        print_info();
        scan_book(&temp);
        if (validate_book(&temp) == ERROR) {
            puts("ISBN is not correct. Re-enter this book.");
            continue;
        }
        if (temp.num_readers > 0)
            temp.readers = calloc(temp.num_readers, sizeof(reader));
        if (temp.readers == NULL) {
            free_books(*books);
            return ERROR_MEMORY;
        }
        int i = 0;
        while (temp.num_readers > i) {
            print_reader_info();
            scan_reader(temp.readers + i);
            if (validate_date(temp.readers[i].date_begin, temp.readers[i].date_end) == SUCCESS)
                i++;
            else
                puts("Dates are not correct. Re-enter this user");
        }
        if (push_back(books, temp) == ERROR_MEMORY) {
            free_books(*books);
            return ERROR_MEMORY;
        }
    }
    return SUCCESS;
}

void print_error(int code) {
    if (code == ERROR_MEMORY)
        puts("Sorry, but I can't get memory(");
    else
        puts("It's unusual mistake...");
}

int is_date_bigger_than_now(const int* date, struct tm* now) {
    if (date[2] > now->tm_year + 1900)
        return SUCCESS;
    if (date[2] == now->tm_year + 1900) {
        if (date[1] > now->tm_mon + 1)
            return SUCCESS;
        if (date[1] == now->tm_mon + 1)
            if (date[0] > now->tm_mday)
                return SUCCESS;
    }
    return 0;
}

int is_reserved(struct tm* now, int* date_begin, int* date_end) {
    if (is_date_bigger_than_now(date_begin, now) == ERROR && is_date_bigger_than_now(date_end, now) == SUCCESS)
        return 1;
    return 0;
}

int count_reserved_books(book* book) {
    int count = 0;
    time_t t = time(NULL);
    struct tm* now = localtime(&t);
    for (int i = 0; i < book->num_readers; i++)
        if (is_reserved(now, book->readers[i].date_begin, book->readers[i].date_end))
            count++;
    if (count > book->count)
        book->count = count;
    return count;
}

void print_book(book* book, int count_of_reserved) {
    printf("isbn: %s\ntitle: %s\ncount of books: %d\ncount of reserved books: %d\n\n",
           book->isbn,
           book->title,
           book->count,
           count_of_reserved);
}

void print_reserved_books(book* books) {
    while (books != NULL) {
        int count = count_reserved_books(books);
        if (count > 0)
            print_book(books, count);
        books = books->next;
    }
}

void free_books(book* books) {
    while (books) {
        book* temp = books;
        books = books->next;
        free(temp->readers);
        free(temp);
    }
}
