//
//  ranking.h
//  FileSearch
//
//  Created by Joyce Xu on 11/26/15.
//  Copyright © 2015 Joyce Xu. All rights reserved.
//

#ifndef ranking_h
#define ranking_h

#include "inverted_index.h"

using std::unordered_map;
using std::string;
using std::vector;

vector<string> tf_idf(string query, const unordered_map<string, vector<InvertedIndex::Match>> &documents, int collectionSize,
            const unordered_map<string, vector<InvertedIndex::Record> > &index);

vector<int> termFreq(vector<string> terms, const vector<InvertedIndex::Match> &matches);
vector<double> invDocFreq(vector<string> terms, const unordered_map<string, vector<InvertedIndex::Match>> &documents,
                       int collectionSize, const unordered_map<string, vector<InvertedIndex::Record> > &index);
#endif /* ranking_h */
