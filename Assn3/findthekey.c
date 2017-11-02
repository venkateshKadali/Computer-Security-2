#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/evp.h>
void padding(char *s, int length);
int result(char *buf, char *s, int len, char *match);
int compare( char const *a, char const *b);
int main()
{
	char plainText[]="This is a top secret";
	char cipherText[]="8d20e5056a8d24d0462ce74e4904c1b513e10d1df4a2ef2ad4540fae1ca0aaf9";	
	char match[]="Match";
	int num, i,outlen,tmplen;
	FILE *key;
	char pbuffer[1024], outbuf[1024+EVP_MAX_BLOCK_LENGTH], words[16],t;
	char iv[16]={0};
	EVP_CIPHER_CTX ctx;
	EVP_CIPHER_CTX_init(&ctx);
	key=fopen("dictionary.txt","r");
	while(fgets(words,16,key))
	{
		words[i-1]='\0';
		i=strlen(words);
		if(i<16){padding(words,(16));}
		EVP_EncryptInit_ex(&ctx, EVP_aes_128_cbc(), NULL, words, iv);
		if(!EVP_EncryptUpdate(&ctx, outbuf, &outlen, plainText, strlen(plainText)))
		{	EVP_CIPHER_CTX_cleanup(&ctx);
			return 0;
		}
		if(!EVP_EncryptFinal_ex(&ctx, outbuf+outlen, &tmplen))
		{
			EVP_CIPHER_CTX_cleanup(&ctx);
			return 0;
		}
		outlen+=tmplen;
		int i;
		char * buf_str=(char*) malloc(2*outlen+1);
		char* buf_ptr=buf_str;
		for(i=0;i<outlen;i++)
		{
			buf_ptr+=sprintf(buf_ptr,"%02X",outbuf[i]);
		}
		*(buf_ptr+1)='\0';
		if(compare(cipherText,buf_str)==0)
			result(outbuf, words, outlen, match);
		
	}
	fclose(key);
	return 1;
}
void padding(char *c, int length)
{
	int len;
	len=strlen(c);
	while(len<length)
	{c[len]=' '; len++;}
	c[len]='\0';
}
int compare(char const *str1, char const *str2)
{	for(;;str1++,str2++)	{
		int res=tolower(*str1)- tolower(*str2);
		if(res!=0||!*str1)
			return res;
	}
}
int result(char *buf, char *s, int len, char *match)
{
	int i, n, j, k;
	for(j=0;j<strlen(s);j++)
	{printf("%c", s[j]);}
	printf("\t");
	for(i=0;i<len;i++)
	{printf("%02x", buf[i]);}
	printf("\t");
	for(k=0;k<strlen(match);k++)
	{printf("%c", match[k]);}
	printf("\n");	
	return(0);
}

