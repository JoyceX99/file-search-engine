//
//  ranking.cpp
//  FileSearch
//
//  Created by Joyce Xu on 11/26/15.
//  Copyright © 2015 Joyce Xu. All rights reserved.
//

#include "ranking.h"
#include "transform.h"
#include <iostream>
#include <map>
#include <math.h>
#include <numeric>
#include <functional>

using std::map;

/* operator to sort scores */
struct sort_scores {
  bool operator()(const std::pair<string, double> &left, const std::pair<string, double> &right) {
    return left.second > right.second;
  }
};


/* term frequency-inverse document frequency (IR algorithm): score document importance through dot product of:
      term frequency (how often the term appears in the document)
      inverse document frequency (how infrequently does the term occur across the entire collection of documents
   returns ordered list of documents passed in, sorted by tf-idf score */
vector<string> tf_idf(string query, const unordered_map<string, vector<InvertedIndex::Match>> &documents, int collectionSize,
            const unordered_map<string, vector<InvertedIndex::Record> > &index) {
  vector<string> tokens = getTokens(query);
  vector<std::pair<string, double>> docScores;

  vector<double> idf = invDocFreq(tokens, documents, collectionSize, index);
//  for (auto it = idf.begin(); it != idf.end(); it++) {
//    std::cout << *it << "; ";
//  }
//
//  std::cout << "\n\n";
  for (auto docPair: documents) {
    string file = docPair.first;
//    std::cout << file << "\n";
    vector<int> tf = termFreq(tokens, docPair.second);
    double score = std::inner_product(idf.begin(), idf.end(), tf.begin(), 0.0);
    docScores.push_back(std::make_pair(file, score));

//    for (auto it = tf.begin(); it != tf.end(); it++) {
//      std::cout << *it << "; ";
//    }
//    std::cout << "\n\n";
  }

  //sort documents by score
  std::sort(docScores.begin(), docScores.end(), sort_scores());

//  std::cout << "DOCUMENT SCORES: " << "\n";
//  for (auto ds : docScores) {
//    std::cout << ds.first << ": " << ds.second << "\n";
//  }

  vector<string> orderedFiles;
  for (auto d : docScores) {
    orderedFiles.push_back(d.first);
  }
  return orderedFiles;
}

/* Obtain term frequency (how often word appears in doc) */
vector<int> termFreq(vector<string> terms, const vector<InvertedIndex::Match> &matches) {
  vector<int> frequencies;
  map<string, int> termFreq; //(word, freq)

  for (int i = 0; i < matches.size(); i++) { //for each file
    int numAppearances = (int)matches[i].positions.size();
    termFreq[matches[i].keyWord] += numAppearances;
  }

//  std::cout << "TERM FREQUENCIES: " << "\n";
  for (int i = 0; i < terms.size(); i++) {
    string term = terms[i];
    frequencies.push_back(termFreq[term]);
//    std::cout << term << ": " << termFreq[term] << "\n";
  }

  return frequencies;
}


/* Obtain inverse doc frequency: log(N/df), N being total number of documents in collection, and df being number of docs the term occus in */
vector<double> invDocFreq(vector<string> terms, const unordered_map<string, vector<InvertedIndex::Match>> &documents,
                       int collectionSize, const unordered_map<string, vector<InvertedIndex::Record> > &index) {
  vector<double> idfs;
  map<string, int> docsContaining; //(term, numDocsContaining)

  //for each term, count number of appearances across all documents
  for (string term : terms) {
    auto it = index.find(term);
    if (it != index.end()) { //single word term, just count occurences in index
      vector<InvertedIndex::Record> docs = it->second;
      docsContaining[term] = (int)docs.size();
    } else { //phrase term: look in documents
      for (auto kv : documents) {
        vector<InvertedIndex::Match> matches = kv.second;
        for (InvertedIndex::Match m : matches) {
          if (m.keyWord == term) {
            docsContaining[term]++;
            break;
          }
        }
      }
    }
  }

  double n = collectionSize;
  for (string term : terms) {
    int nf = docsContaining[term];
    double idf = log10(n/nf);
    idfs.push_back(idf);

//    std::cout << term << ": " << idf << "\n";
  }
  return idfs;
}
