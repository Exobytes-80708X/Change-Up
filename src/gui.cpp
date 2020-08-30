#include "main.h"
#include "display/lvgl.h"

int auton = 0;
const int NUM_OF_AUTONS = 4;
const std::string AUTON_HEADING = "AUTON SELECTED: ";

LV_FONT_DECLARE(armadura_solid);
LV_FONT_DECLARE(armadura_solid_big);
bool onHomeScreen = true;
const int DEBUG_X_1 = 100;
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
lv_obj_t * autonNext_button;
lv_obj_t * autonPrev_button;

lv_obj_t * sensorLabel5;
lv_obj_t * sensorLabel6;
lv_obj_t * sensorLabel7;
lv_obj_t * sensorLabel8;

lv_obj_t * sensorValue5;
lv_obj_t * sensorValue6;
lv_obj_t * sensorValue7;
lv_obj_t * sensorValue8;

lv_obj_t * imuLabel;
lv_obj_t * imuButton;

lv_obj_t * nameLabel;
lv_obj_t * img;

lv_obj_t * sensorDebug_button;
lv_obj_t * autonDebug_button;
lv_obj_t * back_button;

lv_obj_t * sensorLabel1;
lv_obj_t * sensorLabel4;
lv_obj_t * sensorLabel3;
lv_obj_t * sensorLabel2;

lv_obj_t * sensorValue1;
lv_obj_t * sensorValue4;
lv_obj_t * sensorValue3;
lv_obj_t * sensorValue2;

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

void updateValueLabel(lv_obj_t *parent, double var, std::string varUnits, int numOfSpaces)
{
  std::string text = std::to_string(var).substr(0,numOfSpaces) + " " + varUnits;
  char text_array[text.length()+1];
  strcpy(text_array,text.c_str());
  lv_label_set_text(parent, text_array);
}

void updateVarLabel(lv_obj_t *nameLabel, std::string varName, lv_obj_t* valueLabel, double var, std::string varUnits, int numOfSpaces)
{
  char text_array[varName.length()+1];
  strcpy(text_array,varName.c_str());
  lv_label_set_text(nameLabel, text_array);

  std::string text = std::to_string(var).substr(0,numOfSpaces) + " " + varUnits;
  char text_array2[text.length()+1];
  strcpy(text_array2,text.c_str());
  lv_label_set_text(valueLabel, text_array2);
}

void animate_x(lv_obj_t *obj,int x1,int x2,int duration)
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
  animate_x(autonPrev_button,lv_obj_get_x(autonPrev_button)-480,lv_obj_get_x(autonPrev_button),500);
  animate_x(autonNext_button,lv_obj_get_x(autonNext_button)+480,lv_obj_get_x(autonNext_button),500);
  animate_y(imuButton,lv_obj_get_y(imuButton)+272,lv_obj_get_y(imuButton),500);
  animate_y(sensorDebug_button,lv_obj_get_y(sensorDebug_button)+272,lv_obj_get_y(sensorDebug_button),500);
  animate_y(autonDebug_button,lv_obj_get_y(autonDebug_button)+272,lv_obj_get_y(autonDebug_button),500);
}

void animateHomeScreenLeft()
{
  /*animate_x(nameLabel,lv_obj_get_x(nameLabel),lv_obj_get_x(nameLabel)-480,500);
  animate_x(autonLabel,lv_obj_get_x(autonLabel),lv_obj_get_x(autonLabel)-480,500);
  animate_x(autonPrev_button,lv_obj_get_x(autonPrev_button),lv_obj_get_x(autonPrev_button)-480,500);
  animate_x(autonNext_button,lv_obj_get_x(autonNext_button),lv_obj_get_x(autonNext_button)-480,500);
  animate_x(imuButton,lv_obj_get_x(imuButton),lv_obj_get_x(imuButton)-480,500);
  animate_x(sensorDebug_button,lv_obj_get_x(sensorDebug_button),lv_obj_get_x(sensorDebug_button)-480,500);
  animate_x(autonDebug_button,lv_obj_get_x(autonDebug_button),lv_obj_get_x(autonDebug_button)-480,500);*/

  animate_y_linear(nameLabel,lv_obj_get_y(nameLabel),lv_obj_get_y(nameLabel)-272,500);
  animate_y_linear(autonLabel,lv_obj_get_y(autonLabel),lv_obj_get_y(autonLabel)+272,500);
  animate_y_linear(autonPrev_button,lv_obj_get_y(autonPrev_button),lv_obj_get_y(autonPrev_button)-272,500);
  animate_y_linear(autonNext_button,lv_obj_get_y(autonNext_button),lv_obj_get_y(autonNext_button)-272,500);
  animate_y_linear(imuButton,lv_obj_get_y(imuButton),lv_obj_get_y(imuButton)+272,500);
  animate_y_linear(sensorDebug_button,lv_obj_get_y(sensorDebug_button),lv_obj_get_y(sensorDebug_button)+272,500);
  animate_y_linear(autonDebug_button,lv_obj_get_y(autonDebug_button),lv_obj_get_y(autonDebug_button)+272,500);
}

void animateHomeScreenRight()
{
  /*animate_x(nameLabel,lv_obj_get_x(nameLabel),lv_obj_get_x(nameLabel)+480,500);
  animate_x(autonLabel,lv_obj_get_x(autonLabel),lv_obj_get_x(autonLabel)+480,500);
  animate_x(autonPrev_button,lv_obj_get_x(autonPrev_button),lv_obj_get_x(autonPrev_button)+480,500);
  animate_x(autonNext_button,lv_obj_get_x(autonNext_button),lv_obj_get_x(autonNext_button)+480,500);
  animate_x(imuButton,lv_obj_get_x(imuButton),lv_obj_get_x(imuButton)+480,500);
  animate_x(sensorDebug_button,lv_obj_get_x(sensorDebug_button),lv_obj_get_x(sensorDebug_button)+480,500);
  animate_x(autonDebug_button,lv_obj_get_x(autonDebug_button),lv_obj_get_x(autonDebug_button)+480,500);*/

  animate_y(nameLabel,lv_obj_get_y(nameLabel),lv_obj_get_y(nameLabel)+272,500);
  animate_y(autonLabel,lv_obj_get_y(autonLabel),lv_obj_get_y(autonLabel)-272,500);
  animate_y(autonPrev_button,lv_obj_get_y(autonPrev_button),lv_obj_get_y(autonPrev_button)+272,500);
  animate_y(autonNext_button,lv_obj_get_y(autonNext_button),lv_obj_get_y(autonNext_button)+272,500);
  animate_y(imuButton,lv_obj_get_y(imuButton),lv_obj_get_y(imuButton)-272,500);
  animate_y(sensorDebug_button,lv_obj_get_y(sensorDebug_button),lv_obj_get_y(sensorDebug_button)-272,500);
  animate_y(autonDebug_button,lv_obj_get_y(autonDebug_button),lv_obj_get_y(autonDebug_button)-272,500);
}

void hideHomeScreen()
{
  onHomeScreen = false;
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
  onHomeScreen = true;
  lv_obj_set_hidden(nameLabel,false);
  lv_obj_set_hidden(autonLabel,false);

  lv_obj_set_hidden(autonNext_button,false);
  lv_obj_set_hidden(autonPrev_button,false);
  lv_obj_set_hidden(imuButton,false);
  lv_obj_set_hidden(sensorDebug_button,false);
  lv_obj_set_hidden(autonDebug_button,false);

  //animateHomeScreen();
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
  debugLabel3 = createTextLabel(debugLabel3,"-",DEBUG_X_1,y+DEBUG_X_1);
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
  animate_x(xLabel,DEBUG_X_1+480,DEBUG_X_1,500);
  animate_x(yLabel,DEBUG_X_1+480,DEBUG_X_1,500);
  animate_x(thetaLabel,DEBUG_X_1+480,DEBUG_X_1,500);
  animate_x(debugLabel1,DEBUG_X_1+480,DEBUG_X_1,500);
  animate_x(debugLabel2,DEBUG_X_1+480,DEBUG_X_1,500);
  animate_x(debugLabel3,DEBUG_X_1+480,DEBUG_X_1,500);
  animate_x(debugLabel4,DEBUG_X_1+480,DEBUG_X_1,500);
  animate_x(debugLabel5,DEBUG_X_1+480,DEBUG_X_1,500);
  animate_x(debugLabel6,DEBUG_X_1+480,DEBUG_X_1,500);

  animate_x(xValue,DEBUG_X_2+480,DEBUG_X_2,500);
  animate_x(yValue,DEBUG_X_2+480,DEBUG_X_2,500);
  animate_x(thetaValue,DEBUG_X_2+480,DEBUG_X_2,500);
  animate_x(debugValue1,DEBUG_X_2+480,DEBUG_X_2,500);
  animate_x(debugValue2,DEBUG_X_2+480,DEBUG_X_2,500);
  animate_x(debugValue3,DEBUG_X_2+480,DEBUG_X_2,500);
  animate_x(debugValue4,DEBUG_X_2+480,DEBUG_X_2,500);
  animate_x(debugValue5,DEBUG_X_2+480,DEBUG_X_2,500);
  animate_x(debugValue6,DEBUG_X_2+480,DEBUG_X_2,500);
}

void animateAutonDebugRight()
{
  animate_x(xLabel,DEBUG_X_1,DEBUG_X_1+480,500);
  animate_x(yLabel,DEBUG_X_1,DEBUG_X_1+480,500);
  animate_x(thetaLabel,DEBUG_X_1,DEBUG_X_1+480,500);
  animate_x(debugLabel1,DEBUG_X_1,DEBUG_X_1+480,500);
  animate_x(debugLabel2,DEBUG_X_1,DEBUG_X_1+480,500);
  animate_x(debugLabel3,DEBUG_X_1,DEBUG_X_1+480,500);
  animate_x(debugLabel4,DEBUG_X_1,DEBUG_X_1+480,500);
  animate_x(debugLabel5,DEBUG_X_1,DEBUG_X_1+480,500);
  animate_x(debugLabel6,DEBUG_X_1,DEBUG_X_1+480,500);

  animate_x(xValue,DEBUG_X_2,DEBUG_X_2+480,500);
  animate_x(yValue,DEBUG_X_2,DEBUG_X_2+480,500);
  animate_x(thetaValue,DEBUG_X_2,DEBUG_X_2+480,500);
  animate_x(debugValue1,DEBUG_X_2,DEBUG_X_2+480,500);
  animate_x(debugValue2,DEBUG_X_2,DEBUG_X_2+480,500);
  animate_x(debugValue3,DEBUG_X_2,DEBUG_X_2+480,500);
  animate_x(debugValue4,DEBUG_X_2,DEBUG_X_2+480,500);
  animate_x(debugValue5,DEBUG_X_2,DEBUG_X_2+480,500);
  animate_x(debugValue6,DEBUG_X_2,DEBUG_X_2+480,500);
}

//Motor Temp ==================================================================================================================================


void updateMotorTempLabels()
{
  /*updateVarLabel(sensorLabel5,"LD1",ld_1.getTemperature(),"C",2);
  updateVarLabel(sensorLabel6,"LD2",ld_2.getTemperature(),"C",2);
  updateVarLabel(sensorLabel7,"RD1",rd_1.getTemperature(),"C",2);
  updateVarLabel(sensorLabel8,"RD2",rd_2.getTemperature(),"C",2);*/

  updateValueLabel(sensorValue5,ld_1.getTemperature(),"C",2);
  updateValueLabel(sensorValue6,ld_2.getTemperature(),"C",2);
  updateValueLabel(sensorValue7,rd_1.getTemperature(),"C",2);
  updateValueLabel(sensorValue8,rd_2.getTemperature(),"C",2);
}

void thread_motorTemps(void*p)
{
  while(true)
  {
    updateMotorTempLabels();
    pros::delay(1000);
  }
}

void delMotorTempLabels()
{
  lv_obj_del(sensorLabel5);
  lv_obj_del(sensorLabel6);
  lv_obj_del(sensorLabel7);
  lv_obj_del(sensorLabel8);
}

void hideMotorTempLabels()
{
  lv_obj_set_hidden(sensorLabel5,true);
  lv_obj_set_hidden(sensorLabel6,true);
  lv_obj_set_hidden(sensorLabel7,true);
  lv_obj_set_hidden(sensorLabel8,true);

  lv_obj_set_hidden(sensorValue5,true);
  lv_obj_set_hidden(sensorValue6,true);
  lv_obj_set_hidden(sensorValue7,true);
  lv_obj_set_hidden(sensorValue8,true);
}

void showMotorTempLabels()
{
  lv_obj_set_hidden(sensorLabel5,false);
  lv_obj_set_hidden(sensorLabel6,false);
  lv_obj_set_hidden(sensorLabel7,false);
  lv_obj_set_hidden(sensorLabel8,false);

  lv_obj_set_hidden(sensorValue5,false);
  lv_obj_set_hidden(sensorValue6,false);
  lv_obj_set_hidden(sensorValue7,false);
  lv_obj_set_hidden(sensorValue8,false);
}


//sensor Debug ==================================================================================================================================
void thread_sensorDebug(void*p)
{
  while(true) {
    updateValueLabel(sensorValue1,topDetector.get_value(),"",4);
    updateValueLabel(sensorValue4,topDetector2.get_value(),"",4);
    updateValueLabel(sensorValue3,botDetector.get_value(),"",4);
    updateValueLabel(sensorValue2,ejectDetector.get_value(),"",4);
    updateMotorTempLabels();
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

  lv_obj_set_hidden(sensorValue1,false);
  lv_obj_set_hidden(sensorValue4,false);
  lv_obj_set_hidden(sensorValue3,false);
  lv_obj_set_hidden(sensorValue2,false);

  lv_obj_set_hidden(sensorLabel5,false);
  lv_obj_set_hidden(sensorLabel6,false);
  lv_obj_set_hidden(sensorLabel7,false);
  lv_obj_set_hidden(sensorLabel8,false);

  lv_obj_set_hidden(sensorValue5,false);
  lv_obj_set_hidden(sensorValue6,false);
  lv_obj_set_hidden(sensorValue7,false);
  lv_obj_set_hidden(sensorValue8,false);

}

void hideSensorDebug()
{
  //lv_obj_set_hidden(back_button,true);

  lv_obj_set_hidden(sensorLabel1,true);
  lv_obj_set_hidden(sensorLabel4,true);
  lv_obj_set_hidden(sensorLabel3,true);
  lv_obj_set_hidden(sensorLabel2,true);

  lv_obj_set_hidden(sensorValue1,true);
  lv_obj_set_hidden(sensorValue4,true);
  lv_obj_set_hidden(sensorValue3,true);
  lv_obj_set_hidden(sensorValue2,true);

  lv_obj_set_hidden(sensorLabel5,true);
  lv_obj_set_hidden(sensorLabel6,true);
  lv_obj_set_hidden(sensorLabel7,true);
  lv_obj_set_hidden(sensorLabel8,true);

  lv_obj_set_hidden(sensorValue5,true);
  lv_obj_set_hidden(sensorValue6,true);
  lv_obj_set_hidden(sensorValue7,true);
  lv_obj_set_hidden(sensorValue8,true);
}

void initSensorDebug()
{
  int y=10;
  sensorLabel1 = createTextLabel(sensorLabel1, "[E] TOPBALL LOW", DEBUG_X_1, y);
  sensorLabel2 = createTextLabel(sensorLabel2, "[F] EJECTOR", DEBUG_X_1, y+20);
  sensorLabel3 = createTextLabel(sensorLabel3, "[G] BOTBALL", DEBUG_X_1, y+40);
  sensorLabel4 = createTextLabel(sensorLabel4, "[H] TOPBALL HIGH", DEBUG_X_1, y+60);


  sensorValue1 = createTextLabel(sensorValue1, "-", DEBUG_X_2, y);
  sensorValue2 = createTextLabel(sensorValue2, "-", DEBUG_X_2, y+20);
  sensorValue3 = createTextLabel(sensorValue3, "-", DEBUG_X_2, y+40);
  sensorValue4 = createTextLabel(sensorValue4, "-", DEBUG_X_2, y+60);

  sensorLabel5 = createTextLabel(sensorLabel5, "[6] LEFT DRIVE", DEBUG_X_1, y+80);
  sensorLabel6 = createTextLabel(sensorLabel6, "[20] LEFT DRIVE", DEBUG_X_1, y+100);
  sensorLabel7 = createTextLabel(sensorLabel7, "[7] RIGHT DRIVE", DEBUG_X_1, y+120);
  sensorLabel8 = createTextLabel(sensorLabel8, "[18] RIGHT DRIVE", DEBUG_X_1, y+140);

  sensorValue5 = createTextLabel(sensorValue5, "-", DEBUG_X_2, y+80);
  sensorValue6 = createTextLabel(sensorValue6, "-", DEBUG_X_2, y+100);
  sensorValue7 = createTextLabel(sensorValue7, "-", DEBUG_X_2, y+120);
  sensorValue8 = createTextLabel(sensorValue8, "-", DEBUG_X_2, y+140);

  hideSensorDebug();
  pros::Task sensor_debug (thread_sensorDebug, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
}

void animateSensorDebugLeft()
{
  animate_x(sensorLabel1,DEBUG_X_1+480,DEBUG_X_1,500);
  animate_x(sensorLabel2,DEBUG_X_1+480,DEBUG_X_1,500);
  animate_x(sensorLabel3,DEBUG_X_1+480,DEBUG_X_1,500);
  animate_x(sensorLabel4,DEBUG_X_1+480,DEBUG_X_1,500);
  animate_x(sensorLabel5,DEBUG_X_1+480,DEBUG_X_1,500);
  animate_x(sensorLabel6,DEBUG_X_1+480,DEBUG_X_1,500);
  animate_x(sensorLabel7,DEBUG_X_1+480,DEBUG_X_1,500);
  animate_x(sensorLabel8,DEBUG_X_1+480,DEBUG_X_1,500);

  animate_x(sensorValue1,DEBUG_X_2+480,DEBUG_X_2,500);
  animate_x(sensorValue2,DEBUG_X_2+480,DEBUG_X_2,500);
  animate_x(sensorValue3,DEBUG_X_2+480,DEBUG_X_2,500);
  animate_x(sensorValue4,DEBUG_X_2+480,DEBUG_X_2,500);
  animate_x(sensorValue5,DEBUG_X_2+480,DEBUG_X_2,500);
  animate_x(sensorValue6,DEBUG_X_2+480,DEBUG_X_2,500);
  animate_x(sensorValue7,DEBUG_X_2+480,DEBUG_X_2,500);
  animate_x(sensorValue8,DEBUG_X_2+480,DEBUG_X_2,500);
}

void animateSensorDebugRight()
{
  animate_x(sensorLabel1,DEBUG_X_1,DEBUG_X_1+480,500);
  animate_x(sensorLabel2,DEBUG_X_1,DEBUG_X_1+480,500);
  animate_x(sensorLabel3,DEBUG_X_1,DEBUG_X_1+480,500);
  animate_x(sensorLabel4,DEBUG_X_1,DEBUG_X_1+480,500);
  animate_x(sensorLabel5,DEBUG_X_1,DEBUG_X_1+480,500);
  animate_x(sensorLabel6,DEBUG_X_1,DEBUG_X_1+480,500);
  animate_x(sensorLabel7,DEBUG_X_1,DEBUG_X_1+480,500);
  animate_x(sensorLabel8,DEBUG_X_1,DEBUG_X_1+480,500);

  animate_x(sensorValue1,DEBUG_X_2,DEBUG_X_2+480,500);
  animate_x(sensorValue2,DEBUG_X_2,DEBUG_X_2+480,500);
  animate_x(sensorValue3,DEBUG_X_2,DEBUG_X_2+480,500);
  animate_x(sensorValue4,DEBUG_X_2,DEBUG_X_2+480,500);
  animate_x(sensorValue5,DEBUG_X_2,DEBUG_X_2+480,500);
  animate_x(sensorValue6,DEBUG_X_2,DEBUG_X_2+480,500);
  animate_x(sensorValue7,DEBUG_X_2,DEBUG_X_2+480,500);
  animate_x(sensorValue8,DEBUG_X_2,DEBUG_X_2+480,500);
}

//Button Actions ==================================================================================================================================

void updateAutonLabel(lv_obj_t * label, int autonNumber)
{
  std::string label_str = "";
  switch(autonNumber) {
      case 0:
        label_str = AUTON_HEADING + "  #00FF00 NONE#";
        break;
      case 1:
        label_str = AUTON_HEADING + "  #FF3333 RED#";
        break;
      case 2:
        label_str = AUTON_HEADING + "  #3333FF BLUE#";
        break;
      case 3:
        label_str = AUTON_HEADING + "#FFFF33 SKILLS#";
        break;
  }
  char label_array[label_str.length()+1];
  strcpy(label_array,label_str.c_str());
  lv_label_set_text(label,label_array);
}

void animate_autonNext(void*p)
{
  int x = lv_obj_get_x(autonLabel);
  animate_x_linear(autonLabel,x,-1*lv_obj_get_width(autonLabel),250);
  pros::delay(250);
  updateAutonLabel(autonLabel,auton);
  animate_x(autonLabel,480,x,250);
}

void animate_autonPrev(void*p)
{
  int x = lv_obj_get_x(autonLabel);
  animate_x_linear(autonLabel,x,480,250);
  pros::delay(250);
  updateAutonLabel(autonLabel,auton);
  animate_x(autonLabel,-1*lv_obj_get_width(autonLabel),x,250);
}

static lv_res_t autonNext_action(lv_obj_t * btn)
{
  auton++;
  auton = auton % NUM_OF_AUTONS;
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

void anim_sensorDebug(void*p)
{
  animateHomeScreenLeft();
  pros::delay(250);
  showSensorDebug();
  lv_obj_set_hidden(back_button,false);
  animate_x(back_button,lv_obj_get_x(back_button)+480,lv_obj_get_x(back_button),500);
  animateSensorDebugLeft();
}

void anim_autonDebug(void*p)
{
  animateHomeScreenLeft();
  pros::delay(250);
  showAutonDebug();
  lv_obj_set_hidden(back_button,false);
  animate_x(back_button,lv_obj_get_x(back_button)+480,lv_obj_get_x(back_button),500);
  animateAutonDebugLeft();
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

static lv_res_t back_action(lv_obj_t * btn)
{
  animateAutonDebugRight();
  animateSensorDebugRight();
  animateHomeScreenRight();
  lv_obj_set_hidden(back_button,true);
  return LV_RES_OK;
}

//Initialize whole GUI ==================================================================================================================================

void initAutonGUI()
{
  nameLabel = createTextLabel(nameLabel, "#00FF00 EXO##FFFFFF BYTES#",0,0);
  lv_label_set_style(nameLabel, &style4);
  lv_obj_align(nameLabel, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);

  autonLabel = createTextLabel(autonLabel, "AUTON SELECTED:  #00FF00 NONE# ",230,25);
  lv_obj_align(autonLabel, NULL, LV_ALIGN_IN_TOP_MID, 0, 97);
  updateAutonLabel(autonLabel, auton);

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

  sensorDebug_button = createBtn(sensorDebug_button,10,10,225,50,2,"SENSOR DEBUG");
  lv_obj_align(sensorDebug_button, NULL, LV_ALIGN_IN_TOP_LEFT,10,180);
  lv_btn_set_action(sensorDebug_button,LV_BTN_ACTION_CLICK, sensorDebugButton_action);

  autonDebug_button = createBtn(autonDebug_button,10,10,225,50,2,"AUTON DEBUG");
  lv_obj_align(autonDebug_button, NULL, LV_ALIGN_IN_TOP_RIGHT,-10,180);
  lv_btn_set_action(autonDebug_button,LV_BTN_ACTION_CLICK, autonDebugButton_action);


  back_button = createBtn(back_button,10,10,50,50,2,"<");
  lv_obj_align(back_button, NULL, LV_ALIGN_IN_TOP_LEFT,10,10);
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
}
