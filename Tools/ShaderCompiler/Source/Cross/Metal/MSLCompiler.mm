#include "MSLCompiler.h"

#import <Metal/Metal.h>
#import <Foundation/Foundation.h>
#include <vector>
#include <string>
#include <stdexcept>

std::vector<uint8_t> CompileMSLToMetallib(const std::string& mslSource) {
    id<MTLDevice> device = MTLCreateSystemDefaultDevice();
    if (!device) {
        throw std::runtime_error("Metal is not supported on this Mac");
    }

    NSError* error = nil;
    NSString* source = [NSString stringWithUTF8String:mslSource.c_str()];

    MTLCompileOptions* options = [MTLCompileOptions new];
    options.languageVersion = MTLLanguageVersion2_0;
    options.fastMathEnabled = YES;

    id<MTLLibrary> library = [device newLibraryWithSource:source
                                                  options:options
                                                    error:&error];
    if (!library) {
        NSString* errDesc = error.localizedDescription;
        throw std::runtime_error("Metal compilation failed: " + std::string([errDesc UTF8String]));
    }

    NSData* metallibData = library.binaryData;
    if (!metallibData) {
        throw std::runtime_error("Failed to extract metallib data");
    }

    const uint8_t* bytes = (const uint8_t*)[metallibData bytes];
    size_t length = [metallibData length];
    std::vector<uint8_t> result(bytes, bytes + length);

    return result;
}