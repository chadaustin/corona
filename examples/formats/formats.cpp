#include <iostream>
#include <vector>
#include <corona.h>
using namespace std;
using namespace corona;


void printFormats(vector<FileFormatDesc>& formats) {
  for (unsigned i = 0; i < formats.size(); ++i) {
    cout << "  " << formats[i].description << ":";
    for (unsigned j = 0; j < formats[i].extensions.size(); ++j) {
      cout << " " << formats[i].extensions[j];
    }
    cout << endl;
  }
}


int main() {
  vector<FileFormatDesc> read;
  vector<FileFormatDesc> write;

  GetSupportedReadFormats(read);
  GetSupportedWriteFormats(write);

  cout << "Supported Read Formats:" << endl;
  printFormats(read);
  cout << endl;
  cout << "Supported Write Formats:" << endl;
  printFormats(write);
}
