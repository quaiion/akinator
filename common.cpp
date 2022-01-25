#include "common.hpp"

size_t get_file_size (FILE* const file_in) {

    assert (file_in);

    fseek (file_in, NO_OFFSET, SEEK_END);

    size_t filesize = (size_t) ftell (file_in);

    rewind (file_in);
    return filesize;
}

void flush_input_buffer (char symb_inserted) {

    if (symb_inserted != '\n') {

        while (getchar () != '\n') ;
    }
}
