#ifndef HEADERTUPLE_H
#define HEADERTUPLE_H

#include "typesDataConverter.h"
#include <iostream>
#include <bitset>
#include <vector>
#include <cstdint>

/*

+-----------------------+
| t_xmin                | <-- id of transaction which created 
+-----------------------+
| t_xmax                | <-- id of transaction which delted (optional)
+-----------------------+
| t_cid / t_infomask2   | <-- id instruction  
+-----------------------+
| t_infomask            | <-- NULL , TOAST
+-----------------------+
| t_hoff                | <-- num where clear data start
+-----------------------+
| Null bitmap           | <-- depend on the number of rows 
+-----------------------+
| Optional OID          | <-- OID indetification optional 
+-----------------------+
| Dane kolumn (data)    | <-- data 
+-----------------------+
| free space            | <-- free space - optional with large number of updates 
+-----------------------+
*/

class HeaderTuple {
    private:
        int64_t t_xmin;
        int64_t t_xmax;
        int32_t t_cid;
        int32_t t_infomask;
        int16_t t_hoff;
        bool null_bitmap;
        int64_t optional_oid;
        
    public:
        HeaderTuple() {
            t_xmin = 0;
            t_xmax = 0;
            t_cid = 0;
            t_infomask = 0;
            t_hoff = 0;
            null_bitmap = false;
            optional_oid = 0;
        }
        
        HeaderTuple(int64_t xmin, int64_t xmax, int32_t cid, int32_t infomask, 
                   int16_t hoff, bool bitmap, int64_t oid) 
            : t_xmin(xmin), t_xmax(xmax), t_cid(cid), t_infomask(infomask), 
              t_hoff(hoff), null_bitmap(bitmap), optional_oid(oid) {}
        
        ~HeaderTuple() {}

        int64_t getTxMin() const { return t_xmin; }
        int64_t getTxMax() const { return t_xmax; }
        int32_t getTCid() const { return t_cid; }
        int32_t getTInfomask() const { return t_infomask; }
        int16_t getTHoff() const { return t_hoff; }
        bool getNullBitmap() const { return null_bitmap; }
        int64_t getOptionalOid() const { return optional_oid; }

        void setTxMin(int64_t value) { t_xmin = value; }
        void setTxMax(int64_t value) { t_xmax = value; }
        void setTCid(int32_t value) { t_cid = value; }
        void setTInfomask(int32_t value) { t_infomask = value; }
        void setTHoff(int16_t value) { t_hoff = value; }
        void setNullBitmap(bool value) { null_bitmap = value; }
        void setOptionalOid(int64_t value) { optional_oid = value; }


        std::vector<uint8_t> marshallHeaderTupleWithData(){
            return marshallHeaderTuple(t_xmin ,t_xmax,t_cid,t_infomask,t_hoff,null_bitmap,optional_oid);
        }
        std::vector<uint8_t> marshallHeaderTuple(int64_t t_xmin, int64_t t_xmax, int32_t t_cid, 
                                                 int32_t t_infomask, int16_t t_hoff, 
                                                 bool null_bitmap, int64_t optional_oid) {
            std::vector<uint8_t> tupleHeader;
            //tupleHeader.resize(35); 
            
            std::vector<uint8_t>* txMin = marshalInt64_t(t_xmin);
            std::vector<uint8_t>* txMax = marshalInt64_t(t_xmax);
            std::vector<uint8_t>* tCid = marshalInt32_t(t_cid);
            std::vector<uint8_t>* tInfomask = marshalInt32_t(t_infomask);
            std::vector<uint8_t>* tHoff = marshalInt16_t(t_hoff);
            std::vector<uint8_t>* nullBitmap = marshalBool(null_bitmap);
            std::vector<uint8_t>* optionalOid = marshalInt64_t(optional_oid);

            tupleHeader.insert(tupleHeader.end(),txMin->begin(),txMin->end());
            tupleHeader.insert(tupleHeader.end(),txMax->begin(),txMax->end());
            tupleHeader.insert(tupleHeader.end(),tCid->begin(),tCid->end());
            tupleHeader.insert(tupleHeader.end(),tInfomask->begin(),tInfomask->end());
            tupleHeader.insert(tupleHeader.end(),tHoff->begin(),tHoff->end());
            tupleHeader.insert(tupleHeader.end(),nullBitmap->begin(),nullBitmap->end());
            tupleHeader.insert(tupleHeader.end(),optionalOid->begin(),optionalOid->end());
            
            delete txMin;
            delete txMax;
            delete tCid;
            delete tInfomask;
            delete tHoff;
            delete nullBitmap;
            delete optionalOid;

            return tupleHeader;
        }
        
        void unmarshallHeaderTuple(const std::vector<uint8_t>& tupleHeader) {
            if (tupleHeader.size() < 35) {
                t_xmin = 0;
                t_xmax = 0;
                t_cid = 0;
                t_infomask = 0;
                t_hoff = 0;
                null_bitmap = false;
                optional_oid = 0;
                return;
            }
            
            std::vector<uint8_t> txMinVec(tupleHeader.begin(), tupleHeader.begin() + 8);        // int64_t = 8 bytes
            std::vector<uint8_t> txMaxVec(tupleHeader.begin() + 8, tupleHeader.begin() + 16);  // int64_t = 8 bytes
            std::vector<uint8_t> tCidVec(tupleHeader.begin() + 16, tupleHeader.begin() + 20);  // int32_t = 4 bytes
            std::vector<uint8_t> tInfomaskVec(tupleHeader.begin() + 20, tupleHeader.begin() + 24); // int32_t = 4 bytes
            std::vector<uint8_t> tHoffVec(tupleHeader.begin() + 24, tupleHeader.begin() + 26);     // int16_t = 2 bytes
            std::vector<uint8_t> nullBitmapVec(tupleHeader.begin() + 26, tupleHeader.begin() + 27); // bool = 1 byte
            std::vector<uint8_t> optionalOidVec(tupleHeader.begin() + 27, tupleHeader.begin() + 35); // int64_t = 8 bytes


            UnmarshalInt64_t(&t_xmin, &txMinVec);
            UnmarshalInt64_t(&t_xmax, &txMaxVec);
            UnmarshalInt32_t(&t_cid, &tCidVec);
            UnmarshalInt32_t(&t_infomask, &tInfomaskVec);
            UnmarshalInt16_t(&t_hoff, &tHoffVec);
            UnmarshalBool(&null_bitmap, &nullBitmapVec);
            UnmarshalInt64_t(&optional_oid, &optionalOidVec);
        }
};

#endif