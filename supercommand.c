#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <time.h>
#include <termios.h>

// File Operation Functions
void createOpenFile(char *filename);
void changeFilePerm(char *filename, int permissions);
void readFile(char *filename);
void writeFile(char *filename);
void deleteFile(char *filename);

// Directory Operation Functions
void create_directory(char *path);
void delete_directory(char *path);
void print_current_directory();
void list_directory_contents(char *path);

// Keylogger Function
void keylogger(char *logFile);

int main(int argc, char *argv[]) {
    int operation = -1;
    int mode = -1;
    char *filename = NULL;

    if (argc > 1) {
        // Command line operation
        if (strcmp(argv[1], "-m") == 0) {  // Check for -m flag
            operation = atoi(argv[2]);  // Set operation (1, 2, or 3)
            filename = argv[3];  // Set filename

            if (operation == 3) { // Keylogger case (mode 3)
                if (argc == 4) { // Only need 4 arguments for keylogger (operation + filename)
                    keylogger(filename);
                } else {
                    printf("Usage: ./supercommand -m 3 <filename>\n");
                }
            } else if (operation == 1) { // File operations
                if (argc >= 5) { // Need at least 5 arguments for file operations
                    mode = atoi(argv[3]);  // Set mode (1-5)
                    filename = argv[4];  // Set filename
                    switch (mode) {
                        case 1:  // Create/Open file
                            createOpenFile(filename);
                            break;
                        case 2:  // Change file permissions
                            if (argc >= 6) {
                                changeFilePerm(filename, atoi(argv[5]));  // Permissions argument
                            } else {
                                printf("Error: Missing permission argument.\n");
                            }
                            break;
                        case 3:  // Read file
                            readFile(filename);
                            break;
                        case 4:  // Write file
                            writeFile(filename);
                            break;
                        case 5:  // Delete file
                            deleteFile(filename);
                            break;
                        default:
                            printf("Invalid file operation mode.\n");
                            break;
                    }
                } else {
                    printf("Usage: ./supercommand -m 1 <mode> <filename> [permissions]\n");
                }
            } else if (operation == 2) { // Directory operations
                if (argc >= 5) { // Need at least 5 arguments for directory operations
                    mode = atoi(argv[3]);  // Set mode (1-4)
                    filename = argv[4];  // Set filename
                    switch (mode) {
                        case 1:  // Create directory
                            create_directory(filename);
                            break;
                        case 2:  // Delete directory
                            delete_directory(filename);
                            break;
                        case 3:  // Print current directory
                            print_current_directory();
                            break;
                        case 4:  // List directory contents
                            list_directory_contents(filename);
                            break;
                        default:
                            printf("Invalid directory operation mode.\n");
                            break;
                    }
                } else {
                    printf("Usage: ./supercommand -m 2 <mode> <filename>\n");
                }
            } else {
                printf("Invalid operation mode.\n");
            }
        } else {
            printf("Usage: ./supercommand -m <operation> <mode> <filename> [permissions]\n");
        }
    } else {
        printf("Usage: ./supercommand -m <operation> <mode> <filename> [permissions]\n");
    }
    return 0;  // Exit after executing command-line task

    // Original menu-based execution
    int choice;

    while (1) {
        printf("\nMain Menu:\n");
        printf("1. File Operations\n");
        printf("2. Directory Operations\n");
        printf("3. Keylogger\n");
        printf("4. Exit\n");
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
                            createOpenFile(NULL);  // Prompt inside function
                            break;
                        case 2:
                            changeFilePerm(NULL, 0);  // Prompt inside function
                            break;
                        case 3:
                            readFile(NULL);  // Prompt inside function
                            break;
                        case 4:
                            writeFile(NULL);  // Prompt inside function
                            break;
                        case 5:
                            deleteFile(NULL);  // Prompt inside function
                            break;
                        case 6:
                            goto main_menu; // Return to main menu
                        default:
                            printf("Invalid selection! Please select between option 1-6 ONLY.\n");
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
                            create_directory(NULL);  // Prompt inside function
                            break;
                        case 2:
                            delete_directory(NULL);  // Prompt inside function
                            break;
                        case 3:
                            print_current_directory();
                            break;
                        case 4:
                            list_directory_contents(NULL);  // Prompt inside function
                            break;
                        case 5:
                            goto main_menu; // Return to main menu
                        default:
                            printf("Invalid choice! Please try again.\n");
                    }
                }

            case 3:
                keylogger(NULL);  // Keylogger function
                break;

            case 4:
                printf("Exiting program...\n");
                exit(0);

            default:
                printf("Invalid selection! Please select between options 1-4 ONLY.\n");
        }
        main_menu:; // Label for returning to the main menu
    }

    return 0;
}
}

// --- FILE OPERATIONS ---
void createOpenFile(char *filename) {
    char file[256];
    if (!filename) {
        printf("Enter the file name to create/open: ");
        scanf("%s", file);
        filename = file;
    }

    int fd = open(filename, O_CREAT | O_RDWR, 0644);
    if (fd == -1) {
        perror("Error creating/opening file");
    } else {
        printf("File '%s' created/opened successfully.\n", filename);
        close(fd);
    }
}

void changeFilePerm(char *filename, int permissions) {
    if (!filename) {
        printf("Enter the file name to change permissions: ");
        scanf("%s", filename);
        printf("Enter the new permissions in octal (e.g., 644): ");
        scanf("%o", &permissions);
    }

    if (chmod(filename, permissions) == -1) {
        perror("Error changing file permissions");
    } else {
        printf("Permissions for '%s' changed successfully.\n", filename);
    }
}

void readFile(char *filename) {
    if (!filename) {
        printf("Enter the file name to read: ");
        scanf("%s", filename);
    }

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

void writeFile(char *filename) {
    if (!filename) {
        printf("Enter the file name to write to: ");
        scanf("%s", filename);
    }

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

void deleteFile(char *filename) {
    if (!filename) {
        printf("Enter the file name to delete: ");
        scanf("%s", filename);
    }

    if (unlink(filename) == -1) {
        perror("Error deleting file");
    } else {
        printf("File '%s' deleted successfully.\n", filename);
    }
}

// --- DIRECTORY OPERATIONS ---
void create_directory(char *path) {
    if (!path) {
        printf("Enter directory path to create: ");
        scanf("%s", path);
    }

    if (mkdir(path, 0755) == 0) {
        printf("Directory '%s' created successfully.\n", path);
    } else {
        perror("Error creating directory");
    }
}

void delete_directory(char *path) {
    if (!path) {
        printf("Enter directory path to delete: ");
        scanf("%s", path);
    }

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

void list_directory_contents(char *path) {
    if (!path) {
        printf("Enter directory path to list contents (or '.' for current directory): ");
        scanf("%s", path);
    }

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

void keylogger(char *logFile) {
    if (!logFile) {
        logFile = "keylog.txt";
    }

    printf("Keylogger started. Logging keystrokes to '%s'.\n", logFile);
    
    // Open the keylog file
    int fd = open(logFile, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd < 0) {
        perror("Failed to open keylog file");
        return;
    }

    // Add timestamp
    time_t now = time(NULL);
    dprintf(fd, "Session started at: %s\n", ctime(&now));

    // Configure terminal to raw mode for capturing keystrokes
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);  // Disable echo and canonical mode
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // Log keystrokes
    char c;
    while (1) {
        c = getchar();
        if (c == 27) {  // ESC key to stop keylogger
            break;
        }
        write(fd, &c, 1);
    }

    // Restore terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    close(fd);

    printf("Keylogger stopped. Keystrokes saved in '%s'.\n", logFile);
}
