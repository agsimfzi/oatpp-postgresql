/***************************************************************************
 *
 * Project         _____    __   ____   _      _
 *                (  _  )  /__\ (_  _)_| |_  _| |_
 *                 )(_)(  /(__)\  )( (_   _)(_   _)
 *                (_____)(__)(__)(__)  |_|    |_|
 *
 *
 * Copyright 2018-present, Leonid Stryzhevskyi <lganzzzo@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ***************************************************************************/

#ifndef oatpp_postgresql_mapping_Deserializer_hpp
#define oatpp_postgresql_mapping_Deserializer_hpp

#include "TypeMapper.hpp"
#include "oatpp/core/Types.hpp"

#include <libpq-fe.h>

namespace oatpp { namespace postgresql { namespace mapping {

class Deserializer {
public:
  typedef oatpp::data::mapping::type::Type Type;
public:
  struct InData {
    Oid oid;
    const char* data;
    v_buff_size size;
  };
public:
  typedef oatpp::Void (*DeserializerMethod)(const Deserializer*, const InData&, const Type*);
private:
  static v_int16 deInt2(const InData& data);
  static v_int32 deInt4(const InData& data);
  static v_int64 deInt8(const InData& data);
  static v_int64 deInt(const InData& data);
private:
  std::vector<DeserializerMethod> m_methods;
  TypeMapper m_typeMapper;
public:

  Deserializer();

  void setDeserializerMethod(const data::mapping::type::ClassId& classId, DeserializerMethod method);

  oatpp::Void deserialize(const InData& data, const Type* type) const;

public:

  static oatpp::Void deserializeString(const Deserializer* _this, const InData& data, const Type* type);

  template<class IntWrapper>
  static oatpp::Void deserializeInt(const Deserializer* _this, const InData& data, const Type* type) {
    (void) _this;
    (void) type;
    auto value = deInt(data);
    return IntWrapper((typename IntWrapper::UnderlyingType) value);
  }

  static oatpp::Void deserializeFloat32(const Deserializer* _this, const InData& data, const Type* type);
  static oatpp::Void deserializeFloat64(const Deserializer* _this, const InData& data, const Type* type);

  static oatpp::Void deserializeAny(const Deserializer* _this, const InData& data, const Type* type);

};

}}}

#endif // oatpp_postgresql_mapping_Deserializer_hpp
