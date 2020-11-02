#include "main.h"
#include "display/lvgl.h"

int auton = 0;
const int NUM_OF_AUTONS = 4;
const std::string AUTON_HEADING = "AUTON SELECTED: ";

LV_FONT_DECLARE(armadura_solid);
LV_FONT_DECLARE(armadura_solid_big);

const int DEBUG_X_1 = 75;
const int DEBUG_X_2 = 300;
lv_obj_t * xLabel;
lv_obj_t * yLabel;
lv_obj_t * thetaLabel;
lv_obj_t * debugLabel1;
lv_obj_t * debugLabel2;
lv_obj_t * debugLabel3;
lv_obj_t * debugLabel4;
lv_obj_t * debugLabel5;
lv_obj_t * debugLabel6;

lv_obj_t * xValue;
lv_obj_t * yValue;
lv_obj_t * thetaValue;
lv_obj_t * debugValue1;
lv_obj_t * debugValue2;
lv_obj_t * debugValue3;
lv_obj_t * debugValue4;
lv_obj_t * debugValue5;
lv_obj_t * debugValue6;

lv_obj_t * autonLabel;
lv_obj_t * autonSelect;
lv_obj_t * autonNext_button;
lv_obj_t * autonPrev_button;

lv_obj_t * imuLabel;
lv_obj_t * imuButton;

lv_obj_t * nameLabel;
lv_obj_t * img;

lv_obj_t * sensorDebug_button;
lv_obj_t * autonDebug_button;
lv_obj_t * deviceDebug_button;
lv_obj_t * back_button;

lv_obj_t * sensorLabel1;
lv_obj_t * sensorLabel2;
lv_obj_t * sensorLabel3;
lv_obj_t * sensorLabel4;
lv_obj_t * sensorLabel5;
lv_obj_t * sensorLabel6;
lv_obj_t * sensorLabel7;
lv_obj_t * sensorLabel8;
lv_obj_t * sensorLabel9;

lv_obj_t * sensorValue1;
lv_obj_t * sensorValue2;
lv_obj_t * sensorValue3;
lv_obj_t * sensorValue4;
lv_obj_t * sensorValue5;
lv_obj_t * sensorValue6;
lv_obj_t * sensorValue7;
lv_obj_t * sensorValue8;
lv_obj_t * sensorValue9;

lv_obj_t * deviceLabel1;
lv_obj_t * deviceLabel2;
lv_obj_t * deviceLabel3;
lv_obj_t * deviceLabel4;
lv_obj_t * deviceLabel5;
lv_obj_t * deviceLabel6;
lv_obj_t * deviceLabel7;
lv_obj_t * deviceLabel8;

lv_obj_t * deviceVel1;
lv_obj_t * deviceVel2;
lv_obj_t * deviceVel3;
lv_obj_t * deviceVel4;
lv_obj_t * deviceVel5;
lv_obj_t * deviceVel6;
lv_obj_t * deviceVel7;
lv_obj_t * deviceVel8;

lv_obj_t * deviceTemp1;
lv_obj_t * deviceTemp2;
lv_obj_t * deviceTemp3;
lv_obj_t * deviceTemp4;
lv_obj_t * deviceTemp5;
lv_obj_t * deviceTemp6;
lv_obj_t * deviceTemp7;
lv_obj_t * deviceTemp8;

lv_obj_t * devicePower1;
lv_obj_t * devicePower2;
lv_obj_t * devicePower3;
lv_obj_t * devicePower4;
lv_obj_t * devicePower5;
lv_obj_t * devicePower6;
lv_obj_t * devicePower7;
lv_obj_t * devicePower8;

lv_anim_t a;

static lv_style_t style1;
static lv_style_t style2;
static lv_style_t style3;
static lv_style_t style4;

//General functions ==================================================================================================================================
void initStyles()
{
  lv_style_copy(&style1, &lv_style_plain);
  style1.text.color = LV_COLOR_WHITE;
  style1.text.font = &armadura_solid;

  lv_style_copy(&style4, &lv_style_plain);
  style4.text.color = LV_COLOR_WHITE;
  style4.text.font = &armadura_solid_big;

  lv_style_copy(&style2, &lv_style_btn_rel);
  style2.body.border.color = LV_COLOR_GREEN;
  style2.body.main_color = LV_COLOR_BLACK;
  style2.body.grad_color = LV_COLOR_BLACK;

  lv_style_copy(&style3, &lv_style_btn_pr);
  style3.body.main_color = LV_COLOR_GREEN;
  style3.body.grad_color = LV_COLOR_GREEN;
}

lv_obj_t* createBtn(lv_obj_t * parent, lv_coord_t x, lv_coord_t y, lv_coord_t width, lv_coord_t height, int id, const char* title)
{
  parent = lv_btn_create(lv_scr_act(), NULL);
  lv_obj_set_pos(parent, x, y);
  lv_obj_set_size(parent,width,height);
  lv_btn_set_style(parent,LV_BTN_STYLE_REL, &style2);
  lv_btn_set_style(parent,LV_BTN_STYLE_PR, &style3);
  lv_obj_set_free_num(parent, id);

  lv_obj_t * label = lv_label_create(parent, NULL);
  lv_label_set_style(label, &style1);
  lv_label_set_text(label, title);

  return parent;
}

lv_obj_t* createBtn(lv_obj_t * parent, lv_coord_t x, lv_coord_t y, lv_coord_t width, lv_coord_t height, int id)
{
  parent = lv_btn_create(lv_scr_act(), NULL);
  lv_obj_set_pos(parent, x, y);
  lv_obj_set_size(parent,width,height);
  lv_btn_set_style(parent,LV_BTN_STYLE_REL, &style2);
  lv_btn_set_style(parent,LV_BTN_STYLE_PR, &style3);
  lv_obj_set_free_num(parent, id);

  return parent;
}

lv_obj_t* createBtnLabel(lv_obj_t* label, lv_obj_t * parent, const char* title)
{
  label = lv_label_create(parent, NULL);
  lv_label_set_style(label, &style1);
  lv_label_set_text(label, title);
  return label;
}


lv_obj_t* createTextLabel(lv_obj_t *parent, const char* text, lv_coord_t x, lv_coord_t y)
{
  parent = lv_label_create(lv_scr_act(), NULL);
  lv_label_set_recolor(parent, true);
  lv_label_set_style(parent, &style1);
  lv_label_set_text(parent, text);
  lv_obj_set_pos(parent,x,y);
  return parent;
}

void updateVarLabel(lv_obj_t *parent, std::string varName, double var, std::string varUnits)
{
  std::string text = varName + " = " + std::to_string(var).substr(0,8) + " " + varUnits;
  char text_array[text.length()+1];
  strcpy(text_array,text.c_str());
  lv_label_set_text(parent, text_array);
}

void updateVarLabel(lv_obj_t *parent, std::string varName, double var, std::string varUnits, int numOfSpaces)
{
  std::string text = varName + " = " + std::to_string(var).substr(0,numOfSpaces) + " " + varUnits;
  char text_array[text.length()+1];
  strcpy(text_array,text.c_str());
  lv_label_set_text(parent, text_array);
}

void updateValueLabel(lv_obj_t *parent, double var, std::string varUnits, int ppd)
{
  std::string varString = std::to_string(var);
  if(var == 0)
    varString = varString.substr(varString.find("0"));
  int decimal_place = varString.find(".");
  if(ppd > 0)
    varString = varString.substr(0,decimal_place+ppd+1);
  else
    varString = varString.substr(0,decimal_place);
  std::string text = varString + " " + varUnits;
  char text_array[text.length()+1];
  strcpy(text_array,text.c_str());
  lv_label_set_text(parent, text_array);
}

void updateVarLabel(lv_obj_t *nameLabel, std::string varName, lv_obj_t* valueLabel, double var, std::string varUnits, int ppd)
{
  char text_array[varName.length()+1];
  strcpy(text_array,varName.c_str());
  lv_label_set_text(nameLabel, text_array);

  std::string varString = std::to_string(var);
  int decimal_place = varString.find(".");
  if(ppd > 0)
    varString = varString.substr(0,decimal_place+ppd+1);
  else
    varString = varString.substr(0,decimal_place);
  std::string text = varString + " " + varUnits;
  char text_array2[text.length()+1];
  strcpy(text_array2,text.c_str());
  lv_label_set_text(valueLabel, text_array2);
}

void animate_x_ease_out(lv_obj_t *obj,int x1,int x2,int duration)
{

  a.var = obj;                             /*Variable to animate*/
  a.start = x1;                                 /*Start value*/
  a.end = x2;                                 /*End value*/
  a.fp = (lv_anim_fp_t)lv_obj_set_x;                                 /*Function to be used to animate*/
  a.path = lv_anim_path_ease_out;                                            /*Path of animation*/
  a.end_cb = NULL;                               /*Callback when the animation is ready*/
  a.act_time = 0;                                 /*Set < 0 to make a delay [ms]*/
  a.time = duration;                                 /*Animation length [ms]*/
  a.playback = 0;                                 /*1: animate in reverse direction too when the normal is ready*/
  a.playback_pause = 0;                                 /*Wait before playback [ms]*/
  a.repeat = 0;                                 /*1: Repeat the animation (with or without playback)*/
  a.repeat_pause = 0;                             /*Wait before repeat [ms]*/

  lv_anim_create(&a);                          /*Start the animation*/
}

void animate_x_linear(lv_obj_t *obj,int x1,int x2,int duration)
{

  a.var = obj;                             /*Variable to animate*/
  a.start = x1;                                 /*Start value*/
  a.end = x2;                                 /*End value*/
  a.fp = (lv_anim_fp_t)lv_obj_set_x;                                 /*Function to be used to animate*/
  a.path = lv_anim_path_linear;                                            /*Path of animation*/
  a.end_cb = NULL;                               /*Callback when the animation is ready*/
  a.act_time = 0;                                 /*Set < 0 to make a delay [ms]*/
  a.time = duration;                                 /*Animation length [ms]*/
  a.playback = 0;                                 /*1: animate in reverse direction too when the normal is ready*/
  a.playback_pause = 0;                                 /*Wait before playback [ms]*/
  a.repeat = 0;                                 /*1: Repeat the animation (with or without playback)*/
  a.repeat_pause = 0;                             /*Wait before repeat [ms]*/

  lv_anim_create(&a);                          /*Start the animation*/
}

void animate_x_ease_in(lv_obj_t *obj,int x1,int x2,int duration)
{

  a.var = obj;                             /*Variable to animate*/
  a.start = x1;                                 /*Start value*/
  a.end = x2;                                 /*End value*/
  a.fp = (lv_anim_fp_t)lv_obj_set_x;                                 /*Function to be used to animate*/
  a.path = lv_anim_path_ease_in;                                            /*Path of animation*/
  a.end_cb = NULL;                               /*Callback when the animation is ready*/
  a.act_time = 0;                                 /*Set < 0 to make a delay [ms]*/
  a.time = duration;                                 /*Animation length [ms]*/
  a.playback = 0;                                 /*1: animate in reverse direction too when the normal is ready*/
  a.playback_pause = 0;                                 /*Wait before playback [ms]*/
  a.repeat = 0;                                 /*1: Repeat the animation (with or without playback)*/
  a.repeat_pause = 0;                             /*Wait before repeat [ms]*/

  lv_anim_create(&a);                          /*Start the animation*/
}

void animate_x_ease_in_out(lv_obj_t *obj,int x1,int x2,int duration)
{

  a.var = obj;                             /*Variable to animate*/
  a.start = x1;                                 /*Start value*/
  a.end = x2;                                 /*End value*/
  a.fp = (lv_anim_fp_t)lv_obj_set_x;                                 /*Function to be used to animate*/
  a.path = lv_anim_path_ease_in_out;                                            /*Path of animation*/
  a.end_cb = NULL;                               /*Callback when the animation is ready*/
  a.act_time = 0;                                 /*Set < 0 to make a delay [ms]*/
  a.time = duration;                                 /*Animation length [ms]*/
  a.playback = 0;                                 /*1: animate in reverse direction too when the normal is ready*/
  a.playback_pause = 0;                                 /*Wait before playback [ms]*/
  a.repeat = 0;                                 /*1: Repeat the animation (with or without playback)*/
  a.repeat_pause = 0;                             /*Wait before repeat [ms]*/

  lv_anim_create(&a);                          /*Start the animation*/
}

void animate_y(lv_obj_t *obj,int y1,int y2,int duration)
{

  a.var = obj;                             /*Variable to animate*/
  a.start = y1;                                 /*Start value*/
  a.end = y2;                                 /*End value*/
  a.fp = (lv_anim_fp_t)lv_obj_set_y;                                 /*Function to be used to animate*/
  a.path = lv_anim_path_ease_out;                                            /*Path of animation*/
  a.end_cb = NULL;                               /*Callback when the animation is ready*/
  a.act_time = 0;                                 /*Set < 0 to make a delay [ms]*/
  a.time = duration;                                 /*Animation length [ms]*/
  a.playback = 0;                                 /*1: animate in reverse direction too when the normal is ready*/
  a.playback_pause = 0;                                 /*Wait before playback [ms]*/
  a.repeat = 0;                                 /*1: Repeat the animation (with or without playback)*/
  a.repeat_pause = 0;                             /*Wait before repeat [ms]*/

  lv_anim_create(&a);                          /*Start the animation*/
}

void animate_y_ease_in(lv_obj_t *obj,int y1,int y2,int duration)
{

  a.var = obj;                             /*Variable to animate*/
  a.start = y1;                                 /*Start value*/
  a.end = y2;                                 /*End value*/
  a.fp = (lv_anim_fp_t)lv_obj_set_y;                                 /*Function to be used to animate*/
  a.path = lv_anim_path_ease_in;                                            /*Path of animation*/
  a.end_cb = NULL;                               /*Callback when the animation is ready*/
  a.act_time = 0;                                 /*Set < 0 to make a delay [ms]*/
  a.time = duration;                                 /*Animation length [ms]*/
  a.playback = 0;                                 /*1: animate in reverse direction too when the normal is ready*/
  a.playback_pause = 0;                                 /*Wait before playback [ms]*/
  a.repeat = 0;                                 /*1: Repeat the animation (with or without playback)*/
  a.repeat_pause = 0;                             /*Wait before repeat [ms]*/

  lv_anim_create(&a);                          /*Start the animation*/
}

void animate_y_linear(lv_obj_t *obj,int y1,int y2,int duration)
{

  a.var = obj;                             /*Variable to animate*/
  a.start = y1;                                 /*Start value*/
  a.end = y2;                                 /*End value*/
  a.fp = (lv_anim_fp_t)lv_obj_set_y;                                 /*Function to be used to animate*/
  a.path = lv_anim_path_linear;                                            /*Path of animation*/
  a.end_cb = NULL;                               /*Callback when the animation is ready*/
  a.act_time = 0;                                 /*Set < 0 to make a delay [ms]*/
  a.time = duration;                                 /*Animation length [ms]*/
  a.playback = 0;                                 /*1: animate in reverse direction too when the normal is ready*/
  a.playback_pause = 0;                                 /*Wait before playback [ms]*/
  a.repeat = 0;                                 /*1: Repeat the animation (with or without playback)*/
  a.repeat_pause = 0;                             /*Wait before repeat [ms]*/

  lv_anim_create(&a);                          /*Start the animation*/
}


//Home Screen ==================================================================================================================================

void animateHomeScreen()
{
  animate_y(nameLabel,lv_obj_get_y(nameLabel)-272,lv_obj_get_y(nameLabel),500);
  animate_y(autonLabel,lv_obj_get_y(autonLabel)-272,lv_obj_get_y(autonLabel),500);
  animate_y(autonSelect,lv_obj_get_y(autonSelect)-272,lv_obj_get_y(autonSelect),500);
  animate_x_ease_out(autonPrev_button,lv_obj_get_x(autonPrev_button)-480,lv_obj_get_x(autonPrev_button),500);
  animate_x_ease_out(autonNext_button,lv_obj_get_x(autonNext_button)+480,lv_obj_get_x(autonNext_button),500);
  animate_y(imuButton,lv_obj_get_y(imuButton)+272,lv_obj_get_y(imuButton),500);
  animate_y(sensorDebug_button,lv_obj_get_y(sensorDebug_button)+272,lv_obj_get_y(sensorDebug_button),500);
  animate_x_ease_out(sensorDebug_button,lv_obj_get_x(sensorDebug_button)-480,lv_obj_get_x(sensorDebug_button),500);
  animate_y(autonDebug_button,lv_obj_get_y(autonDebug_button)+272,lv_obj_get_y(autonDebug_button),500);
  animate_y(deviceDebug_button,lv_obj_get_y(deviceDebug_button)+272,lv_obj_get_y(deviceDebug_button),500);
  animate_x_ease_out(deviceDebug_button,lv_obj_get_x(deviceDebug_button)+480,lv_obj_get_x(deviceDebug_button),500);
}

void animateHomeScreenLeft()
{
  animate_y_linear(nameLabel,lv_obj_get_y(nameLabel),lv_obj_get_y(nameLabel)-272,500);
  animate_y_linear(autonLabel,lv_obj_get_y(autonLabel),lv_obj_get_y(autonLabel)+272,500);
  animate_y_linear(autonSelect,lv_obj_get_y(autonSelect),lv_obj_get_y(autonSelect)+272,500);
  animate_y_linear(autonPrev_button,lv_obj_get_y(autonPrev_button),lv_obj_get_y(autonPrev_button)-272,500);
  animate_y_linear(autonNext_button,lv_obj_get_y(autonNext_button),lv_obj_get_y(autonNext_button)-272,500);
  animate_y_linear(imuButton,lv_obj_get_y(imuButton),lv_obj_get_y(imuButton)+272,500);
  animate_y_linear(sensorDebug_button,lv_obj_get_y(sensorDebug_button),lv_obj_get_y(sensorDebug_button)+272,500);
  animate_y_linear(autonDebug_button,lv_obj_get_y(autonDebug_button),lv_obj_get_y(autonDebug_button)+272,500);
  animate_y_linear(deviceDebug_button,lv_obj_get_y(deviceDebug_button),lv_obj_get_y(deviceDebug_button)+272,500);
}

void animateHomeScreenRight()
{
  animate_y(nameLabel,lv_obj_get_y(nameLabel),lv_obj_get_y(nameLabel)+272,500);
  animate_y(autonLabel,lv_obj_get_y(autonLabel),lv_obj_get_y(autonLabel)-272,500);
  animate_y(autonSelect,lv_obj_get_y(autonSelect),lv_obj_get_y(autonSelect)-272,500);
  animate_y(autonPrev_button,lv_obj_get_y(autonPrev_button),lv_obj_get_y(autonPrev_button)+272,500);
  animate_y(autonNext_button,lv_obj_get_y(autonNext_button),lv_obj_get_y(autonNext_button)+272,500);
  animate_y(imuButton,lv_obj_get_y(imuButton),lv_obj_get_y(imuButton)-272,500);
  animate_y(sensorDebug_button,lv_obj_get_y(sensorDebug_button),lv_obj_get_y(sensorDebug_button)-272,500);
  animate_y(autonDebug_button,lv_obj_get_y(autonDebug_button),lv_obj_get_y(autonDebug_button)-272,500);
  animate_y(deviceDebug_button,lv_obj_get_y(deviceDebug_button),lv_obj_get_y(deviceDebug_button)-272,500);
}

void hideHomeScreen()
{
  lv_obj_set_hidden(nameLabel,true);
  lv_obj_set_hidden(autonLabel,true);

  lv_obj_set_hidden(autonNext_button,true);
  lv_obj_set_hidden(autonPrev_button,true);
  lv_obj_set_hidden(imuButton,true);
  lv_obj_set_hidden(sensorDebug_button,true);
  lv_obj_set_hidden(autonDebug_button,true);
}

void showHomeScreen()
{
  lv_obj_set_hidden(nameLabel,false);
  lv_obj_set_hidden(autonLabel,false);

  lv_obj_set_hidden(autonNext_button,false);
  lv_obj_set_hidden(autonPrev_button,false);
  lv_obj_set_hidden(imuButton,false);
  lv_obj_set_hidden(sensorDebug_button,false);
  lv_obj_set_hidden(autonDebug_button,false);
}


//auton Debug ==================================================================================================================================

void resetAutonDebug()
{
  lv_label_set_text(debugLabel1, "-");
  lv_label_set_text(debugValue1, "-");

  lv_label_set_text(debugLabel2, "-");
  lv_label_set_text(debugValue2, "-");

  lv_label_set_text(debugLabel3, "-");
  lv_label_set_text(debugValue3, "-");

  lv_label_set_text(debugLabel4, "-");
  lv_label_set_text(debugValue4, "-");

  lv_label_set_text(debugLabel5, "-");
  lv_label_set_text(debugValue5, "-");

  lv_label_set_text(debugLabel6, "-");
  lv_label_set_text(debugValue6, "-");
}

void delDebugLables()
{
  lv_obj_del(xLabel);
  lv_obj_del(yLabel);
  lv_obj_del(thetaLabel);

  lv_obj_del(debugLabel1);
  lv_obj_del(debugLabel2);
  lv_obj_del(debugLabel3);
  lv_obj_del(debugLabel4);
  lv_obj_del(debugLabel5);
  lv_obj_del(debugLabel6);
}

void hideAutonDebug()
{
  lv_obj_set_hidden(xLabel, true);
  lv_obj_set_hidden(yLabel, true);
  lv_obj_set_hidden(thetaLabel, true);
  lv_obj_set_hidden(debugLabel1, true);
  lv_obj_set_hidden(debugLabel2, true);
  lv_obj_set_hidden(debugLabel3, true);
  lv_obj_set_hidden(debugLabel4, true);
  lv_obj_set_hidden(debugLabel5, true);
  lv_obj_set_hidden(debugLabel6, true);

  lv_obj_set_hidden(xValue, true);
  lv_obj_set_hidden(yValue, true);
  lv_obj_set_hidden(thetaValue, true);
  lv_obj_set_hidden(debugValue1, true);
  lv_obj_set_hidden(debugValue2, true);
  lv_obj_set_hidden(debugValue3, true);
  lv_obj_set_hidden(debugValue4, true);
  lv_obj_set_hidden(debugValue5, true);
  lv_obj_set_hidden(debugValue6, true);
}

void showAutonDebug()
{
  lv_obj_set_hidden(xLabel, false);
  lv_obj_set_hidden(yLabel, false);
  lv_obj_set_hidden(thetaLabel, false);
  lv_obj_set_hidden(debugLabel1, false);
  lv_obj_set_hidden(debugLabel2, false);
  lv_obj_set_hidden(debugLabel3, false);
  lv_obj_set_hidden(debugLabel4, false);
  lv_obj_set_hidden(debugLabel5, false);
  lv_obj_set_hidden(debugLabel6, false);

  lv_obj_set_hidden(xValue, false);
  lv_obj_set_hidden(yValue, false);
  lv_obj_set_hidden(thetaValue, false);
  lv_obj_set_hidden(debugValue1, false);
  lv_obj_set_hidden(debugValue2, false);
  lv_obj_set_hidden(debugValue3, false);
  lv_obj_set_hidden(debugValue4, false);
  lv_obj_set_hidden(debugValue5, false);
  lv_obj_set_hidden(debugValue6, false);
}

void initAutonDebug()
{
  int y = 10;
  xLabel = createTextLabel(xLabel, "X POSITION", DEBUG_X_1, y);
  yLabel = createTextLabel(yLabel, "Y POSITION", DEBUG_X_1, y+20);
  thetaLabel = createTextLabel(thetaLabel, "THETA", DEBUG_X_1, y+40);

  debugLabel1 = createTextLabel(debugLabel1,"-",DEBUG_X_1,y+60);
  debugLabel2 = createTextLabel(debugLabel2,"-",DEBUG_X_1,y+80);
  debugLabel3 = createTextLabel(debugLabel3,"-",DEBUG_X_1,y+100);
  debugLabel4 = createTextLabel(debugLabel4,"-",DEBUG_X_1,y+120);
  debugLabel5 = createTextLabel(debugLabel5,"-",DEBUG_X_1,y+140);
  debugLabel6 = createTextLabel(debugLabel6,"-",DEBUG_X_1,y+160);

  xValue = createTextLabel(xValue, "-", DEBUG_X_2, y);
  yValue = createTextLabel(yValue, "-", DEBUG_X_2, y+20);
  thetaValue = createTextLabel(thetaValue, "-", DEBUG_X_2, y+40);

  debugValue1 = createTextLabel(debugValue1,"-",DEBUG_X_2,y+60);
  debugValue2 = createTextLabel(debugValue2,"-",DEBUG_X_2,y+80);
  debugValue3 = createTextLabel(debugValue3,"-",DEBUG_X_2,y+100);
  debugValue4 = createTextLabel(debugValue4,"-",DEBUG_X_2,y+120);
  debugValue5 = createTextLabel(debugValue5,"-",DEBUG_X_2,y+140);
  debugValue6 = createTextLabel(debugValue6,"-",DEBUG_X_2,y+160);

  hideAutonDebug();
}

void animateAutonDebugLeft()
{
  animate_x_ease_out(xLabel,DEBUG_X_1+480,DEBUG_X_1,500);
  animate_x_ease_out(yLabel,DEBUG_X_1+480,DEBUG_X_1,500);
  animate_x_ease_out(thetaLabel,DEBUG_X_1+480,DEBUG_X_1,500);
  animate_x_ease_out(debugLabel1,DEBUG_X_1+480,DEBUG_X_1,500);
  animate_x_ease_out(debugLabel2,DEBUG_X_1+480,DEBUG_X_1,500);
  animate_x_ease_out(debugLabel3,DEBUG_X_1+480,DEBUG_X_1,500);
  animate_x_ease_out(debugLabel4,DEBUG_X_1+480,DEBUG_X_1,500);
  animate_x_ease_out(debugLabel5,DEBUG_X_1+480,DEBUG_X_1,500);
  animate_x_ease_out(debugLabel6,DEBUG_X_1+480,DEBUG_X_1,500);

  animate_x_ease_out(xValue,DEBUG_X_2+480,DEBUG_X_2,500);
  animate_x_ease_out(yValue,DEBUG_X_2+480,DEBUG_X_2,500);
  animate_x_ease_out(thetaValue,DEBUG_X_2+480,DEBUG_X_2,500);
  animate_x_ease_out(debugValue1,DEBUG_X_2+480,DEBUG_X_2,500);
  animate_x_ease_out(debugValue2,DEBUG_X_2+480,DEBUG_X_2,500);
  animate_x_ease_out(debugValue3,DEBUG_X_2+480,DEBUG_X_2,500);
  animate_x_ease_out(debugValue4,DEBUG_X_2+480,DEBUG_X_2,500);
  animate_x_ease_out(debugValue5,DEBUG_X_2+480,DEBUG_X_2,500);
  animate_x_ease_out(debugValue6,DEBUG_X_2+480,DEBUG_X_2,500);
}

void animateAutonDebugRight()
{
  animate_x_linear(xLabel,DEBUG_X_1,DEBUG_X_1+480,500);
  animate_x_linear(yLabel,DEBUG_X_1,DEBUG_X_1+480,500);
  animate_x_linear(thetaLabel,DEBUG_X_1,DEBUG_X_1+480,500);
  animate_x_linear(debugLabel1,DEBUG_X_1,DEBUG_X_1+480,500);
  animate_x_linear(debugLabel2,DEBUG_X_1,DEBUG_X_1+480,500);
  animate_x_linear(debugLabel3,DEBUG_X_1,DEBUG_X_1+480,500);
  animate_x_linear(debugLabel4,DEBUG_X_1,DEBUG_X_1+480,500);
  animate_x_linear(debugLabel5,DEBUG_X_1,DEBUG_X_1+480,500);
  animate_x_linear(debugLabel6,DEBUG_X_1,DEBUG_X_1+480,500);

  animate_x_linear(xValue,DEBUG_X_2,DEBUG_X_2+480,500);
  animate_x_linear(yValue,DEBUG_X_2,DEBUG_X_2+480,500);
  animate_x_linear(thetaValue,DEBUG_X_2,DEBUG_X_2+480,500);
  animate_x_linear(debugValue1,DEBUG_X_2,DEBUG_X_2+480,500);
  animate_x_linear(debugValue2,DEBUG_X_2,DEBUG_X_2+480,500);
  animate_x_linear(debugValue3,DEBUG_X_2,DEBUG_X_2+480,500);
  animate_x_linear(debugValue4,DEBUG_X_2,DEBUG_X_2+480,500);
  animate_x_linear(debugValue5,DEBUG_X_2,DEBUG_X_2+480,500);
  animate_x_linear(debugValue6,DEBUG_X_2,DEBUG_X_2+480,500);
}

//Motor Temp ==================================================================================================================================

void thread_deviceDebug(void*p)
{
  while(true) {
    updateValueLabel(deviceVel1, ld_1.getActualVelocity(), "RPM", 0);
    updateValueLabel(deviceVel2, ld_2.getActualVelocity(), "RPM", 0);
    updateValueLabel(deviceVel3, rd_1.getActualVelocity(), "RPM", 0);
    updateValueLabel(deviceVel4, rd_2.getActualVelocity(), "RPM", 0);
    updateValueLabel(deviceVel5, leftIntake.get_actual_velocity(), "RPM", 0);
    updateValueLabel(deviceVel6, rightIntake.get_actual_velocity(), "RPM", 0);
    updateValueLabel(deviceVel7, topConveyor.get_actual_velocity(), "RPM", 0);
    updateValueLabel(deviceVel8, botConveyor.get_actual_velocity(), "RPM", 0);

    updateValueLabel(deviceTemp1, ld_1.getTemperature(), "C", 0);
    updateValueLabel(deviceTemp2, ld_2.getTemperature(), "C", 0);
    updateValueLabel(deviceTemp3, rd_1.getTemperature(), "C", 0);
    updateValueLabel(deviceTemp4, rd_2.getTemperature(), "C", 0);
    updateValueLabel(deviceTemp5, leftIntake.get_temperature(), "C", 0);
    updateValueLabel(deviceTemp6, rightIntake.get_temperature(), "C", 0);
    updateValueLabel(deviceTemp7, topConveyor.get_temperature(), "C", 0);
    updateValueLabel(deviceTemp8, botConveyor.get_temperature(), "C", 0);

    pros::delay(100);
  }
}

void hideDeviceDebug()
{
  lv_obj_set_hidden(deviceLabel1,true);
  lv_obj_set_hidden(deviceLabel4,true);
  lv_obj_set_hidden(deviceLabel3,true);
  lv_obj_set_hidden(deviceLabel2,true);
  lv_obj_set_hidden(deviceLabel5,true);
  lv_obj_set_hidden(deviceLabel6,true);
  lv_obj_set_hidden(deviceLabel7,true);
  lv_obj_set_hidden(deviceLabel8,true);

  lv_obj_set_hidden(deviceVel1,true);
  lv_obj_set_hidden(deviceVel4,true);
  lv_obj_set_hidden(deviceVel3,true);
  lv_obj_set_hidden(deviceVel2,true);
  lv_obj_set_hidden(deviceVel5,true);
  lv_obj_set_hidden(deviceVel6,true);
  lv_obj_set_hidden(deviceVel7,true);
  lv_obj_set_hidden(deviceVel8,true);

  lv_obj_set_hidden(deviceTemp1,true);
  lv_obj_set_hidden(deviceTemp4,true);
  lv_obj_set_hidden(deviceTemp3,true);
  lv_obj_set_hidden(deviceTemp2,true);
  lv_obj_set_hidden(deviceTemp5,true);
  lv_obj_set_hidden(deviceTemp6,true);
  lv_obj_set_hidden(deviceTemp7,true);
  lv_obj_set_hidden(deviceTemp8,true);
}

void showDeviceDebug()
{
  lv_obj_set_hidden(deviceLabel1,false);
  lv_obj_set_hidden(deviceLabel4,false);
  lv_obj_set_hidden(deviceLabel3,false);
  lv_obj_set_hidden(deviceLabel2,false);
  lv_obj_set_hidden(deviceLabel5,false);
  lv_obj_set_hidden(deviceLabel6,false);
  lv_obj_set_hidden(deviceLabel7,false);
  lv_obj_set_hidden(deviceLabel8,false);

  lv_obj_set_hidden(deviceVel1,false);
  lv_obj_set_hidden(deviceVel4,false);
  lv_obj_set_hidden(deviceVel3,false);
  lv_obj_set_hidden(deviceVel2,false);
  lv_obj_set_hidden(deviceVel5,false);
  lv_obj_set_hidden(deviceVel6,false);
  lv_obj_set_hidden(deviceVel7,false);
  lv_obj_set_hidden(deviceVel8,false);

  lv_obj_set_hidden(deviceTemp1,false);
  lv_obj_set_hidden(deviceTemp4,false);
  lv_obj_set_hidden(deviceTemp3,false);
  lv_obj_set_hidden(deviceTemp2,false);
  lv_obj_set_hidden(deviceTemp5,false);
  lv_obj_set_hidden(deviceTemp6,false);
  lv_obj_set_hidden(deviceTemp7,false);
  lv_obj_set_hidden(deviceTemp8,false);
}

void initDeviceDebug()
{
  int y=10;
  deviceLabel1 = createTextLabel(deviceLabel1, "[6] LEFT DRIVE", DEBUG_X_1, y);
  deviceLabel2 = createTextLabel(deviceLabel2, "[20] LEFT DRIVE", DEBUG_X_1, y+20);
  deviceLabel3 = createTextLabel(deviceLabel3, "[7] RIGHT DRIVE", DEBUG_X_1, y+40);
  deviceLabel4 = createTextLabel(deviceLabel4, "[18] RIGHT DRIVE", DEBUG_X_1, y+60);
  deviceLabel5 = createTextLabel(deviceLabel5, "[12] LEFT INTAKE", DEBUG_X_1, y+80);
  deviceLabel6 = createTextLabel(deviceLabel6, "[11] RIGHT INTAKE", DEBUG_X_1, y+100);
  deviceLabel7 = createTextLabel(deviceLabel7, "[15] TOP CONEYOR", DEBUG_X_1, y+120);
  deviceLabel8 = createTextLabel(deviceLabel8, "[14] BOT CONVEYOR", DEBUG_X_1, y+140);

  deviceVel1 = createTextLabel(deviceVel1, "-", DEBUG_X_2, y);
  deviceVel2 = createTextLabel(deviceVel2, "-", DEBUG_X_2, y+20);
  deviceVel3 = createTextLabel(deviceVel3, "-", DEBUG_X_2, y+40);
  deviceVel4 = createTextLabel(deviceVel4, "-", DEBUG_X_2, y+60);
  deviceVel5 = createTextLabel(deviceVel5, "-", DEBUG_X_2, y+80);
  deviceVel6 = createTextLabel(deviceVel6, "-", DEBUG_X_2, y+100);
  deviceVel7 = createTextLabel(deviceVel7, "-", DEBUG_X_2, y+120);
  deviceVel8 = createTextLabel(deviceVel8, "-", DEBUG_X_2, y+140);

  deviceTemp1 = createTextLabel(deviceTemp1, "-", DEBUG_X_2+100, y);
  deviceTemp2 = createTextLabel(deviceTemp2, "-", DEBUG_X_2+100, y+20);
  deviceTemp3 = createTextLabel(deviceTemp3, "-", DEBUG_X_2+100, y+40);
  deviceTemp4 = createTextLabel(deviceTemp4, "-", DEBUG_X_2+100, y+60);
  deviceTemp5 = createTextLabel(deviceTemp5, "-", DEBUG_X_2+100, y+80);
  deviceTemp6 = createTextLabel(deviceTemp6, "-", DEBUG_X_2+100, y+100);
  deviceTemp7 = createTextLabel(deviceTemp7, "-", DEBUG_X_2+100, y+120);
  deviceTemp8 = createTextLabel(deviceTemp8, "-", DEBUG_X_2+100, y+140);

  hideDeviceDebug();
  pros::Task device_debug (thread_deviceDebug, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
}

void animateDeviceDebugLeft()
{
  animate_x_ease_out(deviceLabel1,DEBUG_X_1+480,DEBUG_X_1,500);
  animate_x_ease_out(deviceLabel2,DEBUG_X_1+480,DEBUG_X_1,500);
  animate_x_ease_out(deviceLabel3,DEBUG_X_1+480,DEBUG_X_1,500);
  animate_x_ease_out(deviceLabel4,DEBUG_X_1+480,DEBUG_X_1,500);
  animate_x_ease_out(deviceLabel5,DEBUG_X_1+480,DEBUG_X_1,500);
  animate_x_ease_out(deviceLabel6,DEBUG_X_1+480,DEBUG_X_1,500);
  animate_x_ease_out(deviceLabel7,DEBUG_X_1+480,DEBUG_X_1,500);
  animate_x_ease_out(deviceLabel8,DEBUG_X_1+480,DEBUG_X_1,500);

  animate_x_ease_out(deviceVel1,DEBUG_X_2+480,DEBUG_X_2,500);
  animate_x_ease_out(deviceVel2,DEBUG_X_2+480,DEBUG_X_2,500);
  animate_x_ease_out(deviceVel3,DEBUG_X_2+480,DEBUG_X_2,500);
  animate_x_ease_out(deviceVel4,DEBUG_X_2+480,DEBUG_X_2,500);
  animate_x_ease_out(deviceVel5,DEBUG_X_2+480,DEBUG_X_2,500);
  animate_x_ease_out(deviceVel6,DEBUG_X_2+480,DEBUG_X_2,500);
  animate_x_ease_out(deviceVel7,DEBUG_X_2+480,DEBUG_X_2,500);
  animate_x_ease_out(deviceVel8,DEBUG_X_2+480,DEBUG_X_2,500);

  animate_x_ease_out(deviceTemp1,DEBUG_X_2+580,DEBUG_X_2+100,500);
  animate_x_ease_out(deviceTemp2,DEBUG_X_2+580,DEBUG_X_2+100,500);
  animate_x_ease_out(deviceTemp3,DEBUG_X_2+580,DEBUG_X_2+100,500);
  animate_x_ease_out(deviceTemp4,DEBUG_X_2+580,DEBUG_X_2+100,500);
  animate_x_ease_out(deviceTemp5,DEBUG_X_2+580,DEBUG_X_2+100,500);
  animate_x_ease_out(deviceTemp6,DEBUG_X_2+580,DEBUG_X_2+100,500);
  animate_x_ease_out(deviceTemp7,DEBUG_X_2+580,DEBUG_X_2+100,500);
  animate_x_ease_out(deviceTemp8,DEBUG_X_2+580,DEBUG_X_2+100,500);
}

void animateDeviceDebugRight()
{
  animate_x_linear(deviceLabel1,DEBUG_X_1,DEBUG_X_1+480,500);
  animate_x_linear(deviceLabel2,DEBUG_X_1,DEBUG_X_1+480,500);
  animate_x_linear(deviceLabel3,DEBUG_X_1,DEBUG_X_1+480,500);
  animate_x_linear(deviceLabel4,DEBUG_X_1,DEBUG_X_1+480,500);
  animate_x_linear(deviceLabel5,DEBUG_X_1,DEBUG_X_1+480,500);
  animate_x_linear(deviceLabel6,DEBUG_X_1,DEBUG_X_1+480,500);
  animate_x_linear(deviceLabel7,DEBUG_X_1,DEBUG_X_1+480,500);
  animate_x_linear(deviceLabel8,DEBUG_X_1,DEBUG_X_1+480,500);

  animate_x_linear(deviceVel1,DEBUG_X_2,DEBUG_X_2+480,500);
  animate_x_linear(deviceVel2,DEBUG_X_2,DEBUG_X_2+480,500);
  animate_x_linear(deviceVel3,DEBUG_X_2,DEBUG_X_2+480,500);
  animate_x_linear(deviceVel4,DEBUG_X_2,DEBUG_X_2+480,500);
  animate_x_linear(deviceVel5,DEBUG_X_2,DEBUG_X_2+480,500);
  animate_x_linear(deviceVel6,DEBUG_X_2,DEBUG_X_2+480,500);
  animate_x_linear(deviceVel7,DEBUG_X_2,DEBUG_X_2+480,500);
  animate_x_linear(deviceVel8,DEBUG_X_2,DEBUG_X_2+480,500);

  animate_x_linear(deviceTemp1,DEBUG_X_2+100,DEBUG_X_2+580,500);
  animate_x_linear(deviceTemp2,DEBUG_X_2+100,DEBUG_X_2+580,500);
  animate_x_linear(deviceTemp3,DEBUG_X_2+100,DEBUG_X_2+580,500);
  animate_x_linear(deviceTemp4,DEBUG_X_2+100,DEBUG_X_2+580,500);
  animate_x_linear(deviceTemp5,DEBUG_X_2+100,DEBUG_X_2+580,500);
  animate_x_linear(deviceTemp6,DEBUG_X_2+100,DEBUG_X_2+580,500);
  animate_x_linear(deviceTemp7,DEBUG_X_2+100,DEBUG_X_2+580,500);
  animate_x_linear(deviceTemp8,DEBUG_X_2+100,DEBUG_X_2+580,500);
}

//sensor Debug ==================================================================================================================================
void thread_sensorDebug(void*p)
{
  int store = 5000;
  while(true) {
    //if(store  > (int)pros::c::ext_adi_analog_read(5,'A') && (int)pros::c::ext_adi_analog_read(5,'A') > 100)
    //  store = (int)pros::c::ext_adi_analog_read(5,'A');

    updateValueLabel(sensorValue4,topDetector.get_value(),"",0);
    updateValueLabel(sensorValue5,pros::c::ext_adi_analog_read(5,'C'),"",0);
    updateValueLabel(sensorValue6,botDetector.get_value(),"",0);
    updateValueLabel(sensorValue7,pros::c::ext_adi_analog_read(5,'B'),"",0);
    updateValueLabel(sensorValue8,pros::c::ext_adi_analog_read(5,'A'),"",0);
    updateValueLabel(sensorValue1,imu.get_heading(),"DEG",3);
    updateValueLabel(sensorValue2,left.get(),"",0);
    updateValueLabel(sensorValue3,right.get(),"",0);
    updateValueLabel(sensorValue9,optical.get_hue(),"",0);
    pros::delay(100);
  }
}

void showSensorDebug()
{
  //lv_obj_set_hidden(back_button,false);

  lv_obj_set_hidden(sensorLabel1,false);
  lv_obj_set_hidden(sensorLabel4,false);
  lv_obj_set_hidden(sensorLabel3,false);
  lv_obj_set_hidden(sensorLabel2,false);
  lv_obj_set_hidden(sensorLabel5,false);
  lv_obj_set_hidden(sensorLabel6,false);
  lv_obj_set_hidden(sensorLabel7,false);
  lv_obj_set_hidden(sensorLabel8,false);
  lv_obj_set_hidden(sensorLabel9,false);

  lv_obj_set_hidden(sensorValue1,false);
  lv_obj_set_hidden(sensorValue4,false);
  lv_obj_set_hidden(sensorValue3,false);
  lv_obj_set_hidden(sensorValue2,false);
  lv_obj_set_hidden(sensorValue5,false);
  lv_obj_set_hidden(sensorValue6,false);
  lv_obj_set_hidden(sensorValue7,false);
  lv_obj_set_hidden(sensorValue8,false);
  lv_obj_set_hidden(sensorValue9,false);
}

void hideSensorDebug()
{
  //lv_obj_set_hidden(back_button,true);

  lv_obj_set_hidden(sensorLabel1,true);
  lv_obj_set_hidden(sensorLabel4,true);
  lv_obj_set_hidden(sensorLabel3,true);
  lv_obj_set_hidden(sensorLabel2,true);
  lv_obj_set_hidden(sensorLabel5,true);
  lv_obj_set_hidden(sensorLabel6,true);
  lv_obj_set_hidden(sensorLabel7,true);
  lv_obj_set_hidden(sensorLabel8,true);
  lv_obj_set_hidden(sensorLabel9,true);

  lv_obj_set_hidden(sensorValue1,true);
  lv_obj_set_hidden(sensorValue4,true);
  lv_obj_set_hidden(sensorValue3,true);
  lv_obj_set_hidden(sensorValue2,true);
  lv_obj_set_hidden(sensorValue5,true);
  lv_obj_set_hidden(sensorValue6,true);
  lv_obj_set_hidden(sensorValue7,true);
  lv_obj_set_hidden(sensorValue8,true);
  lv_obj_set_hidden(sensorValue9,true);
}

void initSensorDebug()
{
  int y=10;
  sensorLabel1 = createTextLabel(sensorLabel1, "[1] INERTIAL SENSOR", DEBUG_X_1, y);
  sensorLabel2 = createTextLabel(sensorLabel2, "[AB] LEFT ENCODER", DEBUG_X_1, y+20);
  sensorLabel3 = createTextLabel(sensorLabel3, "[CD] RIGHT ENCODER", DEBUG_X_1, y+40);
  sensorLabel4 = createTextLabel(sensorLabel4, "[E] TOPBALL LOW", DEBUG_X_1, y+60);
  sensorLabel5 = createTextLabel(sensorLabel5, "[F] EJECTOR", DEBUG_X_1, y+80);
  sensorLabel6 = createTextLabel(sensorLabel6, "[G] BOTBALL", DEBUG_X_1, y+100);
  sensorLabel7 = createTextLabel(sensorLabel7, "[H] TOPBALL HIGH", DEBUG_X_1, y+120);
  sensorLabel8 = createTextLabel(sensorLabel8, "[EXT] BOTBALL LOW", DEBUG_X_1, y+140);
  sensorLabel9 = createTextLabel(sensorLabel9, "[4] OPTICAL", DEBUG_X_1, y+160);


  sensorValue1 = createTextLabel(sensorValue1, "-", DEBUG_X_2, y);
  sensorValue2 = createTextLabel(sensorValue2, "-", DEBUG_X_2, y+20);
  sensorValue3 = createTextLabel(sensorValue3, "-", DEBUG_X_2, y+40);
  sensorValue4 = createTextLabel(sensorValue4, "-", DEBUG_X_2, y+60);
  sensorValue5 = createTextLabel(sensorValue5, "-", DEBUG_X_2, y+80);
  sensorValue6 = createTextLabel(sensorValue6, "-", DEBUG_X_2, y+100);
  sensorValue7 = createTextLabel(sensorValue7, "-", DEBUG_X_2, y+120);
  sensorValue8 = createTextLabel(sensorValue8, "-", DEBUG_X_2, y+140);
  sensorValue9 = createTextLabel(sensorValue9, "-", DEBUG_X_2, y+160);

  hideSensorDebug();
  pros::Task sensor_debug (thread_sensorDebug, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
}

void animateSensorDebugLeft()
{
  animate_x_ease_out(sensorLabel1,DEBUG_X_1+480,DEBUG_X_1,500);
  animate_x_ease_out(sensorLabel2,DEBUG_X_1+480,DEBUG_X_1,500);
  animate_x_ease_out(sensorLabel3,DEBUG_X_1+480,DEBUG_X_1,500);
  animate_x_ease_out(sensorLabel4,DEBUG_X_1+480,DEBUG_X_1,500);
  animate_x_ease_out(sensorLabel5,DEBUG_X_1+480,DEBUG_X_1,500);
  animate_x_ease_out(sensorLabel6,DEBUG_X_1+480,DEBUG_X_1,500);
  animate_x_ease_out(sensorLabel7,DEBUG_X_1+480,DEBUG_X_1,500);
  animate_x_ease_out(sensorLabel8,DEBUG_X_1+480,DEBUG_X_1,500);
  animate_x_ease_out(sensorLabel9,DEBUG_X_1+480,DEBUG_X_1,500);

  animate_x_ease_out(sensorValue1,DEBUG_X_2+480,DEBUG_X_2,500);
  animate_x_ease_out(sensorValue2,DEBUG_X_2+480,DEBUG_X_2,500);
  animate_x_ease_out(sensorValue3,DEBUG_X_2+480,DEBUG_X_2,500);
  animate_x_ease_out(sensorValue4,DEBUG_X_2+480,DEBUG_X_2,500);
  animate_x_ease_out(sensorValue5,DEBUG_X_2+480,DEBUG_X_2,500);
  animate_x_ease_out(sensorValue6,DEBUG_X_2+480,DEBUG_X_2,500);
  animate_x_ease_out(sensorValue7,DEBUG_X_2+480,DEBUG_X_2,500);
  animate_x_ease_out(sensorValue8,DEBUG_X_2+480,DEBUG_X_2,500);
  animate_x_ease_out(sensorValue9,DEBUG_X_2+480,DEBUG_X_2,500);
}

void animateSensorDebugRight()
{
  animate_x_linear(sensorLabel1,DEBUG_X_1,DEBUG_X_1+480,500);
  animate_x_linear(sensorLabel2,DEBUG_X_1,DEBUG_X_1+480,500);
  animate_x_linear(sensorLabel3,DEBUG_X_1,DEBUG_X_1+480,500);
  animate_x_linear(sensorLabel4,DEBUG_X_1,DEBUG_X_1+480,500);
  animate_x_linear(sensorLabel5,DEBUG_X_1,DEBUG_X_1+480,500);
  animate_x_linear(sensorLabel6,DEBUG_X_1,DEBUG_X_1+480,500);
  animate_x_linear(sensorLabel7,DEBUG_X_1,DEBUG_X_1+480,500);
  animate_x_linear(sensorLabel8,DEBUG_X_1,DEBUG_X_1+480,500);
  animate_x_linear(sensorLabel9,DEBUG_X_1,DEBUG_X_1+480,500);

  animate_x_linear(sensorValue1,DEBUG_X_2,DEBUG_X_2+480,500);
  animate_x_linear(sensorValue2,DEBUG_X_2,DEBUG_X_2+480,500);
  animate_x_linear(sensorValue3,DEBUG_X_2,DEBUG_X_2+480,500);
  animate_x_linear(sensorValue4,DEBUG_X_2,DEBUG_X_2+480,500);
  animate_x_linear(sensorValue5,DEBUG_X_2,DEBUG_X_2+480,500);
  animate_x_linear(sensorValue6,DEBUG_X_2,DEBUG_X_2+480,500);
  animate_x_linear(sensorValue7,DEBUG_X_2,DEBUG_X_2+480,500);
  animate_x_linear(sensorValue8,DEBUG_X_2,DEBUG_X_2+480,500);
  animate_x_linear(sensorValue9,DEBUG_X_2,DEBUG_X_2+480,500);
}

//Button Actions ==================================================================================================================================

void updateAutonLabel(lv_obj_t * label, int autonNumber)
{
  std::string label_str = "";
  switch(autonNumber%NUM_OF_AUTONS) {
      case 0:
        label_str = "#00FF00 NONE#";
        break;
      case 1:
        label_str = "#FF3333 RED#";
        break;
      case 2:
        label_str = "#3333FF BLUE#";
        break;
      case 3:
        label_str = "#FFFF33 SKILLS#";
        break;
  }
  char label_array[label_str.length()+1];
  strcpy(label_array,label_str.c_str());
  lv_label_set_text(label,label_array);
}

void animate_autonNext(void*p)
{
  int x = lv_obj_get_x(autonSelect);
  animate_x_ease_in(autonSelect,x,130,250);
  pros::delay(250);
  updateAutonLabel(autonSelect,auton);
  lv_obj_align(autonSelect, NULL, LV_ALIGN_IN_TOP_MID, 0, 107);
  animate_x_ease_out(autonSelect,350-lv_obj_get_width(autonSelect),lv_obj_get_x(autonSelect),250);
}

void animate_autonPrev(void*p)
{
  int x = lv_obj_get_x(autonSelect);
  animate_x_ease_in(autonSelect,x,350-lv_obj_get_width(autonSelect),250);
  pros::delay(250);
  updateAutonLabel(autonSelect,auton);
  lv_obj_align(autonSelect, NULL, LV_ALIGN_IN_TOP_MID, 0, 107);
  animate_x_ease_out(autonSelect,130,lv_obj_get_x(autonSelect),250);
}

static lv_res_t autonNext_action(lv_obj_t * btn)
{
  auton++;
  pros::Task anim (animate_autonNext, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
  return LV_RES_OK;
}

static lv_res_t autonPrev_action(lv_obj_t * btn)
{
  auton--;
  if(auton < 0) auton = NUM_OF_AUTONS-1;
  pros::Task anim (animate_autonPrev, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
  return LV_RES_OK;
}

void thread_imu(void*p)
{
  imu.reset();
  lv_btn_set_style(imuButton,LV_BTN_STYLE_REL, &style3);
  lv_label_set_text(imuLabel,"RECALIBRATING...");
  pros::delay(100);
  while(imu.is_calibrating()) {
    pros::delay(10);
  }
  lv_btn_set_style(imuButton,LV_BTN_STYLE_REL, &style2);
  lv_label_set_text(imuLabel,"CALIBRATED");
}

static lv_res_t imuButton_action(lv_obj_t * btn)
{
  pros::Task imu_calibrate (thread_imu, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
  return LV_RES_OK;
}

int debugging_mode;

void anim_sensorDebug(void*p)
{
  debugging_mode = 0;
  animateHomeScreenLeft();
  pros::delay(250);
  showSensorDebug();
  lv_obj_set_hidden(back_button,false);
  animate_x_ease_out(back_button,15+480,15,500);
  animateSensorDebugLeft();
}

void anim_autonDebug(void*p)
{
  debugging_mode = 1;
  animateHomeScreenLeft();
  pros::delay(250);
  showAutonDebug();
  lv_obj_set_hidden(back_button,false);
  animate_x_ease_out(back_button,15+480,15,500);
  animateAutonDebugLeft();
}

void anim_deviceDebug(void*p)
{
  debugging_mode = 2;
  animateHomeScreenLeft();
  pros::delay(250);
  showDeviceDebug();
  lv_obj_set_hidden(back_button,false);
  animate_x_ease_out(back_button,15+480,15,500);
  animateDeviceDebugLeft();
}

void anim_back(void*p)
{
  switch(debugging_mode) {
    case 0:
      animateSensorDebugRight(); break;
    case 1:
      animateAutonDebugRight(); break;
    case 2:
      animateDeviceDebugRight(); break;

  }
  animate_x_linear(back_button,10,10+480,500);
  pros::delay(250);
  animateHomeScreenRight();
}


static lv_res_t sensorDebugButton_action(lv_obj_t * btn)
{
  pros::Task anim (anim_sensorDebug, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
  return LV_RES_OK;
}

static lv_res_t autonDebugButton_action(lv_obj_t * btn)
{
  pros::Task anim (anim_autonDebug, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
  return LV_RES_OK;
}

static lv_res_t deviceDebugButton_action(lv_obj_t * btn)
{
  pros::Task anim (anim_deviceDebug, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
  return LV_RES_OK;
}

static lv_res_t back_action(lv_obj_t * btn)
{
  pros::Task anim (anim_back, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
  return LV_RES_OK;
}

//Initialize whole GUI ==================================================================================================================================

void initAutonGUI()
{
  nameLabel = createTextLabel(nameLabel, "#00FF00 EXO##FFFFFF BYTES#",0,0);
  lv_label_set_style(nameLabel, &style4);
  lv_obj_align(nameLabel, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);

  autonLabel = createTextLabel(autonLabel, "AUTONOMOUS:",230,25);
  lv_obj_align(autonLabel, NULL, LV_ALIGN_IN_TOP_MID, 0, 87);

  autonSelect = createTextLabel(autonSelect, "#00FF00 NONE#",230,25);
  lv_obj_align(autonSelect, NULL, LV_ALIGN_IN_TOP_MID, 0, 107);
  updateAutonLabel(autonSelect, auton);

  autonPrev_button = createBtn(autonPrev_button,10,10,110,95,2,"<");
  lv_btn_set_action(autonPrev_button,LV_BTN_ACTION_CLICK, autonPrev_action);
  lv_obj_align(autonPrev_button, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 80);

  autonNext_button = createBtn(autonNext_button,120,10,110,95,1,">");
  lv_btn_set_action(autonNext_button,LV_BTN_ACTION_CLICK, autonNext_action);
  lv_obj_align(autonNext_button, NULL, LV_ALIGN_IN_TOP_RIGHT, -10, 80);

  imuButton = createBtn(imuButton,10,100,220,50,1);
  imuLabel = createBtnLabel(imuLabel,imuButton, "NOT CALIBRATED");
  lv_btn_set_action(imuButton,LV_BTN_ACTION_PR, imuButton_action);
  lv_obj_align(imuButton, NULL, LV_ALIGN_IN_TOP_MID,0,125);

  sensorDebug_button = createBtn(sensorDebug_button,10,10,150,50,2,"SENSOR DEBUG");
  lv_obj_align(sensorDebug_button, NULL, LV_ALIGN_IN_TOP_LEFT,10,180);
  lv_btn_set_action(sensorDebug_button,LV_BTN_ACTION_CLICK, sensorDebugButton_action);

  autonDebug_button = createBtn(autonDebug_button,10,10,150,50,2,"AUTON DEBUG");
  lv_obj_align(autonDebug_button, NULL, LV_ALIGN_IN_TOP_MID,0,180);
  lv_btn_set_action(autonDebug_button,LV_BTN_ACTION_CLICK, autonDebugButton_action);

  deviceDebug_button = createBtn(deviceDebug_button,10,10,150,50,2,"MOTOR DEBUG");
  lv_obj_align(deviceDebug_button, NULL, LV_ALIGN_IN_TOP_RIGHT,-10,180);
  lv_btn_set_action(deviceDebug_button,LV_BTN_ACTION_CLICK, deviceDebugButton_action);

  back_button = createBtn(back_button,10,20,50,50,2,"<");
  lv_obj_align(back_button, NULL, LV_ALIGN_IN_TOP_LEFT,20,10);
  lv_btn_set_action(back_button,LV_BTN_ACTION_CLICK, back_action);
  lv_obj_set_hidden(back_button,true);

  animateHomeScreen();
}

void delAutonGUI()
{
  lv_obj_del(autonLabel);
  lv_obj_del(autonNext_button);
  lv_obj_del(autonPrev_button);
  lv_obj_del(imuLabel);
  lv_obj_del(imuButton);
}

void initGUI(void*p)
{
  initStyles();
  initAutonGUI();
  initSensorDebug();
  initAutonDebug();
  initDeviceDebug();
}
