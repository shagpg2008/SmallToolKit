#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE (512<<10)

int fsplit_by_size(const char *filename, int size, const char *dstfile, const char *suffix)
{
	char name[260] = {0};
	char *pname = name;
	int ret = 0, index = 0;
	int read_count = 0;
	FILE *fp = fopen(filename, "rb");
	char *pbuff = NULL;

	if(fp == NULL){
		ret = -1;
		goto done;
	}

	if((pbuff = malloc(size)) == NULL) {
		ret =-1;
		goto done;
	}

	strcpy(name, dstfile);
	pname = name+strlen(dstfile);
	for(index = 0; !feof(fp); index++) {
		FILE *dstfp = NULL;
		read_count = fread(pbuff, 1, size, fp);
		sprintf(pname, suffix, index);
		if((dstfp = fopen(name, "wb")) == NULL) {
			continue;
		}
		
		fwrite(pbuff, 1, read_count, dstfp);
		fclose(dstfp);
	}

done:
	if(fp != NULL){
		fclose(fp);
	}

	if(pbuff != NULL) {
		free(pbuff);
	}

	return ret;
}

int fsplit_by_key(const char *filename, const char *key, int len, const char *dstfile)
{
	char name[260] = {0};
	int ret = 0, index = 0;
	int read_count = 0;
	FILE *fp = fopen(filename, "rb");
	char *pbuff = NULL;

	if(fp == NULL){
		ret = -1;
		goto done;
	}

	if((pbuff = malloc(BUF_SIZE)) == NULL) {
		ret =-1;
		goto done;
	}

	read_count = 0;

	for(index = 0; read_count >= len || !feof(fp); index++) {
		FILE *dstfp = NULL;
		char *tmp = pbuff+1;
		char *pBig = NULL;
		int n = 0;

		sprintf(name, dstfile, index);
		if((dstfp = fopen(name, "wb")) == NULL) {
			continue;
		}

		while(read_count >= len  || (n = fread(pbuff + read_count, 1, BUF_SIZE - read_count, fp)) > 0) {
			read_count += n;
			pBig = pbuff + read_count - len + 1;

			while(tmp < pBig && memcmp(tmp, key, len) != 0) {
				tmp++;
			}

			fwrite(pbuff, 1, tmp-pbuff, dstfp);
			read_count = read_count-(tmp-pbuff);
			memcpy(pbuff, tmp, read_count);

			if(tmp < pBig) {
				break;
			}

			tmp = pbuff;
		}

		if(read_count < len &&  feof(fp)) {
			fwrite(pbuff, 1, read_count, dstfp);
			read_count = 0;
		}

		n = 0;
		fclose(dstfp);
	}

done:
	if(fp != NULL){
		fclose(fp);
	}

	if(pbuff != NULL) {
		free(pbuff);
	}
	
	return ret;
}

int merge_files(const char *dstfilename, const char *argv[])
{
	int index = 0, ret = 0;
	char *buf = NULL;
	int len = 0;
	FILE *fp = fopen(dstfilename, "wb");

	if(fp == NULL ) {
		ret = -1;
		goto done;
	}

	buf = malloc(BUF_SIZE);
	while(argv[index] != NULL) {
		FILE *src = fopen(argv[index], "rb");
		if(src == NULL) {
			ret = -1;
			goto done;
		}
		while(1) {
			if((len = fread(buf, 1, BUF_SIZE, src)) >= 0) {
				fwrite(buf, 1, len, fp);
				if(len < BUF_SIZE) {
					break;
				}
			} else {
				ret = -1;
				goto done;
			}
		}
		fclose(src);
		index++;
	}

done:
	if(buf != NULL) {
		free(buf);
	}

	if(fp != NULL) {
		fclose(fp);
	}

	return ret;
}

