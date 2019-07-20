#ifndef VALUEHISTORY_VALUEHISTORY_H
#define VALUEHISTORY_VALUEHISTORY_H

class ValueHistory {
public:
    explicit ValueHistory(unsigned int length);
    ~ValueHistory();
    void add(double value);
    double &operator[](unsigned int);
    double average();
    double maximum();
    unsigned int size();
private:
    unsigned int maxLength;
    unsigned int currentLength;
    double *history;

    void shiftLeft();
};

#endif //VALUEHISTORY_VALUEHISTORY_H
