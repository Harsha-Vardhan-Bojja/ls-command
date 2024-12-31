#ifndef DIRECTORY_LISTING_H
#define DIRECTORY_LISTING_H

#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>
#include <dirent.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>


/**
 * @struct DirEntry
 * @brief Represents a directory entry with metadata and pointers for a doubly linked list.
 * 
 * This structure holds the name and metadata of a directory entry, along with
 * pointers to the previous and next entries in the linked list.
 * 
 * @field name A dynamically allocated string holding the name of the directory entry.
 * @field st A `struct stat` object containing metadata about the directory entry such as
 *        file permissions, size, and modification time.
 * @field prev A pointer to the previous `DirEntry` in the doubly linked list.
 * @field next A pointer to the next `DirEntry` in the doubly linked list.
 */
typedef struct DirEntry {
    char *name;             ///< Name of the directory entry
    struct stat st;         ///< Metadata about the entry (file status)
    struct DirEntry *prev;  ///< Pointer to the previous entry in the linked list
    struct DirEntry *next;  ///< Pointer to the next entry in the linked list
} DirEntry;

/**
 * @struct Arguments
 * @brief Holds options for directory listing.
 * 
 * This structure contains boolean flags that determine how the directory
 * should be listed, such as whether to show detailed information, include hidden files,
 * list directories recursively, or show inode numbers.
 * 
 * @field long_listing A boolean flag to enable long listing format which includes detailed
 *        information such as permissions, number of links, owner, group, size, and modification time.
 * @field hidden_list A boolean flag indicating whether hidden files (those starting with '.')
 *        should be included in the listing.
 * @field recursive_list A boolean flag to enable recursive listing of directories.
 * @field show_inode A boolean flag indicating whether to display inode numbers in the listing.
 */
typedef struct arguments {
    bool long_listing;    ///< Flag to enable long listing format
    bool hidden_list;     ///< Flag to include hidden files (those starting with '.')
    bool recursive_list;  ///< Flag to list directories recursively
    bool show_inode;      ///< Flag to show inode numbers
} Arguments;

/**
 * @brief Lists the contents of a directory.
 * 
 * This function reads the directory specified by `path`, sorts the directory entries,
 * and prints them. It also handles recursive listing of subdirectories if the
 * `recursive_list` option is enabled.
 * 
 * @param path The path of the directory to list.
 * @param options The listing options, including long format, hidden files, recursive listing, and inode display.
 * 
 */
void list_directory(char *path, Arguments options);

/**
 * @brief Reads the contents of a directory and populates a doubly linked list with directory entries.
 * 
 * This function opens the directory specified by `path`, creates a new `DirEntry` for each
 * file or directory within it, and appends these entries to the linked list.
 * 
 * @param path The path of the directory to read.
 * @param head Pointer to the head of the doubly linked list to be populated.
 * @param tail Pointer to the tail of the doubly linked list to be populated.
 * @param options The listing options, including whether to include hidden files.
 * 
 * @return 0 on success, or -1 on error.
 */
int read_directory(char *path, DirEntry **head, DirEntry **tail, Arguments options);

/**
 * @brief Prints the details of a directory entry.
 * 
 * This function prints the metadata and name of a directory entry. The output format
 * depends on the `Arguments` options, including long listing format and inode display.
 * 
 * @param entry The directory entry to print.
 * @param options The listing options, including long format and inode display.
 * 
 */
void print_directory(DirEntry *entry, Arguments options);

/**
 * @brief Sorts the directory entries in a doubly linked list.
 * 
 * This function sorts the directory entries in ascending order based on their names
 * using a merge sort algorithm. The sorting is performed in-place, maintaining the
 * integrity of the doubly linked list.
 * 
 * @param head Pointer to the head of the doubly linked list to be sorted.
 */
void sort_entries(DirEntry **head);

/**
 * @brief Frees the memory allocated for the directory entries in a doubly linked list.
 * 
 * This function traverses the linked list, freeing the memory allocated for each
 * `DirEntry` and its name. It ensures that all dynamically allocated memory is properly
 * deallocated to prevent memory leaks.
 * 
 * @param head The head of the doubly linked list to be freed.
 */
void free_entries(DirEntry *head);

#endif // DIRECTORY_LISTING_H

