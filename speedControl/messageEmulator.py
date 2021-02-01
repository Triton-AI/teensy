# 
# while(True):
#     ser.write(b'a\n')
#     while(ser.in_waiting):
#         print (ser.readline().split())
#         ser.write(b'a\n')

import serial
ser = serial.Serial('COM3', 9600, timeout=0.050)
import pygame as pg
import sys
from pygame.locals import *


pg.init()
BLACK  = (0, 0,0)
WHITE  = (255, 255, 255)
RED    = (255, 0, 0)
YELLOW = (255, 255, 0)
BLUE   = (0,0,255)
GREEN = (0,255,0)
COLOR_INACTIVE = pg.Color('lightskyblue3')
COLOR_ACTIVE = pg.Color('dodgerblue2')
FONT = pg.font.Font(None, 20)
DISPLAYSURF = pg.display.set_mode((900,325), RESIZABLE)
DISPLAYSURF.fill(WHITE)

# Data Definition
class window:
    def __init__(self,width,height):
        pg.init()
        self.width = width
        self.height = height
        DISPLAYSURF = pg.display.set_mode((self.width,self.height), RESIZABLE)
        DISPLAYSURF.fill(WHITE)

    def CreateWindow(self,width,height):
        '''Updates the window width and height '''
        self.width = width
        self.height = height
        pg.display.set_caption("Press ESC to quit")
        DISPLAYSURF = pg.display.set_mode((width,height),RESIZABLE)
        DISPLAYSURF.fill(WHITE)

class InputBox:

    def __init__(self, x, y, w, h, text=''):
        self.rect = pg.Rect(x, y, w, h)
        self.color = COLOR_INACTIVE
        self.text = text
        self.txt_surface = FONT.render(text, True, self.color)
        self.active = False

    def handle_event(self, event):
        if event.type == pg.MOUSEBUTTONDOWN:
            # If the user clicked on the input_box rect.
            if self.rect.collidepoint(event.pos):
                # Toggle the active variable.
                self.active = not self.active
            else:
                self.active = False
            # Change the current color of the input box.
            self.color = COLOR_ACTIVE if self.active else COLOR_INACTIVE
        if event.type == pg.KEYDOWN:
            if self.active:
                if event.key == pg.K_RETURN:
                    print(self.text)
                    self.text = ''
                    return self.text
                elif event.key == pg.K_BACKSPACE:
                    self.text = self.text[:-1]
                else:
                    self.text += event.unicode
                # Re-render the text.
                self.txt_surface = FONT.render(self.text, True, self.color)

    def update(self,window,x):
        # Resize the box if the text is too long.
        self.rect.w = window.width/2
        self.rect.x = x

    def draw(self, screen):
        # Blit the text.
        screen.blit(self.txt_surface, (self.rect.x+5, self.rect.y+5))
        # Blit the rect.
        pg.draw.rect(screen, self.color, self.rect, 2)

class DisplayBox:

    def __init__(self, x, y, w, h, text=""):
        self.rect = pg.Rect(x, y, w, h)
        self.color = COLOR_INACTIVE
        self.text = text
        self.txt_surface = FONT.render(text, True, self.color)
        self.active = False

    def handle_event(self, event):
        pass

    def update(self,window,x,y,slide=0,text=""):
        # Resize the box if the text is too long.
        self.rect.w = window.width/2
        self.rect.x = x
        self.rect.y = y
        

    def draw(self, screen):
        #render the text
        self.txt_surface = FONT.render(self.text, True, self.color)
        # Blit the text.
        screen.blit(self.txt_surface, (self.rect.x+5, self.rect.y+5))
        # Blit the rect.
        pg.draw.rect(screen, self.color, self.rect, 2)

# pg.init()
# screen = pg.display.set_mode((640, 480))




def main():
    clock = pg.time.Clock()
    window1 = window(875,325)
    input_box1 = InputBox(5, 5, window1.width/2, 32)
    input_box2 = InputBox(window1.width/2, 5, window1.width/2, 32)
    display_box1 = DisplayBox(5, 5, window1.width/2, 32)
    display_box2 = DisplayBox(window1.width/2, 5, window1.width/2, 32, "thiolj\nklsjdflkj\n")
    boxes = [input_box1, input_box2,display_box1,display_box2]
    done = False

    while not done:
        DISPLAYSURF.fill(WHITE)
        
            
        for event in pg.event.get():
            if event.type == pg.QUIT:
                done = True
            elif event.type == VIDEORESIZE:
                    window1.CreateWindow(event.w,event.h)
            pg.display.update()

            input_box1.handle_event(event)
            input_box2.handle_event(event)


        input_box1.update(window1,0)
        input_box2.update(window1,window1.width/2)

        display_box1.update(window1,0,100)
        display_box2.update(window1,window1.width/2,100)
        text = ser.readline().decode("utf-8").split('\n')[0]
        if(len(text)>1):
            display_box1.text = display_box1.text+text
        else:
            display_box1.text = text
        

        for box in boxes:
            box.draw(DISPLAYSURF)

        pg.display.flip()
        clock.tick(30)

if __name__ == '__main__':
    main()
    pg.quit()
    sys.exit()