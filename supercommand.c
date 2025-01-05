#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

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
