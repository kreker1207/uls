#include "../inc/uls.h"

t_file *mx_create_node_f(t_stat *statistics, char*permiss, char*name, char*path)
{
	t_file *files = (t_file*)malloc(sizeof(t_file));

  	files->stat.st_mode = statistics->st_mode;
  	files->stat.st_ino = statistics->st_ino;
  	files->stat.st_dev = statistics->st_dev;
  	files->stat.st_rdev = statistics->st_rdev;
  	files->stat.st_nlink = statistics->st_nlink;
  	files->stat.st_uid = statistics->st_uid;
    files->stat.st_gid = statistics->st_gid;
    files->stat.st_size = statistics->st_size;
    files->stat.st_atime = statistics->st_atime;
    files->stat.st_mtime = statistics->st_mtime;
    files->stat.st_ctime = statistics->st_ctime;
    files->stat.st_blksize = statistics->st_blksize;
    files->stat.st_blocks = statistics->st_blocks;

  	files->permission = mx_strdup(permiss);
  	files->links = mx_strdup(mx_itoa((nlink_t)(statistics->st_nlink)));
  	files->owner = mx_strdup(getpwuid(statistics->st_uid)->pw_name);
    if(getgrgid(statistics->st_gid) == NULL)
    {
        files->group = mx_strdup(mx_itoa(statistics->st_gid));
    }
    else
  	    files->group = mx_strdup(getgrgid(statistics->st_gid)->gr_name);

  	if(S_ISCHR(statistics->st_mode) || S_ISBLK(statistics->st_mode))
    {
        char* majr = mx_itoa(major(statistics->st_rdev));
        char* minr = mx_itoa(minor(statistics->st_rdev));
        char* oxoo = mx_strnew(4); oxoo[0] = ','; oxoo[1] = ' ';
        oxoo[2] = '0'; oxoo[3] = 'x';
        if(minor(statistics->st_rdev) > 255) 
        {
            for (int i = 0; i < 8 - mx_strlen(mx_nbr_to_hex(minor(statistics->st_rdev))); ++i)
            {
               oxoo = mx_strjoin(oxoo, "0");
            }
            files->bytes = mx_strdup(mx_strjoin(mx_strjoin(majr, oxoo), mx_nbr_to_hex(minor(statistics->st_rdev))));
        }
        else if(mx_strlen(minr) == 3 && minor(statistics->st_rdev) <= 255) files->bytes = mx_strdup(mx_strjoin(mx_strjoin(majr, ", "), mx_itoa(minor(statistics->st_rdev))));
        else if(mx_strlen(minr) == 2) files->bytes = mx_strdup(mx_strjoin(mx_strjoin(majr, ",  "), mx_itoa(minor(statistics->st_rdev))));
        else files->bytes = mx_strdup(mx_strjoin(mx_strjoin(majr, ",   "), mx_itoa(minor(statistics->st_rdev))));
        free(oxoo);
        oxoo = NULL;
    }
    else files->bytes = mx_strdup(mx_itoa((off_t)(statistics->st_size)));

    char* time_temp = ctime(&(statistics->st_mtime));
    char* t_dup = mx_strndup(&time_temp[19], 5);
    time_t lt = time(NULL);
    char* time_now = ctime(&lt);
    char* n_dup = mx_strndup(&time_now[19], 5);

    if(mx_strcmp(t_dup, n_dup)!=0)
    {
        files->time = mx_strdup(mx_strjoin(mx_strndup(&ctime(&(statistics->st_mtime))[4], 7), t_dup));
    }
  	else 
        files->time = mx_strndup(&ctime(&(statistics->st_mtime))[4], 12);

    //files->name = mx_strdup(name);
  	if(!S_ISLNK(statistics->st_mode))
    {
        files->name = mx_strdup(name);
    }
    else
    {
        int bufsiz = statistics->st_size;
        if(statistics->st_size == 0) 
            bufsiz = PATH_MAX;
        char *buf = mx_strnew(bufsiz);

        if(path!=NULL)readlink(path, buf, bufsiz);
        else readlink(name, buf, bufsiz);

        files->name = mx_strdup(mx_strjoin(mx_strjoin(name, " -> "), buf));
        free(buf);
        buf = NULL;
    }
    files->next = NULL;

    return files;
}

void mx_push_back_f(t_file **files, t_stat *statistics, char*permiss, char*name, char*path)
{
	if (!files) 
	{
        return;
    }
    if (!*files) 
    {
        *files = mx_create_node_f(statistics, permiss, name, path);
        return;
    }

    t_file *temp = *files;
    while (temp -> next) 
    {
        temp = temp -> next;
    }
    temp -> next = mx_create_node_f(statistics, permiss, name, path);
}

int mx_list_size_f(t_file *files)
{
	int count = 0;
    while (files) {
        files = files -> next;
        count++;
    }
    return count;
}

t_file *mx_sort_list_f(t_file *files, int(*cmp)(const char*, const char*))
{
	if (cmp == NULL || files == NULL) return files;
    t_file *temp = files;
    int count = 0;
    while (temp) {
        temp = temp -> next;
        count++;
    }

    for (int i = 0; i < count; i++) {
        temp = files;
        for (int j = 0; j < count - 1; j++) {
            if (cmp(temp -> name, temp -> next -> name) > 0) {
                char *permission = temp -> permission; 
                temp -> permission = temp -> next -> permission; 
                temp -> next -> permission = permission;
                char *links = temp -> links; 
                temp -> links = temp -> next -> links; 
                temp -> next -> links = links;
                char *owner = temp -> owner; 
                temp -> owner = temp -> next -> owner; 
                temp -> next -> owner = owner;  
                char *group = temp -> group; 
                temp -> group = temp -> next -> group; 
                temp -> next -> group = group;  
                char *bytes = temp -> bytes; 
                temp -> bytes = temp -> next -> bytes; 
                temp -> next -> bytes = bytes;  
                char *time = temp -> time; 
                temp -> time = temp -> next -> time; 
                temp -> next -> time = time;  
                char *name = temp -> name; 
                temp -> name = temp -> next -> name; 
                temp -> next -> name = name;      
            }
            temp = temp -> next;
        }
    }
    return files;
}

t_file* mx_duplicate_list_f(t_file* list)
{
    t_file *temp = (t_file*)malloc(sizeof(t_file));

    for (int i = 0; list; ++i)
    {
        temp->permission = mx_strdup(list->permission);
        temp->links = mx_strdup(list->links);
        temp->owner = mx_strdup(list->owner);
        temp->group = mx_strdup(list->group);
        temp->bytes = mx_strdup(list->bytes);
        temp->time = mx_strdup(list->time);
        temp->name = mx_strdup(list->name);
        //temp->next = NULL;
        list = list->next;
    }

    return temp;
}

