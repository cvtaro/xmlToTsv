#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <regex>

#define FILE_NAME ("test_tag.txt")

void printXmlToTsv(std::string xmlData);

int main (void) {
  std::ifstream ifs(FILE_NAME);
  if (ifs.fail()) {
    std::cerr << std::string(FILE_NAME) + " is not found." << std::endl;
    return -1;
  }

  // ファイル読み込み
  std::vector<std::string> xmlDataList;
  char ch;
  std::string xmlData;
  bool xmlBegin = false;
  while (ifs.get(ch))
  {
    // std::cout << "read char = " << ch << std::endl;

    // データの開始，終了
    if (ch == '\"') {
      xmlBegin = !xmlBegin;
      // "<request> or </request>" はタグを読み取らない
      continue;
    }

    // tag 読み込み
    if (xmlBegin) {
      xmlData += ch;
    } else {
      // std::cout << "  read line = \n" << xmlData << std::endl;
      printXmlToTsv(xmlData);
      xmlData = "";
    }

  }
  

  return 0;
}

static std::string tags[]  = {
  "taga",
  "tagb",
  "tagc",
};

void printXmlToTsv(std::string xmlData) {
  std::string tsvStr = "";
  
  for (std::string tag : tags) {
    std::string sTag = "<" + tag + ">";
    std::string eTag = "</" + tag + ">";
    std::regex re(sTag + "(\\w+)" + eTag);
    std::smatch res;
    if (std::regex_search(xmlData, res, re)) {
      std::string resStr = res.str();
      // std::cout << "    tag " + tag + " is found.  res = " << resStr << std::endl;
      int begin = sTag.size();
      int end = resStr.size() - sTag.size() - eTag.size();
      tsvStr += "[" + resStr.substr(begin, end) + "]\t";
    } else {
      // std::cout << "    tag " + tag + " is not found." << std::endl;
      // tsvStr += "[]\t";
      tsvStr += "\t";
    }
  }

  std::cout << tsvStr << std::endl;
}