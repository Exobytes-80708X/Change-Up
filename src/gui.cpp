#include "main.h"
lv_obj_t * xLabel;
lv_obj_t * yLabel;
lv_obj_t * thetaLabel;
lv_obj_t * debugLabel1;
lv_obj_t * debugLabel2;
lv_obj_t * debugLabel3;
lv_obj_t * debugLabel4;
lv_obj_t * debugLabel5;
lv_obj_t * debugLabel6;


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

void updateVarLabel(lv_obj_t *parent, std::string varName, double var)
{
  std::string varValue = std::to_string(var);
  std::string var_str = varName + " = " + varValue.substr(0,8);
  char var_array[var_str.length()+1];
  strcpy(var_array,var_str.c_str());

  lv_label_set_text(parent, var_array);
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
