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

    
def drawLine(x1,y1,x2,y2):
  t = 0.0
  while t < 1.0:
      x = int((x1 + t * (x2 - x1)))
      y = int((y1 + t * (y2 - y1))) 
      img.putpixel((x,y),(0,0,0))
      t += 0.001
# def Bezier((x1,y1),(x2,y2),(x3,y3),(x4,y4),t):
#    x = x1*(1-t)*(1-t)*(1-t) + 3*t*x2*(1-t)*(1-t) + 3*t*t*x3*(1-t) + t*t*t*x4
#    y = y1*(1-t)*(1-t)*(1-t) + 3*t*y2*(1-t)*(1-t) + 3*t*t*y3*(1-t) + t*t*t*y4
#    return x,y
#    
def Bezier(xxx_todo_changeme,t):
   ((x1,y1),(x2,y2),(x3,y3),(x4,y4)) = xxx_todo_changeme
   x = x1*(1-t)*(1-t)*(1-t) + 3*t*x2*(1-t)*(1-t) + 3*t*t*x3*(1-t) + t*t*t*x4
   y = y1*(1-t)*(1-t)*(1-t) + 3*t*y2*(1-t)*(1-t) + 3*t*t*y3*(1-t) + t*t*t*y4
   return x,y
   
def drawBezier(xxx_todo_changeme1, xxx_todo_changeme2, xxx_todo_changeme3, xxx_todo_changeme4):
   (x1,y1) = xxx_todo_changeme1
   (x2,y2) = xxx_todo_changeme2
   (x3,y3) = xxx_todo_changeme3
   (x4,y4) = xxx_todo_changeme4
   t = 0.0
   while t < 1.0:
      x = int(x1*(1-t)*(1-t)*(1-t) + 3*t*x2*(1-t)*(1-t) + 3*t*t*x3*(1-t) + t*t*t*x4)
      y = int(y1*(1-t)*(1-t)*(1-t) + 3*t*y2*(1-t)*(1-t) + 3*t*t*y3*(1-t) + t*t*t*y4)
      create_circle((x,y),1,canvas)
      t+= 0.001
      
def thomas(a,b,c,d):
 # Finding the size of the matrix and determine n
 n1 = len(b)
 #print n # Used for debugging
 # Test the size of a and c
 if len(a) != n1-1:
  print('Wrong index size for a.\n A should have an index of', n1-1, '\n Your a has ', len(a))
  exit()
 while len(c) != n1-1:
  print('Wrong index size for c.\n C should have an index of', n1-1, '\n Your c has', len(c))
  exit()
 
 # Converting to float and appending 0.0 to c
 for i in range(0,len(a)):
  a[i] = float(a[i])
 for i in range(0,len(b)):
  b[i] = float(b[i])
 for i in range(0,len(c)):
  c[i] = float(c[i])
 for i in range(0,len(d)):
  d[i] = float(d[i])
 c.append(0.0) # Hack to make the function to work

 # Calculate p and q
 p = []; q= [] 
 p.append(c[0]/b[0]); q.append(d[0]/b[0])
 for j in range(1,n1):
  pj = c[j]/(b[j] - a[j-1]* p[j-1])
  qj = (d[j] - a[j-1]*q[j-1])/(b[j] - a[j-1]* p[j-1])
  p.append(pj); q.append(qj)
 #print p,q # Used for debugging the code!

 # Back sub
 x = []; x.append(q[n1-1])
 for j in range(n1-2,-1,-1):
  xj = q[j] - p[j]*x[0]  # Value holder
  x.insert(0,xj)   # Building the list backwards

 # Return the value
 return x

def create_circle(xxx_todo_changeme5, r,color,tf, canvasName): #center coordinates, radius
    (x, y) = xxx_todo_changeme5
    x0 = x - r
    y0 = y - r
    x1 = x + r
    y1 = y + r
    if(tf==1):
      return canvasName.create_oval(x0, y0, x1, y1,outline=color,fill=color)
    else:
      return canvasName.create_oval(x0, y0, x1, y1,outline=color)
      

root = tk.Tk()                         
Image_path="changeUPBackground.gif" 
image1 = tk.PhotoImage(file=Image_path)
canvas = tk.Canvas(root, width=720, height=720, bg='white')
canvas.pack()
canvas.create_image(360,360,image=image1)
imgD = Image.open('bot.png')
pmgD = ImageTk.PhotoImage(imgD)
canvas.image = pmgD
robo = canvas.create_image(589, 672, image=pmgD) #590, 680
#robo = canvas.create_rectangle((600, 600),(690, 690), outline = "black")
time = 0.0
def bezMaker(Kx,Ky):
   #Kx = [79,151,86, 181,277, 367,367] # X values of points to go to
   #Ky = [82,142,184,184,183, 190,190] # Y values of points to go to
   n=len(Kx)-1
   bot = [1 for i in range(n-2)] + [2]
   mid = [2] + [4 for i in range(n-2)] + [7]
   top = [1for i in range(n-1)]
   
   Rx = [Kx[0] + 2*Kx[1]] + [4*Kx[i]+2*Kx[i+1] for i in range(1,n-1)] + [8*Kx[n-1] + Kx[n]]
   Ry = [Ky[0] + 2*Ky[1]] + [4*Ky[i]+2*Ky[i+1] for i in range(1,n-1)] + [8*Ky[n-1] + Ky[n]]
   
   P0x = [Kx[i] for i in range(n)]
   P0y = [Ky[i] for i in range(n)]
   
   P1x = thomas(bot,mid,top,Rx)
   bot = [1 for i in range(n-2)] + [2]
   mid = [2] + [4 for i in range(n-2)] + [7]
   top = [1for i in range(n-1)]
   P1y = thomas(bot,mid,top,Ry)
   
   P2x = [2*Kx[i+1] - P1x[i+1] for i in range(n-1)] + [(Kx[n] + P1x[n-1])/2]
   P2y = [2*Ky[i+1] - P1y[i+1] for i in range(n-1)] + [(Ky[n]+P1y[n-1])/2]
   
   P3x = [Kx[i] for i in range(1,n+1)]
   P3y = [Ky[i] for i in range(1,n+1)]
   
   Pts = [((P0x[i],P0y[i]),(P1x[i],P1y[i]),(P2x[i],P2y[i]),(P3x[i],P3y[i])) for i in range(n)]
   
   #img.putpixel((Kx[0],Ky[0]),(0,0,0))5
   # for i in range(6):
   #    for j in range(6):
   #       canvas.create_rectangle(i*120, j*120, i*120+120, j*120+120, outline="black",fill="#bfbfbf")
   for i in range(n):
       create_circle((P0x[i],P0y[i]),3,"red",1,canvas)
   def tick():
       global time
       global robo
       k = int(time)
       i = time % 1.0
       create_circle(Bezier(Pts[k],i),1,"black",333, canvas)
       time += 0.002
       if time < n-1:
         canvas.after(1,tick)
       else:
         return 1;
   canvas.after(3000,tick)
   
bezMaker([79,151,86,86],[82,142,184,184])
bezMaker([86, 181,277, 367,367],[184,184,183, 190,190])
root.mainloop()
