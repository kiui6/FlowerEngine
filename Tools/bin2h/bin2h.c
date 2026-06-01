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
    #endif

    FILE* in = stdin;
    FILE* out = stdout;
    short closeIn = 0;

    if(argc == 2) {
        if(argv[1][0] == '-') {
            if(argv[1][1] == 'h') {
                PrintUsage();
                return 0;
            }
        } else {
            out = fopen(argv[1], "wb");
        }
    } else if(argc == 3) {
        in = fopen(argv[1], "rb");
        // '-' specifies a pipe
        if(argv[2][0] != '-') {
            out = fopen(argv[2], "wb");
        }
        if (!in) return 1;
        closeIn = 255;
    } else {
        PrintUsage();
        return 0;
    }

    unsigned char *data;
    long dataSize;

    // Find file size
    long originalPos = ftell(in);
    fseek(in, 0, SEEK_END);
    dataSize = ftell(in);
    data = (unsigned char*)malloc(dataSize);
    fseek(in, originalPos, SEEK_SET);

    fread(data, 1, dataSize, in);

    // Close if it's not an std file stream
    if (closeIn) fclose(in);

    // Write as C array
    fprintf(out, "static const unsigned char data[%ld] = {\n", dataSize);
    for (size_t i = 0; i < dataSize; ++i) {
        fprintf(out, "0x%02x, ", data[i]);
        if ((i + 1) % 12 == 0) fprintf(out, "\n");
    }
    fprintf(out, "\n};");
    return 0;
}