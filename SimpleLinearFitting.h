#include "GCodeStruct.h"

#include <vector>
#include <string>

#define GfileTotalRows 8000
#define Gfilesize 0x400000

extern char text_wholefile[];
extern GCodeARRAY_STRUCT GfileFloatKerf[];
extern bool is_absolute;

bool ReadGCodeFile(const std::string &file, std::vector<std::string> &code_lines);
bool WriteGCodeFile(const std::string &file, const std::vector<std::string> &code_lines);

void ParseGCodeFile(const std::vector<std::string> &code_lines);
double GetCodeValue(const std::string &code_line, const std::string &match);
double GetRadius(const GCodeARRAY_STRUCT &pG);

void GenerateKerfGCode(std::vector<std::string> &code_lines);
std::string TransformGCodeLine(const GCodeARRAY_STRUCT &gcode_array);

void FitSmallLine(GCodeARRAY_STRUCT *Ptr);
double GetGCodeLength(GCodeARRAY_STRUCT *GCodeArryPtr);
