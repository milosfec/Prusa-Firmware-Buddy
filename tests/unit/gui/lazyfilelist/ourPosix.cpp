#include "ourPosix.hpp"
#include "strlcpy.c"

std::vector<FileEntry> testFiles0;

void MakeLFNSFN(dirent *fno, const char *lfn, size_t fileindex) {
    //    assert(fileindex < 100); // to make the SFN numeric suffix within 2 digits for now
    //    assert(lfn);
    //    assert(fno);
    if (strlen(lfn) >= 13) {
        uint8_t charsWritten = snprintf(fno->d_name, sizeof(fno->d_name), "%-.6s~%02u.GCO", lfn, (unsigned)fileindex);
        fno->d_name[charsWritten + 1] = 1;
        strlcpy(fno->d_name + charsWritten + 2, lfn, sizeof(fno->d_name) - charsWritten - 2);
        fno->lfn = fno->d_name + charsWritten + 2;
    } else {
        uint8_t charsWritten = strlcpy(fno->d_name, lfn, sizeof(fno->d_name));
        fno->d_name[charsWritten + 1] = 1;
        fno->lfn = fno->d_name;
    }
}
DIR *opendir(const char *path) {
    DIR *dp = new DIR;
    dp->obj = 0;
    return dp;
}

int closedir(DIR *dp) {

    delete dp;
    return 1;
}

dirent *readdir(DIR *dp) {
    if (dp->obj >= testFiles0.size()) {
        return nullptr;
    } else {
        MakeLFNSFN(&(dp->dirStruct), testFiles0[dp->obj].lfn.c_str(), dp->obj);
        dp->dirStruct.d_type = testFiles0[dp->obj].dir ? DT_DIR : DT_REG;
        dp->dirStruct.time = testFiles0[dp->obj].time;
        ++dp->obj;
    }
    return &(dp->dirStruct);
}
