//
// Generated file, do not edit! Created by opp_msgtool 6.0 from MyMsg.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include <memory>
#include <type_traits>
#include "MyMsg_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp

Register_Class(MyMsg)

MyMsg::MyMsg(const char *name, short kind) : ::omnetpp::cPacket(name, kind)
{
}

MyMsg::MyMsg(const MyMsg& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

MyMsg::~MyMsg()
{
    delete [] this->myMsg;
}

MyMsg& MyMsg::operator=(const MyMsg& other)
{
    if (this == &other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void MyMsg::copy(const MyMsg& other)
{
    delete [] this->myMsg;
    this->myMsg = (other.myMsg_arraysize==0) ? nullptr : new int[other.myMsg_arraysize];
    myMsg_arraysize = other.myMsg_arraysize;
    for (size_t i = 0; i < myMsg_arraysize; i++) {
        this->myMsg[i] = other.myMsg[i];
    }
}

void MyMsg::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    b->pack(myMsg_arraysize);
    doParsimArrayPacking(b,this->myMsg,myMsg_arraysize);
}

void MyMsg::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    delete [] this->myMsg;
    b->unpack(myMsg_arraysize);
    if (myMsg_arraysize == 0) {
        this->myMsg = nullptr;
    } else {
        this->myMsg = new int[myMsg_arraysize];
        doParsimArrayUnpacking(b,this->myMsg,myMsg_arraysize);
    }
}

size_t MyMsg::getMyMsgArraySize() const
{
    return myMsg_arraysize;
}

int MyMsg::getMyMsg(size_t k) const
{
    if (k >= myMsg_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)myMsg_arraysize, (unsigned long)k);
    return this->myMsg[k];
}

void MyMsg::setMyMsgArraySize(size_t newSize)
{
    int *myMsg2 = (newSize==0) ? nullptr : new int[newSize];
    size_t minSize = myMsg_arraysize < newSize ? myMsg_arraysize : newSize;
    for (size_t i = 0; i < minSize; i++)
        myMsg2[i] = this->myMsg[i];
    for (size_t i = minSize; i < newSize; i++)
        myMsg2[i] = 0;
    delete [] this->myMsg;
    this->myMsg = myMsg2;
    myMsg_arraysize = newSize;
}

void MyMsg::setMyMsg(size_t k, int myMsg)
{
    if (k >= myMsg_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)myMsg_arraysize, (unsigned long)k);
    this->myMsg[k] = myMsg;
}

void MyMsg::insertMyMsg(size_t k, int myMsg)
{
    if (k > myMsg_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)myMsg_arraysize, (unsigned long)k);
    size_t newSize = myMsg_arraysize + 1;
    int *myMsg2 = new int[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        myMsg2[i] = this->myMsg[i];
    myMsg2[k] = myMsg;
    for (i = k + 1; i < newSize; i++)
        myMsg2[i] = this->myMsg[i-1];
    delete [] this->myMsg;
    this->myMsg = myMsg2;
    myMsg_arraysize = newSize;
}

void MyMsg::appendMyMsg(int myMsg)
{
    insertMyMsg(myMsg_arraysize, myMsg);
}

void MyMsg::eraseMyMsg(size_t k)
{
    if (k >= myMsg_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)myMsg_arraysize, (unsigned long)k);
    size_t newSize = myMsg_arraysize - 1;
    int *myMsg2 = (newSize == 0) ? nullptr : new int[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        myMsg2[i] = this->myMsg[i];
    for (i = k; i < newSize; i++)
        myMsg2[i] = this->myMsg[i+1];
    delete [] this->myMsg;
    this->myMsg = myMsg2;
    myMsg_arraysize = newSize;
}

class MyMsgDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_myMsg,
    };
  public:
    MyMsgDescriptor();
    virtual ~MyMsgDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(MyMsgDescriptor)

MyMsgDescriptor::MyMsgDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(MyMsg)), "omnetpp::cPacket")
{
    propertyNames = nullptr;
}

MyMsgDescriptor::~MyMsgDescriptor()
{
    delete[] propertyNames;
}

bool MyMsgDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<MyMsg *>(obj)!=nullptr;
}

const char **MyMsgDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *MyMsgDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int MyMsgDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 1+base->getFieldCount() : 1;
}

unsigned int MyMsgDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISARRAY | FD_ISEDITABLE | FD_ISRESIZABLE,    // FIELD_myMsg
    };
    return (field >= 0 && field < 1) ? fieldTypeFlags[field] : 0;
}

const char *MyMsgDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "myMsg",
    };
    return (field >= 0 && field < 1) ? fieldNames[field] : nullptr;
}

int MyMsgDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "myMsg") == 0) return baseIndex + 0;
    return base ? base->findField(fieldName) : -1;
}

const char *MyMsgDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_myMsg
    };
    return (field >= 0 && field < 1) ? fieldTypeStrings[field] : nullptr;
}

const char **MyMsgDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *MyMsgDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int MyMsgDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    MyMsg *pp = omnetpp::fromAnyPtr<MyMsg>(object); (void)pp;
    switch (field) {
        case FIELD_myMsg: return pp->getMyMsgArraySize();
        default: return 0;
    }
}

void MyMsgDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    MyMsg *pp = omnetpp::fromAnyPtr<MyMsg>(object); (void)pp;
    switch (field) {
        case FIELD_myMsg: pp->setMyMsgArraySize(size); break;
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'MyMsg'", field);
    }
}

const char *MyMsgDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    MyMsg *pp = omnetpp::fromAnyPtr<MyMsg>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string MyMsgDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    MyMsg *pp = omnetpp::fromAnyPtr<MyMsg>(object); (void)pp;
    switch (field) {
        case FIELD_myMsg: return long2string(pp->getMyMsg(i));
        default: return "";
    }
}

void MyMsgDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    MyMsg *pp = omnetpp::fromAnyPtr<MyMsg>(object); (void)pp;
    switch (field) {
        case FIELD_myMsg: pp->setMyMsg(i,string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'MyMsg'", field);
    }
}

omnetpp::cValue MyMsgDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    MyMsg *pp = omnetpp::fromAnyPtr<MyMsg>(object); (void)pp;
    switch (field) {
        case FIELD_myMsg: return pp->getMyMsg(i);
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'MyMsg' as cValue -- field index out of range?", field);
    }
}

void MyMsgDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    MyMsg *pp = omnetpp::fromAnyPtr<MyMsg>(object); (void)pp;
    switch (field) {
        case FIELD_myMsg: pp->setMyMsg(i,omnetpp::checked_int_cast<int>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'MyMsg'", field);
    }
}

const char *MyMsgDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr MyMsgDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    MyMsg *pp = omnetpp::fromAnyPtr<MyMsg>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void MyMsgDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    MyMsg *pp = omnetpp::fromAnyPtr<MyMsg>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'MyMsg'", field);
    }
}

namespace omnetpp {

}  // namespace omnetpp

