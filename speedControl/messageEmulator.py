import serial
import pygame as pg

ser = serial.Serial("COM3",115200)

pg.init()
screen = pg.display.set_mode((875, 325))
COLOR_INACTIVE = pg.Color('lightskyblue3')
COLOR_ACTIVE = pg.Color('dodgerblue2')
fsize = 18
FONT = pg.font.Font(None, fsize)
lineEnding = "\n"


class InputBox:

    def __init__(self, x, y, w, h, text=''):
        self.lines = []
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
                    ser.write(self.text.encode()+lineEnding.encode())
                    print(self.text)
                    textCoppy = self.text
                    self.text = ''
                    self.update()
                    return textCoppy
                elif event.key == pg.K_BACKSPACE:
                    self.text = self.text[:-1]
                else:
                    self.text += event.unicode
        return 

    def update(self):
        pass

    def draw(self, screen):
        # Blit the text.
        lines = self.text.splitlines()
        for i, l in enumerate(lines):
            screen.blit(FONT.render(l, 0, self.color), (self.rect.x+5, self.rect.y+5+ fsize*i))
        lines = [value for value in self.lines if value != ""]
        for i, l in enumerate(lines):
            screen.blit(FONT.render(l, 0, self.color), (self.rect.x+5, self.rect.y+5+ fsize*i))
        # Blit the rect.
        pg.draw.rect(screen, self.color, self.rect, 2)



def main():
    splitVal = 600
    clock = pg.time.Clock()
    input_box1 = InputBox(5, 5, 865, fsize)
    input_box2 = InputBox(5,fsize+10, splitVal, 325-fsize*2)
    input_box3 = InputBox(splitVal+5,fsize+10, 865-splitVal, 325-fsize*2)
    input_boxes = [input_box1, input_box2, input_box3]
    done = False
    strs = ["" for x in range(16)]
    while not done:
        if(ser.in_waiting):
            for i in range(len(strs)-1):
                strs[i] = strs[i+1]
            strs[len(strs)-1] = ser.readline().decode('UTF8')
        for event in pg.event.get():
            if event.type == pg.QUIT:
                done = True
            text = input_box1.handle_event(event)
            input_box2.handle_event(event)
            input_box3.handle_event(event)

        
        if(text!= None):
            print("receiving", text)
            input_box3.text = text
            input_box3.update()

        input_box2.lines = strs

        screen.fill((30, 30, 30))
        for box in input_boxes:
            box.draw(screen)

        pg.display.flip()
        clock.tick(30)


if __name__ == '__main__':
    main()
    pg.quit()