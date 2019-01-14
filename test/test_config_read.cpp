/*
 *      Copy from : fanshao
 */

#include <gtest/gtest.h>
#include "src/zz_string.h"
#include "src/zz_log.h"
#include "src/zz_config_read.h"

using namespace std;
using namespace zz;

TEST(Config, Read) {
  ConfigRead cConfigRead;
  EXPECT_EQ(true, cConfigRead.open("./ut_config.conf"));
  
  string strValue;
  EXPECT_EQ(true, cConfigRead.read("test", "asdf", strValue));
  EXPECT_EQ("123", strValue);
  EXPECT_EQ(true, cConfigRead.read("test", "name", strValue));
  EXPECT_EQ("fanshao", strValue);
  EXPECT_EQ(true, cConfigRead.read("test", "asdf123", strValue));
  EXPECT_EQ("456", strValue);
  EXPECT_EQ(true, cConfigRead.read("test2", "name1", strValue));
  EXPECT_EQ("123", strValue);
  EXPECT_EQ(true, cConfigRead.read("test2", "name3", strValue));
  EXPECT_EQ("789", strValue);

  vector<string> lstSection = cConfigRead.getAllSection();
  EXPECT_EQ(4, lstSection.size());
  for (size_t i = 0; i < lstSection.size(); i++) {
    printf("%s\n", lstSection[i].c_str());
  }

  EXPECT_EQ(true, cConfigRead.read("test2", "name4", strValue));
  EXPECT_EQ("\n123\n456\n789\n", strValue);

  EXPECT_EQ(true, cConfigRead.read("test2", "name5", strValue));
  EXPECT_EQ("555", strValue);

  EXPECT_EQ(true, cConfigRead.read("test2", "name6", strValue));
  EXPECT_EQ("123\n456\n789\n", strValue);

  EXPECT_EQ(true, cConfigRead.read("test2", "name7", strValue));
  EXPECT_EQ("\n123\n456\n789\n", strValue);

  EXPECT_EQ(true, cConfigRead.read("test2", "local_config", strValue));
  EXPECT_EQ(
      "\n{\n  \"common\" : {\n    \"default_executor\" : \"qr_demo\",\n    \"handler_thread_num\" : 20\n  },\n  \"executors\" : [\n    {\n      \"executor_name\" : \"qr_demo\",\n      \"limit_time_ms\" : 100,\n      \"session_name\" : \"DemoSession\",\n      \"steps\" : [\n        [\n          {\n            \"handler_name\" : \"DemoWordSegmentPlugin\"\n          }\n        ],\n        [\n          {\n            \"handler_name\" : \"DemoKey2KeyPlugin\"\n          }\n        ],\n        [\n          {\n            \"handler_name\" : \"DemoSimRankPlugin\"\n          },\n          {\n            \"handler_name\" : \"DemoCidWeightPlugin\"\n          }\n        ],\n        [\n          {\n            \"handler_name\" : \"DemoOutFormatPlugin\"\n          }\n        ]\n      ]\n    },\n    {\n      \"executor_name\" : \"qr_demo2\",\n      \"limit_time_ms\" : 100,\n      \"session_name\" : \"DemoSession\",\n      \"steps\" : [\n        [\n          {\n            \"handler_name\" : \"DemoWordSegmentPlugin\"\n          }\n        ],\n        [\n          {\n            \"handler_name\" : \"DemoKey2KeyPlugin\"\n          }\n        ],\n        [\n          {\n            \"handler_name\" : \"DemoSimRankPlugin\"\n          }\n        ],\n        [\n          {\n            \"handler_name\" : \"DemoOutFormatPlugin\"\n          }\n        ]\n      ]\n    }\n  ]\n}\n",
      strValue);
      

  map<string, string> mapResult = cConfigRead.getSection("test_data");
  map<string, string>::iterator itr = mapResult.begin();
  for (; itr != mapResult.end(); itr++) {
    printf("%s = %s\n", itr->first.c_str(), itr->second.c_str());
  }
  EXPECT_EQ(1, mapResult.size());
  
}



