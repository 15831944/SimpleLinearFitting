#ifndef GCODESTRUCT_H__
#define GCODESTRUCT_H__

typedef enum _GMID {
  M02,
  G00,
  G01,
  G02,
  G03,
  G04,
  G40,
  G41,
  G42,
  G20,
  G21,
  G26,
  G27,
  G28,
  G90,
  G91,
  G92,
  G99,
  M00,
  M07,
  M08,
  M09,
  M10,
  M11,
  M12,

} GMID;

typedef struct {
  unsigned int ShowLine;
  unsigned int LineNoInTotalFile;
  unsigned short Name;
  unsigned short PierceHoleNum;
  unsigned short CycleCount;
  double X0;
  double Y0;
  double X;
  double Y;
  double I;
  double J;
  double F;
  double R;
  double StartAngle;
  double EndAngle;
  double Delay;
  double KerfValue;
  double Length; // current g code line length
  double AngleRatio; // ĩ���������һ����������ļнǵı仯�� 
  double ScaleFactor;
  double RotateAngle;
  double HorizonMirror;
  double VerticalMirror;
} GCodeARRAY_STRUCT;

typedef struct {
	double g_f_XMaxLimit;  //+X�����ֵ
	double g_f_XMinLimit;  //-X����Сֵ
	double g_f_YMaxLimit; //+Y�����ֵ
	double g_f_YMinLimit;//-Y����Сֵ
	double ScaleXY;
	double g_GraphicX_start;
	double g_GraphicY_start;
	double MaxRadius;
	double MinRadius;
} GraphyLimit;

typedef struct {
  double x;
  double y;
  double r;
} Circle_t;

typedef struct {
  double x;
  double y;
} Point_t;

typedef struct {
  double X0;
  double Y0;
  double X;
  double Y;
} Line_t;

#endif // GCODESTRUCT_H__
