#include "../inc/uls.h"

static char get_owner_perms(char old_perm, char new_perm) {

    if (old_perm == '-') {
        return mx_toupper(new_perm);
    } else {
        return new_perm;
    }
}

char* get_permission(t_stat* stat, char* path)
{
	char* buff = mx_strnew(12);
	if (S_ISLNK(stat->st_mode))
    {
    	buff[0] = 'l';
    }
	else if(S_ISDIR(stat->st_mode))
    {
        buff[0] = 'd';
    }
    else if (S_ISBLK(stat->st_mode))
    {
    	buff[0] = 'b';
    }
    else if (S_ISCHR(stat->st_mode))
    {
    	buff[0] = 'c';
    }
    else if (S_ISFIFO(stat->st_mode))
    {
    	buff[0] = 'p';
    }
    else if (S_ISSOCK(stat->st_mode))
    {
    	buff[0] = 's';
    }
    else
    {
        buff[0] = '-';
    }

	const char chars[] = "rwxrwxrwx";
	for (size_t i = 1; i < 10; i++) 
	{
		buff[i] = (stat->st_mode & (1 << (9-i))) ? chars[i-1] : '-';
	}
	buff[10] = ' ';
    if (S_ISUID & stat->st_mode) 
    buff[3] = get_owner_perms(buff[3], 's');
    
    if (S_ISGID & stat->st_mode) 
    buff[6] = get_owner_perms(buff[6], 's');
    
    if (S_ISVTX & stat->st_mode) 
    buff[9] = get_owner_perms(buff[9], 't');
    
    ssize_t buf_len = listxattr(path, NULL, 0, XATTR_NOFOLLOW);
    acl_t acl_info = acl_get_file(path, ACL_TYPE_EXTENDED);
    //flags->e ? file->acl_str = acl_to_text(acl_info, NULL) : NULL;
    if (buf_len > 0) 
    {
        buff[10] = '@';
    } 
    else if (acl_info != NULL) 
    {
        acl_free(acl_info);
        buff[10] = '+';
    }
    return buff;
}

static blkcnt_t format_standart(t_file *files_s, int *lens)
{
    blkcnt_t total = 0;
    while(files_s)
    {
        if(mx_strlen(files_s->permission) > lens[0]) 
            lens[0] = mx_strlen(files_s->permission);
        if(mx_strlen(files_s->links) > lens[1]) 
            lens[1] = mx_strlen(files_s->links);
        if(mx_strlen(files_s->owner) > lens[2]) 
            lens[2] = mx_strlen(files_s->owner);
        if(mx_strlen(files_s->group) > lens[3]) 
            lens[3] = mx_strlen(files_s->group);
        if(mx_strlen(files_s->bytes) > lens[4]) 
            lens[4] = mx_strlen(files_s->bytes);
        if(mx_strlen(files_s->time) > lens[5]) 
            lens[5] = mx_strlen(files_s->time);
        total += files_s->stat.st_blocks;
        files_s = files_s->next;
    }
    return total;
}

static bool work_with_special(t_file *files_s)
{
    while(files_s)
    {
        if(mx_count_substr(files_s->bytes, "0x") == 1)
        {
            return true;
        }
        files_s = files_s->next;
    }
    return false;
}

void format_output(t_file *files_s, int show)
{
    int *lens = (int*)malloc(sizeof(int)*6);
    int wwsp = work_with_special(files_s);

    for (int i = 0; i < 6; ++i)
    {
        lens[i] = 0;
    }
    blkcnt_t total = format_standart(files_s, lens);

    if(show)
    {
        mx_printstr("total ");
        mx_printint(total);
        mx_printstr("\n");
    }

    while(files_s)
    {
        mx_printstr(files_s->permission);
        int sp_p = lens[1] - mx_strlen(files_s->links) + 1;
        for (int i = 0; i < sp_p; ++i)
        {
            mx_printchar(' ');
        }
        mx_printstr(files_s->links);
        mx_printchar(' ');
        mx_printstr(files_s->owner);
        int sp_ow = lens[2] - mx_strlen(files_s->owner) + 2;
        for (int i = 0; i < sp_ow; ++i)
        {
            mx_printchar(' ');
        }
        mx_printstr(files_s->group);
        if(!wwsp)
        {
            int sp_gr = lens[3] - mx_strlen(files_s->group) + 2;
            for (int i = 0; i < sp_gr; ++i)
            {
                mx_printchar(' ');
            }
            int sp_b = lens[4] - mx_strlen(files_s->bytes);
            for (int i = 0; i < sp_b; ++i)
            {
                mx_printchar(' ');
            }
        }
        else
        {
            int sp_gr = lens[3] - mx_strlen(files_s->group) + 3;
            for (int i = 0; i < sp_gr; ++i)
            {
                mx_printchar(' ');
            }
            int sp_b = 7 - mx_strlen(files_s->bytes);
            if(sp_b > 0)
            {
                for (int i = 0; i < sp_b; ++i)
                {
                    mx_printchar(' ');
                }
            }
        }
        mx_printstr(files_s->bytes);
        mx_printchar(' ');
        mx_printstr(files_s->time);
        mx_printchar(' ');
        mx_printstr(files_s->name);
        mx_printchar('\n');
        files_s = files_s->next;
    }
}
