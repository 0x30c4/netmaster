#include<common.h>
#include<handler.h>
#include<stringlib.h>
#include<headerparser.h>

int main(int argc, char const *argv[]){

	SERVER_ROOT server_data;

	char *SR = "/root/CodeZ/C/netmaster/server_root\0";
	// char *SR = "/home/sanaf/CodeZ/netmaster/server_root\0";
	
	server_data.len = strlen(SR);

	strncpy(server_data.path, SR, server_data.len);

	SlaveHandler(0, &server_data);

	return 0;
}

