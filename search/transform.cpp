//
//  transform.cpp
//  FileSearch
//
//  Created by Joyce Xu on 11/26/15.
//  Copyright © 2015 Joyce Xu. All rights reserved.
//

#include <stdio.h>
#include "transform.h"
#include <sstream>

using std::vector;
using std::string;


/* split string based on delimeter */
vector<string> split(const string &str, char delim) {
  vector<string> splits;
  std::stringstream ss(str);
  string item;
  while(std::getline(ss, item, delim)) {
    if (!item.empty())
      splits.push_back(item);
  }
  return splits;
}

/* transform to lowercase, strip punctuation except apostrophes */
void desensitize(string &str) {
  for(size_t i = 0; i<str.length(); i++) {
    if(ispunct(str[i]) && str[i] != '\'')
      str.erase(i, 1);
  }
  std::transform(str.begin(), str.end(), str.begin(), ::tolower); //note: does not work for non-ASCII
}

/* check if only contains blanks */
bool only_whitespace(const string& str) {
  return str.find_first_not_of(' ') == str.npos;
}

/* strip whitespace, add '/' to end of directory name if not there*/
void cleanDirEntry(string &dir) {
  dir.erase(remove_if(dir.begin(), dir.end(), isspace), dir.end());
  if (dir.back() != '/')
    dir += '/';
}

/* get tokens: words and phrases surrounded by quotes */
vector<string> getTokens(string query) {
  vector<string> parts = split(query, '"');
  vector<string> tokens;

  //if no quotations or odd number of quotations => no phrase queries, just split words
  if (countChar(query, '"') == 0 || countChar(query, '"')%2 == 1) {
    for (string part : parts) {
      vector<string> partTokens = split(part, ' ');
      tokens.insert(tokens.end(), partTokens.begin(), partTokens.end());
    }
    return tokens;
  }

  int phrasePos;
  if (query.find_first_of('"') == 0)
    phrasePos = 0;
  else
    phrasePos = 1;
  for (int i = 0; i < parts.size(); i++) {
    if (i%2 == phrasePos) {
      tokens.push_back(parts[i]);
    } else {
      vector<string> partTokens = split(parts[i], ' ');
      tokens.insert(tokens.end(), partTokens.begin(), partTokens.end());
    }
  }
  return tokens;
}

/* count number of occurences of character in string */
int countChar(string str, char c) {
  int count = 0;
  size_t charPos = str.find(c);
  while(charPos != string::npos) {
    count++;
    charPos = str.find(c, charPos+1);
  }
  return count;
}
