//
//  inverted_index.h
//  FileSearch
//
//  Created by Joyce Xu on 10/26/15.
//  Copyright © 2015 Joyce Xu. All rights reserved.
//

#ifndef inverted_index_h
#define inverted_index_h

#include <unordered_map>
#include <string>
#include <vector>

class InvertedIndex {
public:

  /* Records stored by word for inverted index */
  typedef struct Record {
    std::string file;
    std::vector< std::vector<int> > positions;    // vector of all {wordPosition, charPosition} in that file
  } Record;

  /* Words & positions matching search term for each file */
  typedef struct Match {
    std::string keyWord;
    std::vector<std::vector<int> > positions;
  } Match;

  /* Returned from query: file name, file path (hidden), preview of matching text */
  typedef struct Result {
    std::string fileName;
    std::string filePath;
    std::vector<std::pair<std::string, std::string> > previews; //vector of (word, preview) pairs;
  } Result;

  void clearIndex();
  void addDir(std::string dir);
  void addFile(std::string fileName);
  void addRecord(std::string keyWord, std::string file, int line, int pos);
  std::vector<Result> query(std::string query);

  const std::vector<std::string> &getFileList();
  const std::vector<std::string, std::vector<InvertedIndex::Record> > &getIndex();
  std::string getBaseDir();
  int getCollectionSize();

private:
  std::unordered_map<std::string, std::vector<Record> > index;
  std::vector<std::string> fileList;
  std::string baseDir;
  int collectionSize;

  void updateRoot(std::string newDir);

  /* Returns all documents (file, {words & positions}) that match phrase depending on type (SWQ, FTQ, PQ) */
  std::unordered_map<std::string, std::vector<Match> > getDocuments(std::string terms);

  /* Handles logic and returns documents for PQ query */
  std::unordered_map<std::string, std::vector<Match> > getExactMatch(std::string phrase);

  /* Builds and returns file map for all documents containing any of the words */
  std::unordered_map<std::string, std::vector<Match> > getAllDocs(std::string words);

  /* Console output of matching records */
  std::string getPreview(std::string file, int pos, std::string word);

};

#endif /* inverted_index_h */
