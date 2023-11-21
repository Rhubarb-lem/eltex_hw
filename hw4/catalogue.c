#include <stdio.h>

struct abonent
{
    char name[10];
    char second_name[10];
    char tel[10];
};

int main()
{

    int opt, counter = 0, index, flag = 0;
    struct abonent cat[100];
    int free[100] = {};
    char ename[10];
    char esecond_name[10];
    char etel[10];
    while (1)
    {
        printf("Select an option\n\n");
        printf("1) Add a subscriber\n");
        printf("2) Delete a subscriber\n");
        printf("3) Search for subscribers by name\n");
        printf("4) Display all entries\n");
        printf("5) Exit\n");
        scanf("%d", &opt);
        switch (opt)
        {
        case 1:
            if (counter < 100)
            {
                printf("Enter name(less than 10 characters):\n");
                scanf("%s", ename);
                printf("Enter second name(less than 10 characters):\n");
                scanf("%s", esecond_name);
                printf("Enter telephone number(less than 10 characters):\n");
                scanf("%s", etel);
                for (int i = 0; i < 100; i++)
                {
                    if (free[i] == 0)
                    {
                        index = i;

                        break;
                    }
                }

                for (int i = 0; i < 10; i++)
                {
                    cat[index].name[i] = ename[i];
                    if (ename[i] == '\0')
                        break;
                }
                for (int i = 0; i < 10; i++)
                {

                    cat[index].second_name[i] = esecond_name[i];
                    if (esecond_name[i] == '\0')
                        break;
                }
                for (int i = 0; i < 10; i++)
                {

                    cat[index].tel[i] = etel[i];
                    if (etel[i] == '\0')
                        break;
                }
                free[index] = 1;
                counter++;
            }
            else
            {
                printf("Catalogue overflow(100)! You can't add a new subscriber!\n");
            }

            break;
        case 2:
            printf("Enter name(less than 10 characters):\n");
            scanf("%s", ename);
            for (int i = 0; i < 100; i++)
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
                    for (int j = 0; j < 10; j++)
                    {
                        cat[i].name[j] = 0;
                        cat[i].second_name[j] = 0;
                        cat[i].tel[j] = 0;
                    }
                    free[i] = 0;
                    counter--;
                }
            }
            break;
        case 3:
            printf("Enter name(less than 10 characters):\n");
            scanf("%s", ename);
            for (int i = 0; i < 100; i++)
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
                    printf("Name: ");
                    for (int j = 0; j < 10; j++)
                    {
                        printf("%c", cat[i].name[j]);
                        
                    }
                    printf("\n");
                    printf("Second name: ");
                    for (int j = 0; j < 10; j++)
                    {   
                        
                        printf("%c", cat[i].second_name[j]);
                        
                    }
                    printf("\n");
                    printf("Telephone: ");
                    for (int j = 0; j < 10; j++)
                    {
                       
                        printf("%c", cat[i].tel[j]);
                        
                    }
                    printf("\n");
                }
            }
            break;
        case 4:
            for (int i = 0; i < 100; i++)
            {

                if (free[i] == 1)
                {
                    printf("Index [%d]\n", i);
                    printf("Name: ");
                    for (int j = 0; j < 10; j++)
                    {   
                        
                        printf("%c", cat[i].name[j]);
                       
                    }
                    printf("\n");
                    printf("Second name: ");
                    for (int j = 0; j < 10; j++)
                    {
                        
                        printf("%c", cat[i].second_name[j]);
                        
                    }
                    printf("\n");
                    printf("Telephone: ");
                    for (int j = 0; j < 10; j++)
                    {
                       
                        printf("%c", cat[i].tel[j]);
                        
                    }
                    printf("\n");
                }
            }
            break;
        case 5:
            printf("End of programm\n");
            return 0;
            break;

        default:
            printf("Unknow option!\n");
            break;
        }
    }

    return 0;
}
