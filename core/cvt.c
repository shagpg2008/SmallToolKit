
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int calc_param_len(const char *fmt)
{
	char buff[64] = {0};
#if defined(WIN32) || defined(WIN64) || defined(_WINDOWS)
	_snprintf(buff, sizeof(buff)-1, fmt, 0xff);
#else
	snprintf(buff, sizeof(buff)-1, fmt, 0xff);
#endif
	return strlen(buff);
}

int convert_to_bin(char *buff, int maxlen, const char *data/*, int size*/, const char*infmt)
{
	int temp = 0;
	int len = strlen(data);
	int size = calc_param_len(infmt);
	int index = 0;
	unsigned char *buff_bake = (unsigned char *)buff;

	for(index = 0; index < len && maxlen-- > 0; index += size)  {
		if(sscanf(data, infmt, &temp) > 0) {
			*buff_bake++ = (unsigned char)temp;
		}

		data += size;
	}

	return buff_bake - buff;
}


void convert_from_buff(char *buff, int maxlen, const char *data/*, int size*/, const char*infmt, const char *outfmt)
{
	int temp = 0;
	int size = calc_param_len(infmt);
	int len = strlen(data);
	int index = 0;
	char *buff_bake = buff;
	//printf("len=%d,size=%d\n" , len, size);
	for(index = 0; index < len; index += size)  {
		if(sscanf(data, infmt, &temp) > 0) {
#if defined(WIN32) || defined(WIN64) || defined(_WINDOWS)
			buff_bake += _snprintf(buff_bake, maxlen-(buff_bake-buff), outfmt, temp);
#else
			buff_bake += snprintf(buff_bake, maxlen-(buff_bake-buff), outfmt, temp);
#endif
		} else {
			break;
		}
		data += size;
	}
}

void convert_from_file(char *buff, int maxlen, FILE *fp, const char*infmt, const char *outfmt)
{
	int temp = 0;
	int index = 0;
	char *buff_bake = buff;
	//printf("len=%d,size=%d\n" , len, size);
	for(index = 0; !feof(fp); )  {
		if(fscanf(fp, infmt, &temp) > 0) {
#if defined(WIN32) || defined(WIN64) || defined(_WINDOWS)
			buff_bake += _snprintf(buff_bake, maxlen-(buff_bake-buff), outfmt, temp);
#else
			buff_bake += snprintf(buff_bake, maxlen-(buff_bake-buff), outfmt, temp);
#endif
		} else {
			break;
		}
	}
}

#ifdef CVT_CLI
#include <getopt.h>
static void usage()
{
	puts("Usage: \n"
		"-d formatted data from command-line\n"
		"-f formatted data from file\n"
		"-i input data format, default :\"%02x\"\n"
		"-o output data format, default: \"%c\"\n"
		"-u data size unit,     default: 3\n"
		"-h this help menu.\n" 
		"For example: cvt -d \"30,31\" -i \"%02x,\" -o \"0x%02x,\"\n"
		"           : cvt -d \"30 31\" -f file \n"
		"           : cvt -d \"30, 31\" -u 4 \n"
	);
}

//./cvt -d "30 31 32 34" -i "%02x " -o "%c" -f file-data
int cvt_main(int argc, char *argv[])
{
	char buff[1024] = {0};
	const char *of = "%c"; 
	const char *in = "%02x";
	const char *fn = NULL;
	const char *data = NULL;
	char size = 3;
	int temp = {0};
	int c = 0;
	int len = 0, index = 0;

	if(argc < 2) {
		usage();
		return 0;
	}

	while( (c = getopt(argc, argv, "d:i:o:f:u:h")) != -1) {
		switch(c) {
		case 'd': data = optarg;  break;
		case 'i': in = optarg; break;
		case 'o': of = optarg; break;
		case 'f': fn = optarg; break;
		case 'u': size = atoi(optarg); 
			if(size <= 0 || size > 4) {
				usage();
				return -1;
			}
			break;
		default :
			usage(); 
			return -1;
			break;
		}
	}

/*
	printf("-i=%s\n", in);
	printf("-o=%s\n", of);
	if(data != NULL) printf("-d=%s\n", data);
	printf("-o=%s\n", of);
	printf("-u=%d\n", size);
	if(fn != NULL) printf("-f=%s", fn);
*/
	if(data != NULL) {
		convert_from_buff(buff, sizeof(buff), data/*, size*/, in, of);
		
	}
	
	if(fn != NULL) {
		FILE *fp = fopen(fn, "rb");
		if(fp == NULL) {
			printf("Can't find the file %s\n", fn);
			return 0;
		}
//		printf("fp = %p", fp);
		convert_from_file(buff, sizeof(buff), fp, in, of);
		fclose(fp);
	}

	puts(buff);
	return 0;
}



int main(int argc, char *argv[])
{
	return cvt_main(argc, argv);
}

#endif
