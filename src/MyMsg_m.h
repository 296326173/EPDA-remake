//
// Generated file, do not edit! Created by opp_msgtool 6.0 from MyMsg.msg.
//

#ifndef __MYMSG_M_H
#define __MYMSG_M_H

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#include <omnetpp.h>

// opp_msgtool version check
#define MSGC_VERSION 0x0600
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of opp_msgtool: 'make clean' should help.
#endif

class MyMsg;
/**
 * Class generated from <tt>MyMsg.msg:1</tt> by opp_msgtool.
 * <pre>
 * packet MyMsg
 * {
 *     int myMsg[];		//可变大小的数组
 * }
 * </pre>
 */
class MyMsg : public ::omnetpp::cPacket
{
  protected:
    int *myMsg = nullptr;
    size_t myMsg_arraysize = 0;

  private:
    void copy(const MyMsg& other);

  protected:
    bool operator==(const MyMsg&) = delete;

  public:
    MyMsg(const char *name=nullptr, short kind=0);
    MyMsg(const MyMsg& other);
    virtual ~MyMsg();
    MyMsg& operator=(const MyMsg& other);
    virtual MyMsg *dup() const override {return new MyMsg(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    virtual void setMyMsgArraySize(size_t size);
    virtual size_t getMyMsgArraySize() const;
    virtual int getMyMsg(size_t k) const;
    virtual void setMyMsg(size_t k, int myMsg);
    virtual void insertMyMsg(size_t k, int myMsg);
    [[deprecated]] void insertMyMsg(int myMsg) {appendMyMsg(myMsg);}
    virtual void appendMyMsg(int myMsg);
    virtual void eraseMyMsg(size_t k);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const MyMsg& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, MyMsg& obj) {obj.parsimUnpack(b);}


namespace omnetpp {

template<> inline MyMsg *fromAnyPtr(any_ptr ptr) { return check_and_cast<MyMsg*>(ptr.get<cObject>()); }

}  // namespace omnetpp

#endif // ifndef __MYMSG_M_H

