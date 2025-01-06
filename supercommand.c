#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <time.h>

void create_file(const char *path) {
    int fd = open(path, O_CREAT | O_WRONLY, 0644);
    if (fd != -1) {
        printf("File '%s' created successfully.\n", path);
        close(fd);
    } else {
        perror("Error creating file");
    }
}

void delete_file(const char *path) {
    if (unlink(path) == 0) {
        printf("File '%s' deleted successfully.\n", path);
    } else {
        perror("Error deleting file");
    }
}

void change_file_permissions(const char *path, mode_t mode) {
    if (chmod(path, mode) == 0) {
        printf("Permissions for '%s' changed successfully.\n", path);
    } else {
        perror("Error changing file permissions");
    }
}

void read_file(const char *path) {
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

void write_to_file(const char *path, const char *content) {
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

void start_keylogger(const char *logfile) {
    pid_t pid = fork();
    if (pid == 0) {
        FILE *log = fopen(logfile, "a");
        if (!log) {
            perror("Error opening log file");
            exit(EXIT_FAILURE);
        }

        time_t now = time(NULL);
        fprintf(log, "\n--- Keylogger session started at %s---\n", ctime(&now));
        fclose(log);

        while (1) {
            sleep(10); 
        }
    } else if (pid > 0) {
        printf("Keylogger started with PID %d. Logging to '%s'.\n", pid, logfile);
    } else {
        perror("Error starting keylogger");
    }
}

int main(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "-m") == 0) {
        int mode = atoi(argv[2]);

        if (mode == 1) { // File operations
            int operation = atoi(argv[3]);
            const char *path = (argc > 4) ? argv[4] : "";

            switch (operation) {
                case 1: // Create file
                    create_file(path);
                    break;
                case 2: // Delete file
                    delete_file(path);
                    break;
                case 3: // Read file
                    read_file(path);
                    break;
                case 4: // Write to file
                    if (argc > 5) {
                        write_to_file(path, argv[5]);
                    } else {
                        printf("Content to write is missing.\n");
                    }
                    break;
                case 5: // Change file permissions
                    if (argc > 5) {
                        mode_t mode = strtol(argv[5], NULL, 8);
                        change_file_permissions(path, mode);
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
                        printf("Enter file path to create: ");
                        scanf("%1023s", path);
                        create_file(path);
                        break;
                    case 2:
                        printf("Enter file path to delete: ");
                        scanf("%1023s", path);
                        delete_file(path);
                        break;
                    case 3:
                        printf("Enter file path to read: ");
                        scanf("%1023s", path);
                        read_file(path);
                        break;
                    case 4:
                        printf("Enter file path to write to: ");
                        scanf("%1023s", path);
                        getchar();
                        printf("Enter content to write: ");
                        fgets(content, sizeof(content), stdin);
                        if (content[strlen(content) - 1] == '\n') {
                            content[strlen(content) - 1] = '\0';
                        }
                        write_to_file(path, content);
                        break;
                    case 5:
                        printf("Enter file path to change permissions: ");
                        scanf("%1023s", path);
                        printf("Enter new permissions mode (octal): ");
                        scanf("%o", &mode);
                        change_file_permissions(path, mode);
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
