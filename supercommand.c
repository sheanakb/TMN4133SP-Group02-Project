#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

//File Operation
void create_or_open_file();
void change_file_permissions();
void read_file();
void write_to_file();
void delete_file();

int main() {
    int choice;

    while (1) {
        printf("\nFile Operations Menu:\n");
        printf("1. Create/Open a File\n");
        printf("2. Change File Permissions\n");
        printf("3. Read a File\n");
        printf("4. Write to a File\n");
        printf("5. Delete a File\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                create_or_open_file();
                break;
            case 2:
                change_file_permissions();
                break;
            case 3:
                read_file();
                break;
            case 4:
                write_to_file();
                break;
            case 5:
                delete_file();
                break;
            case 6:
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }

    return 0;
}

void create_or_open_file() {
    char filename[256];
    printf("Enter the file name to create/open: ");
    scanf("%s", filename);

    int fd = open(filename, O_CREAT | O_RDWR, 0644);
    if (fd == -1) {
        perror("Error creating/opening file");
    } else {
        printf("File '%s' created/opened successfully.\n", filename);
        close(fd);
    }
}

void change_file_permissions() {
    char filename[256];
    int permissions;

    printf("Enter the file name to change permissions: ");
    scanf("%s", filename);
    printf("Enter the new permissions in octal (e.g., 644): ");
    scanf("%o", &permissions);

    if (chmod(filename, permissions) == -1) {
        perror("Error changing file permissions");
    } else {
        printf("Permissions for '%s' changed successfully.\n", filename);
    }
}

void read_file() {
    char filename[256];
    printf("Enter the file name to read: ");
    scanf("%s", filename);

    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        return;
    }

    char buffer[1024];
    ssize_t bytesRead;

    printf("File contents:\n");
    while ((bytesRead = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytesRead] = '\0';
        printf("%s", buffer);
    }

    if (bytesRead == -1) {
        perror("Error reading file");
    }

    close(fd);
}

void write_to_file() {
    char filename[256];
    printf("Enter the file name to write to: ");
    scanf("%s", filename);

    int fd = open(filename, O_WRONLY | O_APPEND);
    if (fd == -1) {
        perror("Error opening file");
        return;
    }

    printf("Enter text to write to the file (end with a single newline):\n");
    getchar(); // Clear newline from input buffer

    char buffer[1024];
    fgets(buffer, sizeof(buffer), stdin);

    if (write(fd, buffer, strlen(buffer)) == -1) {
        perror("Error writing to file");
    } else {
        printf("Data written successfully to '%s'.\n", filename);
    }

    close(fd);
}

void delete_file() {
    char filename[256];
    printf("Enter the file name to delete: ");
    scanf("%s", filename);

    if (unlink(filename) == -1) {
        perror("Error deleting file");
    } else {
        printf("File '%s' deleted successfully.\n", filename);
    }
}

//Directory operations = Rai
void create_directory(const char *path) {
    if (mkdir(path, 0755) == 0) {
        printf("Directory '%s' created successfully.\n", path);
    } else {
        perror("Error creating directory");
    }
}

void delete_directory(const char *path) {
    if (rmdir(path) == 0) {
        printf("Directory '%s' deleted successfully.\n", path);
    } else {
        perror("Error deleting directory");
    }
}

void print_current_directory() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working directory: %s\n", cwd);
    } else {
        perror("Error getting current working directory");
    }
}

void list_directory_contents(const char *path) {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(path);
    if (dir == NULL) {
        perror("Error opening directory");
        return;
    }

    printf("Contents of directory '%s':\n", path);
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "-m") == 0) {
        int mode = atoi(argv[2]);

        if (mode == 2) { // Directory operations
            int operation = atoi(argv[3]);
            const char *path = (argc > 4) ? argv[4] : ".";

            switch (operation) {
                case 1: // Create directory
                    create_directory(path);
                    break;
                case 2: // Delete directory
                    delete_directory(path);
                    break;
                case 3: // Print current directory
                    print_current_directory();
                    break;
                case 4: // List directory contents
                    list_directory_contents(path);
                    break;
                default:
                    printf("Invalid operation for directory mode.\n");
            }
        } else {
            printf("Invalid mode. Use mode 2 for directory operations.\n");
        }

        return 0;
    }

    int choice;
    char path[1024];

    do {
        printf("\nDirectory Operations:\n");
        printf("1. Create a directory\n");
        printf("2. Delete a directory\n");
        printf("3. Print the current working directory\n");
        printf("4. List directory contents\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // To consume the newline character

        switch (choice) {
            case 1:
                printf("Enter directory path to create: ");
                scanf("%1023s", path);
                create_directory(path);
                break;
            case 2:
                printf("Enter directory path to delete: ");
                scanf("%1023s", path);
                delete_directory(path);
                break;
            case 3:
                print_current_directory();
                break;
            case 4:
                printf("Enter directory path to list (or press Enter for current directory): ");
                fgets(path, sizeof(path), stdin);
                if (path[strlen(path) - 1] == '\n') {
                    path[strlen(path) - 1] = '\0';
                }
                if (strlen(path) == 0) {
                    strcpy(path, ".");
                }
                list_directory_contents(path);
                break;
            case 5:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);

    return 0;
}