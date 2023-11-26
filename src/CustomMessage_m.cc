//
// Generated file, do not edit! Created by opp_msgtool 6.0 from CustomMessage.msg.
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
#include "CustomMessage_m.h"

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

CustomMessage_Base::CustomMessage_Base(const char *name, short kind) : ::omnetpp::cPacket(name, kind)
{
}

CustomMessage_Base::CustomMessage_Base(const CustomMessage_Base& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

CustomMessage_Base::~CustomMessage_Base()
{
}

CustomMessage_Base& CustomMessage_Base::operator=(const CustomMessage_Base& other)
{
    if (this == &other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void CustomMessage_Base::copy(const CustomMessage_Base& other)
{
    this->header = other.header;
    this->payload = other.payload;
    this->type = other.type;
    this->trailer = other.trailer;
    this->ack_num = other.ack_num;
}

void CustomMessage_Base::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->header);
    doParsimPacking(b,this->payload);
    doParsimPacking(b,this->type);
    doParsimPacking(b,this->trailer);
    doParsimPacking(b,this->ack_num);
}

void CustomMessage_Base::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->header);
    doParsimUnpacking(b,this->payload);
    doParsimUnpacking(b,this->type);
    doParsimUnpacking(b,this->trailer);
    doParsimUnpacking(b,this->ack_num);
}

char CustomMessage_Base::getHeader() const
{
    return this->header;
}

void CustomMessage_Base::setHeader(char header)
{
    this->header = header;
}

const char * CustomMessage_Base::getPayload() const
{
    return this->payload.c_str();
}

void CustomMessage_Base::setPayload(const char * payload)
{
    this->payload = payload;
}

char CustomMessage_Base::getType() const
{
    return this->type;
}

void CustomMessage_Base::setType(char type)
{
    this->type = type;
}

char CustomMessage_Base::getTrailer() const
{
    return this->trailer;
}

void CustomMessage_Base::setTrailer(char trailer)
{
    this->trailer = trailer;
}

char CustomMessage_Base::getAck_num() const
{
    return this->ack_num;
}

void CustomMessage_Base::setAck_num(char ack_num)
{
    this->ack_num = ack_num;
}

class CustomMessageDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_header,
        FIELD_payload,
        FIELD_type,
        FIELD_trailer,
        FIELD_ack_num,
    };
  public:
    CustomMessageDescriptor();
    virtual ~CustomMessageDescriptor();

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

Register_ClassDescriptor(CustomMessageDescriptor)

CustomMessageDescriptor::CustomMessageDescriptor() : omnetpp::cClassDescriptor("CustomMessage", "omnetpp::cPacket")
{
    propertyNames = nullptr;
}

CustomMessageDescriptor::~CustomMessageDescriptor()
{
    delete[] propertyNames;
}

bool CustomMessageDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<CustomMessage_Base *>(obj)!=nullptr;
}

const char **CustomMessageDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = { "customize",  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *CustomMessageDescriptor::getProperty(const char *propertyName) const
{
    if (!strcmp(propertyName, "customize")) return "true";
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int CustomMessageDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 5+base->getFieldCount() : 5;
}

unsigned int CustomMessageDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_header
        FD_ISEDITABLE,    // FIELD_payload
        FD_ISEDITABLE,    // FIELD_type
        FD_ISEDITABLE,    // FIELD_trailer
        FD_ISEDITABLE,    // FIELD_ack_num
    };
    return (field >= 0 && field < 5) ? fieldTypeFlags[field] : 0;
}

const char *CustomMessageDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "header",
        "payload",
        "type",
        "trailer",
        "ack_num",
    };
    return (field >= 0 && field < 5) ? fieldNames[field] : nullptr;
}

int CustomMessageDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "header") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "payload") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "type") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "trailer") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "ack_num") == 0) return baseIndex + 4;
    return base ? base->findField(fieldName) : -1;
}

const char *CustomMessageDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "char",    // FIELD_header
        "string",    // FIELD_payload
        "char",    // FIELD_type
        "char",    // FIELD_trailer
        "char",    // FIELD_ack_num
    };
    return (field >= 0 && field < 5) ? fieldTypeStrings[field] : nullptr;
}

const char **CustomMessageDescriptor::getFieldPropertyNames(int field) const
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

const char *CustomMessageDescriptor::getFieldProperty(int field, const char *propertyName) const
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

int CustomMessageDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    CustomMessage_Base *pp = omnetpp::fromAnyPtr<CustomMessage_Base>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void CustomMessageDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    CustomMessage_Base *pp = omnetpp::fromAnyPtr<CustomMessage_Base>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'CustomMessage_Base'", field);
    }
}

const char *CustomMessageDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    CustomMessage_Base *pp = omnetpp::fromAnyPtr<CustomMessage_Base>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string CustomMessageDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    CustomMessage_Base *pp = omnetpp::fromAnyPtr<CustomMessage_Base>(object); (void)pp;
    switch (field) {
        case FIELD_header: return long2string(pp->getHeader());
        case FIELD_payload: return oppstring2string(pp->getPayload());
        case FIELD_type: return long2string(pp->getType());
        case FIELD_trailer: return long2string(pp->getTrailer());
        case FIELD_ack_num: return long2string(pp->getAck_num());
        default: return "";
    }
}

void CustomMessageDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    CustomMessage_Base *pp = omnetpp::fromAnyPtr<CustomMessage_Base>(object); (void)pp;
    switch (field) {
        case FIELD_header: pp->setHeader(string2long(value)); break;
        case FIELD_payload: pp->setPayload((value)); break;
        case FIELD_type: pp->setType(string2long(value)); break;
        case FIELD_trailer: pp->setTrailer(string2long(value)); break;
        case FIELD_ack_num: pp->setAck_num(string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'CustomMessage_Base'", field);
    }
}

omnetpp::cValue CustomMessageDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    CustomMessage_Base *pp = omnetpp::fromAnyPtr<CustomMessage_Base>(object); (void)pp;
    switch (field) {
        case FIELD_header: return pp->getHeader();
        case FIELD_payload: return pp->getPayload();
        case FIELD_type: return pp->getType();
        case FIELD_trailer: return pp->getTrailer();
        case FIELD_ack_num: return pp->getAck_num();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'CustomMessage_Base' as cValue -- field index out of range?", field);
    }
}

void CustomMessageDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    CustomMessage_Base *pp = omnetpp::fromAnyPtr<CustomMessage_Base>(object); (void)pp;
    switch (field) {
        case FIELD_header: pp->setHeader(omnetpp::checked_int_cast<char>(value.intValue())); break;
        case FIELD_payload: pp->setPayload(value.stringValue()); break;
        case FIELD_type: pp->setType(omnetpp::checked_int_cast<char>(value.intValue())); break;
        case FIELD_trailer: pp->setTrailer(omnetpp::checked_int_cast<char>(value.intValue())); break;
        case FIELD_ack_num: pp->setAck_num(omnetpp::checked_int_cast<char>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'CustomMessage_Base'", field);
    }
}

const char *CustomMessageDescriptor::getFieldStructName(int field) const
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

omnetpp::any_ptr CustomMessageDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    CustomMessage_Base *pp = omnetpp::fromAnyPtr<CustomMessage_Base>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void CustomMessageDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    CustomMessage_Base *pp = omnetpp::fromAnyPtr<CustomMessage_Base>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'CustomMessage_Base'", field);
    }
}

namespace omnetpp {

}  // namespace omnetpp

