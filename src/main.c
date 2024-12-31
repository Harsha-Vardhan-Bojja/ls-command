#include "header.h"
#include "colors.h"

int main(int argc, char *argv[]) {
    // Initialize options with default values (0)
    Arguments options = {0};
    int8_t opt;  // Variable to store option characters from getopt
    uint8_t path_number;  // Variable to iterate through directory arguments

    // Check if the number of arguments is within the expected range
    if (argc < 2 || argc > 6) {
        errno = EINVAL;  // Set errno to indicate invalid argument count
        perror("Invalid no.of arguments");
        return errno;  // Return the error code
    }
    // Parse command-line options using getopt
    while ((opt = getopt(argc, argv, "laRi")) != -1) {
        switch (opt) {
            case 'l':
                options.long_listing = 1;  // Enable long listing format
                break;
            case 'a':
                options.hidden_list = 1;  // Include hidden files (starting with '.')
                break;
            case 'R':
                options.recursive_list = 1;  // Enable recursive listing of directories
                break;
            case 'i':
                options.show_inode = 1;  // Display inode numbers
                break;
            case '?':
                errno = EINVAL;  // Set errno to indicate invalid option
                perror("Usage: Invalid option");
                return errno;  // Return the error code
            default:
                errno = EINVAL;  // Set errno to indicate invalid option
                perror("Usage: Invalid option");
                return errno;  // Return the error code
        }
    }

    // Check if at least one directory path is provided after options
    if (optind >= argc) {
        errno = EINVAL;  // Set errno to indicate missing directory argument
        perror("Usage: Missing directory argument");
        return errno;  // Return the error code
    }

    // Process each directory argument provided after options
    for (path_number = optind; argv[path_number] != NULL; path_number++) {
        list_directory(argv[path_number], options);  // List contents of the directory
    }

    return 0;  // Return success
}

