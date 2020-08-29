#include "main.h"
extern int auton;
extern lv_obj_t * xLabel;
extern lv_obj_t * yLabel;
extern lv_obj_t * thetaLabel;
extern lv_obj_t * debugLabel1;
extern lv_obj_t * debugLabel2;
extern lv_obj_t * debugLabel3;
extern lv_obj_t * debugLabel4;
extern lv_obj_t * debugLabel5;
extern lv_obj_t * debugLabel6;
extern lv_obj_t * createBtn(lv_obj_t * parent, lv_coord_t x, lv_coord_t y, lv_coord_t width, lv_coord_t height, int id, const char* title);
extern lv_obj_t * createTextLabel(lv_obj_t * parent, const char* text, lv_coord_t x, lv_coord_t y);
extern void updateVarLabel(lv_obj_t * parent, std::string varName, double var, std::string varUnits);
extern void initDebugLabels();
extern void clearDebugLabels();
extern void initGUI(void*p);
