module;
#include <rapidjson/document.h>
export module rapidjson;

export namespace rapidjson {
  // export common types
  using rapidjson::Document;
  using rapidjson::GenericArray;
  using rapidjson::GenericDocument;
  using rapidjson::GenericObject;
  using rapidjson::GenericValue;
  using rapidjson::SizeType;
  using rapidjson::Value;
} // namespace rapidjson
