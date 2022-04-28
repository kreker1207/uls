#ifndef ULS
#define ULS
#define major(x)        ((int32_t)(((u_int32_t)(x) >> 24) & 0xff))
#define minor(x)        ((int32_t)((x) & 0xffffff))

#include "../libmx/inc/libmx.h" //write, malloc, malloc_size, free, exit, perror, strerror

/*
	char * strerror(int errnum);
	void perror(const char *s);
	int isatty(int fd);
-------------------------------------------------------------------------------------------------------------------------
	The strerror() and perror() functions look up the error message string corresponding to an error
    number.

    	The strerror() function accepts an error number argument errnum and returns a pointer to the corresponding message
    string.
    	The perror() function finds the error message corresponding to the current value of the global variable errno
    and writes it, followed by a newline, to the standard error file descriptor.

    The isatty() function determines if the file descriptor fd refers to a valid terminal type device.
    Determine whether device terminal, consol, printer or not.

    EXAMPLE: 
    	if(isatty(fd)) printf ("is a character device");
		else printf("is not a character device");
-------------------------------------------------------------------------------------------------------------------------
info: man
*/

#include <dirent.h> //opendir(), readdir(), closedir()
/*
	DIR * opendir(const char *filename);
	struct dirent * readdir(DIR *dirp);
	int closedir(DIR *dirp);

	struct dirent {
		long d_ino;					//это номер, уникальный для каждого файла файловой системы.
		off_t d_off; 				//содержит смещение этой структуры каталога в действительном каталоге файловой системы.
		unsigned short d_reclen; 	//задает длину записи этой структуры.
		unsigned char d_type; 		//тип файла типа d_name
		char d_name[1]; 			//содержит начало символьного массива, дающего имя этой структуры каталога.
	};

	struct __dirstream {
		char *__data; 
		int __entry_data; 
		char *__ptr; 
		int __entry_ptr; 
		size_t __allocation; 
		size_t __size; 
	} typedef struct __dirstream DIR;

	!!!для флага ls -l следует использовать функцию stat в комбинации с структурой stat (стр. )
-------------------------------------------------------------------------------------------------------------------------
	The opendir() function opens the directory named by filename, associates a directory stream with it and returns a
    pointer to be used to identify the directory stream in subsequent operations.  The pointer NULL is returned if
    filename cannot be accessed, or if it cannot malloc(3) enough memory to hold the whole thing.

    The readdir() function returns a pointer to the next directory entry.  The directory entry remains valid until the
    next call to readdir() or closedir() on the same directory stream.  The function returns NULL upon reaching the end
    of the directory or on error.  In the event of an error, errno may be set to any of the values documented for the
    getdirentries(2) system call. Note that the order of the directory entries vended by readdir() is not specified.
    Some filesystems may return entries in lexicographic sort order and others may not.  Also note that not all
    filesystems will provide a value for d_type and may instead set the field to DT_UNKNOWN.

    The closedir() function closes the named directory stream and frees the structure associated with the dirp pointer,
    returning 0 on success.  On failure, -1 is returned and the global variable errno is set to indicate the error.
-------------------------------------------------------------------------------------------------------------------------
info: man, https://russianblogs.com/article/4319360981/
*/

#include <sys/stat.h> //stat(), lstat()
/*
	int stat(const char *restrict path, struct stat *restrict buf);
	int lstat(const char *restrict path, struct stat *restrict buf);

	struct stat {
		mode_t st_mode;			// Права доступа к файлу   
        ino_t st_ino;			// Номер узла индекса   
        dev_t st_dev;			// Номер устройства, используемый файлом   
        dev_t st_rdev;			// Номер устройства файла устройства   
        nlink_t st_nlink;		// Количество жестких ссылок в файле   
        uid_t st_uid;			// Идентификационный номер пользователя владельца   
        gid_t st_gid;			// Идентификационный номер группы   
        off_t st_size;			// Емкость файла в байтах   
        time_t st_atime;		// В последний раз доступ к файлу   
        time_t st_mtime;		// Время последнего изменения файла   
        time_t st_ctime;    	// В последний раз статус файла был изменен   
        blksize_t st_blksize;	// Размер блока диска, содержащего файл   
        blkcnt_t st_blocks;		// Дисковые блоки, занятые файлом   
	};  
-------------------------------------------------------------------------------------------------------------------------
	The stat() function obtains information about the file pointed to by path.  Read, write or execute permission of
    the named file is not required, but all directories listed in the path name leading to the file must be searchable.

    The lstat() function is like stat() except in the case where the named file is a symbolic link; lstat() returns
    information about the link, while stat() returns information about the file the link references.  For symbolic
    links, the st_mode member contains meaningful information when used with the !file type macros!, and the st_size mem-
    ber contains the length of the pathname contained in the symbolic link. File mode bits and the contents of the
    remaining members of the stat structure are unspecified. The value returned in the st_size member is the length of
    the contents of the symbolic link, and does not count any trailing null.
-------------------------------------------------------------------------------------------------------------------------
info: man, 	https://russianblogs.com/article/4319360981/
*/

#include <sys/types.h>//getpwuid()
#include <uuid/uuid.h>//getpwuid(), getgrgid
#include <pwd.h>//getpwuid()
#include <grp.h>//getgrgid()
/*
	struct passwd * getpwuid(uid_t uid);
	struct group * getgrgid(gid_t gid);
	struct passwd {
                   char    *pw_name;       // user name
                   char    *pw_passwd;     // encrypted password
                   uid_t   pw_uid;         // user uid
                   gid_t   pw_gid;         // user gid 
                   time_t  pw_change;      // password change time
                   char    *pw_class;      // user access class
                   char    *pw_gecos;      // Honeywell login info
                   char    *pw_dir;        // home directory
                   char    *pw_shell;      // default shell
                   time_t  pw_expire;      // account expiration
                   int     pw_fields;      // internal: fields filled in
           };
    struct group {
                   char    *gr_name;       // group name
                   char    *gr_passwd;     // group password
                   gid_t   gr_gid;         // group id
                   char    **gr_mem;       // group members
           };
-------------------------------------------------------------------------------------------------------------------------
    The function getpwuid() searches the password database for the given login name, user uid, or user uuid respectively,
    always returning the first one encountered. The function getpwuid() returns a valid pointer to a passwd structure on
    success or NULL if the entry is not found or if an error occurs.  If an error does occur, errno will be set.

    ERRORS
     These routines may fail for any of the errors specified in open(2), dbopen(3), socket(2), and connect(2), in addi-
     tion to the following:

     [ERANGE]           The buffer specified by the buffer and bufsize arguments was insufficiently sized to store the
                        result. The caller should retry with a larger buffer.

    The function getgrgid() searches the group database for the given group name pointed to by gid, 
    returning the first one encountered. Identical group gids may result in undefined behavior. The function getgrgid() 
    returns a pointer to a group structure on success or NULL if the entry is not found or if an error occurs.  
    If an error does occur, errno will be set.
-------------------------------------------------------------------------------------------------------------------------
info: man.
*/

#include <sys/xattr.h> //listxattr(), getxattr()
/*
	ssize_t listxattr(const char *path, char *namebuf, size_t size, int options);
	ssize_t getxattr(const char *path, const char *name, void *value, size_t size, u_int32_t position, int options);
-------------------------------------------------------------------------------------------------------------------------
	Function listxattr() retrieves a list of names of extended attributes associated with the given path in the file 
	system.

	(namebuf) is a data buffer of size bytes for the names of the extended attributes associated with path.  The extended
    attribute names are simple NULL-terminated UTF-8 strings and are returned in arbitrary order.  No extra padding is
    provided between names in the buffer.  The list will only include names of extended attributes to which the calling
    process has access.  The function returns the size of the list of names.

    (options) controls how the attribute list is generated:

    XATTR_NOFOLLOW  do not follow symbolic links.  listxattr() normally lists attributes of the target of path if it is
                    a symbolic link.  With this option, listxattr() will list attributes of the link itself.

    XATTR_SHOWCOMPRESSION
                    listxattr() and flistxattr() will list HFS Plus Compression extended attribute(s) (if present) for
                    the file referred to by path or fd.

    If namebuf is set to NULL, the function returns the size of the list of extended attribute names.  This facility
    can be used to determine the size of a buffer sufficiently large to hold the names of the attributes currently
    associated with path.
    RETURN VALUES
     On success, the size of the extended attribute name list is returned.  If no accessible extended attributes are
     associated with the given path or fd, the function returns zero.  On failure, -1 is returned and the global vari-
     able errno is set as follows.
    ERRORS
     [ENOTSUP]          The file system does not support extended attributes or has the feature disabled.
     [ERANGE]           namebuf (as indicated by size) is too small to hold the list of names.
     [EPERM]            path or fd refer to a file system object that does not support extended attributes.  For exam-
                        ple, resource forks don't support extended attributes.
     [ENOTDIR]          A component of path 's prefix is not a directory.
     [ENAMETOOLONG]     name exceeds XATTR_MAXNAMELEN UTF-8 bytes, or a component of path exceeds NAME_MAX characters,
                        or the entire path exceeds PATH_MAX characters.
     [EACCES]           Search permission is denied for a component of path or permission is denied to read the list of
                        attributes from this file.
     [ELOOP]            Too many symbolic links were encountered resolving path.
     [EFAULT]           path points to an invalid address.
     [EIO]              An I/O error occurred.
     [EINVAL]           options does not make sense.

    The getxattr() function retrieves up to size bytes of data from the extended attribute identified by name 
    associated with path into the pre-allocated buffer pointed to by value. The function returns 
    the number of bytes of data retrieved.

    An extended attribute's name is a simple NULL-terminated UTF-8 string.  position specifies an offset within the
    extended attribute.  In the current implementation, this argument is only used with the resource fork attribute.
    For all other extended attributes, this parameter is reserved and should be zero.

    On success, value contains the data associated with name.  When value is set to NULL, getxattr() returns current
    size of the named attribute.  This facility can be used to determine the size of a buffer sufficiently large to
    hold the data currently associated with the attribute.

	RETURN VALUES
     On success, the size of the extended attribute data is returned.  On failure, -1 is returned and the global vari-
     able errno is set as follows.
-------------------------------------------------------------------------------------------------------------------------
info: man.
*/
#include <sys/ioctl.h>//ioctl()
/*
	int ioctl(int fildes, unsigned long request, ...); //работает совместно с isatty
-------------------------------------------------------------------------------------------------------------------------
	The ioctl() function manipulates the underlying device parameters of special files.  In particular, many operating
    characteristics of character special files (e.g. terminals) may be controlled with ioctl() requests.  The argument
    fildes must be an open file descriptor. 
    !!An  ioctl request has encoded in it whether the argument is an ``in'' parameter or ``out'' parameter, and the size
    of the argument argp in bytes.  Macros and defines used in specifying an ioctl request are located in the file
    <sys/ioctl.h>.

    RETURN VALUES
    If an error has occurred, a value of -1 is returned and errno is set to indicate the error.

	ERRORS
    ioctl() will fail if:
    [EBADF]            fildes is not a valid descriptor.
    [EINVAL]           Request or argp is not valid.
    [ENOTTY]           fildes is not associated with a character special device.
    [ENOTTY]           The specified request does not apply to the kind of object that the descriptor fildes references.
-------------------------------------------------------------------------------------------------------------------------
info: man.
*/
#include <sys/acl.h> //acl_get_file(), acl_to_text(), acl_free()
/*
	acl_t acl_get_file(const char *path_p, acl_type_t type);
	char * acl_to_text(acl_t acl, ssize_t *len_p);
	int acl_free(void *obj_p);
-------------------------------------------------------------------------------------------------------------------------
	The acl_get_file() allow the retrieval of an ACL from a file. The acl_get_file() function is a POSIX.1e call that 
	allows the retrieval of a specified type of ACL from a file by name. These functions may cause memory to be allocated.  
	The caller should free any releasable memory, when the new ACL is no longer required, by calling acl_free().

	The acl_to_text() function translates the ACL pointed to by argument acl into a NULL terminated character string.
    If the pointer len_p is not NULL, then the function shall return the length of the string (not including the NULL
    terminator) in the location pointed to by len_p. The format of the text string returned by acl_to_text() for an
    ACL of type ACL_TYPE_EXTENDED differs from that specified by the POSIX.1e standard, and this function cannot 
    translate between formats. 
    This function allocates any memory necessary to contain the string and returns a pointer to the string.  
    The caller should free any releasable memory, when the new string is no longer required, by calling acl_free() 
    with the (void*)char as an argument.

    RETURN VALUES
     Upon successful completion, the function shall return a pointer to the long text form of an ACL.  Otherwise, a
     value of (char*)NULL shall be returned and errno shall be set to indicate the error.

	ERRORS
     If any of the following conditions occur, the acl_to_text() function shall return a value of (acl_t)NULL and set
     errno to the corresponding value:

     [EINVAL]       Argument acl does not point to a valid ACL.

                    The ACL denoted by acl contains one or more improperly formed ACL entries, or for some other
                    reason cannot be translated into a text form of an ACL.

     [ENOMEM]		The character string to be returned requires more memory than is allowed by the hardware or
                    software-imposed memory management constraints.

    The acl_free() call allows the freeing of ACL working space, such as is allocated by acl_dup(3), or acl_from_text(3).

    RETURN VALUES
     The acl_free() function returns the value 0 if successful; otherwise the value -1 is returned and the global 
     variable errno is set to indicate the error.
    ERRORS
     If any of the following conditions occur, the acl_free() function shall return -1 and set errno to the correspond-
     ing value:
	 [EINVAL]	The value of the obj_p argument is invalid.
-------------------------------------------------------------------------------------------------------------------------
info: man.
*/

#include <sys/errno.h> //используется в других функциях
#include <time.h> //time(), ctime()? используются в других функциях
#include <string.h>

typedef struct dirent t_dirent;
typedef struct stat t_stat;
typedef struct passwd t_passwd;
typedef struct group t_group;
typedef struct winsize t_winsize;
typedef struct s_file
{
	char *permission;
	char *links;
	char *owner;
	char *group;
	char *bytes;
	char *time;
	
    char* name;
    char* path;
    //char type;
    t_stat stat;
    struct s_file* next; 
}t_file;

t_file *mx_create_node_f(t_stat *statistics, char*permiss, char*name, char*path);
//void mx_push_front(t_file **list, void *data);
void mx_push_back_f(t_file **files, t_stat *statistics, char*permiss, char*name, char*path);
//void mx_pop_front(t_file **head);
//void mx_pop_back(t_file **head);
int mx_list_size_f(t_file *files);
t_file *mx_sort_list_f(t_file *lst, int(*cmp)(const char*, const char*));
t_file* mx_duplicate_list_f(t_file* list);

typedef struct s_flags {
    int a;
    int A;
    int G;
    int l;
    int r;
    int t;
    int c;
    int u;
    int S;
    int T;
    int h;
    int at;
    int e;
    int R;
    int C;
    int one;
    int F;
    int f;
    int m;
}              t_flags;

void mx_output_for_L(char **argv, int fils, int dirs, int*exit_value);
void mx_standart_output(char **argv, int fils, int dirs, int*exit_value);
char* get_permission(t_stat* stat, char* path);
void format_output(t_file *files_s, int show);

#endif


//stat(инфа о файле), lstat(о файле и ссылках), getpwuid(все по пользователю), getgrgid(все по группе), time(для ctime), ctime(Sep, 27, 00:00:00), 
//ioctl(непонятно), isatty(0 - дисковый, 1 - терминал), acl_get_file(), acl_to_text(), acl_free(),  listxattr(), getxattr(), errno(), readlink().

