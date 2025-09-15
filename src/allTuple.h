#ifndef ALLTUPLE_H
#define ALLTUPLE_H

#include <iostream>
#include <vector>
#include "headerTuple.h"
#include "dataNullBitMapTuple.h"
#include "typeManagerAllVarsTypes.h"


class tuple {
    private:
        HeaderTuple header;
        DataNullBitMapTuple dataNullBitMap;
    public:
        tuple();

        ~tuple();

        std::vector<uint8_t> marshallTuple(int64_t xmin, int64_t xmax, int32_t cid, int32_t infomask, int16_t hoff, bool bitmap, int64_t oid, std::vector<bool>bitMap,std::vector<allVars>data);

        void unmarshallTuple(const std::vector<uint8_t>& data) ;

        int64_t getTxMin() const { return header.getTxMin(); }
        int64_t getTxMax() const { return header.getTxMax(); }
        int32_t getTCid() const { return header.getTCid(); }
        int32_t getTInfomask() const { return header.getTInfomask(); }
        int16_t getTHoff() const { return header.getTHoff(); }
        bool getNullBitmap() const { return header.getNullBitmap(); }
        int64_t getOptionalOid() const { return header.getOptionalOid(); }

        std::vector<bool> getBitMap() const { return dataNullBitMap.getBitMap(); }
        std::vector<allVars> getData() const { return dataNullBitMap.getData(); }

        
};









#endif