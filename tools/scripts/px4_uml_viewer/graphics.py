from tkinter import*
import time

class graphics:
    primary = ''
    def __init__(self, w, h, title):
        ''' Initialize the graphics object.
        Creates a new tkinter Tk object, 
        and a tkinter Canvas object,
        placed insize the Tk object.
        '''
        self.primary = Tk()
        self.primary.title(title)
        self.primary.geometry('%dx%d+%d+%d' % (w, h, 50, 100))
        self.canvas = Canvas(self.primary, width=w, height=h, highlightthickness=0)
        self.canvas.focus_set()
        self.canvas.pack()
        self.mouse_x = 0
        self.mouse_y = 0
        self.images = {}
        self.frame_count = 0
        self.__handle_motion()

    

    '''
    BEGIN PRIVATE FUNCTION(S)
    '''

    def __handle_motion(self):
        ''' Ensure mouse x and y coordinates are updated when mouse moves.
        '''
        def motion_action(event):
            self.mouse_x = event.x
            self.mouse_y = event.y
        self.canvas.bind('<Motion>', motion_action)

    '''
    END PRIVATE FUNCTION(S)
    '''

    def resize(self, width, height):
        self.primary.geometry(str(width) + 'x' + str(height))

    def text(self, x, y, content, fill='black', size=17):
        ''' Draw text on the canvas.
        Must always specify the text, x, y position.
        Can optionally specify the fill color and size.
        '''
        text = self.canvas.create_text(x, y, text=content, fill=fill, font=('Arial', size), anchor='nw')
        self.canvas.move(text, 0, 0)
   
    def set_left_click_action(self, callee):
        ''' Call the callee function whenever the left click happens.
        callee should take two parameters, the mouse x and mouse y coordinates.
        '''
        def left_click_action(event):
            callee(self, event.x, event.y)
        ''' <Button-1> is the left-most mouse button '''
        self.canvas.bind('<Button-1>', left_click_action)
    
    def set_right_click_action(self, callee):
        ''' Call the callee function whenever the right click happens.
        callee should take two parameters, the mouse x and mouse y coordinates.
        '''
        def right_click_action(event):
            callee(self, event.x, event.y)
        ''' <Button-2> or <Button-3> is the right-most mouse button.
        Both are set just in case '''
        self.canvas.bind('<Button-2>', right_click_action)
        self.canvas.bind('<Button-3>', right_click_action)
    
    def set_keyboard_action(self, callee):
        ''' Call the callee function whenever a keyboard key is pressed.
        callee should take one parameter, a char representing the key.
        '''
        def keyboard_action(event):
            callee(self, event.char)
        self.canvas.bind('<KeyPress>', keyboard_action)

    def get_color_string(self, red, green, blue):
        ''' accepts three ints that should represent and RGB color.
        Returns a hex string'''
        hex_string = hex(red)[2:].rjust(2, '0') + \
                     hex(green)[2:].rjust(2, '0') + \
                     hex(blue)[2:].rjust(2, '0')
        return '#' + hex_string

    def triangle(self, x1, y1, x2, y2, x3, y3, fill='black'):
        ''' Draw a triangle.
        The three corners of the triangle are specified with the parameter coordinates.
        '''
        r = self.canvas.create_polygon(x1, y1, x2, y2, x3, y3, fill=fill)
        self.canvas.move(r, 0, 0)
    
    def line(self, x1, y1, x2, y2, fill='black', width=3):
        ''' Draw a line.
        The two ends of the line are specified with the parameter coordinates.
        '''
        r = self.canvas.create_line(x1, y1, x2, y2, fill=fill, width=width)
        self.canvas.move(r, 0, 0)
    
    def ellipse(self, x, y, w, h, fill='black'):
        ''' Draw an ellipse on the canvas.
        Specify x, y (center of ellipse) and width / height.
        '''
        r = self.canvas.create_oval(x-(w/2), y-(h/2), x+(w/2), y+(h/2), fill=fill, outline='')
        self.canvas.move(r, 0, 0)
    
    def rectangle(self, x, y, w, h, fill='black'):
        ''' Draw a rectangle on the canvas.
        Specify x, y (top-left corner) and width / height.
        '''
        r = self.canvas.create_rectangle(x, y, x+w, h+y, fill=fill, outline='')
        self.canvas.move(r, 0, 0)
    
    def image(self, x, y, up_scale, down_scale, file_name):
        ''' Draw an image on the canvas.
        Specify x, y (top-left corner) and width / height.
        '''
        if file_name not in self.images:
            self.images[file_name] = PhotoImage(file=file_name)
        self.images[file_name] = self.images[file_name].zoom(up_scale, up_scale)
        self.images[file_name] = self.images[file_name].subsample(down_scale, down_scale)
        i = self.canvas.create_image(x, y, anchor='nw', image=self.images[file_name])
        self.canvas.move(i, 0, 0)
        return self.images[file_name]

    def update(self):
        ''' Does an idle task update and regular update.
        '''
        self.primary.update_idletasks()
        self.primary.update()

    def frame_space(self, frame_rate):
        ''' Sleeps for a time that corresponds to the provided frame rate.
        '''
        sleep_ms = 1.0 / float(frame_rate)
        time.sleep(sleep_ms)

    def update_frame(self, frame_rate):
        ''' Updates and sleeps.
        This should be called at the end of each iteration of a users draw loop.
        '''
        self.update()
        self.frame_space(frame_rate)
        self.frame_count += 1
    
    def clear(self):
        ''' Clears the canvas.
        '''
        self.canvas.delete('all')

