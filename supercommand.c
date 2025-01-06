#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

// File Operation Functions
void createOpenFile();
void changeFilePerm();
void readFile();
void writeFile();
void deleteFile();

// Directory Operation Functions
void create_directory();
void delete_directory();
void print_current_directory();
void list_directory_contents();

int main() {
    int choice;

    while (1) {
        printf("\nMain Menu:\n");
        printf("1. File Operations\n");
        printf("2. Directory Operations\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                // Sub-menu for file operations
                while (1) {
                    printf("\nFile Operations Menu:\n");
                    printf("1. Create/Open a File\n");
                    printf("2. Change File Permissions\n");
                    printf("3. Read a File\n");
                    printf("4. Write to a File\n");
                    printf("5. Delete a File\n");
                    printf("6. Back to Main Menu\n");
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
                            goto main_menu; // Return to main menu
                        default:
                            printf("Invalid choice! Please try again.\n");
                    }
                }

            case 2:
                // Sub-menu for directory operations
                while (1) {
                    printf("\nDirectory Operations Menu:\n");
                    printf("1. Create a Directory\n");
                    printf("2. Delete a Directory\n");
                    printf("3. Print Current Directory\n");
                    printf("4. List Directory Contents\n");
                    printf("5. Back to Main Menu\n");
                    printf("Enter your choice: ");
                    scanf("%d", &choice);

                    switch (choice) {
                        case 1:
                            create_directory();
                            break;
                        case 2:
                            delete_directory();
                            break;
                        case 3:
                            print_current_directory();
                            break;
                        case 4:
                            list_directory_contents();
                            break;
                        case 5:
                            goto main_menu; // Return to main menu
                        default:
                            printf("Invalid choice! Please try again.\n");
                    }
                }

            case 3:
                printf("Exiting program...\n");
                exit(0);

            default:
                printf("Invalid choice! Please try again.\n");
        }
        main_menu:; // Label for returning to the main menu
    }

    return 0;
}

// --- FILE OPERATIONS ---
void createOpenFile() {
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

void changeFilePerm() {
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

void readFile() {
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

void writeFile() {
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

void deleteFile() {
    char filename[256];
    printf("Enter the file name to delete: ");
    scanf("%s", filename);

    if (unlink(filename) == -1) {
        perror("Error deleting file");
    } else {
        printf("File '%s' deleted successfully.\n", filename);
    }
}

// --- DIRECTORY OPERATIONS ---
void create_directory() {
    char path[256];
    printf("Enter directory path to create: ");
    scanf("%s", path);

    if (mkdir(path, 0755) == 0) {
        printf("Directory '%s' created successfully.\n", path);
    } else {
        perror("Error creating directory");
    }
}

void delete_directory() {
    char path[256];
    printf("Enter directory path to delete: ");
    scanf("%s", path);

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

void list_directory_contents() {
    char path[256];
    printf("Enter directory path to list contents (or '.' for current directory): ");
    scanf("%s", path);

    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("Error opening directory");
        return;
    }

    struct dirent *entry;
    printf("Contents of directory '%s':\n", path);
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    closedir(dir);
}