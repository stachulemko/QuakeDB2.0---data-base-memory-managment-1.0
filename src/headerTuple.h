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

        std::bitset<280> marshallHeaderTuple(int64_t t_xmin, int64_t t_xmax, int32_t t_cid, 
                                           int32_t t_infomask, int16_t t_hoff, 
                                           bool null_bitmap, int64_t optional_oid) {
            std::bitset<280> tupleHeader;
            
            std::vector<uint8_t>* txMin = marshalInt64_t(t_xmin);
            std::vector<uint8_t>* txMax = marshalInt64_t(t_xmax);
            std::vector<uint8_t>* tCid = marshalInt32_t(t_cid);
            std::vector<uint8_t>* tInfomask = marshalInt32_t(t_infomask);
            std::vector<uint8_t>* tHoff = marshalInt16_t(t_hoff);
            std::vector<uint8_t>* nullBitmap = marshalBool(null_bitmap);
            std::vector<uint8_t>* optionalOid = marshalInt64_t(optional_oid);

            size_t bit_offset = 0;

            auto copyBytesToBitset = [&](std::vector<uint8_t>* vec, int maxBits) {
                for (size_t i = 0; i < vec->size() && bit_offset < 280 && maxBits > 0; ++i) {
                    for (int bit = 0; bit < 8 && bit_offset < 280 && maxBits > 0; ++bit, --maxBits) {
                        if ((*vec)[i] & (1 << bit)) {
                            tupleHeader.set(bit_offset);
                        }
                        bit_offset++;
                    }
                }
            };
            
            copyBytesToBitset(txMin, 64);      // t_xmin: 64 bity
            copyBytesToBitset(txMax, 64);      // t_xmax: 64 bity
            copyBytesToBitset(tCid, 32);       // t_cid: 32 bity
            copyBytesToBitset(tInfomask, 32);  // t_infomask: 32 bity
            copyBytesToBitset(tHoff, 16);      // t_hoff: 16 bitów
            copyBytesToBitset(nullBitmap, 1);  // null_bitmap: 1 bit
            copyBytesToBitset(optionalOid, 64); // optional_oid: 64 bity

            delete txMin;
            delete txMax;
            delete tCid;
            delete tInfomask;
            delete tHoff;
            delete nullBitmap;
            delete optionalOid;

            return tupleHeader;
        }
        
        void unmarshallHeaderTuple(const std::bitset<280>& tupleHeader) {
            size_t offset = 0;
            
            std::vector<uint8_t>* txMinBytes = bitsetToVector(tupleHeader, offset, 64);
            UnmarshalInt64_t(&t_xmin, txMinBytes);
            delete txMinBytes;
            
            std::vector<uint8_t>* txMaxBytes = bitsetToVector(tupleHeader, offset, 64);
            UnmarshalInt64_t(&t_xmax, txMaxBytes);
            delete txMaxBytes;
            
            std::vector<uint8_t>* tCidBytes = bitsetToVector(tupleHeader, offset, 32);
            UnmarshalInt32_t(&t_cid, tCidBytes);
            delete tCidBytes;
            
            std::vector<uint8_t>* tInfomaskBytes = bitsetToVector(tupleHeader, offset, 32);
            UnmarshalInt32_t(&t_infomask, tInfomaskBytes);
            delete tInfomaskBytes;
            
            std::vector<uint8_t>* tHoffBytes = bitsetToVector(tupleHeader, offset, 16);
            UnmarshalInt16_t(&t_hoff, tHoffBytes);
            delete tHoffBytes;
            
            std::vector<uint8_t>* nullBitmapBytes = bitsetToVector(tupleHeader, offset, 1);
            UnmarshalBool(&null_bitmap, nullBitmapBytes);
            delete nullBitmapBytes;
            
            std::vector<uint8_t>* optionalOidBytes = bitsetToVector(tupleHeader, offset, 64);
            UnmarshalInt64_t(&optional_oid, optionalOidBytes);
            delete optionalOidBytes;
        }

    private:
        std::vector<uint8_t>* bitsetToVector(const std::bitset<280>& bitset, size_t& offset, int bits) {
            std::vector<uint8_t>* result = new std::vector<uint8_t>();
            
            int bytes = (bits + 7) / 8; 
            result->resize(bytes);
            
            for (int byte = 0; byte < bytes && offset < 280; ++byte) {
                uint8_t value = 0;
                for (int bit = 0; bit < 8 && offset < 280 && bits > 0; ++bit, --bits) {
                    if (bitset.test(offset)) {
                        value |= (1 << bit);
                    }
                    offset++;
                }
                (*result)[byte] = value;
            }
            
            return result;
        }
};

#endif