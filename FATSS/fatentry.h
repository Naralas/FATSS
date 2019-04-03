#ifndef FATENTRY_H
#define FATENTRY_H

#endif // FATENTRY_H

class FatEntry
{
public:
    FatEntry()
    {
        setVals();
    }

    void setVals(int nextEntry = -1, int state = 0)
    {
        this->nextEntry = nextEntry;
        this->state = state;
    }
    int nextEntry;
    int state;
private:
};
