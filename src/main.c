#include <stdio.h>
#include "../include/library.h"


#define SUCCESS 1

int main() {
    book* books = NULL;
    int code = scan_books(&books);
    if (code != SUCCESS) {
        print_error(code);
    } else {
        print_reserved_books(books);
        free_books(books);
    }
    return 0;
}
