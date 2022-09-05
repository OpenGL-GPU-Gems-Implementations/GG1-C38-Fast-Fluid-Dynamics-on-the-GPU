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
using std::ifstream;
using std::ofstream;
using std::fstream;
using std::cout;

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
 * @brief Accepts already open files, and copies the contents of inFile to the current position in outFile
 * 
 * @param inFile Input file to be copied
 * @param outFile Output file to be written to
 */
inline void copyIn(ifstream &inFile, ofstream &outFile) {
    string line;
    while (getline(inFile, line)) {
        outFile << line << "\n";
    }
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
    string fName = inputPath.substr(inputPath.find_last_of('/')); // includes leading forward slash
    
    // trim paths (whitespace and extraneous forward slashes)
    string trimInput = rtrim(inputPath);
    string trimOutput = rtrim(outputPath);

    // if source file directory and trimmed output path are the same, throw an error
    // notably, this is incomplete; if one path is absolute, and the other path is relative, then although the condition should be true, this does not catch it
    //  for my purposes, this is fine, but if used without knowledge, this might cause an issue
    if (directory.compare(trimOutput) == 0)
        throw std::runtime_error("Read/write conflict; output directory contains source file");

    // open files
    ifstream inputFile;
    ofstream outputFile;
    inputFile.open(trimInput);
    outputFile.open(trimOutput + fName);

    // read lines, look for the #include directive, and handle cases when we identify the macro
    if (!inputFile.is_open())
        throw std::runtime_error("Input file " + trimInput + " unable to be read");

    string line, impFile;
    size_t ind;
    while (getline(inputFile, line)) {
        ind = line.find("#include");
        if (ind != string::npos) {
            // everything after #include (trimmed) should be a relative file path
            impFile = rtrim(ltrim(line.substr(ind+8)));

            ifstream impFileStream;
            impFileStream.open(directory + "/" + impFile);

            copyIn(impFileStream, outputFile);

            impFileStream.close();
        } else {
            outputFile << line << "\n";
        }
    }

    // close files
    inputFile.close();
    outputFile.close();

    return trimOutput + fName;
}

#endif