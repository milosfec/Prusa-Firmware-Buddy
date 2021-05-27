#include <errno.h>
#include <ff.h>
#include <fcntl.h>
#include <sys/iosupport.h>
#include <sys/stat.h>
#include <sys/syslimits.h>
#include "filesystem_fatfs.h"

int getErrno(FRESULT result) {
    switch (result) {
    case FR_OK:
        return 0;
    case FR_EXIST:
        return EEXIST;
    case FR_NO_FILE:
        return ENOENT;
    case FR_NO_PATH:
        return ENOENT;
    case FR_INVALID_NAME:
        return ENOTDIR;
    case FR_WRITE_PROTECTED:
        return EROFS;
    case FR_DENIED:
        return EACCES;
    case FR_TOO_MANY_OPEN_FILES:
        return ENFILE;
    case FR_NOT_ENOUGH_CORE:
        return ENOMEM;
    case FR_DISK_ERR:
        return EIO;
    case FR_INT_ERR:
        return EFAULT; // FatFS issue, possible stack overflow
    case FR_NOT_READY:
        return EBUSY;
    case FR_INVALID_OBJECT:
        return EINVAL;
    case FR_INVALID_DRIVE:
        return ENODEV;
    case FR_NOT_ENABLED:
        return EIO;
    case FR_NO_FILESYSTEM:
        return EIO;
    case FR_MKFS_ABORTED:
        return EIO;
    case FR_TIMEOUT:
        return EIO;
    case FR_LOCKED:
        return EBUSY;
    case FR_INVALID_PARAMETER:
        return EINVAL;
    default:
        return EINVAL;
    }
}

int getFatfsMode(int posixMode) {
    int mode = 0;

    switch (posixMode & O_ACCMODE) {
    case O_RDWR:
        mode |= FA_READ;
    case O_WRONLY:
        mode |= FA_WRITE;
        break;
    case O_RDONLY:
    default:
        mode |= FA_READ;
    }

    return mode;
}

int open_r(struct _reent *r, void *fileStruct, const char *path, int flags, int mode) {
    FRESULT result;

    if (!path || !path[0]) {
        r->_errno = EINVAL;
        return -1;
    }

    mode = getFatfsMode(mode);

    if (!mode) {
        r->_errno = EINVAL;
        return -1;
    }

    FIL *fp = fileStruct;
    result = f_open(fp, path, mode);
    r->_errno = getErrno(result);

    return result == FR_OK ? 0 : -1;
}

int close_r(struct _reent *r, void *fileStruct) {
    FRESULT result;

    FIL *fp = fileStruct;
    result = f_close(fp);
    r->_errno = getErrno(result);

    return result == FR_OK ? 0 : -1;
}

ssize_t write_r(struct _reent *r, void *fileStruct, const char *ptr, size_t len) {
    FRESULT result;
    UINT bytesWritten;

    if (!ptr) {
        r->_errno = EINVAL;
        return 0;
    }

    if (len == 0) {
        r->_errno = 0;
        return 0;
    }

    result = f_write(fileStruct, ptr, len, &bytesWritten);
    r->_errno = getErrno(result);

    return bytesWritten;
}

ssize_t read_r(struct _reent *r, void *fileStruct, char *ptr, size_t len) {
    FRESULT result;
    UINT bytesRead;

    if (!ptr) {
        r->_errno = EINVAL;
        return 0;
    }

    if (len == 0) {
        r->_errno = 0;
        return 0;
    }

    result = f_read(fileStruct, ptr, len, &bytesRead);
    r->_errno = getErrno(result);

    return bytesRead;
}

off_t seek_r(struct _reent *r, void *fd, off_t pos, int dir) {
    return pos;
}

int fstat_r(struct _reent *r, void *fd, struct stat *st) {
    st->st_mode = S_IFCHR;
    return 0;
}

DIR_ITER *diropen_r(struct _reent *r, DIR_ITER *dirState, const char *path) {
    FRESULT result;

    if (path == NULL || !path[0]) {
        r->_errno = EINVAL;
        return NULL;
    }

    result = f_opendir(dirState->dirStruct, path);
    r->_errno = getErrno(result);

    if (result != FR_OK) {
        return NULL;
    }

    return dirState;
}

int dirreset_r(struct _reent *r, DIR_ITER *dirState) {
    FRESULT result;

    result = f_rewinddir(dirState->dirStruct);
    r->_errno = getErrno(result);

    return result == FR_OK ? 0 : -1;
}

int dirnext_r(struct _reent *r, DIR_ITER *dirState, char *filename, struct stat *filestat) {
    FRESULT result;
    FILINFO fno;

    if (filename == NULL || filestat == NULL) {
        r->_errno = EINVAL;
        return -1;
    }

    result = f_readdir(dirState->dirStruct, &fno);
    r->_errno = getErrno(result);

    if (result != FR_OK) {
        return -1;
    }

    if (!fno.fname[0]) {
        // Empty filename means end of directory
        r->_errno = ENOENT; // has to be set at the end
        return -1;
    }

    strncpy(filename, fno.fname, NAME_MAX);

    filestat->st_mode = fno.fattrib & AM_DIR ? S_IFDIR : S_IFREG;

    return 0;
}

int dirclose_r(struct _reent *r, DIR_ITER *dirState) {
    FRESULT result;

    result = f_closedir(dirState->dirStruct);
    r->_errno = getErrno(result);

    return 0;
}

const devoptab_t devoptab_fatfs = {
    .name = "usb",
    .structSize = sizeof(FIL),
    .open_r = open_r,
    .close_r = close_r,
    .write_r = write_r,
    .read_r = read_r,
    .seek_r = seek_r,
    .fstat_r = fstat_r,
    .stat_r = NULL,
    .link_r = NULL,
    .unlink_r = NULL,
    .chdir_r = NULL,
    .rename_r = NULL,
    .mkdir_r = NULL,
    .dirStateSize = sizeof(DIR),
    .diropen_r = diropen_r,
    .dirreset_r = dirreset_r,
    .dirnext_r = dirnext_r,
    .dirclose_r = dirclose_r,
    .statvfs_r = NULL,
    .ftruncate_r = NULL,
    .fsync_r = NULL,
    .deviceData = NULL,
    .chmod_r = NULL,
    .fchmod_r = NULL,
    .rmdir_r = NULL,
    .lstat_r = NULL,
    .utimes_r = NULL,
};

void keep_open_c();
int filesystem_fatfs_init() {
    int device = AddDevice(&devoptab_fatfs);
    setDefaultDevice(device);

    //keep_open_c();

    return device;
}
