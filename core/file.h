
#ifndef FILE__H
#define FILE__H

#ifdef __cplusplus
extern "C" {
#endif

int fsplit_by_size(const char *filename, int size, const char *dstfile, const char *suffix);
int fsplit_by_key(const char *filename, const char *key, int len, const char *dstfile);
int merge_files(const char *dstfilename, const char *argv[]);


#ifdef __cplusplus
}
#endif
#endif