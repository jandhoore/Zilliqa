/**
* Copyright (c) 2018 Zilliqa
* This source code is being disclosed to you solely for the purpose of your participation in
* testing Zilliqa. You may view, compile and run the code for that purpose and pursuant to
* the protocols and algorithms that are programmed into, and intended by, the code. You may
* not do anything else with the code without express permission from Zilliqa Research Pte. Ltd.,
* including modifying or publishing the code (or any part of it), and developing or forming
* another public or private blockchain network. This source code is provided ‘as is’ and no
* warranties are given as to title or non-infringement, merchantability or fitness for purpose
* and, to the extent permitted by law, all liability for your use of the code is disclaimed.
* Some programs in this code are governed by the GNU General Public License v3.0 (available at
* https://www.gnu.org/licenses/gpl-3.0.en.html) (‘GPLv3’). The programs that are governed by
* GPLv3.0 are those programs that are located in the folders src/depends and tests/depends
* and which include a reference to GPLv3 in their program files.
**/

#include <fstream>
#include <limits.h>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <vector>
using namespace std;

#ifndef __GetTxnFromFile_H__
#define __GetTxnFromFile_H__

#include "Logger.h"
#include "libData/AccountData/Transaction.h"

unsigned int TXN_SIZE = Transaction::GetMinSerializedSize();
unsigned int NUM_TXN = 100;

bool getTransactionsFromFile(fstream& f, unsigned int startNum,
                             unsigned int totalNum, vector<unsigned char>& vec)
{
    f.seekg((startNum - 1) * TXN_SIZE, ios::beg);
    vec.resize(TXN_SIZE * totalNum);
    for (unsigned int i = 0; i < TXN_SIZE * totalNum; i++)
    {
        if (!f.good())
        {
            LOG_GENERAL(WARNING, "Bad byte accessed");
            return false;
        }
        vec[i] = f.get();
    }
    return true;
}

class GetTxnFromFile
{
public:
    //clears vec
    static bool GetFromFile(Address addr, unsigned int startNum,
                            unsigned int totalNum, vector<unsigned char>& vec)
    {
        fstream file;
        vec.clear();

        if (startNum % NUM_TXN + totalNum - 1 > NUM_TXN)
        {
            LOG_GENERAL(WARNING,
                        "A single file does not hold txns " << startNum << " "
                                                            << totalNum);
            return false;
        }

        unsigned int num = startNum / NUM_TXN;

        const string fileString
            = TXN_PATH + addr.hex() + "_" + to_string(num * NUM_TXN) + ".zil";

        file.open(fileString, ios::binary | ios::in);

        if (!file.is_open())
        {
            LOG_GENERAL(WARNING, "File failed to open " << fileString);
            return false;
        }

        bool b
            = getTransactionsFromFile(file, startNum % NUM_TXN, totalNum, vec);

        file.close();

        return b;
    }
};

#endif