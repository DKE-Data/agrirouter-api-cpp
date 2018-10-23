// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: commons/message.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "commons/message.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/port.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace agrirouter {
namespace commons {

namespace {

const ::google::protobuf::Descriptor* Message_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  Message_reflection_ = NULL;
const ::google::protobuf::Descriptor* Message_ArgsEntry_descriptor_ = NULL;
const ::google::protobuf::Descriptor* Messages_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  Messages_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_commons_2fmessage_2eproto() GOOGLE_ATTRIBUTE_COLD;
void protobuf_AssignDesc_commons_2fmessage_2eproto() {
  protobuf_AddDesc_commons_2fmessage_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "commons/message.proto");
  GOOGLE_CHECK(file != NULL);
  Message_descriptor_ = file->message_type(0);
  static const int Message_offsets_[3] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Message, message_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Message, message_code_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Message, args_),
  };
  Message_reflection_ =
    ::google::protobuf::internal::GeneratedMessageReflection::NewGeneratedMessageReflection(
      Message_descriptor_,
      Message::internal_default_instance(),
      Message_offsets_,
      -1,
      -1,
      -1,
      sizeof(Message),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Message, _internal_metadata_));
  Message_ArgsEntry_descriptor_ = Message_descriptor_->nested_type(0);
  Messages_descriptor_ = file->message_type(1);
  static const int Messages_offsets_[1] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Messages, messages_),
  };
  Messages_reflection_ =
    ::google::protobuf::internal::GeneratedMessageReflection::NewGeneratedMessageReflection(
      Messages_descriptor_,
      Messages::internal_default_instance(),
      Messages_offsets_,
      -1,
      -1,
      -1,
      sizeof(Messages),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Messages, _internal_metadata_));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_commons_2fmessage_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) GOOGLE_ATTRIBUTE_COLD;
void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
      Message_descriptor_, Message::internal_default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
        Message_ArgsEntry_descriptor_,
        ::google::protobuf::internal::MapEntry<
            ::std::string,
            ::std::string,
            ::google::protobuf::internal::WireFormatLite::TYPE_STRING,
            ::google::protobuf::internal::WireFormatLite::TYPE_STRING,
            0>::CreateDefaultInstance(
                Message_ArgsEntry_descriptor_));
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
      Messages_descriptor_, Messages::internal_default_instance());
}

}  // namespace

void protobuf_ShutdownFile_commons_2fmessage_2eproto() {
  Message_default_instance_.Shutdown();
  delete Message_reflection_;
  Messages_default_instance_.Shutdown();
  delete Messages_reflection_;
}

void protobuf_InitDefaults_commons_2fmessage_2eproto_impl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::internal::GetEmptyString();
  Message_default_instance_.DefaultConstruct();
  Messages_default_instance_.DefaultConstruct();
  Message_default_instance_.get_mutable()->InitAsDefaultInstance();
  Messages_default_instance_.get_mutable()->InitAsDefaultInstance();
}

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_InitDefaults_commons_2fmessage_2eproto_once_);
void protobuf_InitDefaults_commons_2fmessage_2eproto() {
  ::google::protobuf::GoogleOnceInit(&protobuf_InitDefaults_commons_2fmessage_2eproto_once_,
                 &protobuf_InitDefaults_commons_2fmessage_2eproto_impl);
}
void protobuf_AddDesc_commons_2fmessage_2eproto_impl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  protobuf_InitDefaults_commons_2fmessage_2eproto();
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\025commons/message.proto\022\022agrirouter.comm"
    "ons\"\222\001\n\007Message\022\017\n\007message\030\001 \001(\t\022\024\n\014mess"
    "age_code\030\002 \001(\t\0223\n\004args\030\003 \003(\0132%.agriroute"
    "r.commons.Message.ArgsEntry\032+\n\tArgsEntry"
    "\022\013\n\003key\030\001 \001(\t\022\r\n\005value\030\002 \001(\t:\0028\001\"9\n\010Mess"
    "ages\022-\n\010messages\030\001 \003(\0132\033.agrirouter.comm"
    "ons.Messageb\006proto3", 259);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "commons/message.proto", &protobuf_RegisterTypes);
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_commons_2fmessage_2eproto);
}

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AddDesc_commons_2fmessage_2eproto_once_);
void protobuf_AddDesc_commons_2fmessage_2eproto() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AddDesc_commons_2fmessage_2eproto_once_,
                 &protobuf_AddDesc_commons_2fmessage_2eproto_impl);
}
// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_commons_2fmessage_2eproto {
  StaticDescriptorInitializer_commons_2fmessage_2eproto() {
    protobuf_AddDesc_commons_2fmessage_2eproto();
  }
} static_descriptor_initializer_commons_2fmessage_2eproto_;

namespace {

static void MergeFromFail(int line) GOOGLE_ATTRIBUTE_COLD GOOGLE_ATTRIBUTE_NORETURN;
static void MergeFromFail(int line) {
  ::google::protobuf::internal::MergeFromFail(__FILE__, line);
}

}  // namespace


// ===================================================================

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int Message::kMessageFieldNumber;
const int Message::kMessageCodeFieldNumber;
const int Message::kArgsFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

Message::Message()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  if (this != internal_default_instance()) protobuf_InitDefaults_commons_2fmessage_2eproto();
  SharedCtor();
  // @@protoc_insertion_point(constructor:agrirouter.commons.Message)
}

void Message::InitAsDefaultInstance() {
}

Message::Message(const Message& from)
  : ::google::protobuf::Message(),
    _internal_metadata_(NULL) {
  SharedCtor();
  UnsafeMergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:agrirouter.commons.Message)
}

void Message::SharedCtor() {
  args_.SetAssignDescriptorCallback(
      protobuf_AssignDescriptorsOnce);
  args_.SetEntryDescriptor(
      &::agrirouter::commons::Message_ArgsEntry_descriptor_);
  message_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  message_code_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  _cached_size_ = 0;
}

Message::~Message() {
  // @@protoc_insertion_point(destructor:agrirouter.commons.Message)
  SharedDtor();
}

void Message::SharedDtor() {
  message_.DestroyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  message_code_.DestroyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}

void Message::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* Message::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return Message_descriptor_;
}

const Message& Message::default_instance() {
  protobuf_InitDefaults_commons_2fmessage_2eproto();
  return *internal_default_instance();
}

::google::protobuf::internal::ExplicitlyConstructed<Message> Message_default_instance_;

Message* Message::New(::google::protobuf::Arena* arena) const {
  Message* n = new Message;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void Message::Clear() {
// @@protoc_insertion_point(message_clear_start:agrirouter.commons.Message)
  message_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  message_code_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  args_.Clear();
}

bool Message::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:agrirouter.commons.Message)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional string message = 1;
      case 1: {
        if (tag == 10) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_message()));
          DO_(::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
            this->message().data(), this->message().length(),
            ::google::protobuf::internal::WireFormatLite::PARSE,
            "agrirouter.commons.Message.message"));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(18)) goto parse_message_code;
        break;
      }

      // optional string message_code = 2;
      case 2: {
        if (tag == 18) {
         parse_message_code:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_message_code()));
          DO_(::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
            this->message_code().data(), this->message_code().length(),
            ::google::protobuf::internal::WireFormatLite::PARSE,
            "agrirouter.commons.Message.message_code"));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(26)) goto parse_args;
        break;
      }

      // map<string, string> args = 3;
      case 3: {
        if (tag == 26) {
         parse_args:
          DO_(input->IncrementRecursionDepth());
         parse_loop_args:
          Message_ArgsEntry::Parser< ::google::protobuf::internal::MapField<
              ::std::string, ::std::string,
              ::google::protobuf::internal::WireFormatLite::TYPE_STRING,
              ::google::protobuf::internal::WireFormatLite::TYPE_STRING,
              0 >,
            ::google::protobuf::Map< ::std::string, ::std::string > > parser(&args_);
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
              input, &parser));
          DO_(::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
            parser.key().data(), parser.key().length(),
            ::google::protobuf::internal::WireFormatLite::PARSE,
            "agrirouter.commons.Message.ArgsEntry.key"));
          DO_(::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
            parser.value().data(), parser.value().length(),
            ::google::protobuf::internal::WireFormatLite::PARSE,
            "agrirouter.commons.Message.ArgsEntry.value"));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(26)) goto parse_loop_args;
        input->UnsafeDecrementRecursionDepth();
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormatLite::SkipField(input, tag));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:agrirouter.commons.Message)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:agrirouter.commons.Message)
  return false;
#undef DO_
}

void Message::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:agrirouter.commons.Message)
  // optional string message = 1;
  if (this->message().size() > 0) {
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
      this->message().data(), this->message().length(),
      ::google::protobuf::internal::WireFormatLite::SERIALIZE,
      "agrirouter.commons.Message.message");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      1, this->message(), output);
  }

  // optional string message_code = 2;
  if (this->message_code().size() > 0) {
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
      this->message_code().data(), this->message_code().length(),
      ::google::protobuf::internal::WireFormatLite::SERIALIZE,
      "agrirouter.commons.Message.message_code");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      2, this->message_code(), output);
  }

  // map<string, string> args = 3;
  if (!this->args().empty()) {
    typedef ::google::protobuf::Map< ::std::string, ::std::string >::const_pointer
        ConstPtr;
    typedef ConstPtr SortItem;
    typedef ::google::protobuf::internal::CompareByDerefFirst<SortItem> Less;
    struct Utf8Check {
      static void Check(ConstPtr p) {
        ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
          p->first.data(), p->first.length(),
          ::google::protobuf::internal::WireFormatLite::SERIALIZE,
          "agrirouter.commons.Message.ArgsEntry.key");
        ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
          p->second.data(), p->second.length(),
          ::google::protobuf::internal::WireFormatLite::SERIALIZE,
          "agrirouter.commons.Message.ArgsEntry.value");
      }
    };

    if (output->IsSerializationDeterminstic() &&
        this->args().size() > 1) {
      ::google::protobuf::scoped_array<SortItem> items(
          new SortItem[this->args().size()]);
      typedef ::google::protobuf::Map< ::std::string, ::std::string >::size_type size_type;
      size_type n = 0;
      for (::google::protobuf::Map< ::std::string, ::std::string >::const_iterator
          it = this->args().begin();
          it != this->args().end(); ++it, ++n) {
        items[n] = SortItem(&*it);
      }
      ::std::sort(&items[0], &items[n], Less());
      ::google::protobuf::scoped_ptr<Message_ArgsEntry> entry;
      for (size_type i = 0; i < n; i++) {
        entry.reset(args_.NewEntryWrapper(
            items[i]->first, items[i]->second));
        ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
            3, *entry, output);
        Utf8Check::Check(items[i]);
      }
    } else {
      ::google::protobuf::scoped_ptr<Message_ArgsEntry> entry;
      for (::google::protobuf::Map< ::std::string, ::std::string >::const_iterator
          it = this->args().begin();
          it != this->args().end(); ++it) {
        entry.reset(args_.NewEntryWrapper(
            it->first, it->second));
        ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
            3, *entry, output);
        Utf8Check::Check(&*it);
      }
    }
  }

  // @@protoc_insertion_point(serialize_end:agrirouter.commons.Message)
}

::google::protobuf::uint8* Message::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  (void)deterministic; // Unused
  // @@protoc_insertion_point(serialize_to_array_start:agrirouter.commons.Message)
  // optional string message = 1;
  if (this->message().size() > 0) {
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
      this->message().data(), this->message().length(),
      ::google::protobuf::internal::WireFormatLite::SERIALIZE,
      "agrirouter.commons.Message.message");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->message(), target);
  }

  // optional string message_code = 2;
  if (this->message_code().size() > 0) {
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
      this->message_code().data(), this->message_code().length(),
      ::google::protobuf::internal::WireFormatLite::SERIALIZE,
      "agrirouter.commons.Message.message_code");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        2, this->message_code(), target);
  }

  // map<string, string> args = 3;
  if (!this->args().empty()) {
    typedef ::google::protobuf::Map< ::std::string, ::std::string >::const_pointer
        ConstPtr;
    typedef ConstPtr SortItem;
    typedef ::google::protobuf::internal::CompareByDerefFirst<SortItem> Less;
    struct Utf8Check {
      static void Check(ConstPtr p) {
        ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
          p->first.data(), p->first.length(),
          ::google::protobuf::internal::WireFormatLite::SERIALIZE,
          "agrirouter.commons.Message.ArgsEntry.key");
        ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
          p->second.data(), p->second.length(),
          ::google::protobuf::internal::WireFormatLite::SERIALIZE,
          "agrirouter.commons.Message.ArgsEntry.value");
      }
    };

    if (deterministic &&
        this->args().size() > 1) {
      ::google::protobuf::scoped_array<SortItem> items(
          new SortItem[this->args().size()]);
      typedef ::google::protobuf::Map< ::std::string, ::std::string >::size_type size_type;
      size_type n = 0;
      for (::google::protobuf::Map< ::std::string, ::std::string >::const_iterator
          it = this->args().begin();
          it != this->args().end(); ++it, ++n) {
        items[n] = SortItem(&*it);
      }
      ::std::sort(&items[0], &items[n], Less());
      ::google::protobuf::scoped_ptr<Message_ArgsEntry> entry;
      for (size_type i = 0; i < n; i++) {
        entry.reset(args_.NewEntryWrapper(
            items[i]->first, items[i]->second));
        target = ::google::protobuf::internal::WireFormatLite::
                   InternalWriteMessageNoVirtualToArray(
                       3, *entry, deterministic, target);
;
        Utf8Check::Check(items[i]);
      }
    } else {
      ::google::protobuf::scoped_ptr<Message_ArgsEntry> entry;
      for (::google::protobuf::Map< ::std::string, ::std::string >::const_iterator
          it = this->args().begin();
          it != this->args().end(); ++it) {
        entry.reset(args_.NewEntryWrapper(
            it->first, it->second));
        target = ::google::protobuf::internal::WireFormatLite::
                   InternalWriteMessageNoVirtualToArray(
                       3, *entry, deterministic, target);
;
        Utf8Check::Check(&*it);
      }
    }
  }

  // @@protoc_insertion_point(serialize_to_array_end:agrirouter.commons.Message)
  return target;
}

size_t Message::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:agrirouter.commons.Message)
  size_t total_size = 0;

  // optional string message = 1;
  if (this->message().size() > 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::StringSize(
        this->message());
  }

  // optional string message_code = 2;
  if (this->message_code().size() > 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::StringSize(
        this->message_code());
  }

  // map<string, string> args = 3;
  total_size += 1 *
      ::google::protobuf::internal::FromIntSize(this->args_size());
  {
    ::google::protobuf::scoped_ptr<Message_ArgsEntry> entry;
    for (::google::protobuf::Map< ::std::string, ::std::string >::const_iterator
        it = this->args().begin();
        it != this->args().end(); ++it) {
      entry.reset(args_.NewEntryWrapper(it->first, it->second));
      total_size += ::google::protobuf::internal::WireFormatLite::
          MessageSizeNoVirtual(*entry);
    }
  }

  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = cached_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void Message::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:agrirouter.commons.Message)
  if (GOOGLE_PREDICT_FALSE(&from == this)) MergeFromFail(__LINE__);
  const Message* source =
      ::google::protobuf::internal::DynamicCastToGenerated<const Message>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:agrirouter.commons.Message)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:agrirouter.commons.Message)
    UnsafeMergeFrom(*source);
  }
}

void Message::MergeFrom(const Message& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:agrirouter.commons.Message)
  if (GOOGLE_PREDICT_TRUE(&from != this)) {
    UnsafeMergeFrom(from);
  } else {
    MergeFromFail(__LINE__);
  }
}

void Message::UnsafeMergeFrom(const Message& from) {
  GOOGLE_DCHECK(&from != this);
  args_.MergeFrom(from.args_);
  if (from.message().size() > 0) {

    message_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.message_);
  }
  if (from.message_code().size() > 0) {

    message_code_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.message_code_);
  }
}

void Message::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:agrirouter.commons.Message)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Message::CopyFrom(const Message& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:agrirouter.commons.Message)
  if (&from == this) return;
  Clear();
  UnsafeMergeFrom(from);
}

bool Message::IsInitialized() const {

  return true;
}

void Message::Swap(Message* other) {
  if (other == this) return;
  InternalSwap(other);
}
void Message::InternalSwap(Message* other) {
  message_.Swap(&other->message_);
  message_code_.Swap(&other->message_code_);
  args_.Swap(&other->args_);
  _internal_metadata_.Swap(&other->_internal_metadata_);
  std::swap(_cached_size_, other->_cached_size_);
}

::google::protobuf::Metadata Message::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = Message_descriptor_;
  metadata.reflection = Message_reflection_;
  return metadata;
}

#if PROTOBUF_INLINE_NOT_IN_HEADERS
// Message

// optional string message = 1;
void Message::clear_message() {
  message_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
const ::std::string& Message::message() const {
  // @@protoc_insertion_point(field_get:agrirouter.commons.Message.message)
  return message_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
void Message::set_message(const ::std::string& value) {
  
  message_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:agrirouter.commons.Message.message)
}
void Message::set_message(const char* value) {
  
  message_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:agrirouter.commons.Message.message)
}
void Message::set_message(const char* value, size_t size) {
  
  message_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:agrirouter.commons.Message.message)
}
::std::string* Message::mutable_message() {
  
  // @@protoc_insertion_point(field_mutable:agrirouter.commons.Message.message)
  return message_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
::std::string* Message::release_message() {
  // @@protoc_insertion_point(field_release:agrirouter.commons.Message.message)
  
  return message_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
void Message::set_allocated_message(::std::string* message) {
  if (message != NULL) {
    
  } else {
    
  }
  message_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), message);
  // @@protoc_insertion_point(field_set_allocated:agrirouter.commons.Message.message)
}

// optional string message_code = 2;
void Message::clear_message_code() {
  message_code_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
const ::std::string& Message::message_code() const {
  // @@protoc_insertion_point(field_get:agrirouter.commons.Message.message_code)
  return message_code_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
void Message::set_message_code(const ::std::string& value) {
  
  message_code_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:agrirouter.commons.Message.message_code)
}
void Message::set_message_code(const char* value) {
  
  message_code_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:agrirouter.commons.Message.message_code)
}
void Message::set_message_code(const char* value, size_t size) {
  
  message_code_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:agrirouter.commons.Message.message_code)
}
::std::string* Message::mutable_message_code() {
  
  // @@protoc_insertion_point(field_mutable:agrirouter.commons.Message.message_code)
  return message_code_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
::std::string* Message::release_message_code() {
  // @@protoc_insertion_point(field_release:agrirouter.commons.Message.message_code)
  
  return message_code_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
void Message::set_allocated_message_code(::std::string* message_code) {
  if (message_code != NULL) {
    
  } else {
    
  }
  message_code_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), message_code);
  // @@protoc_insertion_point(field_set_allocated:agrirouter.commons.Message.message_code)
}

// map<string, string> args = 3;
int Message::args_size() const {
  return args_.size();
}
void Message::clear_args() {
  args_.Clear();
}
 const ::google::protobuf::Map< ::std::string, ::std::string >&
Message::args() const {
  // @@protoc_insertion_point(field_map:agrirouter.commons.Message.args)
  return args_.GetMap();
}
 ::google::protobuf::Map< ::std::string, ::std::string >*
Message::mutable_args() {
  // @@protoc_insertion_point(field_mutable_map:agrirouter.commons.Message.args)
  return args_.MutableMap();
}

inline const Message* Message::internal_default_instance() {
  return &Message_default_instance_.get();
}
#endif  // PROTOBUF_INLINE_NOT_IN_HEADERS

// ===================================================================

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int Messages::kMessagesFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

Messages::Messages()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  if (this != internal_default_instance()) protobuf_InitDefaults_commons_2fmessage_2eproto();
  SharedCtor();
  // @@protoc_insertion_point(constructor:agrirouter.commons.Messages)
}

void Messages::InitAsDefaultInstance() {
}

Messages::Messages(const Messages& from)
  : ::google::protobuf::Message(),
    _internal_metadata_(NULL) {
  SharedCtor();
  UnsafeMergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:agrirouter.commons.Messages)
}

void Messages::SharedCtor() {
  _cached_size_ = 0;
}

Messages::~Messages() {
  // @@protoc_insertion_point(destructor:agrirouter.commons.Messages)
  SharedDtor();
}

void Messages::SharedDtor() {
}

void Messages::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* Messages::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return Messages_descriptor_;
}

const Messages& Messages::default_instance() {
  protobuf_InitDefaults_commons_2fmessage_2eproto();
  return *internal_default_instance();
}

::google::protobuf::internal::ExplicitlyConstructed<Messages> Messages_default_instance_;

Messages* Messages::New(::google::protobuf::Arena* arena) const {
  Messages* n = new Messages;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void Messages::Clear() {
// @@protoc_insertion_point(message_clear_start:agrirouter.commons.Messages)
  messages_.Clear();
}

bool Messages::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:agrirouter.commons.Messages)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // repeated .agrirouter.commons.Message messages = 1;
      case 1: {
        if (tag == 10) {
          DO_(input->IncrementRecursionDepth());
         parse_loop_messages:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtualNoRecursionDepth(
                input, add_messages()));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(10)) goto parse_loop_messages;
        input->UnsafeDecrementRecursionDepth();
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormatLite::SkipField(input, tag));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:agrirouter.commons.Messages)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:agrirouter.commons.Messages)
  return false;
#undef DO_
}

void Messages::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:agrirouter.commons.Messages)
  // repeated .agrirouter.commons.Message messages = 1;
  for (unsigned int i = 0, n = this->messages_size(); i < n; i++) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      1, this->messages(i), output);
  }

  // @@protoc_insertion_point(serialize_end:agrirouter.commons.Messages)
}

::google::protobuf::uint8* Messages::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  (void)deterministic; // Unused
  // @@protoc_insertion_point(serialize_to_array_start:agrirouter.commons.Messages)
  // repeated .agrirouter.commons.Message messages = 1;
  for (unsigned int i = 0, n = this->messages_size(); i < n; i++) {
    target = ::google::protobuf::internal::WireFormatLite::
      InternalWriteMessageNoVirtualToArray(
        1, this->messages(i), false, target);
  }

  // @@protoc_insertion_point(serialize_to_array_end:agrirouter.commons.Messages)
  return target;
}

size_t Messages::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:agrirouter.commons.Messages)
  size_t total_size = 0;

  // repeated .agrirouter.commons.Message messages = 1;
  {
    unsigned int count = this->messages_size();
    total_size += 1UL * count;
    for (unsigned int i = 0; i < count; i++) {
      total_size +=
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->messages(i));
    }
  }

  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = cached_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void Messages::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:agrirouter.commons.Messages)
  if (GOOGLE_PREDICT_FALSE(&from == this)) MergeFromFail(__LINE__);
  const Messages* source =
      ::google::protobuf::internal::DynamicCastToGenerated<const Messages>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:agrirouter.commons.Messages)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:agrirouter.commons.Messages)
    UnsafeMergeFrom(*source);
  }
}

void Messages::MergeFrom(const Messages& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:agrirouter.commons.Messages)
  if (GOOGLE_PREDICT_TRUE(&from != this)) {
    UnsafeMergeFrom(from);
  } else {
    MergeFromFail(__LINE__);
  }
}

void Messages::UnsafeMergeFrom(const Messages& from) {
  GOOGLE_DCHECK(&from != this);
  messages_.MergeFrom(from.messages_);
}

void Messages::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:agrirouter.commons.Messages)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Messages::CopyFrom(const Messages& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:agrirouter.commons.Messages)
  if (&from == this) return;
  Clear();
  UnsafeMergeFrom(from);
}

bool Messages::IsInitialized() const {

  return true;
}

void Messages::Swap(Messages* other) {
  if (other == this) return;
  InternalSwap(other);
}
void Messages::InternalSwap(Messages* other) {
  messages_.UnsafeArenaSwap(&other->messages_);
  _internal_metadata_.Swap(&other->_internal_metadata_);
  std::swap(_cached_size_, other->_cached_size_);
}

::google::protobuf::Metadata Messages::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = Messages_descriptor_;
  metadata.reflection = Messages_reflection_;
  return metadata;
}

#if PROTOBUF_INLINE_NOT_IN_HEADERS
// Messages

// repeated .agrirouter.commons.Message messages = 1;
int Messages::messages_size() const {
  return messages_.size();
}
void Messages::clear_messages() {
  messages_.Clear();
}
const ::agrirouter::commons::Message& Messages::messages(int index) const {
  // @@protoc_insertion_point(field_get:agrirouter.commons.Messages.messages)
  return messages_.Get(index);
}
::agrirouter::commons::Message* Messages::mutable_messages(int index) {
  // @@protoc_insertion_point(field_mutable:agrirouter.commons.Messages.messages)
  return messages_.Mutable(index);
}
::agrirouter::commons::Message* Messages::add_messages() {
  // @@protoc_insertion_point(field_add:agrirouter.commons.Messages.messages)
  return messages_.Add();
}
::google::protobuf::RepeatedPtrField< ::agrirouter::commons::Message >*
Messages::mutable_messages() {
  // @@protoc_insertion_point(field_mutable_list:agrirouter.commons.Messages.messages)
  return &messages_;
}
const ::google::protobuf::RepeatedPtrField< ::agrirouter::commons::Message >&
Messages::messages() const {
  // @@protoc_insertion_point(field_list:agrirouter.commons.Messages.messages)
  return messages_;
}

inline const Messages* Messages::internal_default_instance() {
  return &Messages_default_instance_.get();
}
#endif  // PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)

}  // namespace commons
}  // namespace agrirouter

// @@protoc_insertion_point(global_scope)