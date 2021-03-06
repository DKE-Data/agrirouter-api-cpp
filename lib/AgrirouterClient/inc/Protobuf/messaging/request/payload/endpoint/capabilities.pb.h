// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: messaging/request/payload/endpoint/capabilities.proto

#ifndef PROTOBUF_messaging_2frequest_2fpayload_2fendpoint_2fcapabilities_2eproto__INCLUDED
#define PROTOBUF_messaging_2frequest_2fpayload_2fendpoint_2fcapabilities_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3001000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3001000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace agrirouter {
namespace request {
namespace payload {
namespace endpoint {

// Internal implementation detail -- do not call these.
void protobuf_AddDesc_messaging_2frequest_2fpayload_2fendpoint_2fcapabilities_2eproto();
void protobuf_InitDefaults_messaging_2frequest_2fpayload_2fendpoint_2fcapabilities_2eproto();
void protobuf_AssignDesc_messaging_2frequest_2fpayload_2fendpoint_2fcapabilities_2eproto();
void protobuf_ShutdownFile_messaging_2frequest_2fpayload_2fendpoint_2fcapabilities_2eproto();

class CapabilitySpecification;
class CapabilitySpecification_Capability;

enum CapabilitySpecification_Direction {
  CapabilitySpecification_Direction_SEND = 0,
  CapabilitySpecification_Direction_RECEIVE = 1,
  CapabilitySpecification_Direction_SEND_RECEIVE = 2,
  CapabilitySpecification_Direction_CapabilitySpecification_Direction_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  CapabilitySpecification_Direction_CapabilitySpecification_Direction_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
bool CapabilitySpecification_Direction_IsValid(int value);
const CapabilitySpecification_Direction CapabilitySpecification_Direction_Direction_MIN = CapabilitySpecification_Direction_SEND;
const CapabilitySpecification_Direction CapabilitySpecification_Direction_Direction_MAX = CapabilitySpecification_Direction_SEND_RECEIVE;
const int CapabilitySpecification_Direction_Direction_ARRAYSIZE = CapabilitySpecification_Direction_Direction_MAX + 1;

const ::google::protobuf::EnumDescriptor* CapabilitySpecification_Direction_descriptor();
inline const ::std::string& CapabilitySpecification_Direction_Name(CapabilitySpecification_Direction value) {
  return ::google::protobuf::internal::NameOfEnum(
    CapabilitySpecification_Direction_descriptor(), value);
}
inline bool CapabilitySpecification_Direction_Parse(
    const ::std::string& name, CapabilitySpecification_Direction* value) {
  return ::google::protobuf::internal::ParseNamedEnum<CapabilitySpecification_Direction>(
    CapabilitySpecification_Direction_descriptor(), name, value);
}
// ===================================================================

class CapabilitySpecification_Capability : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:agrirouter.request.payload.endpoint.CapabilitySpecification.Capability) */ {
 public:
  CapabilitySpecification_Capability();
  virtual ~CapabilitySpecification_Capability();

  CapabilitySpecification_Capability(const CapabilitySpecification_Capability& from);

  inline CapabilitySpecification_Capability& operator=(const CapabilitySpecification_Capability& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const CapabilitySpecification_Capability& default_instance();

  static const CapabilitySpecification_Capability* internal_default_instance();

  void Swap(CapabilitySpecification_Capability* other);

  // implements Message ----------------------------------------------

  inline CapabilitySpecification_Capability* New() const { return New(NULL); }

  CapabilitySpecification_Capability* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const CapabilitySpecification_Capability& from);
  void MergeFrom(const CapabilitySpecification_Capability& from);
  void Clear();
  bool IsInitialized() const;

  size_t ByteSizeLong() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const {
    return InternalSerializeWithCachedSizesToArray(false, output);
  }
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(CapabilitySpecification_Capability* other);
  void UnsafeMergeFrom(const CapabilitySpecification_Capability& from);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional string technical_message_type = 1;
  void clear_technical_message_type();
  static const int kTechnicalMessageTypeFieldNumber = 1;
  const ::std::string& technical_message_type() const;
  void set_technical_message_type(const ::std::string& value);
  void set_technical_message_type(const char* value);
  void set_technical_message_type(const char* value, size_t size);
  ::std::string* mutable_technical_message_type();
  ::std::string* release_technical_message_type();
  void set_allocated_technical_message_type(::std::string* technical_message_type);

  // optional .agrirouter.request.payload.endpoint.CapabilitySpecification.Direction direction = 2;
  void clear_direction();
  static const int kDirectionFieldNumber = 2;
  ::agrirouter::request::payload::endpoint::CapabilitySpecification_Direction direction() const;
  void set_direction(::agrirouter::request::payload::endpoint::CapabilitySpecification_Direction value);

  // @@protoc_insertion_point(class_scope:agrirouter.request.payload.endpoint.CapabilitySpecification.Capability)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::ArenaStringPtr technical_message_type_;
  int direction_;
  mutable int _cached_size_;
  friend void  protobuf_InitDefaults_messaging_2frequest_2fpayload_2fendpoint_2fcapabilities_2eproto_impl();
  friend void  protobuf_AddDesc_messaging_2frequest_2fpayload_2fendpoint_2fcapabilities_2eproto_impl();
  friend void protobuf_AssignDesc_messaging_2frequest_2fpayload_2fendpoint_2fcapabilities_2eproto();
  friend void protobuf_ShutdownFile_messaging_2frequest_2fpayload_2fendpoint_2fcapabilities_2eproto();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<CapabilitySpecification_Capability> CapabilitySpecification_Capability_default_instance_;

// -------------------------------------------------------------------

class CapabilitySpecification : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:agrirouter.request.payload.endpoint.CapabilitySpecification) */ {
 public:
  CapabilitySpecification();
  virtual ~CapabilitySpecification();

  CapabilitySpecification(const CapabilitySpecification& from);

  inline CapabilitySpecification& operator=(const CapabilitySpecification& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const CapabilitySpecification& default_instance();

  static const CapabilitySpecification* internal_default_instance();

  void Swap(CapabilitySpecification* other);

  // implements Message ----------------------------------------------

  inline CapabilitySpecification* New() const { return New(NULL); }

  CapabilitySpecification* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const CapabilitySpecification& from);
  void MergeFrom(const CapabilitySpecification& from);
  void Clear();
  bool IsInitialized() const;

  size_t ByteSizeLong() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const {
    return InternalSerializeWithCachedSizesToArray(false, output);
  }
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(CapabilitySpecification* other);
  void UnsafeMergeFrom(const CapabilitySpecification& from);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  typedef CapabilitySpecification_Capability Capability;

  typedef CapabilitySpecification_Direction Direction;
  static const Direction SEND =
    CapabilitySpecification_Direction_SEND;
  static const Direction RECEIVE =
    CapabilitySpecification_Direction_RECEIVE;
  static const Direction SEND_RECEIVE =
    CapabilitySpecification_Direction_SEND_RECEIVE;
  static inline bool Direction_IsValid(int value) {
    return CapabilitySpecification_Direction_IsValid(value);
  }
  static const Direction Direction_MIN =
    CapabilitySpecification_Direction_Direction_MIN;
  static const Direction Direction_MAX =
    CapabilitySpecification_Direction_Direction_MAX;
  static const int Direction_ARRAYSIZE =
    CapabilitySpecification_Direction_Direction_ARRAYSIZE;
  static inline const ::google::protobuf::EnumDescriptor*
  Direction_descriptor() {
    return CapabilitySpecification_Direction_descriptor();
  }
  static inline const ::std::string& Direction_Name(Direction value) {
    return CapabilitySpecification_Direction_Name(value);
  }
  static inline bool Direction_Parse(const ::std::string& name,
      Direction* value) {
    return CapabilitySpecification_Direction_Parse(name, value);
  }

  // accessors -------------------------------------------------------

  // repeated .agrirouter.request.payload.endpoint.CapabilitySpecification.Capability capabilities = 1;
  int capabilities_size() const;
  void clear_capabilities();
  static const int kCapabilitiesFieldNumber = 1;
  const ::agrirouter::request::payload::endpoint::CapabilitySpecification_Capability& capabilities(int index) const;
  ::agrirouter::request::payload::endpoint::CapabilitySpecification_Capability* mutable_capabilities(int index);
  ::agrirouter::request::payload::endpoint::CapabilitySpecification_Capability* add_capabilities();
  ::google::protobuf::RepeatedPtrField< ::agrirouter::request::payload::endpoint::CapabilitySpecification_Capability >*
      mutable_capabilities();
  const ::google::protobuf::RepeatedPtrField< ::agrirouter::request::payload::endpoint::CapabilitySpecification_Capability >&
      capabilities() const;

  // optional string app_certification_id = 2;
  void clear_app_certification_id();
  static const int kAppCertificationIdFieldNumber = 2;
  const ::std::string& app_certification_id() const;
  void set_app_certification_id(const ::std::string& value);
  void set_app_certification_id(const char* value);
  void set_app_certification_id(const char* value, size_t size);
  ::std::string* mutable_app_certification_id();
  ::std::string* release_app_certification_id();
  void set_allocated_app_certification_id(::std::string* app_certification_id);

  // optional string app_certification_version_id = 3;
  void clear_app_certification_version_id();
  static const int kAppCertificationVersionIdFieldNumber = 3;
  const ::std::string& app_certification_version_id() const;
  void set_app_certification_version_id(const ::std::string& value);
  void set_app_certification_version_id(const char* value);
  void set_app_certification_version_id(const char* value, size_t size);
  ::std::string* mutable_app_certification_version_id();
  ::std::string* release_app_certification_version_id();
  void set_allocated_app_certification_version_id(::std::string* app_certification_version_id);

  // @@protoc_insertion_point(class_scope:agrirouter.request.payload.endpoint.CapabilitySpecification)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::RepeatedPtrField< ::agrirouter::request::payload::endpoint::CapabilitySpecification_Capability > capabilities_;
  ::google::protobuf::internal::ArenaStringPtr app_certification_id_;
  ::google::protobuf::internal::ArenaStringPtr app_certification_version_id_;
  mutable int _cached_size_;
  friend void  protobuf_InitDefaults_messaging_2frequest_2fpayload_2fendpoint_2fcapabilities_2eproto_impl();
  friend void  protobuf_AddDesc_messaging_2frequest_2fpayload_2fendpoint_2fcapabilities_2eproto_impl();
  friend void protobuf_AssignDesc_messaging_2frequest_2fpayload_2fendpoint_2fcapabilities_2eproto();
  friend void protobuf_ShutdownFile_messaging_2frequest_2fpayload_2fendpoint_2fcapabilities_2eproto();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<CapabilitySpecification> CapabilitySpecification_default_instance_;

// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// CapabilitySpecification_Capability

// optional string technical_message_type = 1;
inline void CapabilitySpecification_Capability::clear_technical_message_type() {
  technical_message_type_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& CapabilitySpecification_Capability::technical_message_type() const {
  // @@protoc_insertion_point(field_get:agrirouter.request.payload.endpoint.CapabilitySpecification.Capability.technical_message_type)
  return technical_message_type_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void CapabilitySpecification_Capability::set_technical_message_type(const ::std::string& value) {
  
  technical_message_type_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:agrirouter.request.payload.endpoint.CapabilitySpecification.Capability.technical_message_type)
}
inline void CapabilitySpecification_Capability::set_technical_message_type(const char* value) {
  
  technical_message_type_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:agrirouter.request.payload.endpoint.CapabilitySpecification.Capability.technical_message_type)
}
inline void CapabilitySpecification_Capability::set_technical_message_type(const char* value, size_t size) {
  
  technical_message_type_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:agrirouter.request.payload.endpoint.CapabilitySpecification.Capability.technical_message_type)
}
inline ::std::string* CapabilitySpecification_Capability::mutable_technical_message_type() {
  
  // @@protoc_insertion_point(field_mutable:agrirouter.request.payload.endpoint.CapabilitySpecification.Capability.technical_message_type)
  return technical_message_type_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* CapabilitySpecification_Capability::release_technical_message_type() {
  // @@protoc_insertion_point(field_release:agrirouter.request.payload.endpoint.CapabilitySpecification.Capability.technical_message_type)
  
  return technical_message_type_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void CapabilitySpecification_Capability::set_allocated_technical_message_type(::std::string* technical_message_type) {
  if (technical_message_type != NULL) {
    
  } else {
    
  }
  technical_message_type_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), technical_message_type);
  // @@protoc_insertion_point(field_set_allocated:agrirouter.request.payload.endpoint.CapabilitySpecification.Capability.technical_message_type)
}

// optional .agrirouter.request.payload.endpoint.CapabilitySpecification.Direction direction = 2;
inline void CapabilitySpecification_Capability::clear_direction() {
  direction_ = 0;
}
inline ::agrirouter::request::payload::endpoint::CapabilitySpecification_Direction CapabilitySpecification_Capability::direction() const {
  // @@protoc_insertion_point(field_get:agrirouter.request.payload.endpoint.CapabilitySpecification.Capability.direction)
  return static_cast< ::agrirouter::request::payload::endpoint::CapabilitySpecification_Direction >(direction_);
}
inline void CapabilitySpecification_Capability::set_direction(::agrirouter::request::payload::endpoint::CapabilitySpecification_Direction value) {
  
  direction_ = value;
  // @@protoc_insertion_point(field_set:agrirouter.request.payload.endpoint.CapabilitySpecification.Capability.direction)
}

inline const CapabilitySpecification_Capability* CapabilitySpecification_Capability::internal_default_instance() {
  return &CapabilitySpecification_Capability_default_instance_.get();
}
// -------------------------------------------------------------------

// CapabilitySpecification

// repeated .agrirouter.request.payload.endpoint.CapabilitySpecification.Capability capabilities = 1;
inline int CapabilitySpecification::capabilities_size() const {
  return capabilities_.size();
}
inline void CapabilitySpecification::clear_capabilities() {
  capabilities_.Clear();
}
inline const ::agrirouter::request::payload::endpoint::CapabilitySpecification_Capability& CapabilitySpecification::capabilities(int index) const {
  // @@protoc_insertion_point(field_get:agrirouter.request.payload.endpoint.CapabilitySpecification.capabilities)
  return capabilities_.Get(index);
}
inline ::agrirouter::request::payload::endpoint::CapabilitySpecification_Capability* CapabilitySpecification::mutable_capabilities(int index) {
  // @@protoc_insertion_point(field_mutable:agrirouter.request.payload.endpoint.CapabilitySpecification.capabilities)
  return capabilities_.Mutable(index);
}
inline ::agrirouter::request::payload::endpoint::CapabilitySpecification_Capability* CapabilitySpecification::add_capabilities() {
  // @@protoc_insertion_point(field_add:agrirouter.request.payload.endpoint.CapabilitySpecification.capabilities)
  return capabilities_.Add();
}
inline ::google::protobuf::RepeatedPtrField< ::agrirouter::request::payload::endpoint::CapabilitySpecification_Capability >*
CapabilitySpecification::mutable_capabilities() {
  // @@protoc_insertion_point(field_mutable_list:agrirouter.request.payload.endpoint.CapabilitySpecification.capabilities)
  return &capabilities_;
}
inline const ::google::protobuf::RepeatedPtrField< ::agrirouter::request::payload::endpoint::CapabilitySpecification_Capability >&
CapabilitySpecification::capabilities() const {
  // @@protoc_insertion_point(field_list:agrirouter.request.payload.endpoint.CapabilitySpecification.capabilities)
  return capabilities_;
}

// optional string app_certification_id = 2;
inline void CapabilitySpecification::clear_app_certification_id() {
  app_certification_id_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& CapabilitySpecification::app_certification_id() const {
  // @@protoc_insertion_point(field_get:agrirouter.request.payload.endpoint.CapabilitySpecification.app_certification_id)
  return app_certification_id_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void CapabilitySpecification::set_app_certification_id(const ::std::string& value) {
  
  app_certification_id_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:agrirouter.request.payload.endpoint.CapabilitySpecification.app_certification_id)
}
inline void CapabilitySpecification::set_app_certification_id(const char* value) {
  
  app_certification_id_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:agrirouter.request.payload.endpoint.CapabilitySpecification.app_certification_id)
}
inline void CapabilitySpecification::set_app_certification_id(const char* value, size_t size) {
  
  app_certification_id_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:agrirouter.request.payload.endpoint.CapabilitySpecification.app_certification_id)
}
inline ::std::string* CapabilitySpecification::mutable_app_certification_id() {
  
  // @@protoc_insertion_point(field_mutable:agrirouter.request.payload.endpoint.CapabilitySpecification.app_certification_id)
  return app_certification_id_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* CapabilitySpecification::release_app_certification_id() {
  // @@protoc_insertion_point(field_release:agrirouter.request.payload.endpoint.CapabilitySpecification.app_certification_id)
  
  return app_certification_id_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void CapabilitySpecification::set_allocated_app_certification_id(::std::string* app_certification_id) {
  if (app_certification_id != NULL) {
    
  } else {
    
  }
  app_certification_id_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), app_certification_id);
  // @@protoc_insertion_point(field_set_allocated:agrirouter.request.payload.endpoint.CapabilitySpecification.app_certification_id)
}

// optional string app_certification_version_id = 3;
inline void CapabilitySpecification::clear_app_certification_version_id() {
  app_certification_version_id_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& CapabilitySpecification::app_certification_version_id() const {
  // @@protoc_insertion_point(field_get:agrirouter.request.payload.endpoint.CapabilitySpecification.app_certification_version_id)
  return app_certification_version_id_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void CapabilitySpecification::set_app_certification_version_id(const ::std::string& value) {
  
  app_certification_version_id_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:agrirouter.request.payload.endpoint.CapabilitySpecification.app_certification_version_id)
}
inline void CapabilitySpecification::set_app_certification_version_id(const char* value) {
  
  app_certification_version_id_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:agrirouter.request.payload.endpoint.CapabilitySpecification.app_certification_version_id)
}
inline void CapabilitySpecification::set_app_certification_version_id(const char* value, size_t size) {
  
  app_certification_version_id_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:agrirouter.request.payload.endpoint.CapabilitySpecification.app_certification_version_id)
}
inline ::std::string* CapabilitySpecification::mutable_app_certification_version_id() {
  
  // @@protoc_insertion_point(field_mutable:agrirouter.request.payload.endpoint.CapabilitySpecification.app_certification_version_id)
  return app_certification_version_id_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* CapabilitySpecification::release_app_certification_version_id() {
  // @@protoc_insertion_point(field_release:agrirouter.request.payload.endpoint.CapabilitySpecification.app_certification_version_id)
  
  return app_certification_version_id_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void CapabilitySpecification::set_allocated_app_certification_version_id(::std::string* app_certification_version_id) {
  if (app_certification_version_id != NULL) {
    
  } else {
    
  }
  app_certification_version_id_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), app_certification_version_id);
  // @@protoc_insertion_point(field_set_allocated:agrirouter.request.payload.endpoint.CapabilitySpecification.app_certification_version_id)
}

inline const CapabilitySpecification* CapabilitySpecification::internal_default_instance() {
  return &CapabilitySpecification_default_instance_.get();
}
#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace endpoint
}  // namespace payload
}  // namespace request
}  // namespace agrirouter

#ifndef SWIG
namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::agrirouter::request::payload::endpoint::CapabilitySpecification_Direction> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::agrirouter::request::payload::endpoint::CapabilitySpecification_Direction>() {
  return ::agrirouter::request::payload::endpoint::CapabilitySpecification_Direction_descriptor();
}

}  // namespace protobuf
}  // namespace google
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_messaging_2frequest_2fpayload_2fendpoint_2fcapabilities_2eproto__INCLUDED
