#include "SimpleLinearFitting.h"

int main(int argc, char *argv[]) {
  for (int i = 0; i < 100; i++) {
  std::vector<std::string> code_lines;
  ReadGCodeFile("d://test.txt", code_lines);
  ParseGCodeFile(code_lines);

  FitSmallLine(GfileFloatKerf);

  std::vector<std::string> new_lines;
  GenerateKerfGCode(new_lines);
  WriteGCodeFile("d://new_yang_cur.txt", new_lines);
  }

  return 1;
}
