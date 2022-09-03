/**
 * @file glslInclude.h
 * @author Eron Ristich (eron@ristich.com)
 * @brief Introduces the #include macro into glsl files for compilation
 * @version 0.1
 * @date 2022-09-03
 */

#ifndef GLSL_INCLUDE
#define GLSL_INCLUDE

#include <iostream>
#include <fstream>
#include <string>
using std::string;

const string TRIMMABLE = " \n\r\t\f\v/";

/**
 * @brief Trims trailing characters identified in TRIMMABLE
 * 
 * @param s string to trim
 * @return trimmed string 
 */
inline string rtrim(const string &s) {
    size_t end = s.find_last_not_of(TRIMMABLE);
    return (end == string::npos) ? "" : s.substr(0, end + 1);
}

/**
 * @brief Trims leading characters identified in TRIMMABLE
 * 
 * @param s string to trim
 * @return trimmed string 
 */
inline string ltrim(const string &s) {
    size_t start = s.find_first_not_of(TRIMMABLE);
    return (start == string::npos) ? "" : s.substr(start);
}

/**
 * @brief Compiles GLSL files with the #include macro into a separate file
 * 
 * @param inputPath Path to the GLSL shader to compile. All #include files listed within must be relative to this file. #include macros should also be on their own line. Doing otherwise will cause errors. Leading and trailing white space is okay.
 * @param outputPath Path to the output compilation. Output filename will be the same as the input filename. Compiling to the same directory as the source file may overwrite source file in unpredictable ways, and is not recommended, and thus a runtime error is thrown if attempted.
 * @return Output path and filename.
 */
inline string compileGLSL(string inputPath, string outputPath) {
    // source file directory and filename
    string directory = inputPath.substr(0, inputPath.find_last_of('/'));
    string fName = inputPath.substr(inputPath.find_last_of('/') + 1);
    
    // trim paths (whitespace and extraneous forward slashes)
    string trimInput = rtrim(inputPath);
    string trimOutput = rtrim(outputPath);

    // if source file directory and trimmed output path are the same, throw an error
    // notably, this is incomplete; if one path is absolute, and the other path is relative, then although the condition should be true, this does not catch it
    //  for my purposes, this is fine, but if used without knowledge, this might cause an issue
    if (directory.compare(trimOutput) == 0)
        throw std::runtime_error("Read/write conflict; output directory contains source file");

    // open files

}

#endif