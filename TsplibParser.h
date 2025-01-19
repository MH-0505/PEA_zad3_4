//
// Created by michal_studia on 19.01.2025.
//

#ifndef PEA_ZAD1_TSPLIBPARSER_H
#define PEA_ZAD1_TSPLIBPARSER_H


#include <map>
#include "AdjMatrix.h"

class TsplibParser {
public:

    static void parseTSPFile(AdjMatrix& graph, const std::string& filename);
};


#endif //PEA_ZAD1_TSPLIBPARSER_H
