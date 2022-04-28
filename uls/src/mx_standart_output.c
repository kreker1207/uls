#include "../inc/uls.h"

static void set_info(const char *dir, t_list **files_s) 
{
    DIR *fd = NULL;
    struct dirent *dp = NULL;
    t_stat * statistics = NULL;

    t_stat *temp = (t_stat *)malloc(sizeof(t_stat));
    lstat(dir, temp);

    if(!S_ISDIR(temp->st_mode))
    {
        char *permission = NULL;
        permission = get_permission(temp, (char*)dir);
        mx_push_back(files_s, (char*)dir);

        free(permission);
        permission = NULL;
    }
    else
    {
        if ((fd = opendir(dir)) == NULL) 
        {
            mx_printerr("uls: ");
            mx_printerr(dir);
            mx_printerr(": Permission denied\n");
            return;
        }

        while ((dp = readdir(fd)) != NULL) 
        {
            if (!mx_strcmp(dp->d_name, ".") || !mx_strcmp(dp->d_name, "..") 
                || dp->d_name[0] == '.' || !mx_strcmp(dp->d_name, "uls"))
            {
                continue;   
            }
            mx_push_back(files_s, dp->d_name);
        
            free(statistics);
            statistics = NULL;
        }
    }

    free(temp); temp = NULL;
    if(fd!=NULL) closedir(fd);
}

static char** convert_list_to_arr(t_list *files_s)
{
    int size = mx_list_size(files_s);
    char**str = (char**)malloc(sizeof(char*)*(size + 1));
    for (int i = 0; i < size; ++i)
    {
        str[i] = mx_strdup(files_s->data);
        files_s = files_s->next;
    }
    str[size] = NULL;
    return str;
}

static void format_output_def(t_list *files_s, int term_col) 
{
    if(isatty(1)) //////////// without pipe 
    {            
        t_list *temp = files_s;
        int count_files = mx_list_size(files_s);
        int size_str = 0;
        int max = 0;

        while(temp!=NULL) {
            if (max < mx_strlen(temp->data))
                    max = mx_strlen(temp->data);
            temp = temp->next;
        }

        int count_tabs = max/8 + 1;

        size_str += count_files * count_tabs * 8;
    
        struct winsize size;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    
        int count_col = count_files;
        int count_rows = 1;
        int temp_size = size_str;
        if (temp_size > size.ws_col) 
        {
            while (temp_size > size.ws_col) 
            {
                count_rows++;
                temp_size = 0;
                count_col = count_files / count_rows;
                if (count_files % count_rows != 0) 
                {
                    count_col++;
                }
                for (int i = 0; i < count_col; i++) 
                {
                    temp_size += count_tabs * 8;
                }
            }
        } 
        char **files = convert_list_to_arr(files_s);
        int cnt = 0;
        for (int i = 0; i < count_rows; ++i)
        {
            cnt = i;
            for (int j = 0; j < count_col; ++j)
            {
                if(files[cnt]!=NULL)mx_printstr(files[cnt]);
                else continue;
                for (int k = 0; k < count_tabs*8 - mx_strlen(files[cnt]); ++k)
                {
                    mx_printchar(' ');
                }
                cnt+=count_rows;
            }
            mx_printstr("\n");
        }

    }
    else
    {
        //mx_sort_list(files_s, &mx_strcmp);
        int count = 0;
        int count_tab = 0;
        while (files_s!= NULL) 
        {
            count += mx_strlen(files_s->data);
            count_tab += mx_strlen("        ");
            if(count + count_tab > term_col - 16) 
            {
                mx_printstr(files_s->data);
                if(files_s->next!=NULL)mx_printstr("\n");
                count = 0;
                count_tab = 0;
                files_s = files_s->next;
            }
            else
            {
                mx_printstr(files_s->data); 
                mx_printstr("\t\t");
                files_s = files_s->next; 
            }
        }  
        mx_printchar('\n');
    } 
}

void mx_standart_output(char **argv, int fils, int dirs, int* exit_value)
{
    t_list* files_s = NULL;
    t_winsize ws;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
    int term_col = ws.ws_col;

    if(fils == 0 && dirs == 0)
    {
        set_info(".", &files_s);
        mx_sort_list(files_s, &mx_strcmp);
        format_output_def(files_s, term_col);
    }
    else if ((fils == 1 && dirs == 0)||(fils == 0 && dirs == 1))
    {
        set_info(argv[0], &files_s);  
        if(files_s == NULL)
        {
            *exit_value = 1;
            return;
        }
        mx_sort_list(files_s, &mx_strcmp);
        format_output_def(files_s, term_col);
    }
    else 
    {   
        for(int i = 0; i < fils; i++)
        {
            set_info(argv[i], &files_s);     
        }
        mx_sort_list(files_s, &mx_strcmp);
        format_output_def(files_s, term_col); 
        mx_clear_list(&files_s);

        //вывод всех папок
        if(dirs!=0 && fils!=0) mx_printchar('\n');
        for(int i = fils; i < fils+dirs; i++)
        {
            mx_printstr(argv[i]);
            mx_printchar(':');
            mx_printchar('\n');
            set_info(argv[i], &files_s);

            //если доступ к папки закрытый
            if(files_s == NULL)
            {
                *exit_value = 1;
                if(i+1 != fils+dirs) mx_printchar('\n');
                continue;
            }
            mx_sort_list(files_s, &mx_strcmp);
            format_output_def(files_s, term_col);
            mx_clear_list(&files_s);
            if(i+1 == fils+dirs) break;
            mx_printchar('\n');
        }
    }

    if(files_s!=NULL){free(files_s); files_s = NULL;}
}

