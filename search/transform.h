//
//  transform.h
//  FileSearch
//
//  Created by Joyce Xu on 11/26/15.
//  Copyright © 2015 Joyce Xu. All rights reserved.
//

#ifndef transform_h
#define transform_h

#include <string>
#include <vector>

/* useful methods */
std::vector<std::string> split(const std::string &str, char delim);
void desensitize(std::string &str);
void cleanDirEntry(std::string &dir);
bool only_whitespace(const std::string& str);

std::vector<std::string> getTokens(std::string query);
int countChar(std::string str, char c);

#endif /* transform_h */
