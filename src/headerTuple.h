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
        int32_t size = 39;
        
    public:

        HeaderTuple() ;
        
        HeaderTuple(int64_t xmin, int64_t xmax, int32_t cid, int32_t infomask, int16_t hoff, bool bitmap, int64_t oid) ;

        ~HeaderTuple() ;

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


        std::vector<uint8_t> marshallHeaderTupleWithData();

        std::vector<uint8_t> marshallHeaderTuple(int64_t t_xmin, int64_t t_xmax, int32_t t_cid, 
                                                 int32_t t_infomask, int16_t t_hoff, 
                                                 bool null_bitmap, int64_t optional_oid);
        
        void unmarshallHeaderTuple(const std::vector<uint8_t>& tupleHeader);

        void setData(int64_t xmin, int64_t xmax, int32_t cid, int32_t infomask, int16_t hoff, bool bitmap, int64_t oid){
            this->t_xmin = xmin;
            this->t_xmax = xmax;
            this->t_cid = cid;
            this->t_infomask = infomask;
            this->t_hoff = hoff;
            this->null_bitmap = bitmap;
            this->optional_oid = oid;
        }
        int32_t getSize() const { return size; }

        void showData(){
            std::cout<<"t_xmin: "<<t_xmin<<std::endl;
            std::cout<<"t_xmax: "<<t_xmax<<std::endl;
            std::cout<<"t_cid: "<<t_cid<<std::endl;
            std::cout<<"t_infomask: "<<t_infomask<<std::endl;
            std::cout<<"t_hoff: "<<t_hoff<<std::endl;
            std::cout<<"null_bitmap: "<<null_bitmap<<std::endl;
            std::cout<<"optional_oid: "<<optional_oid<<std::endl;
        }
};

#endif