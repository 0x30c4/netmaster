#include<common.h>
#include<handler.h>
#include<stringlib.h>
#include<headerparser.h>


void struct_test(LookForHeader *lfh){

	printf("ptr -> %s\n", lfh->header);
	for (size_t i = 0; i <= MAX_HEADERS; i++, lfh++){
		if (lfh->header != NULL)
			printf("%s | %ld \n", lfh->header, lfh->headerLen);
	}
}


int main(int argc, char const *argv[]){

	SERVER_ROOT server_data;

	server_data.uid = getuid();

	char *SR = "/root/CodeZ/C/netmaster/server_root\0";
	// char *SR = "/home/sanaf/CodeZ/netmaster/server_root\0";
	
	server_data.len = strlen(SR);

	strncpy(server_data.path, SR, server_data.len);

	LookForHeader *lfh = calloc(MAX_HEADERS, sizeof lfh);

	printf("%ld\n", sizeof lfh);
	
	// strncpy(lfh[0].header, "User-Agent\0	", 11);
	// lfh[0].headerLen = 10;

	// struct_test(&lfh);
	// SlaveHandler(0, &server_data, lfh);

	return 0;
}

