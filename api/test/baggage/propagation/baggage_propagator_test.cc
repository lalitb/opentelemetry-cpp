#include "opentelemetry/baggage/propagation/baggage_propagator.h"
#include <gtest/gtest.h>
#include <map>
#include <string>

using namespace opentelemetry;
using namespace opentelemetry::baggage::propagation;

class BaggageCarrierTest : public context::propagation::TextMapCarrier
{
public:
  BaggageCarrierTest() = default;
  virtual nostd::string_view Get(nostd::string_view key) const noexcept override
  {
    auto it = headers_.find(std::string(key));
    if (it != headers_.end())
    {
      return nostd::string_view(it->second);
    }
    return "";
  }
  virtual void Set(nostd::string_view key, nostd::string_view value) noexcept override
  {
    headers_[std::string(key)] = std::string(value);
  }

  std::map<std::string, std::string> headers_;
};

static BaggagePropagator format;

TEST(BaggagePropagatorTest, ExtractNoBaggageHeader)
{
  BaggageCarrierTest carrier;
  carrier.headers_      = {};
  context::Context ctx1 = context::Context{};
  context::Context ctx2 = format.Extract(carrier, ctx1);
  auto ctx2_baggage     = baggage::propagation::GetBaggage(ctx2);
  EXPECT_EQ(ctx2_baggage->ToHeader(), "");
}

TEST(BaggagePropagatorTest, ExtractAndInjectBaggage)
{
  std::string max_baggage_kv = std::string(baggage::Baggage::kMaxKeyValueSize / 2 + 1, 'k') + "=" +
                               std::string(baggage::Baggage::kMaxKeyValueSize / 2 + 1, 'v');
  std::map<std::string, std::string> baggages = {
      {"key1=val1,key2=val2", "key1=val1,key2=val2"},                // valid header
      {"key1 =   val1,  key2 =val2   ", "key1=val1,key2=val2"},      // valid header with spaces
      {"key1=val1,key2=val2;prop=1", "key1=val1,key2=val2;prop=1"},  // valid header with properties
      {"key%2C1=val1,key2=val2%2Cval3",
       "key%2C1=val1,key2=val2%2Cval3"},                      // valid header with url escape
      {"key1=val1,key2=val2,a,val3", "key1=val1,key2=val2"},  // valid header with invalid value
      {"key1=,key2=val2", "key1=,key2=val2"},                 // valid header with empty value
      {"invalid_header", ""},                                 // invalid header
      {max_baggage_kv, ""}};                                  // max key-value pair length

  for (auto baggage : baggages)
  {
    BaggageCarrierTest carrier1;
    carrier1.headers_[kBaggageHeader.data()] = baggage.first;
    context::Context ctx1                    = context::Context{};
    context::Context ctx2                    = format.Extract(carrier1, ctx1);

    BaggageCarrierTest carrier2;
    format.Inject(carrier2, ctx2);
    EXPECT_EQ(carrier2.headers_[kBaggageHeader.data()], baggage.second);
  }
}
