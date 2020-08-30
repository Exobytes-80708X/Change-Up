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

lv_obj_t * LD1_temp_label;
lv_obj_t * LD2_temp_label;
lv_obj_t * RD1_temp_label;
lv_obj_t * RD2_temp_label;

lv_obj_t * LD1_temp_value;
lv_obj_t * LD2_temp_value;
lv_obj_t * RD1_temp_value;
lv_obj_t * RD2_temp_value;

lv_obj_t * imuLabel;
lv_obj_t * imuButton;

lv_obj_t * nameLabel;
lv_obj_t * img;

lv_obj_t * sensorDebug_button;
lv_obj_t * autonDebug_button;
lv_obj_t * back_button;

lv_obj_t * topBall_low_label;
lv_obj_t * topBall_high_label;
lv_obj_t * botBall_label;
lv_obj_t * ejector_label;

lv_obj_t * topBall_low_value;
lv_obj_t * topBall_high_value;
lv_obj_t * botBall_value;
lv_obj_t * ejector_value;


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


//Home Screen ==================================================================================================================================

void hideMotorTempLabels();
void showMotorTempLabels();
void hideHomeScreen()
{
  onHomeScreen = false;
  lv_obj_set_hidden(nameLabel,true);
  lv_obj_set_hidden(autonLabel,true);
  hideMotorTempLabels();

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
  showMotorTempLabels();

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
  //lv_obj_set_hidden(back_button,true);

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
  //lv_obj_set_hidden(back_button,false);

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

//Motor Temp ==================================================================================================================================


void updateMotorTempLabels()
{
  /*updateVarLabel(LD1_temp_label,"LD1",ld_1.getTemperature(),"C",2);
  updateVarLabel(LD2_temp_label,"LD2",ld_2.getTemperature(),"C",2);
  updateVarLabel(RD1_temp_label,"RD1",rd_1.getTemperature(),"C",2);
  updateVarLabel(RD2_temp_label,"RD2",rd_2.getTemperature(),"C",2);*/

  updateValueLabel(LD1_temp_value,ld_1.getTemperature(),"C",2);
  updateValueLabel(LD2_temp_value,ld_2.getTemperature(),"C",2);
  updateValueLabel(RD1_temp_value,rd_1.getTemperature(),"C",2);
  updateValueLabel(RD2_temp_value,rd_2.getTemperature(),"C",2);
}

void thread_motorTemps(void*p)
{
  while(true)
  {
    updateMotorTempLabels();
    pros::delay(1000);
  }
}

void initMotorTemp()
{
  LD1_temp_label = createTextLabel(LD1_temp_label, "LD1", 15, 135);
  LD2_temp_label = createTextLabel(LD2_temp_label, "LD2", 15, 155);
  RD1_temp_label = createTextLabel(RD1_temp_label, "RD1", 390, 135);
  RD2_temp_label = createTextLabel(RD2_temp_label, "RD2", 390, 155);

  LD1_temp_value = createTextLabel(LD1_temp_value, "", 55, 135);
  LD2_temp_value = createTextLabel(LD2_temp_value, "", 55, 155);
  RD1_temp_value = createTextLabel(RD1_temp_value, "", 430, 135);
  RD2_temp_value = createTextLabel(RD2_temp_value, "", 430, 155);

  updateMotorTempLabels();

  pros::Task task_motorTemps (thread_motorTemps, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
}

void delMotorTempLabels()
{
  lv_obj_del(LD1_temp_label);
  lv_obj_del(LD2_temp_label);
  lv_obj_del(RD1_temp_label);
  lv_obj_del(RD2_temp_label);
}

void hideMotorTempLabels()
{
  lv_obj_set_hidden(LD1_temp_label,true);
  lv_obj_set_hidden(LD2_temp_label,true);
  lv_obj_set_hidden(RD1_temp_label,true);
  lv_obj_set_hidden(RD2_temp_label,true);

  lv_obj_set_hidden(LD1_temp_value,true);
  lv_obj_set_hidden(LD2_temp_value,true);
  lv_obj_set_hidden(RD1_temp_value,true);
  lv_obj_set_hidden(RD2_temp_value,true);
}

void showMotorTempLabels()
{
  lv_obj_set_hidden(LD1_temp_label,false);
  lv_obj_set_hidden(LD2_temp_label,false);
  lv_obj_set_hidden(RD1_temp_label,false);
  lv_obj_set_hidden(RD2_temp_label,false);

  lv_obj_set_hidden(LD1_temp_value,false);
  lv_obj_set_hidden(LD2_temp_value,false);
  lv_obj_set_hidden(RD1_temp_value,false);
  lv_obj_set_hidden(RD2_temp_value,false);
}


//sensor Debug ==================================================================================================================================

void showSensorDebug()
{
  //lv_obj_set_hidden(back_button,false);

  lv_obj_set_hidden(topBall_low_label,false);
  lv_obj_set_hidden(topBall_high_label,false);
  lv_obj_set_hidden(botBall_label,false);
  lv_obj_set_hidden(ejector_label,false);

  lv_obj_set_hidden(topBall_low_value,false);
  lv_obj_set_hidden(topBall_high_value,false);
  lv_obj_set_hidden(botBall_value,false);
  lv_obj_set_hidden(ejector_value,false);
}

void hideSensorDebug()
{
  //lv_obj_set_hidden(back_button,true);

  lv_obj_set_hidden(topBall_low_label,true);
  lv_obj_set_hidden(topBall_high_label,true);
  lv_obj_set_hidden(botBall_label,true);
  lv_obj_set_hidden(ejector_label,true);

  lv_obj_set_hidden(topBall_low_value,true);
  lv_obj_set_hidden(topBall_high_value,true);
  lv_obj_set_hidden(botBall_value,true);
  lv_obj_set_hidden(ejector_value,true);
}

void initSensorDebug()
{
  topBall_low_label = createTextLabel(topBall_low_label, "[E] TOPBALL LOW", 100, 10);
  ejector_label = createTextLabel(ejector_label, "[F] EJECTOR", 100, 30);
  botBall_label = createTextLabel(botBall_label, "[G] BOTBALL", 100, 50);
  topBall_high_label = createTextLabel(topBall_high_label, "[H] TOPBALL HIGH", 100, 70);


  topBall_low_value = createTextLabel(topBall_low_label, "-", 300, 10);
  ejector_value = createTextLabel(ejector_label, "-", 300, 30);
  botBall_value = createTextLabel(botBall_label, "-", 300, 50);
  topBall_high_value = createTextLabel(topBall_high_label, "-", 300, 70);

  hideSensorDebug();
}


void thread_sensorDebug(void*p)
{
  while(!onHomeScreen) {
    updateValueLabel(topBall_low_value,topDetector.get_value(),"",4);
    updateValueLabel(topBall_high_value,topDetector2.get_value(),"",4);
    updateValueLabel(botBall_value,botDetector.get_value(),"",4);
    updateValueLabel(ejector_value,ejectDetector.get_value(),"",4);
    pros::delay(100);
  }
}

//Button Actions ==================================================================================================================================

void updateAutonLabel(lv_obj_t * label, int autonNumber)
{
  std::string label_str = "";
  switch(autonNumber) {
      case 0:
        label_str = AUTON_HEADING + "#00FF00 NONE#";
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

static lv_res_t sensorDebugButton_action(lv_obj_t * btn)
{
  hideHomeScreen();
  lv_obj_set_hidden(back_button,false);
  pros::Task sensor_debug (thread_sensorDebug, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
  showSensorDebug();
  return LV_RES_OK;
}

static lv_res_t autonDebugButton_action(lv_obj_t * btn)
{
  hideHomeScreen();
  lv_obj_set_hidden(back_button,false);
  showAutonDebug();
  return LV_RES_OK;
}

static lv_res_t back_action(lv_obj_t * btn)
{
  hideSensorDebug();
  hideAutonDebug();
  lv_obj_set_hidden(back_button,true);
  showHomeScreen();
  return LV_RES_OK;
}

//Initialize whole GUI ==================================================================================================================================

void initAutonGUI()
{
  nameLabel = createTextLabel(nameLabel, "#00FF00 EXO##FFFFFF BYTES#",0,0);
  lv_label_set_style(nameLabel, &style4);
  lv_obj_align(nameLabel, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);

  autonLabel = createTextLabel(autonLabel, "AUTON SELECTED: #00FF00 NONE#",230,25);
  lv_obj_align(autonLabel, NULL, LV_ALIGN_IN_TOP_MID, 0, 97);

  autonPrev_button = createBtn(autonPrev_button,10,10,100,50,2,"<");
  lv_btn_set_action(autonPrev_button,LV_BTN_ACTION_CLICK, autonPrev_action);
  lv_obj_align(autonPrev_button, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 80);

  autonNext_button = createBtn(autonNext_button,120,10,100,50,1,">");
  lv_btn_set_action(autonNext_button,LV_BTN_ACTION_CLICK, autonNext_action);
  lv_obj_align(autonNext_button, NULL, LV_ALIGN_IN_TOP_RIGHT, -10, 80);

  imuButton = createBtn(imuButton,10,100,210,50,1);
  imuLabel = createBtnLabel(imuLabel,imuButton, "NOT CALIBRATED");
  lv_btn_set_action(imuButton,LV_BTN_ACTION_PR, imuButton_action);
  lv_obj_align(imuButton, NULL, LV_ALIGN_IN_TOP_MID,0,125);

  sensorDebug_button = createBtn(sensorDebug_button,10,10,220,50,2,"SENSOR DEBUG");
  lv_obj_align(sensorDebug_button, NULL, LV_ALIGN_IN_TOP_LEFT,10,180);
  lv_btn_set_action(sensorDebug_button,LV_BTN_ACTION_CLICK, sensorDebugButton_action);

  autonDebug_button = createBtn(autonDebug_button,10,10,220,50,2,"AUTON DEBUG");
  lv_obj_align(autonDebug_button, NULL, LV_ALIGN_IN_TOP_RIGHT,-10,180);
  lv_btn_set_action(autonDebug_button,LV_BTN_ACTION_CLICK, autonDebugButton_action);

  back_button = createBtn(back_button,10,10,50,50,2,"<");
  lv_obj_align(back_button, NULL, LV_ALIGN_IN_TOP_LEFT,10,10);
  lv_btn_set_action(back_button,LV_BTN_ACTION_CLICK, back_action);
  lv_obj_set_hidden(back_button,true);
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
  initMotorTemp();
}
