#Name: Rushil Sidhu
#Date: 3/8/20
from random import *
from array import *
import numpy as np
from random import randint
from timeit import default_timer
import tkinter as tk
from PIL import Image, ImageTk
import os
import math
import time

##############CUSTOMIZABLE VARIABLES##################
vel = 5 # pixel/second, the picture field is 5 pixels/real inch
ang_vel = 2 # pixel * theta / second






##############CURRENTLY USELESS CODE##################   
def drawRobo(p1,p2,p3,p4):
   canvas.create_line(p1,p2)
   canvas.create_line(p2,p3)
   canvas.create_line(p3,p4)
   canvas.create_line(p4,p1) # I can use this for a trace model later
   
##############CURRENTLY USELESS CODE##################   

##############DERIVATIVES####################
##############DERIVATIVES####################


############ROTATION OF ROBOT########################
def rad(deg):
   return deg*math.pi/180
   
def rotateX(xxx_todo_changeme,theta):
   (x,y) = xxx_todo_changeme
   global center
   return center[0] + (x-center[0])*math.cos(theta) + (y-center[1])*math.sin(theta)
   
def rotateY(xxx_todo_changeme1,theta):
   (x,y) = xxx_todo_changeme1
   return center[1] + (y-center[1])*math.cos(theta) - (x-center[0])*math.sin(theta)
   
def rotateBot(temp_theta):
  global p1,p2,p3,p4,theta
  global center
  temp_theta = rad(temp_theta)
  theta = theta + temp_theta
  p1 = (rotateX(p1,temp_theta),rotateY(p1,temp_theta))
  p2 = (rotateX(p2,temp_theta),rotateY(p2,temp_theta))
  p3 = (rotateX(p3,temp_theta),rotateY(p3,temp_theta))
  p4 = (rotateX(p4,temp_theta),rotateY(p4,temp_theta))
  updatecoords()
  
############ROTATION OF ROBOT########################

############MOVEMENT OF ROBOT########################
def forward(vel):
   global p1,p2,p3,p4,p5,theta
   p1 = (p1[0] - vel*math.sin(theta),p1[1] - vel*math.cos(theta))
   p2 = (p2[0] - vel*math.sin(theta),p2[1] - vel*math.cos(theta))
   p3 = (p3[0] - vel*math.sin(theta),p3[1] - vel*math.cos(theta))
   p4 = (p4[0] - vel*math.sin(theta),p4[1] - vel*math.cos(theta))
   updatecoords()
   
############MOVEMENT OF ROBOT########################

############UPDATE COORDS AND THETA##################

def updatecoords():
   global center, robo,p1,p2,p3,p4,p5,theta
   center = (p1[0] - .5*(p1[0]-p3[0]),p1[1] - .5*(p1[1]-p3[1]))
   canvas.coords(robo,p1[0],p1[1],p2[0],p2[1],p3[0],p3[1],p4[0],p4[1])
   canvas.coords(dir,center[0],center[1],p1[0] - .5*(p1[0]-p2[0])-402*math.sin(theta),p1[1] - .5*(p1[1]-p2[1])-402*math.cos(theta))
   if(theta >= 2*math.pi):
      theta = theta - 2*math.pi
   if(theta <= -2*math.pi):
      theta = theta + 2*math.pi
############UPDATE COORDS AND THETA##################

############FOLLOW LINES#################
def getLineX(x1,y1,x2,y2,t):
      return int((x1 + t * (x2 - x1)))
      
def getLineY(x1,y1,x2,y2,t):
      return int((y1 + t * (y2 - y1))) 
      
def drawLine(x1,y1,x2,y2):
  t = 0.0
  while t < 1.0:
      x = getLineX(x1,y1,x2,y2,t)
      y = getLineY(x1,y1,x2,y2,t)
      canvas.create_line((x,y),(x+1,y+1),fill="green")
      t += 0.001
############FOLLOW LINES#################   

############TURN TO POINT###############
def calcThetaToTurn(x,y):
   global center,theta
   if(x-center[0] == 0):
      if(y-center[1] > 0):
         return math.pi
      else:
         return 0
         
   if(y <= center[1]):
      return math.atan((x-center[0]) / (y- center[1]))
   else:
      return math.pi + math.atan((x-center[0]) / (y - center[1]))


def turnTo(x,y,velocity):
   turnTheta = calcThetaToTurn(x,y)
   global theta
   if(theta < turnTheta):
      return velocity

############TURN TO POINT###############
   
########BACKGROUND STUFF#############
root = tk.Tk()                         
Image_path="changeUPBackground.gif" 
image1 = tk.PhotoImage(file=Image_path)
canvas = tk.Canvas(root, width=720, height=720, bg='white')
canvas.create_image(360,360,image=image1)
canvas.pack()
########BACKGROUND STUFF#############

#########STARTING POINTS###########
# KEEP WIDTH 90 px or 80px
p1 = (205,620)
p2 = (285,620)
p3 = (285,705)
p4 = (205,705)
center = (p1[0] - .5*(p1[0]-p3[0]),p1[1] - .5*(p1[1]-p3[1]))
theta = 0
robo = canvas.create_polygon(p1,p2,p3,p4, outline = "green")
drawLine(center[0],center[1],center[0]-100,center[1]-100)
dir = canvas.create_line(center[0],center[1],p1[0] - .5*(p1[0]-p2[0])-202*math.sin(theta),p1[1] - .5*(p1[1]-p2[1])-202*math.cos(theta), fill = "red")


#########STARTING POINTS###########


###########RUN METHOD################
temp = (center[0],center[1])
rotateBot(-35)
forward(400)
rotateBot(65)
forward(200)
rotateBot(-95)
temp1 = calcThetaToTurn(temp[0],temp[1])


rot = 0
if(theta - temp1 > temp1 - theta):
   rot = 1
else:
   rot = -1
 #  temp1 =-2*math.pi + temp1
      
print(theta)#*180/math.pi)
print(temp1)#*180/math.pi)
while(True):
   time.sleep(0.05)
   root.update_idletasks()
   root.update()
   updatecoords()
   canvas.create_line(center,(center[0]+1,center[1]+1),fill="red")
   print(theta)
   rotateBot(turnTo(temp[0],temp[1],ang_vel))
   #if(theta < temp1):
   #canvas.after(10,tick)
   #else:
   #   break
   
###########RUN METHOD################
