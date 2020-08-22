#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "httprequest.h"
#include "methods.h"

int verbose = 0;
static const char* const defaultUrl = "http://localhost:8000/api/json/v2";

void usage(const char* prog)
{
    printf("Usage:\n%s\n", prog);
    printf("\t -u [url]");
    printf("\tDefault URL: %s \n", defaultUrl);
    printf("\t -m [method name]\n");
    printf("Possible methods:\n");
    printMethodsList();
}

int main(int argc, char** argv)
{
    char methodName[256];
    memset(methodName, 0, sizeof(methodName));

    char url[256];
    memset(methodName, 0, sizeof(methodName));

    int opt;
    while ((opt = getopt(argc, argv, "m:u:vh")) != -1){
        switch (opt) {
        case 'm':
            strncpy(methodName, optarg, sizeof(methodName) - 1);
            break;
        case 'v':
            verbose++;
            break;
        case 'u':
            strncpy(url, optarg, sizeof(url) - 1);
            break;
        case 'h':
            usage(argv[0]);
            return EXIT_SUCCESS;
        default:
            break;
        };
    };

    if (strlen(methodName) == 0) {
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    if (strlen(url) == 0) {
        strcpy(methodName, defaultUrl);
    }

    char data[512];
    memset(data, 0, sizeof(data));
    doMethodByName(methodName, data);
    if (strlen(data) == 0) {
        fprintf(stderr, "Method '%s' not exist. Possible methods:\n", methodName);
        printMethodsList();
        return EXIT_FAILURE;
    }

    const long code = sendPOST(url, data, strlen(data));
    if (code != 201) {
        fprintf(stderr, "Error sendPOST(): %s\n", (code == -1) ? "Inernal error" : "");
    } else {
        printf("Ok!\n");
    }

    return EXIT_SUCCESS;
}
