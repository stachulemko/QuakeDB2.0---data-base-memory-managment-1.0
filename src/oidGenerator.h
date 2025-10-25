#ifndef OIDGENERATOR_H
#define OIDGENERATOR_H

#include <iostream>
#include <cstdint>


oidGenerator& oid = oidGenerator::getInstance();

class oidGenerator {
private:
    static int32_t currentOid;
    
public:
    oidGenerator();
    ~oidGenerator();
    
    static int32_t getNextOid();
    static int32_t getCurrentOid();
    
    void saveToFile();
    void loadFromFile();

    static oidGenerator& getInstance();
};



#endif