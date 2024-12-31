#include "header.h"
#include "colors.h"

// Function to list directory contents
void list_directory(char *path, Arguments options) {
    DirEntry *head = NULL;
    DirEntry *tail = NULL;

    // Read the directory and populate the list
    if (read_directory(path, &head, &tail, options) != 0) {
        fprintf(stderr, "Error reading directory %s\n", path);
        return;
    }

    // Sort the list
    sort_entries(&head);

    printf("\n%s:\n", path);

    // Print directory entries
    DirEntry *current = head;
    while (current != NULL) {
        print_directory(current, options);
        current = current->next;
    }

    // Recursive listing if the option is set
    if (options.recursive_list) {
        current = head;
        while (current != NULL) {
            if (S_ISDIR(current->st.st_mode) && current->name[0] != '.') {
                char subdir_path[512];
                snprintf(subdir_path, sizeof(subdir_path), "%s/%s", path, current->name);
                list_directory(subdir_path, options);
            }
            current = current->next;
        }
    }

    // Free the allocated memory for the list
    free_entries(head);
}

// Function to read directory entries
int read_directory(char *path, DirEntry **head, DirEntry **tail, Arguments options) {
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("Error opening directory");
        return -1; // Indicate failure
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // Skip hidden files if not listed
        if (!(options.hidden_list) && (entry->d_name[0] == '.')) {
            continue;
        }

        // Allocate memory for new entry
        DirEntry *new_entry = calloc(1, sizeof(*new_entry));
        if (new_entry == NULL) {
            perror("Failed to allocate memory");
            closedir(dir);
            return -1; // Indicate failure
        }

        // Allocate and copy the name
        new_entry->name = malloc(strlen(entry->d_name) + 1);
        if (new_entry->name == NULL) {
            perror("Failed to allocate memory for name");
            free(new_entry);
            closedir(dir);
            return -1; // Indicate failure
        }
        strcpy(new_entry->name, entry->d_name);

        // Get full path for stat
        char full_path[512];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        // Get file status
        if (stat(full_path, &new_entry->st) == -1) {
            perror("stat");
            free(new_entry->name);
            free(new_entry);
            continue; // Continue with the next entry
        }

        // Insert the new entry into the list
        new_entry->next = NULL;
        new_entry->prev = *tail;
        if (*tail != NULL) {
            (*tail)->next = new_entry;
        } else {
            *head = new_entry;
        }
        *tail = new_entry;
    }

    closedir(dir);
    return 0; // Success
}

// Function to print a directory entry
void print_directory(DirEntry *entry, Arguments options) {
    if (options.show_inode) {
        printf("%ld ", entry->st.st_ino);
    }

    if (options.long_listing) {
        printf("%c", S_ISDIR(entry->st.st_mode) ? 'd' : '-');
        printf("%c", (entry->st.st_mode & S_IRUSR) ? 'r' : '-');
        printf("%c", (entry->st.st_mode & S_IWUSR) ? 'w' : '-');
        printf("%c", (entry->st.st_mode & S_IXUSR) ? 'x' : '-');
        printf("%c", (entry->st.st_mode & S_IRGRP) ? 'r' : '-');
        printf("%c", (entry->st.st_mode & S_IWGRP) ? 'w' : '-');
        printf("%c", (entry->st.st_mode & S_IXGRP) ? 'x' : '-');
        printf("%c", (entry->st.st_mode & S_IROTH) ? 'r' : '-');
        printf("%c", (entry->st.st_mode & S_IWOTH) ? 'w' : '-');
        printf("%c", (entry->st.st_mode & S_IXOTH) ? 'x' : '-');

        printf(" %lu", entry->st.st_nlink);

        struct passwd *user_name;
        struct passwd pwd;
        char buf[1024];
        int ret = getpwuid_r(entry->st.st_uid, &pwd, buf, sizeof(buf), &user_name);
        if (ret != 0) {
            perror("getpwuid_r failed");
            return;
        }
        printf(" %s", user_name->pw_name);

        struct group *grp_name;
        struct group grp;
        ret = getgrgid_r(entry->st.st_gid, &grp, buf, sizeof(buf), &grp_name);
        if (ret != 0) {
            perror("getgrgid_r failed");
            return;
        }
        printf(" %s", grp_name->gr_name);
        printf(" %7lu", entry->st.st_size);

        char time_buf[256];
        strftime(time_buf, sizeof(time_buf), "%b %d %H:%M", localtime(&entry->st.st_mtime));
        printf(" %s", time_buf);
    }
    if (S_ISDIR(entry->st.st_mode)){
    printf(" %s%s%s\n",BLUE,entry->name,RESET);
    }
    else{
    printf(" %s\n", entry->name);
    }
}

// Function to sort entries using merge sort
void sort_entries(DirEntry **head) {
    if (*head == NULL || (*head)->next == NULL) {
        return;
    }

    // Split the list into two halves
    DirEntry *mid = *head;
    DirEntry *fast = (*head)->next;
    while (fast != NULL && fast->next != NULL) {
        mid = mid->next;
        fast = fast->next->next;
    }

    DirEntry *left = *head;
    DirEntry *right = mid->next;
    mid->next = NULL;
    if (right != NULL) right->prev = NULL;

    // Recursively sort the two halves
    sort_entries(&left);
    sort_entries(&right);

    // Merge the sorted halves
    DirEntry dummy;
    DirEntry *tail = &dummy;
    dummy.next = NULL;

    while (left != NULL && right != NULL) {
        if (strcmp(left->name, right->name) < 0) {
            tail->next = left;
            left->prev = tail;
            left = left->next;
        } else {
            tail->next = right;
            right->prev = tail;
            right = right->next;
        }
        tail = tail->next;
    }
    tail->next = (left != NULL) ? left : right;
    if (tail->next != NULL) tail->next->prev = tail;

    *head = dummy.next;
}

// Function to free all entries in the list
void free_entries(DirEntry *head) {
    DirEntry *current = head;
    while (current != NULL) {
        DirEntry *next = current->next;
        free(current->name); // Free the name
        free(current); // Free the entry
        current = next;
    }
}
