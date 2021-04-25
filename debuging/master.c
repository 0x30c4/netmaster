#include "../include/common.h"
#include "../include/handler.h"
#include "../include/parser.h"

// 414 (Request-URI Too Long)

int parse(const char* line);

int main(int argc, char const *argv[]){
	// parse("GET /path/script.cgi?field1=value1&field2=value2&fbclid=IwAR0gw6sBY47CIVxty4Qw8brrUnTbpRrh-uaKaP4nXsT9NckRGKSCHeTWReo HTTP/1.1");

	char header[MAX_HEADER];

	bzero(header, MAX_HEADER);

	int rec = 0, header_no = 0;

	while ((rec = readCRLF(0, header)) > 0){
		if (header_no == 0){
			parse(header);
			printf("%d\n", rec);
		}

		
		bzero(header, MAX_HEADER);
		header_no++;
	}
	


	return 0;
}

int parse(const char * line){
    /* Find out where everything is */
    const char *start_of_path = strchr(line, ' ') + 1;
    const char *start_of_query = strchr(start_of_path, '?');
    const char *end_of_query = strchr(start_of_query, ' ');

    /* Get the right amount of memory */
    char path[start_of_query - start_of_path];
    char query[end_of_query - start_of_query];

    /* Copy the strings into our memory */
    strncpy(path, start_of_path,  start_of_query - start_of_path);
    strncpy(query, start_of_query, end_of_query - start_of_query);

    /* Null terminators (because strncpy does not provide them) */
    path[sizeof(path)] = 0;
    query[sizeof(query)] = 0;

	/* header type */
	start_of_path = strchr(line, ' ');
	printf("%c", INDEX_FILE[0]);
	// maek str pointer cmp
	do {
		printf("%c-\n", (char)(*line++));
	}while (*start_of_path != *line);


    /*Print */
    // printf("%s %ld\n", query, sizeof(query));
    // printf("%s %ld\n", path, sizeof(path));

	// printf("\n%c", (char)(*start_of_path--));
	// printf("\n%c", (char)(*start_of_path--));
	// printf("\n%c", (char)(*start_of_path--));
	// printf("\n%c", (char)(*start_of_path--));
	// printf("\n%d\n", *line == *start_of_path--);
	return 0;
}