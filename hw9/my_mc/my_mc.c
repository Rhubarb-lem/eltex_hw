#include <ncurses.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILES 100

struct FileInfo
{
    char *name;
    int is_directory;
};

void scanDirectory(char *path, struct FileInfo *files, int *n_files)
{
    struct dirent *d;
    DIR *dir = opendir(path);

    if (dir == NULL)
    {
        printw("Failed to open the directory.");
        refresh();
        getch();
        endwin();
        exit(1);
    }

    *n_files = 0;
    while ((d = readdir(dir)) != NULL && *n_files < MAX_FILES)
    {
        files[*n_files].name = strdup(d->d_name);
        files[*n_files].is_directory = (d->d_type == DT_DIR);
        (*n_files)++;
    }

    closedir(dir);
}

int main()
{
    initscr();
    keypad(stdscr, TRUE);
    noecho();

    struct FileInfo files[MAX_FILES];
    int n_files = 0;

    scanDirectory(".", files, &n_files);

    int highlight1 = 0;
    int highlight2 = 0;
    int current_list = 1; 
    int choice;

    while (1)
    {
        clear();

       
        for (int i = 0; i < n_files; i++)
        {
            if ((i == highlight1) && (current_list == 1))
            {
                attron(A_REVERSE);
            }
            mvprintw(i, 0, files[i].name);
            attroff(A_REVERSE);
        }
        for (int i = 0; i < n_files; i++)
        {
            if (i == highlight2 && current_list == 2)
            {
                attron(A_REVERSE);
            }
            mvprintw(i, COLS / 2, files[i].name);
            attroff(A_REVERSE);
        }

        
        choice = getch();

        switch (choice)
        {
        case KEY_UP:
            if (current_list == 1)
            {
                highlight1 = (highlight1 - 1 + n_files) % n_files;
            }
            else
            {
                highlight2 = (highlight2 - 1 + n_files) % n_files;
            }

            break;
        case KEY_DOWN:
            if (current_list == 1)
            {
                highlight1 = (highlight1 + 1) % n_files;
            }
            else
            {
                highlight2 = (highlight2 + 1) % n_files;
            }

            break;
        case 9: 
            current_list = (current_list == 1) ? 2 : 1;
            break;
        case 10: 
            if (current_list == 1)
            {
                if (files[highlight1].is_directory)
                {
                    char path[256];
                    snprintf(path, sizeof(path), "./%s", files[highlight1].name);
                    scanDirectory(path, files, &n_files);
                    highlight1 = 0;
                }
            }
            else{
                if (files[highlight2].is_directory)
                {
                    char path[256];
                    snprintf(path, sizeof(path), "./%s", files[highlight2].name);
                    scanDirectory(path, files, &n_files);
                    highlight2 = 0;
                }
            }

            break;
        default:
            break;
        }

        if (choice == 27) 
        {
            break;
        }
    }

    for (int i = 0; i < n_files; i++)
    {
        free(files[i].name);
    }

    endwin();
    return 0;
}
