#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
    #include <io.h>
    #include <fcntl.h>
#endif

void PrintUsage()
{
    fprintf(stderr, "Usage: \n");
    fprintf(stderr, "\tbin2h [Input] [Output]\n");
    fprintf(stderr, "\tbin2h [Output] - reads from stdin\n");
    fprintf(stderr, "using '-' for output will forward output into stdout");
}

int main(int argc, char* argv[]) {
    #ifdef _WIN32
    _setmode(_fileno(stdin), _O_BINARY);
    _setmode(_fileno(stdout), _O_BINARY);
    #endif

    FILE* in = stdin;
    FILE* out = stdout;
    short closeIn = 0;

    if(argc == 2) {
        if(argv[1][0] == '-') {
            if(argv[1][1] == 'h') {
                PrintUsage();
                return 0;
            } else {
                if (isatty(fileno(in))) {
                    fprintf(stderr, "Error: Can't read from terminal.");
                    return 1;
                }
            }
        } else {
            if (isatty(fileno(in))) {
                fprintf(stderr, "Error: Can't read from terminal.");
                return 1;
            }
            out = fopen(argv[1], "wb");
        }
    } else if(argc == 3) {
        in = fopen(argv[1], "rb");
        if (!in) {
            fprintf(stderr, "bin2h: Invalid input file name");
            return 1;
        }

        // '-' specifies a pipe
        if(argv[2][0] != '-') {
            out = fopen(argv[2], "wb");
            if(!out) {
                fprintf(stderr, "bin2h: Unable to open output file");
                return 1;
            }
        }
        closeIn = 255;
    } else {
        PrintUsage();
        return 0;
    }

    // Write as C array
    int c, i = 0;

    fprintf(out, "static const unsigned char data[] = {\n");
    while ((c = fgetc(in)) != EOF) {
        fprintf(out, "0x%02x, ", c);
        if ((i + 1) % 12 == 0) fprintf(out, "\n");

        ++i;
    }
    fprintf(out, "\n};");

    // Close if it's not an std file stream
    if (closeIn) fclose(in);

    return 0;
}