import serial
import time
import pygame as pg

pg.init()
screen = pg.display.set_mode((400, 40))
ser = serial.Serial("COM3",115200)
color = pg.Color('dodgerblue2')
text = ''
fsize = 30
font = pg.font.SysFont(None, 24)
lineEnding = "\n"
pg.display.update()
while(True):
	

	for event in pg.event.get():
		if (event.type == pg.QUIT):
			pg.quit()
		elif event.type == pg.MOUSEBUTTONDOWN:
			screen.fill((50, 50, 50))
		elif event.type == pg.KEYDOWN:
			if event.key == pg.K_RETURN:
				ser.write(text.encode()+lineEnding.encode())
				text = ''
			elif event.key == pg.K_BACKSPACE:
				text = text[:-1]
			else:
				text += event.unicode
			lines = text.splitlines()
			for i, l in enumerate(lines):
				img = font.render(text, True, ((30,30,30)))
				screen.blit(img, (20, 20))
			pg.display.update()
	

	if(ser.in_waiting>0):
		print(ser.readline().decode(), ser.in_waiting)