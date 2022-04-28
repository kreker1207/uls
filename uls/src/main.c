#include "../inc/uls.h"

static int set_no_errors(const char** argv, int argc, int spec, char ***errors, char***correct)
{
	t_stat * statistics1 = NULL;
	int counter = 0;
	for(int i = spec; i < argc; i++)
	{
		statistics1 = (t_stat *)malloc(sizeof(t_stat));
		int lst = lstat(argv[i], statistics1);
		if(lst == -1) counter++;
		free(statistics1);
		statistics1 = NULL;
	}

	if (counter == 0) return 0;
	else
	{
		if((argc - spec - counter)!=0)
		{
			*correct = (char**)malloc(sizeof(char*)*(argc - spec - counter));
			for (int i = 0; i < argc - spec - counter; ++i)
			{
				(*correct)[i] = mx_strnew(256);
			}
			int fd = 0;
			for(int i = spec; i < argc; i++)
			{
				statistics1 = (t_stat *)malloc(sizeof(t_stat));
				int lst = lstat(argv[i], statistics1);
				if(lst != -1)
				{
					(*correct)[fd++] = mx_strdup(argv[i]);
				}
				free(statistics1);
				statistics1 = NULL;
			}

		}

		*errors = (char**)malloc(sizeof(char*)*counter);
		for (int i = 0; i < counter; ++i)
		{
			(*errors)[i] = mx_strnew(256);
		}
		int fd = 0;
		for(int i = spec; i < argc; i++)
		{
			statistics1 = (t_stat *)malloc(sizeof(t_stat));
			int lst = lstat(argv[i], statistics1);
			if(lst == -1)
			{
				(*errors)[fd++] = mx_strdup(argv[i]);
			}
			free(statistics1);
			statistics1 = NULL;
		}
	}

	return counter;
}

static char** get_file_array(int argc, const char** argv, int spec, int*fils, int*dirs) 
{
	int size = argc - spec;
    char** files = malloc(sizeof(char*) * (size + 1));
	for(int i = 0; i < size; i++)
	{
		files[i] = mx_strdup(argv[i+spec]);
	}
	files[size] = NULL;
	t_stat * statistics1 = NULL;
	t_stat * statistics2 = NULL;

	for (int i = 0; i < size; ++i) 
	{
		statistics1 = (t_stat *)malloc(sizeof(t_stat));
		lstat(files[i], statistics1);

		if(!S_ISDIR(statistics1->st_mode)) 
		{
			(*fils)++;
		}
		else (*dirs)++;
		free(statistics1); statistics1 = NULL;
    }

	for(int j = 0; j < size; j++)
	{
		for (int i = 0; i < size - 1; ++i) 
		{
			statistics1 = (t_stat *)malloc(sizeof(t_stat));
			lstat(files[i], statistics1);
			statistics2 = (t_stat *)malloc(sizeof(t_stat));
			lstat(files[i+1], statistics2);

			if(S_ISDIR(statistics1->st_mode) && !S_ISDIR(statistics2->st_mode)) 
			{
				char*temp = mx_strdup(files[i]);
				files[i] = mx_strdup(files[i+1]);
				files[i+1] = mx_strdup(temp);
			}
			free(statistics1); statistics1 = NULL;
			free(statistics2); statistics2 = NULL;
    	}
	}

	for(int j = 0; j < (*fils); j++)
	{
		for(int i = 0; i < (*fils)-1; i++)
		{
			if(mx_strcmp(files[i], files[i+1]) > 0)
			{
				char*temp = mx_strdup(files[i]);
				files[i] = mx_strdup(files[i+1]);
				files[i+1] = mx_strdup(temp);
			}
		}
	}

	for(int j = 0; j < (*dirs); j++)
	{
		for(int i = (*fils); i < size - 1; i++)
		{
			if(mx_strcmp(files[i], files[i+1]) > 0)
			{
				char*temp = mx_strdup(files[i]);
				files[i] = mx_strdup(files[i+1]);
				files[i+1] = mx_strdup(temp);
			}
		}
	}
    return files;

}

int main(int argc, char const *argv[])
{
	t_flags flags;
	int exit_value = 0;

	int i = 1;
	while(i!=argc && argv[i][0] == '-')
	{
		for (int j = 1; j < mx_strlen(argv[i]); ++j)
		{
			if(argv[i][j] == 'l') flags.l = 1;
			else 
			{
				mx_printerr("uls: illegal option -- ");
				mx_printerr(&argv[i][j]);
				mx_printerr("\nusage: uls [-l] [file ...]\n");
				return 1;
			}
		}
		i++;
	}

	int fils = 0, dirs = 0;
	char**errors = NULL;
	char**correct = NULL;

	if(flags.l == 1)
	{
		int is_wrong = set_no_errors(argv, argc, 2, &errors, &correct);
		if(is_wrong == 0)
		{
			char **files = get_file_array(argc, argv, 2, &fils, &dirs);
			mx_output_for_L(files, fils, dirs, &exit_value);
		}
		else
		{
			if(is_wrong == argc - 2)
			{
				for(int i = 0; i < is_wrong; i++)
				{
					mx_printerr("uls: ");
					mx_printerr(errors[i]);
					mx_printerr(": No such file or directory\n");
				}
			}
			else
			{
				for(int i = 0; i < is_wrong; i++)
				{
					mx_printerr("uls: ");
					mx_printerr(errors[i]);
					mx_printerr(": No such file or directory\n");
				}
				char **files = get_file_array(argc - 2 - is_wrong, (const char**)correct, 0, &fils, &dirs);
				mx_output_for_L(files, fils, dirs, &exit_value);
			}
			exit_value = 1;
		}
	}

	else
	{
		int is_wrong = set_no_errors(argv, argc, 1, &errors, &correct);
		if(is_wrong == 0)
		{
			char **files = get_file_array(argc, argv, 1, &fils, &dirs);
			mx_standart_output(files, fils, dirs, &exit_value);
		}
		else
		{
			if(is_wrong == argc - 1)
			{
				for(int i = 0; i < is_wrong; i++)
				{
					mx_printerr("uls: ");
					mx_printerr(errors[i]);
					mx_printerr(": No such file or directory\n");
				}
			}
			else
			{
				for(int i = 0; i < is_wrong; i++)
				{
					mx_printerr("uls: ");
					mx_printerr(errors[i]);
					mx_printerr(": No such file or directory\n");
				}
				char **files = get_file_array(argc - 1 - is_wrong, (const char**)correct, 0, &fils, &dirs);
				mx_standart_output(files, fils, dirs, &exit_value);
			}
		}

	}

	return exit_value;
}

