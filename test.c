#define USEFUL_THINGS_IMPLEMENTATION
#define USEFUL_THINGS_STDLIB
#define USEFUL_THINGS_STRIP_PREFIX
#include "useful_things.h"

int main(void) {
    Arena *arena = arena_create();
    printf("Hello, world!\n");
    printf("%s\n", to_upper(STR("Hello!"), arena).data);
    printf("Hello again!\n");

    String csv = read_entire_file_as_string(STR("people-100.csv"), arena);
    // printf("%s\n", csv);
    size_t substrings_length = 0;
    String *substrings = split_string(csv, '\n', &substrings_length, true, arena);
    for(int i = 0; i < substrings_length; ++i) {
        String upper = to_upper(substrings[i], arena);
        printf("string: %s\n", substrings[i].data);
        printf("upper: %s\n", upper.data);
        printf("string: %s\nupper: %s\n", upper.data, substrings[i].data);
    }
    printf("%zu substrings\n", substrings_length);


    // size_t file_count = 0;
    // String *files = list_directory("./", &file_count, arena);
    // for(int i = 0; i < file_count; ++i) {
    //     printf("%s\n", files[i].data);
    // }
    // if(file_count >= 2) {
    //     String s = concat_strings(files[file_count - 1], files[file_count - 2], arena);
    //     printf("%s\n", s.data);
    // }
    // String s = STR("This is a string to test; splitting;strings does; it;; work?");
    // int substrings_length = 0;
    // String *substrings = split_string(s, ';', &substrings_length, true, arena);
    // for(int i = 0; i < substrings_length; ++i) {
    //     printf("%s\n", substrings[i]);
    // }
    // substrings = split_string(s, ';', &substrings_length, false, arena);
    // for(int i = 0; i < substrings_length; ++i) {
    //     printf("%.*s\n", substrings[i].length, substrings[i]);
    // }
    // printf("%s\n", concat_strings(STR("concat"), STR("test"), arena).data);

}
