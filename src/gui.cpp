#include "main.h"
int auton = 0;
const int NUM_OF_AUTONS = 4;
const std::string AUTON_HEADING = "Auton Selected: ";
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


lv_obj_t* createBtn(lv_obj_t * parent, lv_coord_t x, lv_coord_t y, lv_coord_t width, lv_coord_t height, int id, const char* title)
{
  parent = lv_btn_create(lv_scr_act(), NULL);
  lv_obj_set_pos(parent, x, y);
  lv_obj_set_size(parent,width,height);
  lv_obj_set_free_num(parent, id);

  lv_obj_t * label = lv_label_create(parent, NULL);
  lv_label_set_text(label, title);

  return parent;
}

lv_obj_t * createTextLabel(lv_obj_t *parent, const char* text, lv_coord_t x, lv_coord_t y)
{
  parent = lv_label_create(lv_scr_act(), NULL);
  lv_label_set_text(parent,text);
  lv_obj_set_pos(parent,x,y);

  return parent;
}

void updateVarLabel(lv_obj_t *parent, std::string varName, double var, std::string varUnits)
{
  std::string varValue = std::to_string(var);
  std::string var_str = varName + " = " + varValue.substr(0,8) + " " + varUnits;
  char var_array[var_str.length()+1];
  strcpy(var_array,var_str.c_str());

  lv_label_set_text(parent, var_array);
}

void initDebugLabels()
{
  xLabel = createTextLabel(xLabel, "", 200, 20);
  yLabel = createTextLabel(yLabel, "", 200, 40);
  thetaLabel = createTextLabel(thetaLabel, "", 200, 60);

  debugLabel1 = createTextLabel(debugLabel1,"",25,20);
  debugLabel2 = createTextLabel(debugLabel2,"",25,40);
  debugLabel3 = createTextLabel(debugLabel3,"",25,60);
  debugLabel4 = createTextLabel(debugLabel4,"",25,80);
  debugLabel5 = createTextLabel(debugLabel5,"",25,100);
  debugLabel6 = createTextLabel(debugLabel6,"",25,120);
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
  LD1_temp_label = createTextLabel(LD1_temp_label, "", 200, 100);
  LD2_temp_label = createTextLabel(LD2_temp_label, "", 200, 120);
  RD1_temp_label = createTextLabel(RD1_temp_label, "", 200, 160);
  RD2_temp_label = createTextLabel(RD2_temp_label, "", 200, 180);
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
        label_str = AUTON_HEADING + "None";
        break;
      case 1:
        label_str = AUTON_HEADING + "Red";
        break;
      case 2:
        label_str = AUTON_HEADING + "Blue";
        break;
      case 3:
        label_str = AUTON_HEADING + "Skills";
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

void initAutonGUI()
{
  autonLabel = createTextLabel(autonLabel, "Auton Selected: None",230,25);
  autonPrev_button = createBtn(autonNext_button,10,10,100,50,2,"Prev");
  autonNext_button = createBtn(autonNext_button,120,10,100,50,1,"Next");

  lv_btn_set_action(autonNext_button,LV_BTN_ACTION_CLICK, autonNext_action);
  lv_btn_set_action(autonPrev_button,LV_BTN_ACTION_CLICK, autonPrev_action);
}

void delAutonGUI()
{
  lv_obj_del(autonLabel);
  lv_obj_del(autonNext_button);
  lv_obj_del(autonPrev_button);
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
  initAutonGUI();
  pros::Task task_motorTemps (thread_motorTemps, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
  while(isRobotDisabled) pros::delay(10);
  delAutonGUI();
}
