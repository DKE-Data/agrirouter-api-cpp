// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: commons/chunk.proto

#ifndef PROTOBUF_commons_2fchunk_2eproto__INCLUDED
#define PROTOBUF_commons_2fchunk_2eproto__INCLUDED

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
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace agrirouter {
namespace commons {

// Internal implementation detail -- do not call these.
void protobuf_AddDesc_commons_2fchunk_2eproto();
void protobuf_InitDefaults_commons_2fchunk_2eproto();
void protobuf_AssignDesc_commons_2fchunk_2eproto();
void protobuf_ShutdownFile_commons_2fchunk_2eproto();

class ChunkComponent;

// ===================================================================

class ChunkComponent : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:agrirouter.commons.ChunkComponent) */ {
 public:
  ChunkComponent();
  virtual ~ChunkComponent();

  ChunkComponent(const ChunkComponent& from);

  inline ChunkComponent& operator=(const ChunkComponent& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const ChunkComponent& default_instance();

  static const ChunkComponent* internal_default_instance();

  void Swap(ChunkComponent* other);

  // implements Message ----------------------------------------------

  inline ChunkComponent* New() const { return New(NULL); }

  ChunkComponent* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ChunkComponent& from);
  void MergeFrom(const ChunkComponent& from);
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
  void InternalSwap(ChunkComponent* other);
  void UnsafeMergeFrom(const ChunkComponent& from);
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

  // optional string context_id = 1;
  void clear_context_id();
  static const int kContextIdFieldNumber = 1;
  const ::std::string& context_id() const;
  void set_context_id(const ::std::string& value);
  void set_context_id(const char* value);
  void set_context_id(const char* value, size_t size);
  ::std::string* mutable_context_id();
  ::std::string* release_context_id();
  void set_allocated_context_id(::std::string* context_id);

  // optional int64 current = 2;
  void clear_current();
  static const int kCurrentFieldNumber = 2;
  ::google::protobuf::int64 current() const;
  void set_current(::google::protobuf::int64 value);

  // optional int64 total = 3;
  void clear_total();
  static const int kTotalFieldNumber = 3;
  ::google::protobuf::int64 total() const;
  void set_total(::google::protobuf::int64 value);

  // optional int64 total_size = 4;
  void clear_total_size();
  static const int kTotalSizeFieldNumber = 4;
  ::google::protobuf::int64 total_size() const;
  void set_total_size(::google::protobuf::int64 value);

  // @@protoc_insertion_point(class_scope:agrirouter.commons.ChunkComponent)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::ArenaStringPtr context_id_;
  ::google::protobuf::int64 current_;
  ::google::protobuf::int64 total_;
  ::google::protobuf::int64 total_size_;
  mutable int _cached_size_;
  friend void  protobuf_InitDefaults_commons_2fchunk_2eproto_impl();
  friend void  protobuf_AddDesc_commons_2fchunk_2eproto_impl();
  friend void protobuf_AssignDesc_commons_2fchunk_2eproto();
  friend void protobuf_ShutdownFile_commons_2fchunk_2eproto();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<ChunkComponent> ChunkComponent_default_instance_;

// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// ChunkComponent

// optional string context_id = 1;
inline void ChunkComponent::clear_context_id() {
  context_id_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& ChunkComponent::context_id() const {
  // @@protoc_insertion_point(field_get:agrirouter.commons.ChunkComponent.context_id)
  return context_id_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void ChunkComponent::set_context_id(const ::std::string& value) {
  
  context_id_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:agrirouter.commons.ChunkComponent.context_id)
}
inline void ChunkComponent::set_context_id(const char* value) {
  
  context_id_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:agrirouter.commons.ChunkComponent.context_id)
}
inline void ChunkComponent::set_context_id(const char* value, size_t size) {
  
  context_id_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:agrirouter.commons.ChunkComponent.context_id)
}
inline ::std::string* ChunkComponent::mutable_context_id() {
  
  // @@protoc_insertion_point(field_mutable:agrirouter.commons.ChunkComponent.context_id)
  return context_id_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* ChunkComponent::release_context_id() {
  // @@protoc_insertion_point(field_release:agrirouter.commons.ChunkComponent.context_id)
  
  return context_id_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void ChunkComponent::set_allocated_context_id(::std::string* context_id) {
  if (context_id != NULL) {
    
  } else {
    
  }
  context_id_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), context_id);
  // @@protoc_insertion_point(field_set_allocated:agrirouter.commons.ChunkComponent.context_id)
}

// optional int64 current = 2;
inline void ChunkComponent::clear_current() {
  current_ = GOOGLE_LONGLONG(0);
}
inline ::google::protobuf::int64 ChunkComponent::current() const {
  // @@protoc_insertion_point(field_get:agrirouter.commons.ChunkComponent.current)
  return current_;
}
inline void ChunkComponent::set_current(::google::protobuf::int64 value) {
  
  current_ = value;
  // @@protoc_insertion_point(field_set:agrirouter.commons.ChunkComponent.current)
}

// optional int64 total = 3;
inline void ChunkComponent::clear_total() {
  total_ = GOOGLE_LONGLONG(0);
}
inline ::google::protobuf::int64 ChunkComponent::total() const {
  // @@protoc_insertion_point(field_get:agrirouter.commons.ChunkComponent.total)
  return total_;
}
inline void ChunkComponent::set_total(::google::protobuf::int64 value) {
  
  total_ = value;
  // @@protoc_insertion_point(field_set:agrirouter.commons.ChunkComponent.total)
}

// optional int64 total_size = 4;
inline void ChunkComponent::clear_total_size() {
  total_size_ = GOOGLE_LONGLONG(0);
}
inline ::google::protobuf::int64 ChunkComponent::total_size() const {
  // @@protoc_insertion_point(field_get:agrirouter.commons.ChunkComponent.total_size)
  return total_size_;
}
inline void ChunkComponent::set_total_size(::google::protobuf::int64 value) {
  
  total_size_ = value;
  // @@protoc_insertion_point(field_set:agrirouter.commons.ChunkComponent.total_size)
}

inline const ChunkComponent* ChunkComponent::internal_default_instance() {
  return &ChunkComponent_default_instance_.get();
}
#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)

}  // namespace commons
}  // namespace agrirouter

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_commons_2fchunk_2eproto__INCLUDED
