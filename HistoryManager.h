#ifndef HISTORYMANAGER_H
#define HISTORYMANAGER_H

#include <vector>
#include "HistoryRecord.h"

class HistoryManager
{
public:
    void addRecord(const HistoryRecord& record);
    const std::vector<HistoryRecord>& getRecords() const;
private:
    std::vector<HistoryRecord> history;
};

#endif // HISTORYMANAGER_H
