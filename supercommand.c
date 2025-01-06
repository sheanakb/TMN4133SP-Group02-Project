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

//for File Operations
void getDirectoryName(char *fullPath, size_t size) {
    char dirName[256], filename[256];

    // Prompt for directory name and filename
    printf("Enter directory name: ");
    fgets(dirName, sizeof(dirName), stdin);
    dirName[strcspn(dirName, "\n")] = 0;  // Remove newline character

    printf("Enter filename: ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0;  // Remove newline character

    // Combine directory name and filename to create full path
    snprintf(fullPath, size, "%s/%s", dirName, filename);
}

void createOpenFile() {
    char fullPath[512];

    // Check if file exists
    int fd = open(fullPath, O_RDWR | O_CREAT, 0644);  // Open file with read/write and create if doesn't exist
    if (fd != -1) {
        // Check if file is newly created or just opened
        if (access(fullPath, F_OK) == 0) {
            printf("File '%s' opened successfully.\n", fullPath);  // File exists, just opened
        } else {
            printf("File '%s' created successfully.\n", fullPath);  // New file created
        }
        close(fd);  // Close the file descriptor
    } else {
        perror("Error creating or opening file");
    }
}

// Delete file function
void deleteFile() {
    char fullPath[512];
    if (unlink(fullPath) == 0) {
        printf("File '%s' deleted successfully.\n", fullPath);
    } else {
        perror("Error deleting file");
    }
}

// Change file permissions function
void changeFilePerm() {
    char fullPath[512];
    mode_t mode;
    if (chmod(fullPath, mode) == 0) {
        printf("Permissions for '%s' changed successfully.\n", fullPath);
    } else {
        perror("Error changing file permissions");
    }
}

// Read file function
void readFile() {
    char fullPath[512];
    char buffer[1024];
    int fd = open(fullPath, O_RDONLY);
    if (fd != -1) {
        ssize_t bytes_read;
        printf("Contents of file '%s':\n", fullPath);
        while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
            buffer[bytes_read] = '\0';
            printf("%s", buffer);
        }
        close(fd);
    } else {
        perror("Error reading file");
    }
}

// Write to file function
void writeFile() {
    char fullPath[512];
    char content[1024];
    int fd = open(fullPath, O_WRONLY | O_APPEND);
    if (fd != -1) {
        if (write(fd, content, strlen(content)) != -1) {
            printf("Content written to '%s' successfully.\n", fullPath);
        } else {
            perror("Error writing to file");
        }
        close(fd);
    } else {
        perror("Error opening file for writing");
    }
}

//for Directory Operations
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
            const char *logfile = (argc > 4) ? argv[4] : "keylog.txt";

            if (operation == 1) {
                start_keylogger(logfile);
            } else {
                printf("Invalid operation for keylogger mode.\n");
            }
        } else {
            printf("Invalid mode.\n");
        }

        return 0;
    }

    int choice;
    char path[1024], filename[256];
    char content[1024];
    char fullPath[512];
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
            case 1:
                printf("\nFile Operations:\n");
                printf("1. Create a file\n");
                printf("2. Delete a file\n");
                printf("3. Read a file\n");
                printf("4. Write to a file\n");
                printf("5. Change file permissions\n");
                printf("Enter your choice: ");
                int file_choice;
                scanf("%d", &file_choice);
                getchar();
                switch (file_choice) {
                    case 1:
                        // Prompt for directory path and filename
                        printf("Enter directory path: ");
                        fgets(path, sizeof(path), stdin);
                        path[strcspn(path, "\n")] = 0;  // Remove the newline character

                        printf("Enter filename: ");
                        fgets(filename, sizeof(filename), stdin);
                        filename[strcspn(filename, "\n")] = 0;  // Remove the newline character

                        // Combine path and filename to create full path
                        snprintf(fullPath, sizeof(fullPath), "%s/%s", path, filename);
                        createOpenFile(path);
                        break;
                    case 2:
                        getDirectoryName(fullPath, sizeof(fullPath));  // Get the full path for the file
                        deleteFile(path);
                        break;
                    case 3:
                        getDirectoryName(fullPath, sizeof(fullPath));  // Get the full path for the file
                        readFile(path);
                        break;
                    case 4:
                        getDirectoryName(fullPath, sizeof(fullPath));  // Get the full path for the file
                        printf("Enter content to write to the file: ");
                        fgets(content, sizeof(content), stdin);
                        content[strcspn(content, "\n")] = 0;  // Remove newline character
                        writeFile(path, content);
                        break;
                    case 5:
                        getDirectoryName(fullPath, sizeof(fullPath));  // Get the full path for the file
                        printf("Enter permissions (e.g., 0644): ");
                        scanf("%o", &mode);
                        getchar();  // To consume the newline character after entering the permissions
                        changeFilePerm(path, mode);
                        break;
                    default:
                        printf("Invalid choice.\n");
                }
                break;
            case 2:
                printf("\nDirectory Operations:\n");
                printf("1. Create a directory\n");
                printf("2. Delete a directory\n");
                printf("3. Print current working directory\n");
                printf("4. List directory contents\n");
                printf("Enter your choice: ");
                int dir_choice;
                scanf("%d", &dir_choice);
                getchar();
                switch (dir_choice) {
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
                        printf("Enter directory path to list contents (or leave empty for current directory): ");
                        getchar();
                        fgets(path, sizeof(path), stdin);
                        if (path[strlen(path) - 1] == '\n') {
                            path[strlen(path) - 1] = '\0';
                        }
                        if (strlen(path) == 0) {
                            strcpy(path, ".");
                        }
                        list_directory_contents(path);
                        break;
                    default:
                        printf("Invalid choice.\n");
                }
                break;
            case 3:
                printf("\nKeylogger Operations:\n");
                printf("1. Start keylogger\n");
                printf("Enter your choice: ");
                int keylogger_choice;
                scanf("%d", &keylogger_choice);
                if (keylogger_choice == 1) {
                    printf("Enter log file path (or leave empty for 'keylog.txt'): ");
                    getchar();
                    fgets(path, sizeof(path), stdin);
                    if (path[strlen(path) - 1] == '\n') {
                        path[strlen(path) - 1] = '\0';
                    }
                    if (strlen(path) == 0) {
                        strcpy(path, "keylog.txt");
                    }
                    start_keylogger(path);
                } else {
                    printf("Invalid choice.\n");
                }
                break;
            case 4:
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while (choice != 4);

    return 0;
}
