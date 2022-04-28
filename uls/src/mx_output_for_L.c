#include "../inc/uls.h"

static void listinfo(const char *dir, t_file **files_s) 
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
        mx_push_back_f(files_s, temp, permission, (char*)dir, NULL);

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

            statistics = (t_stat *)malloc(sizeof(t_stat));
            int st = lstat(mx_strjoin(mx_strjoin(dir, "/"), dp->d_name), statistics);

            if(st == -1)
            {
                mx_printerr(strerror(ENOENT));
                mx_printchar('\n');
                exit(-1);
            }
            char* path = mx_strjoin(mx_strjoin(dir, "/"), dp->d_name);
            char *permission = NULL;
            permission = get_permission(statistics, path);
            mx_push_back_f(files_s, statistics, permission, dp->d_name, path);
        
            free(statistics);
            free(permission);
            statistics = NULL;
            permission = NULL;
        }
    }

    free(temp); temp = NULL;
    if(fd!=NULL) closedir(fd);
}

void mx_output_for_L(char **argv, int fils, int dirs, int*exit_value)
{
    t_file *files_s = NULL;

    //если нет ни папок ни файлов
	if(fils == 0 && dirs == 0)
	{
		listinfo(".", &files_s);  
        mx_sort_list_f(files_s, &mx_strcmp);
        format_output(files_s, 1);
	}

    //если есть либо папка либо файл
    else if((fils == 1 && dirs == 0)||(fils == 0 && dirs == 1))
    {
        listinfo(argv[0], &files_s);  
        if(files_s == NULL)
        {
            *exit_value = 1;
            return;
        }
        mx_sort_list_f(files_s, &mx_strcmp);
        if((fils == 1 && dirs == 0))format_output(files_s, 0);
        else format_output(files_s, 1);
    }

    //если в наборе есть либо папки либо файлы либо все вместе 
	else 
	{   
        t_file* files[fils + dirs];

        //вывод всех файлов
        for(int i = 0; i < fils; i++)
        {
            listinfo(argv[i], &files_s);     
        }
        mx_sort_list_f(files_s, &mx_strcmp);
        format_output(files_s, 0); 
        free(files_s);
        files_s = NULL; 
        
        //вывод всех папок
        if(dirs!=0 && fils!=0) mx_printchar('\n');
		for(int i = fils; i < fils+dirs; i++)
		{
            mx_printstr(argv[i]);
            mx_printchar(':');
            mx_printchar('\n');
            listinfo(argv[i], &files_s);
            //если доступ к папки закрытый
            if(files_s == NULL)
            {
                files[i] = NULL;
                *exit_value = 1;
                if(i+1 != fils+dirs) mx_printchar('\n');
                continue;
            }
            files[i] = files_s;
            free(files_s);
            files_s = NULL;
            mx_sort_list_f(files[i], &mx_strcmp);
            format_output(files[i], 1);
            if(i+1 == fils+dirs) break;
            mx_printchar('\n');
		}
	}
}

