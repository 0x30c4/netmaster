#include "argumentparser.h"

void arg_parser(int argc, char *argv[], char __special_dir[BUFSIZE / 4], char __pass[BUFSIZE / 128], int *port) { 
    if (argc < 2)
        print_usage(argv[0]);

    bzero(__special_dir, BUFSIZE / 4);
    bzero(__pass, BUFSIZE / 128);
    char *docroot = "\0";
    char *special_dir = docroot;
    char *pass = docroot;
    short opt;
    short int _special = 0;
    short int _port = _special;
    short int _pass = _special;
    short int _docroot = _special;


    while((opt = getopt(argc, argv, "S:t:p:P:h")) != -1){  
        if (strlen(optarg) > BUFSIZE / 4 || (opt == 'P' && strlen(optarg) > 128)){
        	fprintf(stderr, "usage: %s : too long argument for \"-%c\".\n", argv[0], opt);
        	exit(EXIT_FAILURE);
        }

        switch(opt){
            // help menu 
            case 'h':
                print_usage(argv[0]);
            case 'p':
                if (_port > 0) print_usage(argv[0]);
                *port = port_parser(optarg);
                _port++;
                break;
            case 'P':
                if (_pass > 0) print_usage(argv[0]);
                pass = optarg;
                _pass++;
                break;
            case 't':
                if (_docroot > 0) print_usage(argv[0]);
                docroot = optarg;
                _docroot++;
                break;
            case 'S':
                if (_special > 0) print_usage(argv[0]);
                special_dir = optarg;
                _special++;
                break;    
            case ':':
                print_usage(argv[0]);
            case '?':
                print_usage(argv[0]);
        }  
    }

    if (_port == 0 || _pass == 0 || _special == 0)
    	print_usage(argv[0]);
	
    if (_docroot == 0)
		docroot = realpath(".", NULL);
    
    if (_docroot != 0){
    	if (chdir(realpath(docroot, NULL))){
        	fprintf(stderr, "usage: %s : couldn't change the directory to \"%s\".\n", argv[0], docroot);
        	exit(EXIT_FAILURE);
    	}
    }

    realpath(special_dir, __special_dir);
    struct stat sb;
    if (stat(__special_dir, &sb) == -1) _special = 69;
    if ((sb.st_mode & S_IFMT) != S_IFDIR)
    	_special = 69;

    if (stringcmp(__special_dir, realpath(docroot, NULL)) || _special == 69){
       	fprintf(stderr, "usage: %s : couldn't initialize \"%s\" as a special directory.\n", argv[0], __special_dir);
        exit(EXIT_FAILURE);
    }

    strncpy(__pass, pass, strlen(pass));
}