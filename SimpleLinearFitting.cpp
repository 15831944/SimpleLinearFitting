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

/**************************************************************************************************
函数功能: 小线段合并到圆弧上

返回值:
	0 : 成功
	-1: 失败

注意:
	(1): 小线段合并到圆弧上时，必须保证圆弧的弦长大于小线段的长度，否则有可能导致圆弧起末点发生变化
		 使整圆变成小圆弧，或者小圆弧变成整圆，这里取1.5倍弦长
	(2): 合并后圆心偏差超过精度范围，则取消合并。
**************************************************************************************************/
int small_line_merge_to_arc (GCodeARRAY_STRUCT *PtrLine, GCodeARRAY_STRUCT *PtrArc)
{
	GCodeARRAY_STRUCT NewArc;
	double r1, r2;
	double arc_chord_length;				//圆弧弦长
	double line_length;						//线段长度
	double length;
  
  NewArc = *PtrArc;
  //1. 检查弦长是否小于小线段长度
	arc_chord_length = sqrt (pow (NewArc.X0 - NewArc.X, 2) + pow (NewArc.Y0 - NewArc.Y, 2));
  line_length = sqrt (pow (PtrLine->X0 - PtrLine->X, 2) + pow (PtrLine->Y0 - PtrLine->Y, 2));
  
  if (IsLesser (1.5 * arc_chord_length, line_length))
		return -1;								//圆弧弦长小于线段长度，拟合后圆弧可能会出错，整圆变成小圆弧，或者小圆弧变成整圆。

  //2. 检查直线和圆弧是否相接    
	if (IsEqual (PtrLine->X0, NewArc.X) && IsEqual (PtrLine->Y0, NewArc.Y))
	{
    NewArc.X = PtrLine->X;
		NewArc.Y = PtrLine->Y;
  }
  else if (IsEqual (PtrLine->X, NewArc.X0) && IsEqual (PtrLine->Y, NewArc.Y0))
	{
		NewArc.X0 = PtrLine->X0;
		NewArc.Y0 = PtrLine->Y0;
	}
  else
    return -1;								//直线和圆弧不相接，无法合并
  
  //3. 合并重新计算圆心，圆心在精度范围内，则合并成功
  r1 = sqrt ((NewArc.X0 - NewArc.I) * (NewArc.X0 - NewArc.I) + (NewArc.Y0 - NewArc.J) * (NewArc.Y0 - NewArc.J));		//计算起点相对圆心半径
  r2 = sqrt ((NewArc.X - NewArc.I) * (NewArc.X - NewArc.I) + (NewArc.Y - NewArc.J) * (NewArc.Y - NewArc.J));			//计算末点相对圆心半径
  if (calculate_arc_center (&NewArc, r1, r2, arc_center_adjust_accuracy) == 0)
        *PtrArc = NewArc;					    //计算成功	
	else
		return -1;

	return 0;
  
}

/**************************************************************************************************
拟合圆弧之间的小线段

参数:
    fit_line_accuracy  : 小线段长度阈值
    not_fit_min_length : 未拟合的线段最小长度
    not_fit_cnt        : 未拟合的小线段个数
    fit_to_arc_cnt     : 拟合进圆弧的小线段个数

优先选择半径与弦长比值较小的圆弧拟合
**************************************************************************************************/
void fit_small_line_into_circle(GCodeARRAY_STRUCT *Ptr, double fit_line_accuracy, double *not_fit_min_length, U32 *not_fit_cnt, U32 *fit_to_arc_cnt)
{
  GCodeARRAY_STRUCT *PtrHeader = Ptr, *NextPtr, *PrePtr, *PtrArc;
	double line_not_fit_min_length = fit_line_accuracy;
	int line_not_fit_cnt = 0, line_fit_to_arc_cnt = 0;
  
  if (Ptr->Name == M02)
    return;

  //1. 小线段合并到圆弧上
	PrePtr = Ptr;
	Ptr++;
	NextPtr = Ptr+1;
	while (Ptr->Name != M02 && NextPtr != M02)
  {
    if (Ptr->Name == G01)
		{
			Ptr->Length = sqrt (pow (Ptr->X - Ptr->X0, 2) + pow (Ptr->Y - Ptr->Y0, 2));
			if (IsLesser (Ptr->Length, fit_line_accuracy))				//线段长度不足
			{
        //前一段是圆弧
        if ((PrePtr->Name == G02 || PrePtr->Name == G03) &&
           (small_line_merge_to_arc (Ptr, PrePtr) == 0))       //合并到前一个圆弧成功
        {
          Ptr->X0 = Ptr->X;
					Ptr->Y0 = Ptr->Y;
					Ptr->Name = GGG;							        //删去G01
					line_fit_to_arc_cnt++;
        }
        else if ((NextPtr->Name == G02 || NextPtr->Name == G03) &&
                 (small_line_merge_to_arc (Ptr, NextPtr) == 0)) //合并到后一个圆弧成功
        {
          Ptr->X = Ptr->X0;
					Ptr->Y = Ptr->Y0;
					Ptr->Name = GGG;                                    //删去G01
					line_fit_to_arc_cnt++;
        }
        else
        {
          if (IsLesser (Ptr->Length, line_not_fit_min_length))
            line_not_fit_min_length = Ptr->Length;
          line_not_fit_cnt++;								    //无法合并
        }
      }
    }
    PrePtr = Ptr;
		Ptr++;
		NextPtr = Ptr+1;
  }
  //2. 删除无效的线段
  Ptr = PtrHeader;
	while (Ptr->Name != M02)
	{
		if (Ptr->Name == GGG)
    {
      Ptr++;														//跳过无效的指令
			continue;
    }
    *(PtrHeader++) = *(Ptr++);
  }
  *PtrHeader = *Ptr;													//拷贝M02
  
	if (not_fit_cnt != NULL)
		  *not_fit_cnt = line_not_fit_cnt;
  if (fit_to_arc_cnt != NULL)
      *fit_to_arc_cnt = line_fit_to_arc_cnt;
  if (not_fit_min_length != NULL)
      *not_fit_min_length = line_not_fit_min_length;      
}

/****************************************************************************************
将小圆弧拟合成小线段
****************************************************************************************/
void fit_small_arc (GCodeARRAY_STRUCT *Ptr)
{
  while (Ptr->Name != M02)
  {
    if (Ptr->Name == G02 || Ptr->Name == G03)
    {
      // calculate_arc_length_radius_and_angle (Ptr);
      double alpha = (Ptr->Name == G02) ? (Ptr->StartAngle - Ptr->EndAngle) : (Ptr->EndAngle - Ptr->StartAngle);
      if (is_arc_in_fit_to_line_accuracy (Ptr->R, alpha, arc_fit_to_line_accuracy) == 0)
      {
         Ptr->Name = G01;
      }
    }
    Ptr++;
  }
}

/**********************************************************************************************
函数功能: 小线段拟合

拟合步骤:
    1. 小圆弧拟合成线段
    2. 低于阈值的小线段合并到直线上
    3. 低于阈值的小线段合并到圆弧上
**********************************************************************************************/
void FitSmallLine(GCodeARRAY_STRUCT *Ptr) {
  double ArcLimit, TotalLen, Len;
  GCodeARRAY_STRUCT *start_line, *next_line, *new_start_line, *new_next_line;
  char *fit_flag;
  double not_fit_min_length;
  unsigned int not_fit_cnt, fit_to_arc_cnt;
  //double MINFITLENGTH = AllPara_New.SystemPara.MaxCutSpeed/60*INTERPOLATION_T/2;
  double MINFITLENGTH = 2;
  if (MINFITLENGTH > 1.0) MINFITLENGTH = 1;
  ArcLimit = MINFITLENGTH;

//1. 小圆弧拟合成小线段
  fit_small_arc(Ptr);

//2. 计算小线段拟合的最大长度
	if (MINFITLENGTH > 1.0) MINFITLENGTH = 1;
	#if (KERFVALUE_TO_SMALLLINELEN == 1)
  if (MINFITLENGTH < AllPara_New.GeneralPara.KerfValue)			//小于割缝值
	{
		start_line = Ptr;
		while (start_line->Name!=M02)								//检查代码中是否需要加割缝
		{
      if (start_line->Name == G41 || start_line->Name == G42)	//需要加割缝
			{
				MINFITLENGTH = AllPara_New.GeneralPara.KerfValue;	//小线段拟合的长度不能低于割缝的长度
			}
      start_line++;
    }
  }
  #endif

//3. 小线段拟合
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

  //4. 整合拟合后的结构体数组
  start_line = Ptr;
  new_start_line = Ptr;
  fit_flag = text_wholefile;
  while (start_line->Name != M02) {
    if (*(fit_flag++) != 0) {
      start_line++;
      continue;
    }
    *(new_start_line++) = *(start_line++);
  }
  *new_start_line = *start_line; // 拷贝M02
  
  //5. 将剩余的小线段拟合进圆弧
  fit_small_line_into_circle (Ptr, MINFITLENGTH, &not_fit_min_length, &not_fit_cnt, &fit_to_arc_cnt);			//拟合圆弧之间的小线段
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
