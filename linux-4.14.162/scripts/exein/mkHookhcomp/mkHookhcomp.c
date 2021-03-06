/* Copyright 2019 Exein. All Rights Reserved.

Licensed under the GNU General Public License, Version 3.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    https://www.gnu.org/licenses/gpl-3.0.html

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <ctype.h>
#include "../config.h"

#define FILE_BUFFER_SIZE 1048510
#define MAXCHAR 1024

char *getMainType(char *s){
	char i;
	for (i=0; i<strlen(s);i++) {
		*(s+i)= *(s+i)==' '?'_':*(s+i);
		}
	while ((*(s+i)=='"') || (*(s+i)=='_') || (*(s+i)=='*')  || (*(s+i)=='\x00')) i--;
	*(s+i+1)='\x00';

	return s+1;
}

int main(int argc, char **argv)
{
	char keynum, secnum;
	sections_t *tmp_sec;
	keys_t *tmp_key;
	sections_t section_head = {0};
	sections_t config_head = {0};
	int pos=0, cur_array_pos=0;
	FILE *fp;
	char str[MAXCHAR];
	char *tmp_str;


	char *H_file=(char *)malloc(FILE_BUFFER_SIZE);
	memset(H_file,0, FILE_BUFFER_SIZE);

	if (argc!=3) {
		printf("call:\n\tmkHookhcomp hookspart.ini config.ini\n");
		exit(1);
		}

	read_ini_file(argv[1], &section_head);
	read_ini_file(argv[2], &config_head);

/*
  _                    _
 | |                  | |
 | |__   ___  __ _  __| | ___ _ __ ___
 | '_ \ / _ \/ _` |/ _` |/ _ \ '__/ __|
 | | | |  __/ (_| | (_| |  __/ |  \__ \
 |_| |_|\___|\__,_|\__,_|\___|_|  |___/

*/
	for (int i=0;tmp_sec=get_section(&i,&section_head, NUMBER); i++){
		int flag=0;
		tmp_str=SectionName(tmp_sec);
		for (int t=0; t<strlen(tmp_str);t++) *(tmp_str+t)= toupper((unsigned char) *(tmp_str+t));
		pos+=sprintf((H_file+pos),"#define %s    ", tmp_str);
		for (int j=0; tmp_key=get_key(&i,&j,&section_head, NUMBER_NUMBER); j++){
                        tmp_str=KeyName(tmp_key);
			if (strcmp(tmp_str,"type")) {
				 int tmp=j+1;
				 tmp_str=KeyValue(tmp_key);
				 for (int t=0; t<strlen(tmp_str);t++) *(tmp_str+t)= toupper((unsigned char) *(tmp_str+t));
				 pos+=sprintf((H_file+pos),get_key(&i,&tmp,&section_head, NUMBER_NUMBER)?"%s + ":"%s", getMainType(tmp_str));
				 flag=1;
				}
			}
		 pos+=sprintf((H_file+pos),flag==0?" 0\n":"\n");
		 flag=0;
		}
/*
              _               _
             | |             | |
   ___  _   _| |_ _ __  _   _| |_
  / _ \| | | | __| '_ \| | | | __|
 | (_) | |_| | |_| |_) | |_| | |_
  \___/ \__,_|\__| .__/ \__,_|\__|
                 | |
                 |_|
*/
	fp = fopen(KeyValue(get_key("files","header",&config_head, NAME_NAME)), "r");
	if (fp == NULL){
		printf("Could not open header file\n");
		return 1;
		}
	while (fgets(str, MAXCHAR, fp) != NULL) printf("%s", str);
	fclose(fp);
	puts(H_file);
        fp = fopen(KeyValue(get_key("files","footer",&config_head, NAME_NAME)), "r");
        if (fp == NULL){
                printf("Could not open header file\n");
                return 1;
                }
        while (fgets(str, MAXCHAR, fp) != NULL) printf("%s", str);
        fclose(fp);
	return 0;
}
