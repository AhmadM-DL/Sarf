#ifndef DBITVEC_H_
#define DBITVEC_H_
/**
  * @file   dbitvec.h
  * @date   mar 3, 2010
  * @author Dr. Fadi Zaraket
  * @brief  provides a bitvector with [] access and dynamic size
  */

#include <string.h> // for memset
#include <QDataStream>
#include <cstring>
#include <assert.h>
#include <QDebug>

/**
  * @class  dbitvec
  * @author Dr. Fadi Zaraket
  * @brief  This is a user defined dynamic bit vector with '[]' access operator
  */
class dbitvec {
        unsigned int size;  //!< size of this vector
        unsigned int bytes; //!< bytes
        unsigned char *data;    //!< pointer to data

        friend QDataStream &operator<<(QDataStream &out, const dbitvec &d); //!< overload '<<' operator function declaration
        friend QDataStream &operator>>(QDataStream &in, dbitvec &d);    //!< overload '>>' operator function declaration
    public:
        /**
          * @brief  define a type called exception_em of enum type for exceptions
          */
        typedef enum {
            NONE, MEM_EXCPT, BIT_BOUND_EXCPT
        } exception_em;

        dbitvec(): size(0), bytes(0), data(0) { }   //!< This is the definition of a default constructor of the class
        void resize(unsigned int length);   //!< function defined to resize the bit vector
        dbitvec(unsigned int length): size(length) {
            bytes = (size + 7) >> 3; // (ceiling(size/8))
            data = new unsigned char [bytes];

            if (data == NULL) {
                throw MEM_EXCPT;
            }

            reset();
        }
        dbitvec(const dbitvec &v1): size(v1.length()) {
            bytes = v1.getNumBytes();
            data = new unsigned char [bytes];

            if (data == NULL) {
                throw MEM_EXCPT;
            }

            memcpy(data, v1._data(), bytes);
        }
        ~dbitvec()  {
            delete [] data;
        }
        unsigned char *_data() const {
            return data;
        }
        unsigned int getNumBytes() const {
            return bytes;
        }
        unsigned int length() const {
            return size;
        }
        void reset() {
            memset(data, 0, bytes);
        }
        //TODO: resize, copy constructor
        bool getBit(unsigned int i) const {
            if (i >= size) {
                throw BIT_BOUND_EXCPT;
            }

            unsigned int byte = i >> 3; // i / 8
            unsigned int bit = i & 0x7; // i % 8
            unsigned char mask = 1 << bit;
            return (data[byte] & mask) != 0;
        }
        void setBit(unsigned int i, bool b = true) const {
            if (i >= size) {
                throw BIT_BOUND_EXCPT;
            }

            unsigned int byte = i >> 3; // i / 8
            unsigned int bit = i & 0x7; // i % 8
            unsigned char mask = 1 << bit;
            data [byte] = (b) ?
                          (data[byte] | mask)  :
                          (data[byte] & (~mask));
        }
        bool NothingSet();

        class Byte {
            public:
                unsigned int bit;//offset within the byte
                dbitvec *vec; // pointer to vector
                Byte(unsigned int i, dbitvec *v) : bit(i),  vec(v) {}
                operator bool () {
                    return vec->getBit(bit);
                }
                Byte &operator = (bool b) {
                    vec->setBit(bit, b);
                    return *this;
                }
        };
        Byte operator [](unsigned int bit) {
            return Byte(bit, this);
        }
        dbitvec &operator=(const dbitvec &v1) {
            if (v1.getNumBytes() != getNumBytes()) {
                resize(v1.size);
            }

            memcpy(data, v1._data(), bytes);
            return *this;
        }
        void show();
};
inline bool operator == (const dbitvec &d1, const dbitvec &d2) { //by Jad, TODO: check if could be made faster
    unsigned int bytes1 = d1.getNumBytes();

    if (bytes1 != d2.getNumBytes()) {
        return false;
    }

    return (memcmp(d1._data(), d2._data(), bytes1) == 0);
}
inline bool operator != (const dbitvec &d1, const dbitvec &d2) {
    return !(d1 == d2);
}

inline QDataStream &operator<<(QDataStream &out, const dbitvec &d) {
    out     << (const quint32)d.bytes
            << (const quint32)d.size;
    unsigned int *dump = (unsigned int *) d.data;
    unsigned int l = (d.bytes + 3) / 4;

    for (unsigned int i = 0; i < l; i++) {
        out << dump[i];
    }

    return out;
}

inline QDataStream &operator>>(QDataStream &in, dbitvec &d) {
    quint32 bytes, size;
    QString s;
    in >> bytes;
    in >> size;
    d.resize(bytes * 4 * 8);
    d.bytes = bytes;
    d.size = size;
    unsigned int *dump = (unsigned int *) d.data;
    assert(d.data != NULL);
    unsigned l = (bytes + 3) / 4;

    for (unsigned int i = 0; i < l; i++) {
        in >> dump[i];
    }

    return in;
}
#endif
