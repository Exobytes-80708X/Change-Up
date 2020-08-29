#include "main.h"
#include "display/lvgl.h"
#include "display/lv_core/lv_style.h"
int auton = 0;
const int NUM_OF_AUTONS = 4;
const std::string AUTON_HEADING = "AUTON SELECTED: ";

LV_FONT_DECLARE(armadura_solid);
const int DEBUG_X_1 = 25;
const int DEBUG_X_2 = 255;
lv_obj_t * xLabel;
lv_obj_t * yLabel;
lv_obj_t * thetaLabel;
lv_obj_t * debugLabel1;
lv_obj_t * debugLabel2;
lv_obj_t * debugLabel3;
lv_obj_t * debugLabel4;
lv_obj_t * debugLabel5;
lv_obj_t * debugLabel6;

lv_obj_t * autonLabel;
lv_obj_t * autonNext_button;
lv_obj_t * autonPrev_button;

lv_obj_t * LD1_temp_label;
lv_obj_t * LD2_temp_label;
lv_obj_t * RD1_temp_label;
lv_obj_t * RD2_temp_label;

lv_obj_t * imuLabel;
lv_obj_t * imuButton;

lv_obj_t * nameLabel;
static lv_style_t style1;
static lv_style_t style2;
static lv_style_t style3;
void initStyles()
{
  lv_style_copy(&style1, &lv_style_plain);
  style1.text.color = LV_COLOR_WHITE;
  style1.text.font = &armadura_solid;

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
  lv_label_set_style(parent, &style1);
  lv_label_set_text(parent, text_array);
}

void initDebugLabels()
{
  xLabel = createTextLabel(xLabel, "", DEBUG_X_2, 20);
  yLabel = createTextLabel(yLabel, "", DEBUG_X_2, 40);
  thetaLabel = createTextLabel(thetaLabel, "", DEBUG_X_2, 60);

  debugLabel1 = createTextLabel(debugLabel1,"",DEBUG_X_1,20);
  debugLabel2 = createTextLabel(debugLabel2,"",DEBUG_X_1,40);
  debugLabel3 = createTextLabel(debugLabel3,"",DEBUG_X_1,60);
  debugLabel4 = createTextLabel(debugLabel4,"",DEBUG_X_1,80);
  debugLabel5 = createTextLabel(debugLabel5,"",DEBUG_X_1,100);
  debugLabel6 = createTextLabel(debugLabel6,"",DEBUG_X_1,120);
}

void clearDebugLabels()
{
  lv_label_set_text(debugLabel1, "");
  lv_label_set_text(debugLabel2, "");
  lv_label_set_text(debugLabel3, "");
  lv_label_set_text(debugLabel4, "");
  lv_label_set_text(debugLabel5, "");
  lv_label_set_text(debugLabel6, "");
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

void initMotorTempLabels()
{
  LD1_temp_label = createTextLabel(LD1_temp_label, "", DEBUG_X_2, 120);
  LD2_temp_label = createTextLabel(LD2_temp_label, "", DEBUG_X_2, 140);
  RD1_temp_label = createTextLabel(RD1_temp_label, "", DEBUG_X_2, 180);
  RD2_temp_label = createTextLabel(RD2_temp_label, "", DEBUG_X_2, 200);
}

void updateMotorTempLabels()
{
  updateVarLabel(LD1_temp_label,"LD1",ld_1.getTemperature(),"C");
  updateVarLabel(LD2_temp_label,"LD2",ld_2.getTemperature(),"C");
  updateVarLabel(RD1_temp_label,"RD1",rd_1.getTemperature(),"C");
  updateVarLabel(RD2_temp_label,"RD2",rd_2.getTemperature(),"C");
}

void delMotorTempLabels()
{
  lv_obj_del(LD1_temp_label);
  lv_obj_del(LD2_temp_label);
  lv_obj_del(RD1_temp_label);
  lv_obj_del(RD2_temp_label);
}

void updateAutonLabel(lv_obj_t * label, int autonNumber)
{
  std::string label_str = "";
  switch(autonNumber) {
      case 0:
        label_str = AUTON_HEADING + "NONE";
        break;
      case 1:
        label_str = AUTON_HEADING + "#FF3333 RED#";
        break;
      case 2:
        label_str = AUTON_HEADING + "#3333FF BLUE#";
        break;
      case 3:
        label_str = AUTON_HEADING + "#FFFF33 SKILLS#";
        break;
  }
  char label_array[label_str.length()+1];
  strcpy(label_array,label_str.c_str());
  lv_label_set_text(label,label_array);
}

static lv_res_t autonNext_action(lv_obj_t * btn)
{
  auton++;
  auton = auton % NUM_OF_AUTONS;
  updateAutonLabel(autonLabel,auton);
  return LV_RES_OK;
}

static lv_res_t autonPrev_action(lv_obj_t * btn)
{
  auton--;
  if(auton < 0) auton = 2;
  updateAutonLabel(autonLabel,auton);
  return LV_RES_OK;
}

static lv_res_t imuButton_action(lv_obj_t * btn)
{
  imu.reset();
  pros::delay(100);
  while(imu.is_calibrating()) {
    pros::delay(10);
  }
  lv_label_set_text(imuLabel,"CALIBRATED");
  return LV_RES_OK;
}


void initAutonGUI()
{
  autonLabel = createTextLabel(autonLabel, "AUTON SELECTED: NONE",230,25);
  autonPrev_button = createBtn(autonNext_button,10,10,100,50,2,"PREV");
  autonNext_button = createBtn(autonNext_button,120,10,100,50,1,"NEXT");
  imuButton = createBtn(imuButton,10,70,210,50,1);
  imuLabel = createBtnLabel(imuLabel,imuButton, "NOT CALIBRATED");

  nameLabel = createTextLabel(nameLabel, "#00FF00 EXO##FFFFFF BYTES#",50,150);

  lv_btn_set_action(autonNext_button,LV_BTN_ACTION_CLICK, autonNext_action);
  lv_btn_set_action(autonPrev_button,LV_BTN_ACTION_CLICK, autonPrev_action);
  lv_btn_set_action(imuButton,LV_BTN_ACTION_CLICK, imuButton_action);
}

void delAutonGUI()
{
  lv_obj_del(autonLabel);
  lv_obj_del(autonNext_button);
  lv_obj_del(autonPrev_button);
  lv_obj_del(imuLabel);
  lv_obj_del(imuButton);
  lv_obj_del(nameLabel);
}

void thread_motorTemps(void*p)
{
  initMotorTempLabels();
  while(true)
  {
    updateMotorTempLabels();
    pros::delay(1000);
  }
}

void initGUI(void*p)
{
  initStyles();
  initAutonGUI();
  pros::Task task_motorTemps (thread_motorTemps, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
  while(isRobotDisabled) pros::delay(10);
  delAutonGUI();
}
