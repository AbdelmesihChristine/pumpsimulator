#ifndef HISTORYRECORD_H
#define HISTORYRECORD_H

#include <QString>

enum class RecordType {
    ManualBolus,
    AutoBolus,
    CgmReading,
    Warning,
    Other
};

class HistoryRecord
{
public:
    HistoryRecord(const QString& time,
                  RecordType type,
                  double amount,
                  const QString& notes)
        : timestamp(time),
          recordType(type),
          insulinAmount(amount),
          recordNotes(notes)
    {}

    QString getTimestamp() const { return timestamp; }
    RecordType getRecordType() const { return recordType; }
    double getInsulinAmount() const { return insulinAmount; }
    QString getNotes() const { return recordNotes; }

private:
    QString timestamp;
    RecordType recordType;
    double insulinAmount;
    QString recordNotes;
};

#endif
