// #include <errno.h>
// #include "filesystem_fatfs.h"

// enum {
//     FD_STDIN,
//     FD_STDOUT,
//     FD_STDERR,
//     FD_FIRST,
//     FD_MAX = 10,
// };

// static int fd_to_dev[FD_MAX] = {
//     FD_STDIN, FD_STDOUT, FD_STDERR,
//     -1
// };

// typedef struct {
//    const char *name;
//    int (*open_r )( struct _reent *ptr, const char *path, int flags, int mode );
//    int (*close_r )( struct _reent *ptr, int fd );
//    _ssize_t (*write_r ) ( struct _reent *ptr, int fd, const char *buf, size_t count );
//    _ssize_t (*read_r )( struct _reent *ptr, int fd, char *buf, size_t count );
// } devoptab_t;

// const devoptab_t devoptab_stab = { "", NULL, NULL, NULL, NULL };
// const devoptab_t devoptab_fatfs = { /*"/usb/"*/"", &ff_open_r, &ff_close_r, &ff_write_r, &ff_read_r };

// const devoptab_t *devoptab_list[] = {
//     &devoptab_stab, // reserved for stdin
//     &devoptab_stab, // reserved for stdout
//     &devoptab_stab, // reserved for stderr
//     &devoptab_fatfs,
//     0
// };

// const size_t devoptab_size = sizeof(devoptab_list) / sizeof(devoptab_list[0]) - 1;

// int _open_r(struct _reent *ptr, const char *path, int flags, int mode )
// {
//     int fd = -1;
//     int devop = -1;
//     size_t i;

//     for (i = FD_FIRST; i < FD_MAX; i++) {
//         if (fd_to_dev[i] == -1) {
//             fd = i;
//             break;
//         }
//     }

//     if (fd == -1) {
//       ptr->_errno = ENFILE;
//       return -1;
//     }

//     for (i = FD_FIRST; i < devoptab_size; i++) {
//         if( strncmp(devoptab_list[i]->name, path, strlen(devoptab_list[i]->name)) == 0 ) {
//             devop = i;
//             break;
//         }
//     }

//     if (devop == -1) {
//         ptr->_errno = ENODEV;
//         return -1;
//     }

//     if (!devoptab_list[devop]->open_r(ptr, path, mode, flags)) {
//         return -1;
//     }

//     fd_to_dev[fd] = devop; // Set which device is used for the fd
//     return fd;
// }

// int _close_r(struct _reent *ptr, int fd)
// {
//     if (fd < FD_FIRST || fd >= FD_MAX || fd_to_dev[fd] < 0) {
//         ptr->_errno = EBADF;
//         return  EBADF;
//     }
//     int result = devoptab_list[fd_to_dev[fd]]->close_r(ptr, fd);
//     fd_to_dev[fd] = -1;
//     ptr->_errno = result;
//     return result;
// }

// _ssize_t _read_r(struct _reent *ptr, int fd, void *buf, size_t count)
// {
//     if (fd < 0 || fd >= FD_MAX || fd_to_dev[fd] < 0) {
//         ptr->_errno = EBADF;
//         return EBADF;
//     }
//     int result = devoptab_list[fd_to_dev[fd]]->read_r(ptr, fd, buf, count);
//     ptr->_errno = result < 0 ? EIO : 0;
//     return result;
// }
