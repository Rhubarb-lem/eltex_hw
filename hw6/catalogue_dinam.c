#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct abonent
{
    char *name;
    char *second_name;
    char *tel;
};

void print_menu()
{
    printf("Select an option\n\n");
    printf("1) Add a subscriber\n");
    printf("2) Delete a subscriber\n");
    printf("3) Search for subscribers by name\n");
    printf("4) Display all entries\n");
    printf("5) Exit\n");
}

void add_sub(int *counter, struct abonent **cat, int *size)
{
    char ename[10];
    char esecond_name[10];
    char etel[10];

    (*cat) = realloc((*cat), ((*size) + 1) * sizeof(struct abonent));

    if ((*cat) != NULL)
    {
        printf("Enter name(less than 10 characters):\n");
        scanf("%s", ename);
        printf("Enter second name(less than 10 characters):\n");
        scanf("%s", esecond_name);
        printf("Enter telephone number(less than 10 characters):\n");
        scanf("%s", etel);

        (*cat)[*size].name = strdup(ename);
        (*cat)[*size].second_name = strdup(esecond_name);
        (*cat)[*size].tel = strdup(etel);

        (*size)++;
        (*counter)++;
    }
    else
    {
        printf("Memory allocation failed.\n");
    }
}

void delete_sub(int *counter, struct abonent *cat, int *freepl, int *size)
{
    char ename[10];
    int flag;
    printf("Enter name(less than 10 characters):\n");
    scanf("%s", ename);
    for (int i = 0; i < (*size); i++)
    {
        flag = 0;
        for (int j = 0; j < 10; j++)
        {
            if (cat[i].name[j] != ename[j])
            {
                flag = 1;
                break;
            }
        }
        if (flag == 0)
        {
            freepl[i] = 0;
            (*counter)--;
            if (i != (*size) - 1)
            {

                cat[i] = cat[(*size) - 1];
            }
            (*size)--;
        }
    }
}

void search_sub(struct abonent *cat, int size)
{
    char ename[10];
    int flag;
    printf("Enter name(less than 10 characters):\n");
    scanf("%s", ename);
    for (int i = 0; i < size; i++)
    {
        flag = 0;
        for (int j = 0; j < 10; j++)
        {
            if (cat[i].name[j] != ename[j])
            {
                flag = 1;
                break;
            }
        }
        if (flag == 0)
        {
            printf("Index [%d]\n", i);
            printf("Name: %s\n", cat[i].name);
            printf("Second name: %s\n", cat[i].second_name);
            printf("Telephone: %s\n", cat[i].tel);
        }
    }
}

int main()
{
    int opt, counter = 0, size = 0;
    struct abonent *cat = NULL;
    int *freepl = calloc(100, sizeof(int));

    while (1)
    {
        print_menu();
        scanf("%d", &opt);

        switch (opt)
        {
        case 1:
            add_sub(&counter, &cat, &size);
            break;
        case 2:
            delete_sub(&counter, cat, freepl, &size);
            break;
        case 3:
            search_sub(cat, size);
            break;
        case 4:
            
            for (int i = 0; i < size; i++)
            {
                printf("Index [%d]\n", i);
                printf("Name: %s\n", cat[i].name);
                printf("Second name: %s\n", cat[i].second_name);
                printf("Telephone: %s\n", cat[i].tel);
            }
            break;
        case 5:
            printf("End of program\n");
            
            for (int i = 0; i < size; i++)
            {
                free(cat[i].name);
                free(cat[i].second_name);
                free(cat[i].tel);
            }
            free(cat);
            free(freepl);
            return 0;
        default:
            printf("Unknown option!\n");
            break;
        }
    }

    return 0;
}