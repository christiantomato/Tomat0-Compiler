/**
 * @file file_reader.h
 * @brief Cleanly returns the contents of a file as a string. 
 */

#ifndef FILE_READER_H
#define FILE_READER_H

/**
 * @brief Reads the file contents and builds a string. 
 * 
 * @param filename The name of the file to be read. 
 * @return String of file contents. 
 */

char* read_file(const char* filename);

#endif
