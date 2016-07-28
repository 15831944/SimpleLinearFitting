#include "SimpleLinearFitting.h"

#include <fstream>
#include <math.h>

char text_wholefile[Gfilesize];
GCodeARRAY_STRUCT GfileFloatKerf[GfileTotalRows];
bool is_absolute = false;

void ParseGCodeFile(const std::vector<std::string> &code_lines) {
  memset(GfileFloatKerf, 0, sizeof(GCodeARRAY_STRUCT) * GfileTotalRows);

  double start_X = 0.;
  double start_Y = 0.;

  for (size_t i = 0; i < code_lines.size(); i++) {
    GCodeARRAY_STRUCT code_array;
    code_array.X0 = code_array.X = start_X;
    code_array.Y0 = code_array.Y = start_Y;

    std::string code_type = code_lines[i].substr(0, 3);
    if (code_type.compare("G00") == 0) {
      code_array.Name = G00;
      code_array.X = is_absolute ? GetCodeValue(code_lines[i], "X") : start_X + GetCodeValue(code_lines[i], "X");
      code_array.Y = is_absolute ? GetCodeValue(code_lines[i], "Y") : start_Y + GetCodeValue(code_lines[i], "Y");

      code_array.X0 = start_X;
      code_array.Y0 = start_Y;
      start_X = code_array.X;
      start_Y = code_array.Y;

      code_array.Length = sqrt(pow(code_array.X - code_array.X0, 2)
          + pow(code_array.Y - code_array.Y0, 2));

    } else if (code_type.compare("G01") == 0) {
      code_array.Name = G01;
      code_array.X = is_absolute ? GetCodeValue(code_lines[i], "X") : start_X + GetCodeValue(code_lines[i], "X");
      code_array.Y = is_absolute ? GetCodeValue(code_lines[i], "Y") : start_Y + GetCodeValue(code_lines[i], "Y");
      code_array.F = GetCodeValue(code_lines[i], "F");

      code_array.X0 = start_X;
      code_array.Y0 = start_Y;
      start_X = code_array.X;
      start_Y = code_array.Y;

      code_array.Length = sqrt(pow(code_array.X - code_array.X0, 2)
          + pow(code_array.Y - code_array.Y0, 2));

    } else if (code_type.compare("G02") == 0) {
      code_array.Name = G02;
      code_array.X = is_absolute ? GetCodeValue(code_lines[i], "X") : start_X + GetCodeValue(code_lines[i], "X");
      code_array.Y = is_absolute ? GetCodeValue(code_lines[i], "Y") : start_Y + GetCodeValue(code_lines[i], "Y");
      code_array.I = is_absolute ? GetCodeValue(code_lines[i], "I") : start_X + GetCodeValue(code_lines[i], "I");
      code_array.J = is_absolute ? GetCodeValue(code_lines[i], "J") : start_Y + GetCodeValue(code_lines[i], "J");
      code_array.F = GetCodeValue(code_lines[i], "F");

      code_array.X0 = start_X;
      code_array.Y0 = start_Y;
      start_X = code_array.X;
      start_Y = code_array.Y;

      code_array.R = GetRadius(code_array);
      code_array.Length = 2 * code_array.R;
    } else if (code_type.compare("G03") == 0) {
      code_array.Name = G03;
      code_array.X = is_absolute ? GetCodeValue(code_lines[i], "X") : start_X + GetCodeValue(code_lines[i], "X");
      code_array.Y = is_absolute ? GetCodeValue(code_lines[i], "Y") : start_Y + GetCodeValue(code_lines[i], "Y");
      code_array.I = is_absolute ? GetCodeValue(code_lines[i], "I") : start_X + GetCodeValue(code_lines[i], "I");
      code_array.J = is_absolute ? GetCodeValue(code_lines[i], "J") : start_Y + GetCodeValue(code_lines[i], "J");
      code_array.F = GetCodeValue(code_lines[i], "F");

      code_array.X0 = start_X;
      code_array.Y0 = start_Y;
      start_X = code_array.X;
      start_Y = code_array.Y;

      code_array.R = GetRadius(code_array);
      code_array.Length = 2 * code_array.R;
    } else if (code_type.compare("G41") == 0) {
      code_array.Name = G41;
    } else if (code_type.compare("G42") == 0) {
      code_array.Name = G42;
    } else if (code_type.compare("G40") == 0) {
      code_array.Name = G40;
    } else if (code_type.compare("G04") == 0) {
      code_array.Name = G04;
      code_array.Delay = GetCodeValue(code_lines[i], "F");
    } else if (code_type.compare("G20") == 0) {
      code_array.Name = G20;
    } else if (code_type.compare("G21") == 0) {
      code_array.Name = G21;
    } else if (code_type.compare("G90") == 0) {
      code_array.Name = G90;
      is_absolute = true;
    } else if (code_type.compare("G91") == 0) {
      code_array.Name = G91;
      is_absolute = false;
    } else if (code_type.compare("G99") == 0) {
      code_array.Name = G99;
      code_array.ScaleFactor = GetCodeValue(code_lines[i], "X");
      code_array.RotateAngle = GetCodeValue(code_lines[i], "Y");
      code_array.HorizonMirror = GetCodeValue(code_lines[i], "I");
      code_array.VerticalMirror = GetCodeValue(code_lines[i], "J");
    } else if (code_type.compare("M00") == 0) {
      code_array.Name = M00;
    } else if (code_type.compare("M02") == 0) {
      code_array.Name = M02;
    } else if (code_type.compare("M07") == 0) {
      code_array.Name = M07;
    } else if (code_type.compare("M08") == 0) {
      code_array.Name = M08;
    } else if (code_type.compare("M09") == 0) {
      code_array.Name = M09;
    } else if (code_type.compare("M10") == 0) {
      code_array.Name = M10;
    } else if (code_type.compare("M11") == 0) {
      code_array.Name = M11;
    } else if (code_type.compare("M12") == 0) {
      code_array.Name = M12;
    }

    GfileFloatKerf[i] = code_array;
  }
}

double GetCodeValue(const std::string &code_line, const std::string &match) {
  std::size_t pos = code_line.find(match);
  if (pos == std::string::npos) {
    return 0.;
  }
  std::string value = code_line.substr(pos + 1);
  char *endptr;
  return strtod(value.c_str(), &endptr);
}

double GetRadius(const GCodeARRAY_STRUCT &pG) {
	return sqrt(pow(pG.X-pG.I,2)+pow(pG.Y-pG.J,2));
}

bool ReadGCodeFile(const std::string &file, std::vector<std::string> &code_lines) {
  std::ifstream in_file(file.c_str());
  if (!in_file) {
    return false;
  }
  std::string line_text = "";
  while (getline(in_file, line_text)) {
    code_lines.push_back(line_text);
  }

  in_file.close();
  in_file.clear();

  return true;
}

bool WriteGCodeFile(const std::string &file, const std::vector<std::string> &code_lines) {
  std::ofstream out_file(file.c_str());
  if (!out_file) {
    return false;
  }
  for (size_t i = 0; i < code_lines.size(); i++) {
    out_file << code_lines[i] << "\n";
  }
  out_file.close();
  out_file.clear();
  return true;
}

void GenerateKerfGCode(std::vector<std::string> &code_lines) {
  code_lines.clear();
  for (size_t i = 0; GfileFloatKerf[i].Name != M02; i++) {
    code_lines.push_back(TransformGCodeLine(GfileFloatKerf[i]));
  }
  code_lines.push_back("M02");
}

std::string TransformGCodeLine(const GCodeARRAY_STRUCT &gcode_array) {
  char GCODE[100];
  switch (gcode_array.Name) {
   case G00:
    sprintf_s(GCODE, "G00 X%.3lf Y%.3lf",
        is_absolute ? gcode_array.X : gcode_array.X - gcode_array.X0,
        is_absolute ? gcode_array.Y : gcode_array.Y - gcode_array.Y0);

    break;
   case G01:
    sprintf_s(GCODE, "G01 X%.3lf Y%.3lf F%.3lf",
        is_absolute ? gcode_array.X : gcode_array.X - gcode_array.X0,
        is_absolute ? gcode_array.Y : gcode_array.Y - gcode_array.Y0,
        gcode_array.F);

    break;
   case G02:
    sprintf_s(GCODE, "G02 X%.3lf Y%.3lf I%.3lf J%.3lf F%.3lf",
        is_absolute ? gcode_array.X : gcode_array.X - gcode_array.X0,
        is_absolute ? gcode_array.Y : gcode_array.Y - gcode_array.Y0,
        is_absolute ? gcode_array.I : gcode_array.I - gcode_array.X0,
        is_absolute ? gcode_array.J : gcode_array.J - gcode_array.Y0,
        gcode_array.F);

    break;
   case G03:
    sprintf_s(GCODE, "G03 X%.3lf Y%.3lf I%.3lf J%.3lf F%.3lf",
        is_absolute ? gcode_array.X : gcode_array.X - gcode_array.X0,
        is_absolute ? gcode_array.Y : gcode_array.Y - gcode_array.Y0,
        is_absolute ? gcode_array.I : gcode_array.I - gcode_array.X0,
        is_absolute ? gcode_array.J : gcode_array.J - gcode_array.Y0,
        gcode_array.F);

    break;
   case G04:
    sprintf_s(GCODE, "G04 F%.3lf", gcode_array.Delay);
    break;
   case G40:
    sprintf_s(GCODE, "G40");
    break;
   case G41:
    sprintf_s(GCODE, "G41");
    break;
   case G42:
    sprintf_s(GCODE, "G42");
    break;
   case G20:
    sprintf_s(GCODE, "G20");
    break;
   case G21:
    sprintf_s(GCODE, "G21");
    break;
   case G90:
    sprintf_s(GCODE, "G90");
    break;
   case G91:
    sprintf_s(GCODE, "G91");
    break;
   case G99:
    sprintf_s(GCODE, "G99 X%.3lf Y%.3lf I%.3lf J%.3lf",
        gcode_array.ScaleFactor, gcode_array.RotateAngle,
        gcode_array.HorizonMirror, gcode_array.VerticalMirror);

    break;
   case M00:
    sprintf_s(GCODE, "M00");
    break;
   case M02:
    sprintf_s(GCODE, "M02");
    break;
   case M07:
    sprintf_s(GCODE, "M07");
    break;
   case M08:
    sprintf_s(GCODE, "M08");
    break;
   case M09:
    sprintf_s(GCODE, "M09");
    break;
   case M10:
    sprintf_s(GCODE, "M10");
    break;
   case M11:
    sprintf_s(GCODE, "M11");
    break;
   case M12:
    sprintf_s(GCODE, "M12");
    break;
   default:
    break;
  }
  return std::string(GCODE);
}

double GetGCodeLength(GCodeARRAY_STRUCT *GCodeArryPtr) {
  return sqrt(pow((GCodeArryPtr->X-GCodeArryPtr->X0), 2) + pow((GCodeArryPtr->Y-GCodeArryPtr->Y0), 2));
}

void FitSmallLine(GCodeARRAY_STRUCT *Ptr) {
  double ArcLimit, TotalLen, Len;
  GCodeARRAY_STRUCT *start_line, *next_line, *new_start_line, *new_next_line;
  char *fit_flag;

  //double MINFITLENGTH = AllPara_New.SystemPara.MaxCutSpeed/60*INTERPOLATION_T/2;
  double MINFITLENGTH = 2;
  if (MINFITLENGTH > 1.0) MINFITLENGTH = 1;
  ArcLimit = MINFITLENGTH;

//小圆弧拟合
	//start_line = Ptr;
	//while (start_line->Name != M02)
	//{
	//	if ((start_line->Name==G02 || start_line->Name==G03)
 //       && (start_line->Length <= ArcLimit || start_line->R <= ArcLimit/2 || start_line->R <= AllPara_New.GeneralPara.KerfValue))
	//	{
	//		start_line->Name = G01;
	//		start_line->R = 0;
	//		start_line->I = 0;
	//		start_line->J = 0;
	//		start_line->StartAngle = 0;
	//		start_line->EndAngle = 0;
	//		start_line->Length = GetGCodeLength(start_line);
	//	}
	//	start_line++;
	//}
//小线段拟合
	start_line = next_line = Ptr;
  fit_flag = text_wholefile;
	while(start_line->Name!=M02)
	{
    *(fit_flag++) = 0;
		//合并小线段，在规定的精度和步数内
		next_line = start_line + 1; 				
		TotalLen = start_line->Length;
		if (start_line->Name == G01) 	
    {
			while (TotalLen < MINFITLENGTH && next_line->Name==G01)
			{
				TotalLen = sqrt(pow(start_line->X0-next_line->X, 2) + pow(start_line->Y0-next_line->Y, 2));	//实际拟合后的长度
				next_line++;
			}
			if (TotalLen >= MINFITLENGTH && next_line->Name == G01)		//判断下次拟合的长度
			{
				new_start_line = next_line;
				new_next_line = new_start_line + 1;
				Len = new_start_line->Length;
				if (Len < MINFITLENGTH && new_start_line->Name == G01)
				{
					while (Len < MINFITLENGTH && new_next_line->Name == G01)
					{
						Len = sqrt(pow(new_start_line->X0-new_next_line->X, 2) + pow(new_start_line->Y0-new_next_line->Y, 2)); //实际拟合后的长度
						new_next_line++;
					}
				}
				if (Len < MINFITLENGTH && Len > 0)					//下次拟合达不到规定的长度
					next_line = new_next_line;
			}
    }
		next_line--;
		if(next_line > start_line)
		{
      memset(fit_flag, 1, (next_line - start_line) * sizeof(char));
      fit_flag += (next_line - start_line);

		  start_line->X = next_line->X;
		  start_line->Y = next_line->Y;
      start_line = next_line;
		}
		start_line++;
	}
  // 整合拟合后的结构体数组
  start_line = Ptr;
  new_start_line = Ptr;
  fit_flag = text_wholefile;
  while (new_start_line->Name != M02) {
    if (*(fit_flag++) != 0) {
      start_line++;
      continue;
    }
    *(new_start_line++) = *(start_line++);
  }
  *new_start_line = *start_line;
}

/*
#define STRUCT_DATA_FILE_PATH "/flash/GCodeStructData.txt"

void FitSmallLine(GCodeARRAY_STRUCT *Ptr) {
  	double ArcLimit, TotalLen, Len;
  	GCodeARRAY_STRUCT *start_line, *next_line, *new_start_line, *new_next_line;
  	GCodeARRAY_STRUCT *dst_ptr, *src_ptr;
  	double MINFITLENGTH = AllPara_New.SystemPara.MaxCutSpeed/60*INTERPOLATION_T/2;
	U32 NoKerfUsedNum = 0, NoKerfUsedFlag=0;
	
  	if (MINFITLENGTH > 1.0) MINFITLENGTH = 1;
  	ArcLimit = MINFITLENGTH;

	//计算GfileFloatNoKerf已经使用的大小
	start_line = GfileFloatNoKerf;
	NoKerfUsedNum = 1;
	while (start_line->Name != M02)
	{
		start_line++;
		NoKerfUsedNum++;
	}

	NoKerfUsedFlag = 0;						//从GfileFloatNoKerf中中间开始使用
	if (NoKerfUsedNum > GfileTotalRows/2)		//使用量超过了Gfilesize的一半
	{
  		// 保存GfileFloatNoKerf数据，再清空GfileFloatNoKerf
		yaffs_SaveFile((char *)STRUCT_DATA_FILE_PATH, (char *)GfileFloatNoKerf, O_CREAT | O_TRUNC| O_RDWR, S_IREAD | S_IWRITE, sizeof(GCodeARRAY_STRUCT) * NoKerfUsedNum);
  		memset(GfileFloatNoKerf, 0, sizeof(GCodeARRAY_STRUCT) * GfileTotalRows);
		NoKerfUsedFlag = 1;					//从GfilefloatNokerf的开头使用
	}

  	// 小圆弧拟合, 同时将GfileFloatKerf转移到GfileFloatNoKerf中去，再清空GfileFloatKerf
  	src_ptr = Ptr;
  	dst_ptr = NoKerfUsedFlag==0?(GfileFloatNoKerf+GfileTotalRows/2):GfileFloatNoKerf;
  	while (src_ptr->Name != M02) 
	{
    	if ((src_ptr->Name  == G02 || src_ptr->Name  == G03) &&
        	(src_ptr->Length <= ArcLimit || src_ptr->R <= ArcLimit / 2 || src_ptr->R <= AllPara_New.GeneralPara.KerfValue/2))
      {
          src_ptr->Name = G01;
          src_ptr->R = 0;
          src_ptr->I = 0;
          src_ptr->J = 0;
          src_ptr->StartAngle = 0;
          src_ptr->EndAngle = 0;
          src_ptr->Length = GetGCodeLength(src_ptr);
    	}

    	*(dst_ptr++) = *(src_ptr++);
  	}
	*(dst_ptr++) = *(src_ptr++);											//复制M02
  	memset(GfileFloatKerf, 0, sizeof(GCodeARRAY_STRUCT) * GfileTotalRows);

  	//  小线段拟合
  	dst_ptr = GfileFloatKerf;
  	start_line = next_line = NoKerfUsedFlag==0?(GfileFloatNoKerf+GfileTotalRows/2):GfileFloatNoKerf;;
  	while (start_line->Name != M02) 
	{
    	next_line = start_line + 1;

    	if (start_line->Name == G01) 
		{
      		TotalLen = start_line->Length;
      		while (TotalLen < MINFITLENGTH && next_line->Name == G01)
			{
				TotalLen = sqrt(pow(start_line->X0-next_line->X, 2) + pow(start_line->Y0-next_line->Y, 2));	//实际拟合后的长度
				next_line++;
			}

			if (TotalLen >= MINFITLENGTH && next_line->Name==G01)		//判断下次拟合的长度
			{
				new_start_line = next_line;
				new_next_line = new_start_line + 1;

				Len = new_start_line->Length;
				if (Len <MINFITLENGTH && new_start_line->Name == G01)
				{
					while (Len< MINFITLENGTH && new_next_line->Name==G01)
					{
						Len = sqrt(pow(new_start_line->X0-new_next_line->X, 2) + pow(new_start_line->Y0-new_next_line->Y, 2)); //实际拟合后的长度
						new_next_line++;
					}
				}		
        		if (Len < MINFITLENGTH && Len > 0) 
				{
          			next_line = new_next_line;
        		}
      		}
      		next_line--;
      		if (next_line > start_line) 
			{												//此时的next_line一定是 G01
        		if (next_line->Name == M02) 
        		{
          			start_line->X = next_line->X0;
          			start_line->Y = next_line->Y0;
        		} 
				else 
				{
          			if (next_line->Name == G01) 		
					{
            			start_line->X = next_line->X;
            			start_line->Y = next_line->Y;
          			} 
					else 
					{
            			start_line->X = next_line->X0;
           	 			start_line->Y = next_line->Y0;
          			}
        		}
				*(dst_ptr++) = *start_line;
        		start_line = next_line;
      		} 
			else 
			{
				*(dst_ptr++) = *start_line;  
	  		}
    	} 
		else 
		{
			*(dst_ptr++) = *start_line;
		}
    	start_line++;
  	}

  	// 恢复GfileFloatNoKerf数据
  	if (NoKerfUsedFlag == 1)
  	{
		yaffs_ReadParaFile((char *)STRUCT_DATA_FILE_PATH, (char *)GfileFloatNoKerf);
  	}
}

*/
