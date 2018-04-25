#include <iostream>
#include "cdk.h"
#include <fstream>
#include <stdint.h>
#include <iomanip>
#include <sstream>      
#include <stdio.h>
#define MATRIX_WIDTH 3
#define MATRIX_HEIGHT 5
#define BOX_WIDTH 15
#define MATRIX_NAME_STRING "Test Matrix"

using namespace std;
class BinaryFileHeader
{ 
public:
  uint32_t magicNumber;         /* Should be 0xFEEDFACE */
  uint32_t versionNumber;
  uint64_t numRecords;
};  
string makestring (int number)
{
  string ourstring;
  stringstream tempstream;
  tempstream << number;
  tempstream>>ourstring;
  return ourstring;
  
}
const int maxRecordStringLength = 25;
class BinaryFileRecord
{ 
public:
  uint8_t strLength;
  char   stringBuffer[maxRecordStringLength];
};  

int main()
{
  int j=2;
  stringstream ss;
  WINDOW        *window;
  CDKSCREEN     *cdkscreen;
  CDKMATRIX     *myMatrix;           // CDK Screen Matrix
  BinaryFileHeader *myHeader= new BinaryFileHeader();
  ifstream binInfile ("cs3377.bin", ios::in | ios::binary);
  binInfile.read((char *) myHeader, sizeof(BinaryFileHeader));
  binInfile.seekg(sizeof(BinaryFileHeader));
  //BinaryFileRecord *myRecord = new BinaryFileRecord();
  //  ifstream binInfile ("cs3377.bin", ios::in | ios::binary);
  const char            *rowTitles[] = {"R0", "R1", "R2", "R3", "R4", "R5"};
  const char            *columnTitles[] = {"C0", "C1", "C2", "C3", "C4", "C5"};
  int           boxWidths[] = {BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH};
  int           boxTypes[] = {vMIXED, vMIXED, vMIXED, vMIXED,  vMIXED,  vMIXED};
  window = initscr();
  cdkscreen = initCDKScreen(window);
  initCDKColor();

  /*
   * Create the matrix.  Need to manually cast (const char**) to (char **)
   */
  myMatrix = newCDKMatrix(cdkscreen, CENTER, CENTER, MATRIX_HEIGHT, MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_WIDTH,
                          MATRIX_NAME_STRING, (char **) rowTitles, (char **) columnTitles, boxWidths,
			  boxTypes, 1, 1, ' ', ROW, true, true, false);

  if (myMatrix ==NULL)
    {
      printf("Error creating Matrix\n");
      _exit(1);
    }

  /* Display the Matrix */
  drawCDKMatrix(myMatrix, true);

  /*
   * Dipslay a message
   */
  
  ss<<std::hex<<(uint32_t)myHeader->magicNumber;
  std::string result (ss.str());
  ss.str("");
  ss<<(uint32_t)myHeader->versionNumber;
  std::string result2 (ss.str());
  ss.str("");
  ss<<(uint64_t)myHeader->numRecords;
  std::string result3 (ss.str());
  ss.str("");
  char buff[30];
  setCDKMatrixCell(myMatrix,1, 1,  result.c_str());
  setCDKMatrixCell(myMatrix, 1, 2, result2.c_str());
  setCDKMatrixCell(myMatrix, 1, 3, result3.c_str());
  for (int i=0; i<4; i++)
    {
      BinaryFileRecord *myRecord = new BinaryFileRecord();
      binInfile.read((char *)myRecord, sizeof(BinaryFileRecord));
      sprintf(buff, "%d", myRecord->strLength);
      setCDKMatrixCell (myMatrix, j, 1, buff);
      sprintf(buff, "%s", myRecord->stringBuffer);
      setCDKMatrixCell (myMatrix, j, 2, buff);
      j++;
    }

  //
 setCDKMatrixCell(myMatrix, 2, 2, "Test Message");
   drawCDKMatrix(myMatrix, true);    /* required  */

  /* So we can see results, pause until a key is pressed. */
  unsigned char x;
  cin >> x;

  // Cleanup screen
  endCDK();
}

