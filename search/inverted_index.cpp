//
//  inverted_index.cpp
//  FileSearch
//
//  Created by Joyce Xu on 10/27/15.
//  Copyright © 2015 Joyce Xu. All rights reserved.
//

#include <stdio.h>
#include "inverted_index.h"
#include "transform.h"
#include "ranking.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <dirent.h>
#include <map>


using std::ifstream;
using std::unordered_map;
using std::string;
using std::vector;


//prototypes:
bool contains(const vector<string> &tokens, const string &word);
void intersectRecords(unordered_map<string, vector<InvertedIndex::Match> > &allDocs,
                      const vector<string> &tokens);


const vector<string> &InvertedIndex::getFileList() {
  return this->fileList;
}

string InvertedIndex::getBaseDir() {
  return this->baseDir;
}

int InvertedIndex::getCollectionSize() {
  return this->collectionSize;
}

void InvertedIndex::clearIndex() {
  this->index.clear();
  this->fileList.clear();
  this->baseDir = "";
}

/* Add all files in given directory to the index */
void InvertedIndex::addDir(string dir) {
  cleanDirEntry(dir); // strip whitespace, add '/' to end if not there
  DIR *pdir;
  struct dirent *cur;
  if ((pdir = opendir((dir).c_str())) != NULL) {
    while ((cur = readdir(pdir)) != NULL) {
      if (cur->d_name[0] == '.') // ignore dot files
        continue;
      if (cur->d_type != DT_DIR) {
        string newFile = dir+string(cur->d_name);
        // if file not already present, add
        if (std::find(fileList.begin(), fileList.end(), newFile) == fileList.end()) {
          addFile(dir+string(cur->d_name));
        }
      }
      else { // open subdirectory
        string subdir = string(cur->d_name)+"/";
        addDir(dir+subdir);
      }
    }
    closedir(pdir);
    updateRoot(dir); // update shared path of all files (for display purposes)
  } else {
    std::cout << "Error(" << errno << ") opening " << dir << std::endl;
  }
}


/* Update longest common path among all files (for file display purposes) */
void InvertedIndex::updateRoot(string newDir) {
  if (baseDir == "")
    baseDir = newDir;
  else {
    vector<string> baseTokens = split(baseDir, '/');
    vector<string> newTokens = split(newDir, '/');

    size_t m = std::min(baseTokens.size(), newTokens.size());
    for (int i = 0; i < m; i++) {
      if (baseTokens[i] != newTokens[i]) {
        if (i > 0)
          baseDir.erase(baseDir.find(baseTokens[i]));
        else
          baseDir.erase();
      }
    }
  }
}


/* Parse file, transform each word into a Record object to add to index */
void InvertedIndex::addFile(string fileName) {
  string line, word;
  ifstream in;
  in.open(fileName);

  if (in) {
    int charCounter = 0; //count characters in all preceding lines
    int wordCounter = 0;

    while (std::getline(in, line)) {
      std::istringstream fileLine(line);

      //get length of line
      size_t lineLength = fileLine.str().length();

      string wd;
      int posCounter;

      while(fileLine >> wd) {
        desensitize(wd);

        wordCounter++;

        int posInLine = (fileLine.tellg() != -1) ? (int)fileLine.tellg() : (int)lineLength;
        posCounter = charCounter + posInLine - (int)wd.length();
        addRecord(wd, fileName, wordCounter, posCounter);
        posCounter += wd.length()+1;
      }

      charCounter += lineLength;
    }
    fileList.push_back(fileName);
    collectionSize++;
    in.close();
  } else {
    std::cout << "File not found: " << fileName << std::endl;}

}

/* add Record (word, position in file, etc.) to index: index[keyWord] = {records} */
void InvertedIndex::addRecord(string keyWord, string file, int wordPos, int charPos) {
  vector<Record> records = index[keyWord];
  bool added = false;
  vector<int> pos = {wordPos, charPos};
  for (int i = 0; i < records.size(); i++) {
    if (records[i].file == file) {           // if file already in records (i.e. if word has already appeared in file)
      records[i].positions.push_back(pos);   // just add current word to list of positions
      added = true;
    }
  }
  if (added)
    index[keyWord] = records;
  else {
    Record r = {file, {pos}};                // otherwise, create new record with this file & position
    index[keyWord].push_back(r);
  }
}


/* Get matching documents for search term:
 first check if any exact text match is necessary (terms surrounded by quotes) and fetch corresponding docs -> getExactMatch(phrase),
 then add all documents that match the single-word and free-text (non-exact match) search terms -> getAllDocs(terms) */
unordered_map<string, vector<InvertedIndex::Match>> InvertedIndex::getDocuments(string terms) {
  unordered_map<string, vector<Match>> documents;

  std::size_t start = terms.find_first_of('"');
  std::size_t end = terms.find_last_of('"');
  if (start != string::npos && end != string::npos && start != end) {

    //get term without quotes
    string phrase = terms.substr(start+1, end-start-1);
    terms.erase(start, phrase.length()+2);

    documents = getExactMatch(phrase);
  }

  /* for single-word queries and free-text queries: {file, { {word1, pos1}, {word2, pos2} } */
  unordered_map<string, vector<Match>> allMatches = getAllDocs(terms);

  for (auto fm : allMatches) {
    if (documents.find(fm.first) == documents.end()) { //file not already in collection
      documents[fm.first] = fm.second;
    } else {
      vector<Match> otherMatches = fm.second;
      documents[fm.first].insert(documents[fm.first].end(), otherMatches.begin(), otherMatches.end());
    }
  }
  return documents;
}

/* Fetches all docs that contain any search tokens (words) */
unordered_map<string, vector<InvertedIndex::Match>> InvertedIndex::getAllDocs(string words) {

  // get records for each word, builds map of Match structures:
  // documents[file].push_back({keyWord, positions})

  unordered_map<string, vector<Match>> documents;
  vector<string> tokens = split(words, ' ');

  for (auto it = tokens.begin(); it != tokens.end(); it++) {
    desensitize(*it);
    if (only_whitespace(*it))
      continue;
    vector<Record> records = index[*it];
    for (Record r : records) {
      Match m;
      m.keyWord = *it;
      m.positions = r.positions;
      documents[r.file].push_back(m);
    }
  }
  return documents;
}

/* fetches documents that have an exact match to the searched phrase */
unordered_map<string, vector<InvertedIndex::Match>> InvertedIndex::getExactMatch(string phrase) {
  unordered_map<string, vector<Match>> documents = getAllDocs(phrase);
  unordered_map<string, vector<Match>> results;

  vector<string> tokens = split(phrase, ' ');
  intersectRecords(documents, tokens);


  unordered_map<string, int> posInPhrase;
  for (unsigned int i = 0; i < tokens.size(); i++) {
    string word = tokens[i];
    desensitize(word);
    posInPhrase[word] = i;
  }

  for (auto doc : documents) {
    string file = doc.first;
    vector<Match> matches = doc.second;
    vector<vector<int>> fullMatches; //list of {wordPos, charPos} pairs

    vector<int> startPos;
    std::map<int, int> counter;
    vector<int> phraseLoc;

    for (int i = 0; i < matches.size(); i++) { //for all {word, pos} in file
      Match m = matches[i];
      for (int j = 0; j < m.positions.size(); j++) {

        // if first word in phrase, just add the position(s)
        if (posInPhrase[m.keyWord] == 0) {
          startPos.push_back(m.positions[j][0]);
        }
        // else, subtract position in phrase from position in file to get supposed beginning of phrase
        else {
          startPos.push_back(m.positions[j][0]-posInPhrase[m.keyWord]);
        }
      }
    }


    // find beginning position of all matching phrases
    for (int p : startPos) {
      counter[p] = (counter.find(p) != counter.end()) ? counter[p]+1 : 1;
      if (counter[p] == tokens.size()) {
        phraseLoc.push_back(p);
      }
    }

    // get the starting word of phrase for reference
    Match startWordMatch;
    for (int i = 0; i < matches.size(); i++) {
      if (posInPhrase[matches[i].keyWord] == 0) {
        startWordMatch = matches[i];
      }
    }

    // find position of starting word
    for (int p : phraseLoc) {
      for (vector<int> position : startWordMatch.positions) {
        if (position[0] == p) {
          //full match
          int wordPos = (int)position[0];
          int charPos = (int)position[1];
          vector<int> pos = {wordPos, charPos};
          fullMatches.push_back(pos);
        }
      }
    }

    // create a new match: {phrase, {startWord position} }
    if (!fullMatches.empty()) {
      Match m;
      m.keyWord = phrase;
      for (vector<int> pos : fullMatches) {
        m.positions.push_back(pos);
      }
      results[file] = {m};
    }
  }

  return results;
}


/* Erase all documents that don't contain all search terms ( used in getExactMatch() ) */
void intersectRecords(unordered_map<string, vector<InvertedIndex::Match>> &allDocs,
                      const vector<string> &tokens) {
  for (auto it = allDocs.cbegin(); it != allDocs.cend(); ) {
    int count = 0;
    vector<InvertedIndex::Match> matches = it->second;
    for (int i = 0; i < matches.size(); i++) {
      string keyWord = matches[i].keyWord;
      if (contains(tokens, keyWord)) {
        count++;
      }
    }
    if (count != tokens.size())
      allDocs.erase(it++);
    else
      ++it;
  }
}


bool contains(const vector<string> &tokens, const string &word) {
  return std::find(tokens.begin(), tokens.end(), word) != tokens.end();
}

/* calls getDocuments() to fetch documents from search terms, constructs & returns a vector of Result structs
 (contains text preview for display) */
vector<InvertedIndex::Result> InvertedIndex::query(string query) { //changed from pointer

  unordered_map<string, vector<Match>> documents = getDocuments(query); // (file, {matchs})
  vector<string> ordered = tf_idf(query, documents, collectionSize, index);

  vector<Result> results;

  if (!documents.empty()) {
    for (string file : ordered) { //auto doc : documents
      Result res;

      vector<Match> matches = documents[file];
      string dispFile = file.substr(baseDir.length());
      vector<std::pair<string, string>> previews;

      for (Match m : matches) {
        for (int i = 0; i < m.positions.size(); i++) {
          // positions[i][1] maps to character position in file
          std::pair<string, string> wordPrev = make_pair(m.keyWord, getPreview(file, m.positions[i][1], m.keyWord));
          previews.push_back(wordPrev);
        }
      }

      res.fileName = dispFile;
      res.filePath = file;
      res.previews = previews;
      results.push_back(res);
    }
  }
  return results;
}

/* uses position of word in file to get text preview, e.g. if search term was "blue", could return "...the sky was blue and cloudless..." */
string InvertedIndex::getPreview(string file, int pos, string word) {
  ifstream is (file);

  if (is.is_open()) {
    int EXTRA = 15;
    size_t SIZE = EXTRA*2+word.length();
    char *text = new char[SIZE];
    int posCount = 0;
    int prepos = (pos >= EXTRA) ? EXTRA : pos; // back up position of stream to EXTRA characters
                                                     // (or as much as possible)
    is.seekg(pos - prepos);
    while (!is.eof() && posCount < SIZE) {
      char c = is.get();
      if (c != '\n') {
        text[posCount] = c;
        posCount++;
      }
      else {
        text[posCount] = ' ';
        posCount++;
      }
    }
    if (is.eof())
      text[posCount-1] = '\0';
    else
      text[posCount] = '\0';

    string preview = string(text);
    string preview_reg = preview; // lowercased, de-sensitized version
    desensitize(preview_reg);

    // If word is not first word in preview, chop off first word (in case incomplete)
    if (preview_reg.find(word) != 0) {
      std::size_t firstSpace = preview.find(' ');
      preview = preview.substr(firstSpace+1);
    }
    // if word not last, chop off last word
    if (preview_reg.rfind(word) != preview_reg.length()-word.length()) {
      std::size_t lastSpace = preview.rfind(' ');
      preview = preview.substr(0, lastSpace);
    }

    return preview;
  } else
    return "Error fetching preview!";
}


// testing
void checkFileList(InvertedIndex ind) {
  vector<string> files = ind.getFileList();
  for (int i = 0; i < files.size(); i++)
    std::cout << files[i] << std::endl;
}

