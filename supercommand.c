#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <time.h>
#include <termios.h>

void createOpenFile(const char *path) {
    int fd = open(path, O_CREAT | O_WRONLY, 0644);
    if (fd != -1) {
        printf("File '%s' created successfully.\n", path);
        close(fd);
    } else {
        perror("Error creating file");
    }
}

void deleteFile(const char *path) {
    if (unlink(path) == 0) {
        printf("File '%s' deleted successfully.\n", path);
    } else {
        perror("Error deleting file");
    }
}

void changeFilePerm(const char *path, mode_t mode) {
    if (chmod(path, mode) == 0) {
        printf("Permissions for '%s' changed successfully.\n", path);
    } else {
        perror("Error changing file permissions");
    }
}

void readFile(const char *path) {
    char buffer[1024];
    int fd = open(path, O_RDONLY);
    if (fd != -1) {
        ssize_t bytes_read;
        printf("Contents of file '%s':\n", path);
        while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
            buffer[bytes_read] = '\0';
            printf("%s", buffer);
        }
        close(fd);
    } else {
        perror("Error reading file");
    }
}

void writeFile(const char *path, const char *content) {
    int fd = open(path, O_WRONLY | O_APPEND);
    if (fd != -1) {
        if (write(fd, content, strlen(content)) != -1) {
            printf("Content written to '%s' successfully.\n", path);
        } else {
            perror("Error writing to file");
        }
        close(fd);
    } else {
        perror("Error opening file for writing");
    }
}

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

int main(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "-m") == 0) {
        int mode = atoi(argv[2]);

        if (mode == 1) { // File operations
            int operation = atoi(argv[3]);
            const char *path = (argc > 4) ? argv[4] : "";

            switch (operation) {
                case 1: // Create file
                    createOpenFile(path);
                    break;
                case 2: // Delete file
                    deleteFile(path);
                    break;
                case 3: // Read file
                    readFile(path);
                    break;
                case 4: // Write to file
                    if (argc > 5) {
                        writeFile(path, argv[5]);
                    } else {
                        printf("Content to write is missing.\n");
                    }
                    break;
                case 5: // Change file permissions
                    if (argc > 5) {
                        mode_t mode = strtol(argv[5], NULL, 8);
                        changeFilePerm(path, mode);
                    } else {
                        printf("Permissions mode is missing.\n");
                    }
                    break;
                default:
                    printf("Invalid operation for file mode.\n");
            }
        } else if (mode == 2) { // Directory operations
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
        } else if (mode == 3) { // Keylogger operations
            int operation = atoi(argv[3]);
            char *logfile = (argc > 4) ? argv[4] : "keylog.txt";

            if (operation == 1) {
                keylogger(logfile);
            } else {
                printf("Invalid operation for keylogger mode.\n");
            }
        } else {
            printf("Invalid mode.\n");
        }

        return 0;
    }

    int choice;
    char path[1024];
    char content[1024];
    mode_t mode;

    do {
        printf("\nOperations:\n");
        printf("1. File operations\n");
        printf("2. Directory operations\n");
        printf("3. Keylogger operations\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1: // File Operations
            printf("\nFile Operations:\n");
            printf("1. Create a file\n");
            printf("2. Delete a file\n");
            printf("3. Read a file\n");
            printf("4. Write to a file\n");
            printf("5. Change file permissions\n");
            printf("Enter your choice: ");
            int file_choice;
            scanf("%d", &file_choice);
            getchar(); // Consume newline
            switch (file_choice) {
                case 1: // Create a file
                    printf("Enter the file path to create: ");
                    scanf("%s", path);
                    createOpenFile(path);
                    break;
                case 2: // Delete a file
                    printf("Enter the file path to delete: ");
                    scanf("%s", path);
                    deleteFile(path);
                    break;
                case 3: // Read a file
                    printf("Enter the file path to read: ");
                    scanf("%s", path);
                    readFile(path);
                    break;
                case 4: // Write to a file
                    printf("Enter the file path to write to: ");
                    scanf("%s", path);
                    getchar(); // Consume newline
                    printf("Enter the content to write: ");
                    fgets(content, sizeof(content), stdin);
                    strtok(content, "\n"); // Remove trailing newline
                    writeFile(path, content);
                    break;
                case 5: // Change file permissions
                    printf("Enter the file path: ");
                    scanf("%s", path);
                    printf("Enter the new permissions (octal, e.g., 0644): ");
                    scanf("%o", &mode);
                    changeFilePerm(path, mode);
                    break;
                default:
                    printf("Invalid file operation choice.\n");
            }
            break;

        case 2: // Directory Operations
            printf("\nDirectory Operations:\n");
            printf("1. Create a directory\n");
            printf("2. Delete a directory\n");
            printf("3. Print current directory\n");
            printf("4. List directory contents\n");
            printf("Enter your choice: ");
            int dir_choice;
            scanf("%d", &dir_choice);
            getchar(); // Consume newline
            switch (dir_choice) {
                case 1: // Create a directory
                    printf("Enter the directory path to create: ");
                    scanf("%s", path);
                    create_directory(path);
                    break;
                case 2: // Delete a directory
                    printf("Enter the directory path to delete: ");
                    scanf("%s", path);
                    delete_directory(path);
                    break;
                case 3: // Print current directory
                    print_current_directory();
                    break;
                case 4: // List directory contents
                    printf("Enter the directory path: ");
                    scanf("%s", path);
                    list_directory_contents(path);
                    break;
                default:
                    printf("Invalid directory operation choice.\n");
            }
            break;

        case 3: // Keylogger Operations
            printf("\nKeylogger Operations:\n");
            printf("1. Start keylogger\n");
            printf("Enter your choice: ");
            int keylogger_choice;
            scanf("%d", &keylogger_choice);
            getchar(); // Consume newline
            if (keylogger_choice == 1) {
                printf("Enter the log file name (or press Enter for default 'keylog.txt'): ");
                fgets(path, sizeof(path), stdin);
                strtok(path, "\n"); // Remove trailing newline
                if (strlen(path) == 0) {
                    keylogger(NULL); // Use default log file
                } else {
                    keylogger(path);
                }
            } else {
                printf("Invalid keylogger operation choice.\n");
            }
            break;

        case 4: // Exit
            printf("Exiting the program. Goodbye!\n");
            exit(0);
            break;

        default:
            printf("Invalid choice. Please try again.\n");
    }
    }
}
